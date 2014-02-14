/********************************************************************************
** Form generated from reading UI file 'receivergui.ui'
**
** Created: Fri Feb 14 13:42:50 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECEIVERGUI_H
#define UI_RECEIVERGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReceiverGUI
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QSlider *sl_qf;
    QLabel *label;
    QRadioButton *radio_CTA;
    QLabel *label_3;
    QLabel *label_4;
    QGroupBox *groupBox_2;
    QRadioButton *radio_ATC;
    QCheckBox *ck_coding;
    QCheckBox *ck_showRec;
    QLabel *label_2;
    QSlider *sl_thr;
    QLabel *label_5;
    QLabel *label_6;
    QSlider *sl_maxFeats;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *lab_matchThr;
    QSlider *sl_matchThr;
    QLabel *lab_matchThr0;
    QLabel *lab_matchThr255;
    QPushButton *bt_start;
    QGroupBox *groupBox_3;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QPlainTextEdit *tx_radioDev;
    QPlainTextEdit *tx_delay;
    QGroupBox *groupBox_4;
    QLabel *label_13;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *lab_bw;
    QLabel *lab_recObj;
    QLabel *lab_fps;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *ReceiverGUI)
    {
        if (ReceiverGUI->objectName().isEmpty())
            ReceiverGUI->setObjectName(QString::fromUtf8("ReceiverGUI"));
        ReceiverGUI->resize(539, 507);
        centralWidget = new QWidget(ReceiverGUI);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setEnabled(true);
        groupBox->setGeometry(QRect(10, 1, 311, 81));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"	border: 2px solid gray;\n"
"}\n"
"QGroupBox::title{\n"
"	background-color: transparent;\n"
"    subcontrol-position: top left;\n"
"    padding: 2 5px;\n"
"}"));
        sl_qf = new QSlider(groupBox);
        sl_qf->setObjectName(QString::fromUtf8("sl_qf"));
        sl_qf->setGeometry(QRect(113, 34, 160, 29));
        sl_qf->setAutoFillBackground(true);
        sl_qf->setMaximum(100);
        sl_qf->setSingleStep(5);
        sl_qf->setValue(30);
        sl_qf->setSliderPosition(30);
        sl_qf->setOrientation(Qt::Horizontal);
        sl_qf->setTickPosition(QSlider::TicksBelow);
        sl_qf->setTickInterval(10);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 40, 66, 17));
        radio_CTA = new QRadioButton(groupBox);
        radio_CTA->setObjectName(QString::fromUtf8("radio_CTA"));
        radio_CTA->setGeometry(QRect(10, 10, 116, 22));
        QFont font;
        font.setBold(true);
        font.setItalic(true);
        font.setUnderline(true);
        font.setWeight(75);
        radio_CTA->setFont(font);
        radio_CTA->setChecked(true);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(107, 55, 16, 17));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(8);
        font1.setItalic(true);
        label_3->setFont(font1);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(271, 55, 66, 17));
        label_4->setFont(font1);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setEnabled(true);
        groupBox_2->setGeometry(QRect(10, 86, 311, 231));
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"	border: 2px solid gray;\n"
"}\n"
"QGroupBox::title{\n"
"	background-color: transparent;\n"
"    subcontrol-position: top left;\n"
"    padding: 2 5px;\n"
"}"));
        radio_ATC = new QRadioButton(groupBox_2);
        radio_ATC->setObjectName(QString::fromUtf8("radio_ATC"));
        radio_ATC->setGeometry(QRect(10, 10, 116, 22));
        radio_ATC->setFont(font);
        ck_coding = new QCheckBox(groupBox_2);
        ck_coding->setObjectName(QString::fromUtf8("ck_coding"));
        ck_coding->setGeometry(QRect(20, 122, 161, 22));
        ck_showRec = new QCheckBox(groupBox_2);
        ck_showRec->setObjectName(QString::fromUtf8("ck_showRec"));
        ck_showRec->setGeometry(QRect(20, 152, 171, 22));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(23, 46, 61, 16));
        sl_thr = new QSlider(groupBox_2);
        sl_thr->setObjectName(QString::fromUtf8("sl_thr"));
        sl_thr->setGeometry(QRect(101, 39, 160, 29));
        sl_thr->setAutoFillBackground(true);
        sl_thr->setMinimum(30);
        sl_thr->setMaximum(120);
        sl_thr->setSingleStep(5);
        sl_thr->setValue(60);
        sl_thr->setSliderPosition(60);
        sl_thr->setOrientation(Qt::Horizontal);
        sl_thr->setTickPosition(QSlider::TicksBelow);
        sl_thr->setTickInterval(10);
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(91, 60, 16, 17));
        label_5->setFont(font1);
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(259, 60, 66, 17));
        label_6->setFont(font1);
        sl_maxFeats = new QSlider(groupBox_2);
        sl_maxFeats->setObjectName(QString::fromUtf8("sl_maxFeats"));
        sl_maxFeats->setGeometry(QRect(101, 78, 160, 29));
        sl_maxFeats->setAutoFillBackground(true);
        sl_maxFeats->setMinimum(25);
        sl_maxFeats->setMaximum(500);
        sl_maxFeats->setSingleStep(50);
        sl_maxFeats->setValue(50);
        sl_maxFeats->setSliderPosition(50);
        sl_maxFeats->setOrientation(Qt::Horizontal);
        sl_maxFeats->setTickPosition(QSlider::TicksBelow);
        sl_maxFeats->setTickInterval(50);
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(90, 99, 16, 17));
        label_14->setFont(font1);
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(258, 99, 66, 17));
        label_15->setFont(font1);
        label_16 = new QLabel(groupBox_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(23, 85, 71, 16));
        lab_matchThr = new QLabel(groupBox_2);
        lab_matchThr->setObjectName(QString::fromUtf8("lab_matchThr"));
        lab_matchThr->setGeometry(QRect(51, 186, 71, 16));
        sl_matchThr = new QSlider(groupBox_2);
        sl_matchThr->setObjectName(QString::fromUtf8("sl_matchThr"));
        sl_matchThr->setGeometry(QRect(129, 179, 160, 29));
        sl_matchThr->setAutoFillBackground(true);
        sl_matchThr->setMinimum(0);
        sl_matchThr->setMaximum(255);
        sl_matchThr->setSingleStep(1);
        sl_matchThr->setValue(200);
        sl_matchThr->setSliderPosition(200);
        sl_matchThr->setOrientation(Qt::Horizontal);
        sl_matchThr->setTickPosition(QSlider::TicksBelow);
        sl_matchThr->setTickInterval(20);
        lab_matchThr0 = new QLabel(groupBox_2);
        lab_matchThr0->setObjectName(QString::fromUtf8("lab_matchThr0"));
        lab_matchThr0->setGeometry(QRect(126, 200, 16, 17));
        lab_matchThr0->setFont(font1);
        lab_matchThr255 = new QLabel(groupBox_2);
        lab_matchThr255->setObjectName(QString::fromUtf8("lab_matchThr255"));
        lab_matchThr255->setGeometry(QRect(284, 200, 66, 17));
        lab_matchThr255->setFont(font1);
        bt_start = new QPushButton(centralWidget);
        bt_start->setObjectName(QString::fromUtf8("bt_start"));
        bt_start->setGeometry(QRect(330, 0, 201, 191));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setEnabled(true);
        groupBox_3->setGeometry(QRect(330, 196, 201, 121));
        groupBox_3->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"	border: 2px solid gray;\n"
"}\n"
"QGroupBox::title{\n"
"	background-color: transparent;\n"
"    subcontrol-position: top left;\n"
"    padding: 2 5px;\n"
"}"));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 10, 181, 21));
        label_10->setFont(font);
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(140, 50, 51, 17));
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(140, 83, 51, 17));
        tx_radioDev = new QPlainTextEdit(groupBox_3);
        tx_radioDev->setObjectName(QString::fromUtf8("tx_radioDev"));
        tx_radioDev->setGeometry(QRect(10, 40, 121, 31));
        tx_delay = new QPlainTextEdit(groupBox_3);
        tx_delay->setObjectName(QString::fromUtf8("tx_delay"));
        tx_delay->setGeometry(QRect(70, 75, 61, 31));
        tx_delay->setLayoutDirection(Qt::LeftToRight);
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setEnabled(true);
        groupBox_4->setGeometry(QRect(10, 322, 521, 161));
        groupBox_4->setAutoFillBackground(false);
        groupBox_4->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"	border: 2px solid gray;\n"
"}\n"
"QGroupBox::title{\n"
"	background-color: transparent;\n"
"    subcontrol-position: top left;\n"
"    padding: 2 5px;\n"
"}"));
        label_13 = new QLabel(groupBox_4);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 3, 181, 21));
        label_13->setFont(font);
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(40, 30, 161, 41));
        QFont font2;
        font2.setPointSize(18);
        label_7->setFont(font2);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(40, 70, 161, 41));
        label_8->setFont(font2);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(40, 110, 161, 41));
        label_9->setFont(font2);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lab_bw = new QLabel(groupBox_4);
        lab_bw->setObjectName(QString::fromUtf8("lab_bw"));
        lab_bw->setGeometry(QRect(240, 110, 261, 41));
        lab_bw->setFont(font2);
        lab_recObj = new QLabel(groupBox_4);
        lab_recObj->setObjectName(QString::fromUtf8("lab_recObj"));
        lab_recObj->setGeometry(QRect(240, 30, 261, 41));
        lab_recObj->setFont(font2);
        lab_fps = new QLabel(groupBox_4);
        lab_fps->setObjectName(QString::fromUtf8("lab_fps"));
        lab_fps->setGeometry(QRect(240, 70, 261, 41));
        lab_fps->setFont(font2);
        ReceiverGUI->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(ReceiverGUI);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ReceiverGUI->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(ReceiverGUI);

        QMetaObject::connectSlotsByName(ReceiverGUI);
    } // setupUi

    void retranslateUi(QMainWindow *ReceiverGUI)
    {
        ReceiverGUI->setWindowTitle(QApplication::translate("ReceiverGUI", "ReceiverGUI", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        label->setText(QApplication::translate("ReceiverGUI", "JPEG q.f.", 0, QApplication::UnicodeUTF8));
        radio_CTA->setText(QApplication::translate("ReceiverGUI", "Enable CTA", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ReceiverGUI", "0", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ReceiverGUI", "100", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        radio_ATC->setText(QApplication::translate("ReceiverGUI", "Enable ATC", 0, QApplication::UnicodeUTF8));
        ck_coding->setText(QApplication::translate("ReceiverGUI", "Entropy coding", 0, QApplication::UnicodeUTF8));
        ck_showRec->setText(QApplication::translate("ReceiverGUI", "Show reconstruction", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ReceiverGUI", "det. thr.", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ReceiverGUI", "30", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ReceiverGUI", "120", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("ReceiverGUI", "25", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("ReceiverGUI", "500", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("ReceiverGUI", "max feats.", 0, QApplication::UnicodeUTF8));
        lab_matchThr->setText(QApplication::translate("ReceiverGUI", "match thr.", 0, QApplication::UnicodeUTF8));
        lab_matchThr0->setText(QApplication::translate("ReceiverGUI", "0", 0, QApplication::UnicodeUTF8));
        lab_matchThr255->setText(QApplication::translate("ReceiverGUI", "255", 0, QApplication::UnicodeUTF8));
        bt_start->setText(QApplication::translate("ReceiverGUI", "START", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QString());
        label_10->setText(QApplication::translate("ReceiverGUI", "Transmission parameters", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("ReceiverGUI", "Device", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("ReceiverGUI", "Delay", 0, QApplication::UnicodeUTF8));
        tx_radioDev->setPlainText(QApplication::translate("ReceiverGUI", "/dev/ttyUSB0\n"
"", 0, QApplication::UnicodeUTF8));
        tx_delay->setPlainText(QApplication::translate("ReceiverGUI", "10000", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QString());
        label_13->setText(QApplication::translate("ReceiverGUI", "Received data", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ReceiverGUI", "OBJECT:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("ReceiverGUI", "FRAME RATE:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("ReceiverGUI", "BANDWIDTH:", 0, QApplication::UnicodeUTF8));
        lab_bw->setText(QApplication::translate("ReceiverGUI", "bw", 0, QApplication::UnicodeUTF8));
        lab_recObj->setText(QApplication::translate("ReceiverGUI", "obj", 0, QApplication::UnicodeUTF8));
        lab_fps->setText(QApplication::translate("ReceiverGUI", "fps", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ReceiverGUI: public Ui_ReceiverGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECEIVERGUI_H
