/********************************************************************************
** Form generated from reading UI file 'picture.ui'
**
** Created: Wed Jan 15 15:36:20 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PICTURE_H
#define UI_PICTURE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_picture
{
public:
    QLabel *img;

    void setupUi(QDialog *picture)
    {
        if (picture->objectName().isEmpty())
            picture->setObjectName(QString::fromUtf8("picture"));
        picture->resize(640, 480);
        img = new QLabel(picture);
        img->setObjectName(QString::fromUtf8("img"));
        img->setGeometry(QRect(0, 0, 640, 480));
        img->setAutoFillBackground(true);

        retranslateUi(picture);

        QMetaObject::connectSlotsByName(picture);
    } // setupUi

    void retranslateUi(QDialog *picture)
    {
        picture->setWindowTitle(QApplication::translate("picture", "Dialog", 0, QApplication::UnicodeUTF8));
        img->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class picture: public Ui_picture {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PICTURE_H
