#include "netinfo.h"
#include "ui_netinfo.h"

NetInfo::NetInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetInfo)
{
    ui->setupUi(this);
}

NetInfo::~NetInfo()
{
    delete ui;
}

void NetInfo::catchText(QString &text)
{
    ui->networkInfo_textEdit->setText(text);
}

void NetInfo::on_exitNetInfoDialog_button_clicked()
{
    accept();
}
