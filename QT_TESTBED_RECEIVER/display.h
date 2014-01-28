#ifndef DISPLAY_H
#define DISPLAY_H

#include <QDialog>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace Ui {
class display;
}

class display : public QDialog
{
    Q_OBJECT
    
public:
    explicit display(QWidget *parent = 0);
    void update_image(cv::Mat img);
    ~display();
    
private:
    Ui::display *ui;
};

#endif // DISPLAY_H
