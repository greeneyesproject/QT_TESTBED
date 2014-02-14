/*
 * DataTransmission.cpp
 */
#include "DataTransmission.h"


static char *msgs[] = {
  (char*)"unknown_packet_type",
  (char*)"ack_timeout"	,
  (char*)"sync"	,
  (char*)"too_long"	,
  (char*)"too_short"	,
  (char*)"bad_sync"	,
  (char*)"bad_crc"	,
  (char*)"closed"	,
  (char*)"no_memory"	,
  (char*)"unix_error"
};

void stderr_msg(serial_source_msg problem)
{
  fprintf(stderr, "Note: %s\n", msgs[problem]);
}


int DataTransmission::sendPacketUDP(string server_addr, int port, int imgID, int nFeats, vector<uchar> kpts_bitstream, vector<uchar> feats_bitstream, char* rec_stream){

	create_ATC_Packet(imgID, nFeats, kpts_bitstream, feats_bitstream);

	struct sockaddr_in si_other;
	int s, slen=sizeof(si_other);

	if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		cout << "error opening the socket" << endl;
		return -1;
	}


	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);

	if (inet_aton(server_addr.c_str() , &si_other.sin_addr) == 0)
	{
		cout << "inet_aton() failed" << endl;
		return -2;
	}

	//send the message
	if (sendto(s, message, msg_length , 0 , (struct sockaddr *) &si_other, slen)==-1)
	{
		cout << "error sending the message" << endl;
		return -3;
	}


	memset(rec_stream,'\0', msg_length);
	//try to receive some data, this is a blocking call
	recvfrom(s, rec_stream, msg_length, 0, (struct sockaddr *) &si_other, (socklen_t*)&slen);

	close(s);

	return 0;

}



serial_source DataTransmission::openSerialRADIO(const char *serial_device, int baud_rate, int non_blocking){

	packet = NULL;
    dealloc_packet = false;
	serial_source src;
	src = open_serial_source(serial_device, baud_rate, non_blocking, stderr_msg);
    RADIO_packetID = 0xFF;
	if(!src)
		return NULL;
	else
		return src;

}

int DataTransmission::closeSerialRADIO(serial_source src){

    return close_serial_source(src);

}


int DataTransmission::sendImgInfoRADIO(serial_source src, int radio_dst, int imgID, bool atc_cta, bool compressed,
						 	 	         string descName, int nFeats,
						 	 	         int imWidth, int imHeight)
{

	// CREATE THE INFO PACKET
	// message_type: 1111 (first 4 bytes)

	//free(message);

	if (atc_cta == false){ // CTA paradigm

        message = (char*)malloc(4);
        message[0]  = 0x55; // identifier for CTA (MSB = 0)

        // set byte 2, 3, 4: image size
        // 12 bits for imWidth, 12 bits for imHeight
        // max 4095 x 4095
        message[1]  = (uchar)( (imWidth >> 4 ) & 0x00FF );
        message[2]  = (uchar)( (imWidth << 4 ) & 0x00F0 );
        message[2] |= (uchar)( (imHeight >> 8) & 0x000F );
        message[3]  = (uchar)( imHeight & 0x00FF );

        msg_length  = 4;

	}
	else{ // ATC paradigm

        message = (char*)malloc(6);

        // set byte 0: ATC           -> MSB (bit 7) = 1,
		//			   BIN/nonBIN    -> bit 6 = 1/0
		//             comp/non-comp -> bit 5 = 1/0
		//             DescriptorID  -> bits 1,0
		if      ( strcmp(descName.c_str(),"BRISK")==0 ){
            message[0] = 0xC0;
		}
		else if ( strcmp(descName.c_str(),"ORB")==0 ){
            message[0] = 0xC1;
		}
		else if ( strcmp(descName.c_str(),"BRIEF")==0 ){
            message[0] = 0xC2;
		}
		else if ( strcmp(descName.c_str(),"FREAK")==0 ){
            message[0] = 0xC3;
		}
		else if ( strcmp(descName.c_str(),"SIFT")==0 ){
            message[0] = 0x80;
		}
		else if ( strcmp(descName.c_str(),"SURF")==0 ){
            message[0] = 0x81;
		}

		if(compressed == true){
            message[0] |= 0x20;
		}


		// set byte 2, 3, 4: image size
		// 12 bits for imWidth, 12 bits for imHeight
		// max 4095 x 4095
        message[1]  = (uchar)( (imWidth >> 4 ) & 0x00FF );
        message[2]  = (uchar)( (imWidth << 4 ) & 0x00F0 );
        message[2] |= (uchar)( (imHeight >> 8) & 0x000F );
        message[3]  = (uchar)( imHeight & 0x00FF );

		// set bytes 5,6: number of descriptors
        message[5] = (uchar)(nFeats/256);
        message[4] = (uchar)(nFeats - message[3]*256);

        msg_length = 6;
	}


    return sendPacketRADIO(src,radio_dst,IMG_INFO_PKT,imgID);

}


int DataTransmission::sendNoObjPktRADIO(serial_source src, int radio_dst){

    msg_length = 0;
    return sendPacketRADIO(src,radio_dst,NO_OBJ_PKT,0);

}


int DataTransmission::sendFeaturesRADIO(serial_source src, int radio_dst, int imgID, int nFeats, vector<uchar> kpts_bitstream, vector<uchar> feats_bitstream){

	create_ATC_Packet(imgID, nFeats, kpts_bitstream, feats_bitstream);
    return sendPacketRADIO(src,radio_dst,ATC_IMG_PKT,imgID);

}


int DataTransmission::sendJpegRADIO(serial_source src, int radio_dst, int imgID, vector<uchar> jpeg_bitstream){

	create_CTA_Packet(imgID, jpeg_bitstream);
    return sendPacketRADIO(src,radio_dst,CTA_IMG_PKT,imgID);

}


int DataTransmission::sendACK_RADIO(serial_source src, int radio_dst, ackType at, int imgID, actions *acts){


	switch (at){

		case ACK_REC_INFO:

            msg_length = 0;
            return sendPacketRADIO(src,radio_dst,ACK_REC_INFO_PKT,imgID);
			break;

		case ACK_REC_IMG:

			if(acts == NULL){ // if no actions are requested

                msg_length = 0;
                return sendPacketRADIO(src,radio_dst,ACK_REC_IMG_PKT,imgID);
				break;
			}

			else{ // if actions are requested

                msg_length = 4;
				message = (char*)malloc(msg_length);


				if (acts->stop_program == true){
                    message[0] = 0; // if a stop action is requested set all to zero
				}
				else{ // set the action values
                    message[0] = (int)round(acts->jpeg_qf / 5);
                    message[0] |= (int)acts->atc << 7;
                    message[0] |= (int)acts->cta << 6;
                    message[0] |= (int)acts->do_compression << 5;
                    message[1] = acts->det_threshold;
                    message[2] = (int)round(acts->max_features / 5);
                    message[3] = acts->tx_delay;
				}

                return sendPacketRADIO(src,radio_dst,ACK_REC_IMG_PKT_ACT,imgID);
				break;
			}


		default:
			cout << "Unrecognized ACK type!" << endl;
			return -1; break;
	}



}


int DataTransmission::sendCMD_RADIO(serial_source src, int radio_dst, commandType ct, actions *acts ){


	switch (ct){
		case START:

            msg_length = 4;
			message = (char*)malloc(msg_length);

			if( acts != NULL ){
                message[0] = (int)round(acts->jpeg_qf / 5);
                message[0] |= (int)acts->atc << 7;
                message[0] |= (int)acts->cta << 6;
                message[0] |= (int)acts->do_compression << 5;
                message[1] = acts->det_threshold;
                message[2] = (int)round(acts->max_features / 5);
                message[3] = acts->tx_delay;
			}
			else{ // use defaults
                message[0] = (int)round(60 / 5);
                message[0] |= 1 << 7;
                message[0] |= 1 << 6;
                message[0] |= 1 << 5;
                message[1] = 60;
                message[2] = 25;
                message[3] = 10;
			}

            return sendPacketRADIO(src,radio_dst,CMD_PKT,0); // Remark: 0 means start!
			break;

		case STOP:

			msg_length = 1;
			message = (char*)malloc(msg_length);
            return sendPacketRADIO(src,radio_dst,CMD_PKT,1); // Remark: 1 means stop!
            break;

		default:

			cout << "Unrecognized command type!" << endl;
			return -1; break;

	}

}


int DataTransmission::reqACK_RADIO (serial_source src, int radio_dst, ackType at, int imgID){

    msg_length = 0;

	switch (at){

		case ACK_REC_INFO:
            return sendPacketRADIO(src,radio_dst,REQ_ACK_REC_INFO,imgID);
			break;
		case ACK_REC_IMG:
            return sendPacketRADIO(src,radio_dst,REQ_ACK_REC_IMG,imgID);
			break;
		default:
			return -1;
			break;

	}

}



bool DataTransmission::waitACK_RADIO(serial_source src, ackType at, int imgID, double timeout, actions *acts){

	uchar *pkt;
	int pl;
	bool ackOK = false;

    int rec_pkt_type;  // type of received packet
	int rec_pkt_imgId; // imgID of the received packet
	ackType rec_ack;   // received ACK type


	// wait until ack is received or timeout expired
    int uscita = receivePacketRADIO(src, &pkt, &pl, timeout);
	if ( uscita == 0 ){

        rec_pkt_type   = (packet_info >> 4) & 0xF;
        rec_pkt_imgId = packet_info & 0x0F;

		switch (rec_pkt_type){

			case 0x03:
				rec_ack = ACK_REC_INFO;
				break;

			case 0x04:
				rec_ack = ACK_REC_IMG;
				break;

			case 0x05: // also update the actions

				rec_ack = ACK_REC_IMG;

				if(acts != NULL){

                    if( pkt[0]==0 ){
						acts->stop_program = true;
					}
					else{
                        acts->atc            = (bool)( (pkt[0] >> 7) & 0x01 );
                        acts->cta            = (bool)( (pkt[0] >> 6) & 0x01 );
                        acts->do_compression = (bool)( (pkt[0] >> 5) & 0x01 );
                        acts->jpeg_qf        = (int) (  pkt[0]       & 0x1F ) * 5;
                        acts->det_threshold  = (int)( pkt[1]);
                        acts->max_features   = (int)( pkt[2] * 5);
                        acts->tx_delay       = (int)( pkt[3]);
                        acts->stop_program   = false;
					}

				}

				break;

			default:
				rec_ack = UNKNOWN_ACK;
				break;
		}

		if( rec_ack == at && rec_pkt_imgId == imgID ){
			ackOK = true;
		}

	}

	return ackOK;

}

commandType DataTransmission::waitCMD_RADIO(serial_source src, actions *acts, double timeout){

	uchar *pkt;
	int pl;

	int pktType;

	bool cmdPktOK = false;
	// wait until a command packet is received or timeout expired
    if ( receivePacketRADIO(src, &pkt, &pl, timeout) == 0 ){

        pktType = (packet_info >> 4) & 0xF;
		if(pktType == CMD_PKT){
			cmdPktOK = true;
		}
	}

	if( cmdPktOK ){
        if( packet_info == 0x90 ){
            acts->atc            = (bool)( (pkt[0] >> 7) & 0x01 );
            acts->cta            = (bool)( (pkt[0] >> 6) & 0x01 );
            acts->do_compression = (bool)( (pkt[0] >> 5) & 0x01 );
            acts->jpeg_qf        = (int) (  pkt[0]       & 0x1F ) * 5;
            acts->det_threshold  = (int)( pkt[1]);
            acts->max_features   = (int)( pkt[2] * 5);
            acts->tx_delay       = (int)( pkt[3]);
            acts->stop_program   = false;
			return START;
		}
        else if ( packet_info == 0x91 ){
			return STOP;
		}
	}

	// else
	return NO_CMD;

}

int DataTransmission::receiveDataPacketRADIO(serial_source src, uchar **packet_, int *packetLength, double timeout){

    double initTime = (double)cv::getTickCount();
    double waitTime = 0;

    if(dealloc_packet == true){
        free(packet);
        dealloc_packet = false;
    }

    //uchar *temp_packet;

    int curPacketID = -1;
    int numFrames = -1;

    uchar *frame = NULL;

    int tot_header_length = RADIO_HEADER_LENGTH + APP_HEADER_LENGTH;
    int maxFrameLength = MAX_RADIO_PAYLOAD_LENGTH - APP_HEADER_LENGTH;

    int len;
    int idx = 0;

    // Wait until the first frame is received
    bool firstFrame = false;
    bool packetComplete = false;
    bool timeout_expired = false;

    while( !firstFrame && !timeout_expired ){
        int aux_pkt_info;
        frame = (uchar*)read_serial_packet(src, &len);

        if(frame){
            cout << "frame received: " << (int)frame[15] << " " << (int)frame[16] << " " << (int)frame[17] << endl;
            packet_info = frame[17];
            curPacketID = (int)frame[12];
            recv_RADIO_packetID = curPacketID;
            aux_pkt_info = (int) ((packet_info >> 4)&0x0F);
            cout << "packet info: " << aux_pkt_info<< endl;

        }



        if( frame && (int)frame[15] == 0 && (int)frame[16] == 0 && (aux_pkt_info==CTA_IMG_PKT || aux_pkt_info==ATC_IMG_PKT)){
            
            curPacketID = (int)frame[12];
            numFrames   = *(unsigned short*)&frame[13];
            packet = (uchar*)malloc(maxFrameLength*numFrames);
            dealloc_packet = true;
            memcpy( &packet[idx], &frame[tot_header_length], len-tot_header_length );
            idx+= len - tot_header_length;
            firstFrame = true;
            if (numFrames == 1){
                packetComplete = true;
            }
        }
        else if(timeout>0){
            waitTime = ((double)cv::getTickCount() - initTime)/cv::getTickFrequency();
            if(waitTime > timeout){
                timeout_expired = true;
                return -2;
            }
        }

        if(frame && aux_pkt_info!=CTA_IMG_PKT && aux_pkt_info!=ATC_IMG_PKT)
            return -3;

        if (frame){
            free(frame);
        }

    }

    

    // Receive the remaining frames
    int frameCounter = 1;
    int recFrameNum;
    bool errors = false;

    while(!packetComplete && !errors && !timeout_expired){

        frame = (uchar*)read_serial_packet(src, &len);

        if(frame){
            recFrameNum = *(unsigned short*)&frame[15];

            if( (int)frame[12]!= curPacketID ){

                //if the first frame of a new block has arrived
                if( (int)frame[15] == 0 && (int)frame[16] == 0 && ((packet_info >> 4)&0x0F==CTA_IMG_PKT || (packet_info >> 4)&0x0F==ATC_IMG_PKT  )){

                    //deallocate old packet
                    if(dealloc_packet == true){
                        free(packet);
                        dealloc_packet = false;
                    }

                    //restart from the beginning
                    idx = 0;
                    frameCounter = 1;
                    curPacketID = (int)frame[12];
                    recv_RADIO_packetID = curPacketID;
                    numFrames   = *(unsigned short*)&frame[13];
                    packet = (uchar*)malloc(maxFrameLength*numFrames);
                    dealloc_packet = true;
                    memcpy( &packet[idx], &frame[tot_header_length], len-tot_header_length );
                    idx+= len - tot_header_length;



                    if (numFrames == 1){
                        packetComplete = true;
                    }
                }
                else{
                    errors = true;
                }

            }
            else{
                if(recFrameNum != frameCounter)
                    errors = true;
            }


            if ( !errors ){
                memcpy( &packet[idx], &(frame[tot_header_length]), len-tot_header_length );
                idx+= len - tot_header_length;
                frameCounter++;
                if(frameCounter==numFrames){
                    packetComplete = true;
                }
            }
        }
        else if(timeout>0){
            waitTime = ((double)cv::getTickCount() - initTime)/cv::getTickFrequency();
            if(waitTime > timeout){
                timeout_expired = true;
                return -2;
            }
        }

        if (frame){
            free(frame);
        }

    }

    if ( !errors ){
        *packetLength = idx;
        *packet_ = packet;
        //free(temp_packet);
        
        return 0;

    }
    else{
        *packetLength = -1;
        //free(temp_packet);
        return -1;
    }


}


int DataTransmission::receivePacketRADIO(serial_source src, uchar **packet_, int *packetLength, double timeout){

	double initTime = (double)cv::getTickCount();
	double waitTime = 0;

    if(dealloc_packet == true){
        free(packet);
        dealloc_packet = false;
	}

	//uchar *temp_packet;

	int curPacketID = -1;
	int numFrames = -1;

	uchar *frame = NULL;

    int tot_header_length = RADIO_HEADER_LENGTH + APP_HEADER_LENGTH;
    int maxFrameLength = MAX_RADIO_PAYLOAD_LENGTH - APP_HEADER_LENGTH;


    int len;
    int idx = 0;

    // Wait until the first frame is received
    bool firstFrame = false;
    bool packetComplete = false;
    bool timeout_expired = false;

	while( !firstFrame && !timeout_expired ){

		frame = (uchar*)read_serial_packet(src, &len);

        if(frame){
            packet_info = frame[17];
            curPacketID = (int)frame[12];
            recv_RADIO_packetID = curPacketID;
        }

		if( frame && (int)frame[15] == 0 && (int)frame[16] == 0){
			curPacketID = (int)frame[12];
            recv_RADIO_packetID = curPacketID;
			numFrames   = *(unsigned short*)&frame[13];
			packet = (uchar*)malloc(maxFrameLength*numFrames);
            dealloc_packet = true;
            memcpy( &packet[idx], &frame[tot_header_length], len-tot_header_length );
			idx+= len - tot_header_length;
			firstFrame = true;
			if (numFrames == 1){
				packetComplete = true;
			}
		}
		else if(timeout>0){
			waitTime = ((double)cv::getTickCount() - initTime)/cv::getTickFrequency();
			if(waitTime > timeout){
				timeout_expired = true;
				return -2;
			}
		}

		if (frame){
			free(frame);
		}

	}

	// Receive the remaining frames
	int frameCounter = 1;
	int recFrameNum;
	bool errors = false;

	while(!packetComplete && !errors && !timeout_expired){

		frame = (uchar*)read_serial_packet(src, &len);

        if(frame){
            if( (int)frame[12]!= curPacketID ){
                errors = true;
            }

            recFrameNum = *(unsigned short*)&frame[15];
            if( recFrameNum != frameCounter ){
                errors = true;
            }

            if ( !errors ){
                memcpy( &packet[idx], &(frame[tot_header_length]), len-tot_header_length );
                idx+= len - tot_header_length;
                frameCounter++;
                if(frameCounter==numFrames){
                    packetComplete = true;
                }
            }
        }
		else if(timeout>0){
			waitTime = ((double)cv::getTickCount() - initTime)/cv::getTickFrequency();
			if(waitTime > timeout){
				timeout_expired = true;
				return -2;
			}
		}

		if (frame){
			free(frame);
		}

	}

	if ( !errors ){
		*packetLength = idx;
	    *packet_ = packet;
	    //free(temp_packet);
		return 0;
	}
	else{
		*packetLength = -1;
		//free(temp_packet);
		return -1;
	}


}


// Create the complete packet containing keypoints and features
// IMPORTANT: the length of the packet is, in general, longer than the payload
// of the radio packet.
int DataTransmission::create_ATC_Packet(int imgID, int nFeats, vector<uchar> kpts_bitstream, vector<uchar> feats_bitstream){


	int nBytes_kpts  = kpts_bitstream.size();
	int nBytes_feats = feats_bitstream.size();
    int nBytes_tot = 1 + nBytes_kpts + nBytes_feats; // size of the bitstream (in bytes)

	// convert from 'vector' to 'uchar array'
	message = (char*)malloc(nBytes_tot);

    message[0] = nFeats; // max 255 features per packet!!!

    for(int i=1; i<nBytes_tot; i++){
        if (i<nBytes_kpts+1){
            message[i] = kpts_bitstream[i-1];
		}
		else{
            message[i] = feats_bitstream[i-nBytes_kpts-1];
		}
	}

	msg_length = nBytes_tot;

	return 0;

}


int DataTransmission::create_CTA_Packet(int imgID, vector<uchar> jpeg_bitstream){


    int nBytes_tot  = jpeg_bitstream.size(); // size of the bitstream (in bytes)

	// convert from 'vector' to 'uchar array'
	message = (char*)malloc(nBytes_tot);

    for(int i=0; i<nBytes_tot; i++){
        message[i] = jpeg_bitstream[i];
	}

	msg_length = nBytes_tot;
	return 0;

}


// Send the packet (i.e. the byte composing the message) on a per-frame base
int DataTransmission::sendPacketRADIO(serial_source src, int radio_dst, packetType pt, int imgID){

	// update the packet ID
	RADIO_packetID++;

	char serial_packet[MAX_RADIO_PACKET_LENGTH];

	// Header of the serial packet (8 bytes)
	serial_packet[0] = 0x00;
	serial_packet[1] = 0x00;
	serial_packet[2] = 0x00;
	serial_packet[3] = 0x00;
	serial_packet[4] = 0x00;
	serial_packet[5] = MAX_RADIO_PAYLOAD_LENGTH;
	serial_packet[6] = 0x00;
	serial_packet[7] = 0x47;

	//src address
	serial_packet[8] = 0x00;
	serial_packet[9] = 0x00;

	//dest address
	serial_packet[10] = (uchar)(radio_dst/256);
	serial_packet[11] = (uchar)(radio_dst-serial_packet[8]*256);

    int tot_header_length = RADIO_HEADER_LENGTH + APP_HEADER_LENGTH;

	// Packet fragmentation
    int maxFrameLength = MAX_RADIO_PAYLOAD_LENGTH - APP_HEADER_LENGTH;
	int curFrameLength = maxFrameLength;
	int remainingBytes = msg_length;

    int nFrames;
    if(msg_length>0)
         nFrames = (int)ceil( (float)msg_length / (float) maxFrameLength);
    else
         nFrames = 1;

	serial_packet[12]  = RADIO_packetID;

	// use 2 bytes [9,10] for specifying the number of frames
	serial_packet[14] = (uchar)(nFrames/256);
	serial_packet[13]  = (uchar) (nFrames - serial_packet[14]*256);

	int dataIdx = 0;

	for(int i=0; i<nFrames; i++){

		// compute and set the length of the current frame
		if( remainingBytes < maxFrameLength){
			curFrameLength = remainingBytes;
            serial_packet[5] = APP_HEADER_LENGTH + curFrameLength;
		}

		// set the frame number
		serial_packet[16] = (uchar)(i/256);
		serial_packet[15] = (uchar)(i - serial_packet[16]*256);

        // set the packet info (type + imgID)
        serial_packet[17]  = pt << 4;
        serial_packet[17] |= imgID;

		// fill the frame data
		for(int j=0; j<curFrameLength; j++){
			serial_packet[j+tot_header_length] = message[dataIdx];
			dataIdx++;
		}

		// send the current frame
		int ret = 1;
		int max_resend   = 5;
		int count_resend = 0;

		while( ret!=0 && count_resend<max_resend ){

			/*if(count_resend>0){
				cout << "Resending frame" << endl;
			}*/
			ret = write_serial_packet(src, serial_packet, curFrameLength+tot_header_length);

			if (ret !=0 ){
				/*cout << "ERROR SENDING FRAME TO RADIO SUBSYSTEM!" << endl;
				cout << "FRAME: " << i+1 << " , LENGTH: " << curFrameLength << endl;
				cout << "RET " << ret << endl;*/
				count_resend++;
			}
			//usleep(9500);
		}

		//usleep(30000); //9500
		remainingBytes = remainingBytes - curFrameLength;

	}
    if(msg_length>0)
        free(message);

	if(dataIdx!=msg_length)
		return -1;
	else
		return 0;

}

packetType DataTransmission::parseMessage(uchar *packet){

    int pktType = (packet_info >> 4) & 0x0F;
	packetType ret_val;

	switch (pktType){
		case 0x0F:
			ret_val = IMG_INFO_PKT;        break;
		case 0x01:
			ret_val = CTA_IMG_PKT;         break;
		case 0x02:
			ret_val = ATC_IMG_PKT;         break;
		case 0x03:
			ret_val = ACK_REC_INFO_PKT;    break;
		case 0x04:
			ret_val = ACK_REC_IMG_PKT;     break;
		case 0x05:
			ret_val = ACK_REC_IMG_PKT_ACT; break;
		case 0x06:
			ret_val = REQ_ACK_REC_INFO;    break;
		case 0x07:
			ret_val = REQ_ACK_REC_IMG;     break;
		case 0x08:
			ret_val = ERR_PKT;			   break;
		case 0x09:
			ret_val = CMD_PKT;			   break;
		case 0x0A:
			ret_val = NO_OBJ_PKT;          break;
		default:
			ret_val = UNKNOWN_PKT;         break;
	}

	return ret_val;

}


int DataTransmission::getImgInfo(uchar *packet, imgInfo &info){

	// check if the packet is an info packet
    uchar pktType = (packet_info >> 4) & 0x0F;
	if( pktType != 0x0F ){
		return -1;
	}

	// extract data
    info.imageID = packet_info & 0x0F; // get the image ID

	// get the operating paradigm (ATC / CTA)
    if (packet[0] == 0x55){   // CTA
		info.atc_cta = false;
		info.descName = "\n";
		info.nFeats = -1;
        info.w = ((int)(packet[1])) * 16 + (int)( (packet[2]>>4) & 0x0F);
        info.h = ((int)(packet[2] & 0x0F)) * 256 + (int)(packet[3]);
		info.compressed = false;
	}
	else{ // ATC

		info.atc_cta = true;
        bool binary = (bool) ( (packet[0] >> 6) & 0x01 );
        info.compressed = (bool) ( (packet[0] >> 5) & 0x01 );

		if (binary == true){
            switch(packet[0] & 0x03){
				case 0x00:
					info.descName = "BRISK"; break;
				case 0x01:
					info.descName = "ORB"; break;
				case 0x02:
					info.descName = "BRIEF"; break;
				case 0x03:
					info.descName = "FREAK"; break;
				default:
					info.descName = "BRISK"; break;
			}
		}
		else{
            switch(packet[0] & 0x03){
				case 0x01:
					info.descName = "SIFT"; break;
				case 0x02:
					info.descName = "SURF"; break;
				default:
					info.descName = "SIFT"; break;
			}
		}


        info.w = ((int)(packet[1])) * 16 + (int)( (packet[2]>>4) & 0x0F);
        info.h = ((int)(packet[2] & 0x0F)) * 256 + (int)(packet[3]);

        info.nFeats = *(unsigned short*)&packet[4]; // get the number of frames

	}

	return 0;

}


int DataTransmission::getPacketInfo(uchar *packet, pktInfo &info){

   // cout << "Packet INFO" << (int)packet_info << endl;

	// check if the packet is a data packet
    uchar pktType = (packet_info >> 4) & 0x0F;
	if( pktType != 0x01 && pktType != 0x02 )
		return -1;

    if  ( pktType == 0x01 ){ // CTA packet
		info.atc_cta = false;
        info.imageID = packet_info & 0x0F;
		info.nFeats = -1;
	}
	else if ( pktType == 0x02 ){ // ATC packet
		info.atc_cta = true;
        info.imageID = packet_info & 0x0F;
        info.nFeats  = packet[0];
	}

	return 0;

}

uchar DataTransmission::getPacketID(){
    return RADIO_packetID;
}

uchar DataTransmission::getRecvPacketID(){
    return recv_RADIO_packetID;
}

/*
int DataTransmission::getEncodedKeypoints(uchar *packet, int packetLength, imgInfo &info, vector<uchar> &enc_keypoints){
	
	pktInfo curPktInfo;
	DataTransmission::getPacketInfo(packet,curPktInfo);
	
	
	int w_log2 = ceil(log2(info.w));
	int h_log2 = ceil(log2(info.h));
	int kp_streamLength = (w_log2 + h_log2 + 14)*curPktInfo.nFeats;
	kp_streamLength = ceil ( (float)kp_streamLength / 8.0 );
	
	// Read the keypoints
	enc_keypoints.clear();	
	for(int i=0; i<kp_streamLength; i++){
		enc_keypoints.push_back(packet[i+2]);
	}

	return 0;

}

int DataTransmission::getEncodedFeatures(uchar *packet, int packetLength, imgInfo &info, vector<uchar> &enc_features){

	pktInfo curPktInfo;
	DataTransmission::getPacketInfo(packet,curPktInfo);
	
	
	int w_log2 = ceil(log2(info.w));
	int h_log2 = ceil(log2(info.h));
	int kp_streamLength = (w_log2 + h_log2 + 14)*curPktInfo.nFeats;
	kp_streamLength = ceil ( (float)kp_streamLength / 8.0 );

	// Read the features
	enc_features.clear();
	for(int i=kp_streamLength; i<packetLength-2; i++){
		enc_features.push_back(packet[i+2]);
	}

	return 0;

}


int DataTransmission::getEncodedJPEG(uchar *packet, int packetLength, vector<uchar> &enc_image){
	
	enc_image.clear();
	for(int i=1; i<packetLength; i++){
		enc_image.push_back((uchar)packet[i]);
	}

	return 0;
}
*/
