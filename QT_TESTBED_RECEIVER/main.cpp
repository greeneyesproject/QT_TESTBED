#include <QtGui/QApplication>
#include "receivergui.h"
#include "receivermanager.h"


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    ReceiverGUI w;

    receiverManager mng("obj_database/");
    w.set_receiverManager(&mng);
    w.show();
        
    return a.exec();
}
