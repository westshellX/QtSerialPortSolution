#include "fishdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FishDialog w;
    w.show();

    return a.exec();
}
