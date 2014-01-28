#ifndef RECEIVERGUI_H
#define RECEIVERGUI_H

#include <QMainWindow>
//#include <string>
#include "display.h"

class receiverManager;

namespace Ui {
class ReceiverGUI;
}

class ReceiverGUI : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ReceiverGUI(QWidget *parent = 0);
    void set_receiverManager(receiverManager*);

    void update_display1(cv::Mat img);
    void update_display2(cv::Mat img);
    void update_dataInfo(std::string objRec, float fps, float bw);

    ~ReceiverGUI();

private slots:

    void on_radio_CTA_toggled(bool checked);
    void on_radio_ATC_toggled(bool checked);
    void on_bt_start_clicked();
    void on_sl_qf_valueChanged(int value);
    void on_sl_thr_valueChanged(int value);
    void on_ck_coding_toggled(bool checked);
    void on_ck_showRec_toggled(bool checked);
    void on_sl_maxFeats_valueChanged(int value);
    void on_sl_matchThr_valueChanged(int value);
    void on_tx_radioDev_textChanged();
    void on_tx_delay_textChanged();

private:
    Ui::ReceiverGUI *ui;
    receiverManager *mng;
    display *disp1, *disp2;
    bool is_started;
};

#endif // RECEIVERGUI_H
