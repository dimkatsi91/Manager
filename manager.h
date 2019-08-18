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
    // List / grep real human system users
    void cat_users();
    // List / grep all real system groups
    void cat_groups();
    // Exit the application function
    void exit_app();
    /*
     * Group Management related functions
     */
    // Create a new group
    bool groupadd();
    // Rename an existing group
    bool groupmod();
    // Remove an existing group
    bool groupdel();
    // Check if a group exists
    bool group_exists();

    QString getGroupname() const;
    void setGroupname(const QString &value);

    QString getNew_groupname() const;
    void setNew_groupname(const QString &value);

    QString getGid() const;
    void setGid(const QString &value);

private slots:
    void on_show_password_checkBox_clicked(bool checked);

    void on_submit_button_clicked();

    void on_human_users_checkBox_clicked(bool checked);

    void on_human_groups_checkBox_clicked(bool checked);

    void on_clear_credentials_checkBox_clicked(bool checked);

    void on_clear_user_fields_checkBox_clicked(bool checked);

    void on_clear_group_fields_checkBox_clicked(bool checked);

    void on_exit_manager_checkBox_clicked(bool checked);

    void on_actionExit_Manager_triggered();

    void on_actionAbout_Author_triggered();

    void on_actionAbout_Manager_triggered();

    void on_confirm_group_stuff_checkBox_clicked(bool checked);

    void on_create_group_button_clicked();

    void on_remove_group_button_clicked();

    void on_rename_group_button_clicked();

    void on_show_new_user_password_checkBox_clicked(bool checked);

private:
    Ui::Manager *ui;
    // username & password
    QString username;
    QString password;
    // A bool that is set to true only if username & password
    // provided by the user are the correct ones of course
    bool submit_validation = false;
    // Group Management related string variables
    QString groupname;
    QString new_groupname;
    QString gid;    // The GID (aka Group ID)
};

#endif // MANAGER_H
