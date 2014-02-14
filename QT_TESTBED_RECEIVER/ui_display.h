/********************************************************************************
** Form generated from reading UI file 'display.ui'
**
** Created: Fri Feb 14 13:33:48 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_display
{
public:
    QLabel *img;

    void setupUi(QDialog *display)
    {
        if (display->objectName().isEmpty())
            display->setObjectName(QString::fromUtf8("display"));
        display->resize(640, 480);
        img = new QLabel(display);
        img->setObjectName(QString::fromUtf8("img"));
        img->setGeometry(QRect(0, 0, 640, 480));

        retranslateUi(display);

        QMetaObject::connectSlotsByName(display);
    } // setupUi

    void retranslateUi(QDialog *display)
    {
        display->setWindowTitle(QApplication::translate("display", "Dialog", 0, QApplication::UnicodeUTF8));
        img->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class display: public Ui_display {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAY_H
