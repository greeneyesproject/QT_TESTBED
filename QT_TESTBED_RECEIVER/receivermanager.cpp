#include "receivermanager.h"
#include "receivergui.h"
#include "ui_receivergui.h"
#include "ui_display.h"
#include "utilities.hpp"
#include "sort_like_matlab.h"

receiverManager::receiverManager(string imgDatabasePath_)
{

    // connect the main thread with the gui thread
    if( !QObject::connect(this,SIGNAL(sig_update_image1()),this,SLOT(slot_update_image1())) )
        return;
    if( !QObject::connect(this,SIGNAL(sig_update_image2()),this,SLOT(slot_update_image2())) )
        return;

    // update the system variables
    imgDatabasePath = imgDatabasePath_;  // path of the image database

    // set the default operation parameters
    radioDevice = "/dev/ttyUSB0";
    reqActs.jpeg_qf = 30;
    reqActs.det_threshold = 60; // the default detection threshold
    reqActs.max_features  = 50;   // maximum number of features to be sent
    reqActs.do_compression = false;
    reqActs.cta = true;
    reqActs.atc = false;
    reqActs.tx_delay = 10;
    reqActs.stop_program = false;
    match_threshold = 200;

    // auxiliary variables for computing the frame rate
    frameCounter = 0;
    maxFramesFPS = 3;
    time_start = 0;

    // define the standard image size
    std_W = 640;
    std_H = 480;

    // initialize the data transmission
    dataTx      = new DataTransmission();

    // initialize the extractor
    featExtract = new VisualFeatureExtraction;

    // set the detector
    BRISK_detParams detPrms(reqActs.det_threshold,4);
    featExtract->setDetector("BRISK",&detPrms); // set the detector


    // set the descriptor
    vector<int> pairs;
    int aux_pairs[BRISK_LENGTH_BITS];
    ifstream fileRank;
    if ( BRISK_LENGTH_BITS == 512 ){
        fileRank.open("brisk/ranking_original_optimized512.bin",ios::in|ios::binary);
    }
    else if ( BRISK_LENGTH_BITS == 256 ){
        fileRank.open("brisk/ranking_original_optimized256.bin",ios::in|ios::binary);
    }
    fileRank.read((char*)aux_pairs,sizeof(int)*BRISK_LENGTH_BITS);
    fileRank.close();
    for(int i=0; i<BRISK_LENGTH_BITS; i++){
        pairs.push_back(aux_pairs[i]);
    }


    BRISK_descParams descPrms(pairs);
    featExtract->setDescriptor("BRISK", &descPrms);

    // Initialize the decoder
    decoder = VisualFeatureDecoding();

    // Read the database images
    if( getdir( imgDatabasePath , db_obj) != 0){
        cout << "Error in reading image database path!";
    }
    sort( db_obj.begin(), db_obj.end() );

    n_objects = db_obj.size();

    vector<string> db_files;
    vector<string> obj_files;


    for( int i=0; i<n_objects; i++ ){
        obj_files.clear();

        if( getdir( imgDatabasePath + db_obj[i] + "/", obj_files )!=0 ){
            cout << "Error reading object files" << endl;
        }

        sort( obj_files.begin(), obj_files.end() );

        for(unsigned int j=0; j<obj_files.size(); j++){
            db_files.push_back(imgDatabasePath + db_obj[i] + "/" + obj_files[j]);
            db_class.push_back(i); // the class id for the current image
        }

    }

    // Initialize the recognition pipeline
    objRec = new objRecognition(*featExtract,true,n_objects);


    // Keypoint detection and descriptor extraction for each image in the database
    for(int n=0; n<(int)db_files.size(); n++){
        cout << "Adding object " << n << ": ";
        cout << db_files[n] << endl;
        objRec->dCreateAddToDB(db_files[n]);
    }


    // Initialize the inverse BRISK engine
    invBRISK = new inverse_BRISK(featExtract,640,480);
    invBRISK->build_database(imgDatabasePath);

    cout << "BRISK LENGTH (bits) = " << BRISK_LENGTH_BITS << endl;
    cout << "BRISK LENGTH (bytes) = " << BRISK_LENGTH_BYTES << endl;

    PACKET_DEADLINE = 10;

}


void receiverManager::set_gui(ReceiverGUI *gui_)
{
    gui = gui_;
}

// This function activates the main thread
bool receiverManager::start_mainProcess()
{    
    src = dataTx->openSerialRADIO(radioDevice.c_str(),115200,1);
    if(src==NULL){
        cout << "Error opening the radio device " << radioDevice << endl;
        return false;
    }
    main_process_active = true;    
    reqActs.stop_program = false;
    this->start(); // start the main thread
    return true;
}

// This function stops the main thread
bool receiverManager::end_mainProcess()
{
    dataTx->closeSerialRADIO(src);
    main_process_active = false;
    reqActs.stop_program = true;
    return true;
    //this->terminate();    
}


void receiverManager::set_jpeg_qf(int qf)
{
    reqActs.jpeg_qf = qf;
}

void receiverManager::set_det_threshold(int thr)
{
    reqActs.det_threshold = thr;
}

void receiverManager::set_mode(int mode)
{
    switch(mode){
        default:
        case 0:
            reqActs.atc = false;
            reqActs.cta = true;
            break;
        case 1:
            reqActs.atc = true;
            reqActs.cta = false;
            break;
    }
}

void receiverManager::enable_entropy_coding(bool ec)
{
    reqActs.do_compression = ec;
}

void receiverManager::enable_img_reconstruction(bool imRec)
{
    image_reconstruction = imRec;
}

void receiverManager::set_max_features(int maxFeats)
{
    reqActs.max_features = maxFeats;
}

void receiverManager::set_tx_delay(int del)
{
    reqActs.tx_delay = del;
}

void receiverManager::set_match_threshold(int mThr)
{
    match_threshold = mThr;
}

void receiverManager::set_radioDevice(string rd)
{
    radioDevice = rd;
}

// This slot updates the display 1
void receiverManager::slot_update_image1()
{
    gui->update_display1(imgToShow_1);
    gui->update_dataInfo(recObject, framesPerSecond, bandwidth);
}

// This slot updates the display 2
void receiverManager::slot_update_image2()
{
    gui->update_display2(imgToShow_2);
}

// Main thread implementation
void receiverManager::run()
{

    int radio_dst = 1;

    // Content to be shown
    Mat img_to_show;   // the image        
    Mat white_screen = 255 * Mat::ones(std_H,std_W,CV_8UC1);
    Mat black_screen =   0 * Mat::ones(std_H,std_W,CV_8UC3);

    enum app_states {
        IDLE, REC_CTA_IMG, REC_ATC_IMG, IMG_RECEIVED, MISSING_ATC_PKTS, ERR_CTA
    };


    int curState  = IDLE;
    int prevState = IDLE;


    uchar *packet;
    int packetLength;
    int pktType;

    int bytes_received = 0;
    double tx_time = 0;


    imgInfo curImgInfo;
    pktInfo curPktInfo;

    // prepare vectors containing the received bitstreams
    vector< vector<uchar> > kp_vectors;
    vector< vector<uchar> > ft_vectors;
    vector< int >           num_feats;

    // vectors for received encoded bitstreams
    vector<uchar> rec_JPEGbuff;
    vector<uchar> rec_kpts;
    vector<uchar> rec_feats;

    // data structures for decoding bitstreams
    Mat dec_imJPEG;

    // send the START COMMAND
    dataTx->sendCMD_RADIO(src,radio_dst,START,&reqActs);


    bool end_loop = false;
    while(!end_loop){

        bytes_received = 0;

        // Handling the "IDLE" state
        // -----------------------------------------------------------------------
        if(curState == IDLE){

            cout << endl << "Waiting..." << endl;
            // wait for a packet
            bool recPkt = false;
            while (recPkt == false){
                if( dataTx->receivePacketRADIO(src, &packet, &packetLength) == 0 ){
                    pktType = dataTx->parseMessage(packet);
                }
                if( pktType==IMG_INFO_PKT || pktType==REQ_ACK_REC_IMG || pktType == NO_OBJ_PKT){
                    recPkt = true;
                }

            }




            // if I received a IMG_INFO_PKT, read info and change state
            if( pktType == IMG_INFO_PKT ){

                cout << "INFO packet received." << endl;

                // Read the image information
                dataTx->getImgInfo(packet, curImgInfo);

                // Print some information about the image
                if(curImgInfo.atc_cta == true){
                    cout << "Type: ATC" << endl;
                    cout << "Algorithm: " << curImgInfo.descName << endl;
                    cout << "Number of features: " << curImgInfo.nFeats << endl;
                    cout << "Image size = " << curImgInfo.h << " x " << curImgInfo.w << endl;
                    if(curImgInfo.compressed == true){
                        cout << "Compressed stream" << endl;
                    }
                    else{
                        cout << "Non-compressed stream" << endl;
                    }
                }
                else{
                    cout << "Type: CTA" << endl;
                }

                // Change state
                if ( curImgInfo.atc_cta == false ){ // CTA protocol
                    curState = REC_CTA_IMG;
                }
                else{
                    curState = REC_ATC_IMG;
                }

            }

            // if I received a REQ_ACK_REC_IMG, send a ACK request
            else if( pktType == REQ_ACK_REC_IMG ){
                curState = IMG_RECEIVED;
            }

            // if I received a no-obj packet, draw the black screen...
            else if( pktType == NO_OBJ_PKT ){
                imgToShow_1 = black_screen;
                frameCounter = 0; // ...reset the frame counter...
                timeFrame.clear();
                emit sig_update_image1(); // update the image on the display
                curState = IDLE;  // ...and remain in the IDLE state
            }
            prevState = IDLE;

        }


        // Handling the "REC_CTA_IMG" state
        // -----------------------------------------------------------------------
        else if(curState == REC_CTA_IMG){

            // Send the ACK packet
            dataTx->sendACK_RADIO(src,radio_dst,ACK_REC_INFO,curImgInfo.imageID);

            cout << "Receiving JPEG bitstream........" << endl;
            double time_start_tx = (double)getTickCount();
            int err_ret = dataTx->receivePacketRADIO(src, &packet, &packetLength,PACKET_DEADLINE);
            double time_end_tx = (double)getTickCount();
            tx_time = (time_end_tx - time_start_tx)/getTickFrequency();
            cout << "Done" << endl;
            if (err_ret == 0){
                pktType = dataTx->parseMessage(packet);

                cout << "Packet type: " << pktType << endl;

                // if pktType == REQ_ACK_REC_INFO, re-send the ack
                if ( pktType == REQ_ACK_REC_INFO ){
                    dataTx->sendACK_RADIO(src,radio_dst,ACK_REC_INFO,curImgInfo.imageID);
                }
                // if the packet contains data, then read it
                else if ( pktType == CTA_IMG_PKT ){

                    // read the packet info
                    dataTx->getPacketInfo(packet,curPktInfo);
                    cout << "packet length:" << packetLength << endl;
                    if( curPktInfo.imageID == curImgInfo.imageID ){

                        // fill the JPEG buffer with the packet content
                        rec_JPEGbuff.clear();
                        for(int i=0; i<packetLength; i++){
                            rec_JPEGbuff.push_back((uchar)packet[i]);
                        }

                        curState = IMG_RECEIVED; // update the state
                        cout << "ok" << endl;
                    }
                    else{
                        cout << "WRONG IMG. ID!!!" << endl;
                        curState = ERR_CTA; // set the error state
                    }

                }
                else{
                    cout << "WRONG PKT TYPE" << endl;
                    curState = ERR_CTA; // set the error state
                }
            }
            else{
                cout << "err_ret = " << err_ret << endl;
                curState = ERR_CTA;
            }

            prevState = REC_CTA_IMG;


        }


        // Handling the "REC_ATC_IMG" state
        // -----------------------------------------------------------------------
        else if(curState == REC_ATC_IMG){

            // Send the ACK packet
            dataTx->sendACK_RADIO(src,radio_dst,ACK_REC_INFO,curImgInfo.imageID);

            // Clear the vectors
            kp_vectors.clear();
            ft_vectors.clear();
            num_feats.clear();

            int countRecFeats = 0;
            bool errors = false;

            cout << "Receiving visual features.........." << endl;

            double time_start_tx = (double)getTickCount();
            int err_ret;

            err_ret = dataTx->receivePacketRADIO(src, &packet, &packetLength,PACKET_DEADLINE/2);

            if(err_ret == 0){

                pktType = dataTx->parseMessage(packet);

                // NOT VERY SURE...
                /*
                if (ctrl != 0){
                    do{
                        ctrl = dataTx->receivePacketRADIO(src, &packet, &packetLength);
                        pktType = dataTx->parseMessage(packet);
                        if (ctrl == 0 && pktType==REQ_ACK_REC_INFO){
                            dataTx->sendACK_RADIO(src,ACK_REC_INFO,curImgInfo.imageID);
                        }
                    }
                    while(pktType == REQ_ACK_REC_INFO);
                }
                */


                if ( pktType == REQ_ACK_REC_INFO ){ // if pktType == REQ_ACK_REC_INFO, re-send the ack
                    dataTx->sendACK_RADIO(src,radio_dst,ACK_REC_INFO,curImgInfo.imageID);
                }

                else{ // else receive data

                    do
                    { // loop until all packets are received, or some errors occur

                        if( pktType == ATC_IMG_PKT ){

                            dataTx->getPacketInfo(packet,curPktInfo);

                            if( curPktInfo.imageID == curImgInfo.imageID ){

                                // update the counter of received features
                                countRecFeats += curPktInfo.nFeats;

                                // Store the number of features contained in the current packet
                                num_feats.push_back(curPktInfo.nFeats);

                                // Read the keypoints
                                int w_log2 = ceil(log2(curImgInfo.w));
                                int h_log2 = ceil(log2(curImgInfo.h));
                                //int kp_streamLength = (w_log2 + h_log2 + 14)*curPktInfo.nFeats;
                                int kp_streamLength = (w_log2 + h_log2 + 23)*curPktInfo.nFeats;
                                kp_streamLength = ceil ( (float)kp_streamLength / 8.0 );
                                rec_kpts.clear();
                                for(int i=0; i<kp_streamLength; i++){
                                    rec_kpts.push_back(packet[i+1]);
                                }
                                kp_vectors.push_back(rec_kpts);

                                // Read the features
                                rec_feats.clear();
                                for(int i=kp_streamLength; i<packetLength-1; i++){
                                    rec_feats.push_back(packet[i+1]);
                                }
                                ft_vectors.push_back(rec_feats);

                                // Update the state
                                curState = IMG_RECEIVED;
                                cout << "ok" << endl;
                            }
                            else{ // error: imgID mismatch
                                cout << "WRONG IMG. ID!!!" << endl;
                                errors = true;
                            }

                        }
                        else{
                            cout << "WRONG PACKET TYPE!!!" << endl;
                            errors = true; // wrong packet type
                        }

                        // if no errors, read the next packet
                        if (countRecFeats < curImgInfo.nFeats && !errors){
                            err_ret = dataTx->receivePacketRADIO(src, &packet, &packetLength, PACKET_DEADLINE/2);
                            if (err_ret==0){
                                pktType = dataTx->parseMessage(packet);
                            }
                            else{
                                errors = true;
                            }
                        }

                    }
                    while( countRecFeats < curImgInfo.nFeats && !errors );

                }
            }
            else{
                curState = ERR_CTA;
            }

            if(errors){
                curState = ERR_CTA;
            }

            double time_end_tx = (double)getTickCount();
            tx_time = (time_end_tx - time_start_tx)/getTickFrequency();

            prevState = REC_ATC_IMG;

        }


        // Handling the "IMG_RECEIVED" state
        // -----------------------------------------------------------------------
        else if(curState == IMG_RECEIVED){


            // Set the new commands according to the options selected on the gui
            dataTx->sendACK_RADIO(src,radio_dst,ACK_REC_IMG,curImgInfo.imageID,&reqActs);
            end_loop = reqActs.stop_program;


            if( prevState != IDLE ){

                // Here: 1) show the keypoints (if ATC) or the image+kpts (if CTA)
                //       2) perform object recognition
                //       3) display results

                vector<KeyPoint> kpts_to_show; // will contain the keypoints to be shown
                vector<KeyPoint> kpts;         // will contain the keypoints (to perform obj recognition)
                Mat features;          		   // will contain the feature vectors


                if( curImgInfo.atc_cta == false ){

                    // decode the JPEG stream
                    dec_imJPEG = imdecode(rec_JPEGbuff,1);

                    bytes_received = rec_JPEGbuff.size();

                    cout << "Image JPEG received and decoded successfully:" << endl;
                    cout << "ImageID = " << curImgInfo.imageID << endl;
                    cout << "Size = " << dec_imJPEG.rows << " x " << dec_imJPEG.cols << endl;

                    // extract the features
                    Mat dec_imJPEG_gray;
                    cvtColor(dec_imJPEG,dec_imJPEG_gray,CV_BGR2GRAY); //(needed)

                    featExtract->extractFeatures(dec_imJPEG_gray,&kpts,&features);
                    featExtract->cutFeatures(kpts,features,reqActs.max_features);

                    cout << "Number of extracted features: " << kpts.size() << endl;

                    kpts_to_show = kpts;


                    // center the image and the keypoints (for visualization only), if needed
                    int curH = dec_imJPEG.rows;
                    int curW = dec_imJPEG.cols;

                    if (curH<std_H || curW<std_W){

                        img_to_show = black_screen.clone();
                        int min_c = std_W/2 - round(curW/2.0);
                        int min_r = std_H/2 - round(curH/2.0);
                        Mat aux = img_to_show.rowRange(min_r,min_r+curH).colRange(min_c,min_c+curW);
                        dec_imJPEG.copyTo(aux);

                        for(unsigned int i=0; i<kpts.size(); i++){
                            kpts_to_show[i].pt.x += min_c;
                            kpts_to_show[i].pt.y += min_r;
                        }
                    }
                    else{
                        img_to_show = dec_imJPEG.clone();
                    }

                    // draw the keypoints on the image
                    drawKeypoints(img_to_show,kpts_to_show,img_to_show,Scalar(255,0,0),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                }

                else{

                    //decode the keypoints and the features
                    kpts.clear();

                    vector<KeyPoint> cur_dec_kpts;

                    int imW_pix = curImgInfo.w;
                    int imH_pix = curImgInfo.h;

                    for(unsigned int i=0; i<kp_vectors.size(); i++){
                        decoder.decodeKeyPoints(kp_vectors[i], cur_dec_kpts,imW_pix,imH_pix,true);
                        kpts.insert( kpts.end(), cur_dec_kpts.begin(), cur_dec_kpts.end() );
                        bytes_received += kp_vectors[i].size();
                    }
                    cout << "NUMBER OF DECODED KPTS: " << kpts.size() << endl;


                    Mat dec_feats, cur_dec_feats;

                    for(unsigned int i=0; i<ft_vectors.size(); i++){

                        if(curImgInfo.compressed == true){
                            decoder.decodeBinaryDescriptors(curImgInfo.descName, ft_vectors[i], cur_dec_feats, num_feats[i]);
                        }
                        else{
                            decoder.dummy_decodeBinaryDescriptors(curImgInfo.descName,ft_vectors[i], cur_dec_feats);
                        }

                        bytes_received += ft_vectors[i].size();

                        if(i==0){
                            dec_feats = cur_dec_feats.clone();
                        }
                        else{
                            vconcat(dec_feats, cur_dec_feats, dec_feats);
                        }

                    }


                    // center the image and the keypoints, if needed
                    kpts_to_show = kpts;

                    if (imH_pix<std_H || imW_pix<std_W){
                        for(unsigned int i=0; i<kpts.size(); i++){
                            kpts_to_show[i].pt.x += std_W/2 - round(imW_pix/2.0);
                            kpts_to_show[i].pt.y += std_H/2 - round(imH_pix/2.0);
                        }
                    }

                    // prepare the image to be displayed (white img + keypoints)
                    img_to_show = white_screen.clone();                    
                    drawKeypoints(img_to_show,kpts_to_show,img_to_show,Scalar(255,0,0),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                    Scalar sf = sum(dec_feats);
                    double sk = 0;
                    for(unsigned int i=0; i<kpts.size();i++){
                        sk += kpts[i].pt.x + kpts[i].pt.y + kpts[i].size;
                    }

                    cout << "CONTROL VALUES: " << sf[0] << " , " << sk << endl;

                    features = dec_feats.clone();

                    cout << endl << "Visual features received and decoded successfully" << endl;

                }



                // Perform object recognition
                // -----------------------------------------------------------------------------------------------
                vector< vector<float> > distances;
                vector< unsigned int > ranking;

                cout << "OBJECT RECOGNITION....." << endl;
                ranking = objRec->rankedQueryDB(kpts, // keypoints of the query image
                                  features,           // features of the query image
                                  distances,
                                  do_bin_ratio_test,         // enable/disable ratio-test for binary descriptors
                                  124/2,       // distance threshold for nearest neighbour test
                                  DEFAULT_NNDR_RATIO,        // ratio-test threshold
                                  1,                 // enable/disable ransac
                                  DEFAULT_RANSAC_MIN_MATCHES,// minimum number of matches
                                  DEFAULT_RANSAC_THRESHOLD );// maximum allowed reprojection error

                cout << "OK" << endl;

                // sort the ranking list
                vector<size_t> sorted_idxs;
                sorted_idxs.clear();
                sort_like_matlab(ranking,ranking,sorted_idxs,1);

                // group the object of the same class
                vector<int> classScore (n_objects);
                for(unsigned int i=0; i<ranking.size(); i++){
                    classScore[db_class[sorted_idxs[i]]] += ranking[i];
                }

                // obtain the final ranking
                sorted_idxs.clear();
                sort_like_matlab( classScore, classScore, sorted_idxs, 1);

                if( classScore[0]>10 ){
                    recObject = db_obj[sorted_idxs[0]];
                    cout << "OBJECT: " << db_obj[sorted_idxs[0]] << endl << endl;
                    for(unsigned int i=0; i<classScore.size(); i++){
                        cout << "CLASS " << sorted_idxs[i] << " , rank = " << classScore[i] << endl;
                    }
                }
                else{
                    recObject = "n.r.";
                }


                // Compute the frame rate
                framesPerSecond = -1;

                frameCounter++;
                if(frameCounter == 1){
                    time_start = (double)getTickCount();
                }
                if(frameCounter > 1){
                    double time_now = (double)getTickCount();
                    double curTimeFrame = (time_now - time_start)/getTickFrequency();
                    time_start = time_now;
                    timeFrame.push_back(curTimeFrame);

                    if(timeFrame.size()>maxFramesFPS){
                        timeFrame.erase (timeFrame.begin());
                    }

                    // average
                    double meanTimeFrame = 0;
                    for(unsigned int i=0; i<timeFrame.size(); i++){
                        meanTimeFrame += timeFrame[i];
                    }
                    meanTimeFrame = meanTimeFrame / double(timeFrame.size());

                    framesPerSecond = (float) (1.0 / meanTimeFrame);
                    bandwidth = ((float)bytes_received*8.0/1000.0)/tx_time;
                }


                // Draw the image on the display
                imgToShow_1 = img_to_show;
                emit sig_update_image1();

                // If enabled, reconstruct the image from the descriptors
                if(image_reconstruction==true & curImgInfo.atc_cta == true){
                    cout << "inverting BRISK..." << endl;
                    imgToShow_2 = invBRISK->invert_BRISK(features,kpts_to_show,match_threshold);
                    cout << "OK!" << endl;
                    emit sig_update_image2();
                }
            }



            prevState = IMG_RECEIVED;
            curState  = IDLE;

        }

        // Handling the "MISSING_ATC_PKTS" state
        // -----------------------------------------------------------------------
        else if(curState == MISSING_ATC_PKTS){

            prevState = MISSING_ATC_PKTS;
            curState = IDLE;

        }

        // Handling the "ERR_CTA" state
        // -----------------------------------------------------------------------
        else if(curState == ERR_CTA){

            cout << "Error receiving packets!" << endl;
            //dataTx->sendACK_RADIO(src,radio_dst,ACK_REC_IMG,curImgInfo.imageID,&reqActs);
            prevState = ERR_CTA;
            curState = IDLE;

        }


    }

    // send some extra ACK
    dataTx->sendACK_RADIO(src,radio_dst,ACK_REC_IMG,curImgInfo.imageID,&reqActs);
    dataTx->sendACK_RADIO(src,radio_dst,ACK_REC_IMG,curImgInfo.imageID,&reqActs);
    dataTx->sendACK_RADIO(src,radio_dst,ACK_REC_IMG,curImgInfo.imageID,&reqActs);
}

