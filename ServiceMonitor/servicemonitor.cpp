#include "servicemonitor.h"
#include "ui_servicemonitor.h"

ServiceMonitor::ServiceMonitor(QWidget *parent)
    : QWidget(parent), ui(new Ui::ServiceMonitor) {
    ui->setupUi(this);
    Sender *s{new Sender{}};
    connect(ui->pushButton_3, &QPushButton::clicked, s, &Sender::trigger);
    hlp::connect(s, &Sender::signal, this, &ServiceMonitor::slot);
}

ServiceMonitor::~ServiceMonitor() { delete ui; }

void ServiceMonitor::slot(QString str) { ui->lineEdit->setText(str); }
