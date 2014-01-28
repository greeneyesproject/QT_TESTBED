#ifndef RECEIVERMANAGER_H
#define RECEIVERMANAGER_H

#include <QObject>
#include <QThread>

#include "receivergui.h"

#include "ImageAcquisition.h"
#include "DataTransmission.h"
#include "objRecognition.h"
#include "VisualFeatureDecoding.h"
#include "VisualFeatureExtraction.h"
#include "inverse_BRISK.h"

class receiverManager : public QThread
{
    Q_OBJECT

protected:
    void run(); // the main thread

private:

    DataTransmission        *dataTx;
    objRecognition          *objRec;
    VisualFeatureDecoding    decoder;
    VisualFeatureExtraction *featExtract;
    serial_source            src;

    actions reqActs;

    string radioDevice;

    bool image_reconstruction;

    // object database
    string imgDatabasePath;
    string imgQueryPath;
    int n_objects;
    vector<int> db_class;
    vector<string> db_obj;

    Mat imgToShow_1, imgToShow_2;
    string recObject;
    float framesPerSecond;
    float bandwidth;

    int frameCounter;
    int maxFramesFPS;
    double time_start;
    vector<double> timeFrame;

    int std_W, std_H;

    bool do_bin_ratio_test;
    bool do_ransac;

    ReceiverGUI *gui;
    bool main_process_active;

    inverse_BRISK *invBRISK;
    int match_threshold;

    double PACKET_DEADLINE;

public:

    receiverManager(string imgDatabasePath_);

    void set_gui(ReceiverGUI *gui_);
    bool start_mainProcess();
    bool end_mainProcess();

    void set_mode(int); // 0 -> CTA, 1 -> ATC
    void set_jpeg_qf(int);
    void set_det_threshold(int);
    void enable_entropy_coding(bool);
    void enable_img_reconstruction(bool);
    void set_tx_delay(int);
    void set_max_features(int);
    void set_match_threshold(int);
    void set_radioDevice(string);

public slots:
    void slot_update_image1();
    void slot_update_image2();

signals:
    void sig_update_image1();
    void sig_update_image2();

};

#endif // RECEIVERMANAGER_H
