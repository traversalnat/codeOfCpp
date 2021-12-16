#include "servicemonitor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServiceMonitor w;
    w.show();
    return a.exec();
}
