#include "receivergui.h"
#include "ui_receivergui.h"
#include "display.h"
#include "ui_display.h"
#include "receivermanager.h"

ReceiverGUI::ReceiverGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReceiverGUI)
{
    ui->setupUi(this);
    disp1 = new display;
    disp2 = new display;    
    ui->radio_CTA->setChecked(true);
    ui->radio_ATC->setChecked(false);
    ui->ck_coding->setEnabled(false);
    ui->ck_showRec->setEnabled(false);
    ui->sl_thr->setEnabled(false);
    ui->sl_maxFeats->setEnabled(false);
    ui->sl_matchThr->setEnabled(false);
    ui->sl_qf->setEnabled(true);
    ui->sl_matchThr->setVisible(false);
    ui->lab_matchThr->setVisible(false);
    ui->lab_matchThr0->setVisible(false);
    ui->lab_matchThr255->setVisible(false);
    is_started = false;
    ui->lab_recObj->setText("---");
    ui->lab_fps->setText("---");
    ui->lab_bw->setText("---");
}

ReceiverGUI::~ReceiverGUI()
{
    delete ui;
}

void ReceiverGUI::set_receiverManager(receiverManager *mng_)
{
    mng = mng_;        
    mng->set_gui(this);
}


void ReceiverGUI::update_display1(Mat img)
{
    disp1->update_image(img);
}

void ReceiverGUI::update_display2(Mat img)
{
    disp2->update_image(img);
}

void ReceiverGUI::update_dataInfo(string recObj, float fps, float bw)
{
    ui->lab_recObj->setText(QString::fromStdString(recObj));
    stringstream ss (stringstream::in | stringstream::out);
    ss << (float)((int)(fps*100.0))/100.0;
    ui->lab_fps->setText(QString::fromStdString(ss.str() + " fps"));

    stringstream ss2 (stringstream::in | stringstream::out);
    ss2 << (float)((int)(bw*10.0))/10.0;
    ui->lab_bw->setText(QString::fromStdString(ss2.str() + " kbps"));
}

void ReceiverGUI::on_radio_CTA_toggled(bool toggled)
{
    if(toggled){
        ui->radio_ATC->setChecked(false);
        mng->set_mode(0);        
        disp2->hide();

        ui->ck_coding->setEnabled(false);
        ui->ck_showRec->setEnabled(false);
        ui->sl_thr->setEnabled(false);
        ui->sl_maxFeats->setEnabled(false);
        ui->sl_matchThr->setEnabled(false);
        ui->sl_qf->setEnabled(true);
    }
}

void ReceiverGUI::on_radio_ATC_toggled(bool toggled)
{
    if(toggled){
        ui->radio_CTA->setChecked(false);
        mng->set_mode(1);
        if( is_started & ui->ck_showRec->isChecked()){
            disp2->show();
        }

        ui->ck_coding->setEnabled(true);
        ui->ck_showRec->setEnabled(true);
        ui->sl_thr->setEnabled(true);
        ui->sl_maxFeats->setEnabled(true);
        ui->sl_matchThr->setEnabled(true);
        ui->sl_qf->setEnabled(false);
    }
}


void ReceiverGUI::on_bt_start_clicked()
{

    if(!is_started){
        if(mng->start_mainProcess()==true){
            ui->bt_start->setText("STOP");
            disp1->show();
            if(ui->ck_showRec->isChecked() & ui->radio_ATC->isChecked()){
                disp2->show();
            }
            is_started = true;
        }
    }
    else{
        mng->end_mainProcess();
        ui->bt_start->setText("START");
        disp1->hide();
        disp2->hide();
        is_started = false;
    }

}


// Changing the JPEG quality factor
void ReceiverGUI::on_sl_qf_valueChanged(int value)
{
    mng->set_jpeg_qf(value);
}

// Changing the detection threshold
void ReceiverGUI::on_sl_thr_valueChanged(int value)
{
    mng->set_det_threshold(value);
}

// Enabling/disabling entropy coding
void ReceiverGUI::on_ck_coding_toggled(bool checked)
{
    mng->enable_entropy_coding(checked);
}

// Enabling/disabling image reconstruction from the descriptors
void ReceiverGUI::on_ck_showRec_toggled(bool checked)
{
    mng->enable_img_reconstruction(checked);
    if(checked & ui->radio_ATC->isChecked()){
        ui->sl_matchThr->setVisible(true);
        ui->lab_matchThr->setVisible(true);
        ui->lab_matchThr0->setVisible(true);
        ui->lab_matchThr255->setVisible(true);
        if( is_started ){
            disp2->show();
        }
    }
    else{
        ui->sl_matchThr->setVisible(false);
        ui->lab_matchThr->setVisible(false);
        ui->lab_matchThr0->setVisible(false);
        ui->lab_matchThr255->setVisible(false);
        disp2->hide();
    }
}

void ReceiverGUI::on_sl_maxFeats_valueChanged(int value)
{
    mng->set_max_features(value);
}

void ReceiverGUI::on_sl_matchThr_valueChanged(int value)
{
    mng->set_match_threshold(value);
}

void ReceiverGUI::on_tx_radioDev_textChanged()
{
    QString qstr = ui->tx_radioDev->toPlainText();     
    mng->set_radioDevice(qstr.toStdString());
}

void ReceiverGUI::on_tx_delay_textChanged()
{
    QString qstr = ui->tx_delay->toPlainText();
    mng->set_tx_delay(qstr.toInt());
}
