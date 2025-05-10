#include "carrentalmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CarRentalMainWindow w;
    w.show();
    return a.exec();
}
