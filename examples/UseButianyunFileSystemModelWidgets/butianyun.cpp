/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#include "butianyunwidget.h"
#include "butianyunfilesystemmodel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ButianyunWidget w;
    w.resize(800, 600);
    w.showMaximized();
    return a.exec();
}
