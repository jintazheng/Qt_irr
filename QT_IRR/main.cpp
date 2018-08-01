
#include <QtWidgets/QApplication>

#include "QT_IRR.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QT_IRR w;
    w.show();
    return a.exec();
}
