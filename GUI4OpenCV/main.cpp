#include "GUI4OpenCV.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUI4OpenCV w;
    w.show();
    return a.exec();
}
