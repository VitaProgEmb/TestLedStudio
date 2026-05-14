#include "mainwindow.h"

#include <QApplication>
#include <QQuickWindow>



int main(int argc, char *argv[])
{

    QQuickWindow::setGraphicsApi(QSGRendererInterface::Software);
    QApplication a(argc, argv);
    a.setStyle("fusion");
    MainWindow w;
    w.show();
    return QApplication::exec();
}
