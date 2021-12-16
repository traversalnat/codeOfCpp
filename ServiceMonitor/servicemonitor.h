#ifndef SERVICEMONITOR_H
#define SERVICEMONITOR_H

#include <QWidget>

#include "../any_object/any.h"
#include "../pointer_to_memfunc_c++11/pointer_to_memfunc.h"

using hlp::any;
using hlp::SRObject;

using hlp::SRObject;

QT_BEGIN_NAMESPACE
namespace Ui {
class ServiceMonitor;
}
QT_END_NAMESPACE

class ServiceMonitor : public QWidget {
    Q_OBJECT

 public:
    ServiceMonitor(QWidget *parent = nullptr);
    ~ServiceMonitor();

    void slot(QString str);

 private:
    Ui::ServiceMonitor *ui;
};

class Sender : public QWidget, public SRObject {
    Q_OBJECT

 public:
    Sender(QWidget *parent = nullptr) : QWidget(parent) {}
    ~Sender() {}

    void signal(QString str) { PUBLIC_SIGNAL(Sender, signal, str); }

    void trigger() { signal("你好, 世界"); }
};

#endif  // SERVICEMONITOR_H
