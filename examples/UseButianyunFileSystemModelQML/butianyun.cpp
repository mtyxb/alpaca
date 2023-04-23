/**********************************************************************************
****************** Butianyun QT Video Lesson V2 ***********************************
*********** BUTIANYUN， QT Programming Trainning Professional **********************
***********************************************************************************/

#include "butianyunfilesystemmodel.h"
#include <QApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QQmlApplicationEngine engine;
    ButianyunFileSystemModel* fsmodel = new ButianyunFileSystemModel(&a);
    fsmodel->setRootPath(R"(d:\butianyun)");
    engine.rootContext()->setContextProperty("fsmodel", fsmodel);

     const QUrl url(u"qrc:/UseButianyunFileSystemModelQML/butianyun.qml"_qs);
     QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                      &a, [url](QObject *obj, const QUrl &objUrl) {
         if (!obj && url == objUrl)
             QCoreApplication::exit(-1);
     }, Qt::QueuedConnection);
     engine.load(url);

    return a.exec();
}
