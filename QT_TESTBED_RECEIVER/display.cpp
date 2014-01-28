#include "display.h"
#include "ui_display.h"

#include <iostream>
display::display(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::display)
{
    ui->setupUi(this);    
}

void display::update_image(cv::Mat img)
{
    if(img.empty()) return;
    cv::Mat convertedImg;
    cvtColor(img,convertedImg,CV_BGR2RGB);
    QImage qtestimg((uchar*)convertedImg.data, convertedImg.cols, convertedImg.rows, convertedImg.step, QImage::Format_RGB888);
    ui->img->setPixmap(QPixmap::fromImage(qtestimg));
    return;
}

display::~display()
{
    delete ui;
}
