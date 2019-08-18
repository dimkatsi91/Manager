#ifndef MANAGER_H
#define MANAGER_H

#include <QMainWindow>
#include <QDebug>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class Manager;
}

class Manager : public QMainWindow
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();
    // Initialize a few things
    void init();

    QString getUsername() const;
    void setUsername(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);
    // COmpare username provided by Qt and the entered one
    bool compare_usernames();
    // Find out if password is the correct one!
    bool compare_passwords();

private slots:
    void on_show_password_checkBox_clicked(bool checked);

    void on_submit_button_clicked();

private:
    Ui::Manager *ui;
    // username & password
    QString username;
    QString password;
    // A bool that is set to true only if username & password
    // provided by the user are the correct ones of course
    bool submit_validation = false;
};

#endif // MANAGER_H
