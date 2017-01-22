#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QPalette* palette = new QPalette();
    //palette->setColor(QPalette::Window,Qt::gray);

    MainWindow w;
    //w.setPalette(*palette);
    w.show();
    
    return a.exec();
}
