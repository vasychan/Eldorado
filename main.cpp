
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //init thread loop


    a.exec();

    //close thread loop
    //playlist_loop.quit();
    //playlist_loop.wait();
    return 0;
}
