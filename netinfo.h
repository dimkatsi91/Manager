#ifndef NETINFO_H
#define NETINFO_H

#include <QDialog>

namespace Ui {
class NetInfo;
}

class NetInfo : public QDialog
{
    Q_OBJECT

public:
    explicit NetInfo(QWidget *parent = nullptr);
    ~NetInfo();

    // My text fetcher function
    void catchText(QString &text);

private slots:
    void on_exitNetInfoDialog_button_clicked();

private:
    Ui::NetInfo *ui;
};

#endif // NETINFO_H
