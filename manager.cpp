#include "manager.h"
#include "ui_manager.h"

Manager::Manager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);
    init();
}

Manager::~Manager()
{
    delete ui;
}


QString Manager::getPassword() const
{
    return password;
}

void Manager::setPassword(const QString &value)
{
    password = value;
}

QString Manager::getUsername() const
{
    return username;
}

void Manager::setUsername(const QString &value)
{
    username = value;
}

// A few things to be initialized correctly
void Manager::init()
{
    setUsername("");
    setPassword("");
    // Username can be visible
    ui->username_lineEdit->setEchoMode(QLineEdit::Normal);
    // Password should not be visible by default
    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->retype_password_lineEdit->setEchoMode(QLineEdit::Password);
    // Also the password for the new user who will be created should not
    // be visible by default
    ui->new_user_password_lineEdit->setEchoMode(QLineEdit::Password);
}

/* ============================================================================================================ */
/*                                      SECTION I : Credentials                                                 */
/* ============================================================================================================ */
// Show or hide the password when user checks or unchecks this box
void Manager::on_show_password_checkBox_clicked(bool checked)
{
    if(checked)
    {
        // If user clicks this checkbox the password is shown
        ui->password_lineEdit->setEchoMode(QLineEdit::Normal);
        ui->retype_password_lineEdit->setEchoMode(QLineEdit::Normal);
    }
    else {
        // If this checkbox is un-checked, please hide the password
        ui->password_lineEdit->setEchoMode(QLineEdit::Password);
        ui->retype_password_lineEdit->setEchoMode(QLineEdit::Password);
    }
}

// Compare the entered username with the username that is returned by the Qt
bool Manager::compare_usernames()
{
    QString current_user = getenv("USER");
    if(getUsername()!=current_user)
    {
        return false;
    }
    return true;
}

// Find out if password is correct
bool Manager::compare_passwords()
{
    if(getPassword() != ui->retype_password_lineEdit->text())
    {
        return false;
    }
    return true;
}

// When the SUBMIT button is pushed ::
void Manager::on_submit_button_clicked()
{
    // Grab the username & password at first
    setUsername(ui->username_lineEdit->text());
    setPassword(ui->password_lineEdit->text());
    // If the username or password is empty prompt to enter them
    if(getUsername().isEmpty() || getPassword().isEmpty() || ui->retype_password_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "WARNING", "Username and password should be both specified!");
        return;
    }
    // Username should be the currently logged user's
    if(!compare_usernames())
    {
        QMessageBox::critical(this, "ERROR", "Please provide correct username and try again!");
        return;
    }
    // Next check if the provided password is correct
    // Just execute a trivial 'ls' command using sudo. If it succeeds, password is ok. Otherwise, password is false!
    if(!compare_passwords())
    {
        QMessageBox::critical(this, "ERROR", "Passwords do not match. Try again!");
        return;
    }
    // Checks are ok by now.
    submit_validation = true;
    QMessageBox::information(this, "INFO", "Password entered. Continuing..");
}

/* ============================================================================================================ */
/*                                      SECTION II : Information Section                                        */
/* ============================================================================================================ */
// List human users
void Manager::cat_users()
{
    QProcess users_proc;
    QString real_users;
    users_proc.start("bash", QStringList() << "-c" << "cut -d: -f1,3 /etc/passwd | egrep ':[0-9]{4}$' | cut -d: -f1");
    if(!users_proc.waitForFinished(3000) || users_proc.exitCode()!=0)
    {
        QMessageBox::critical(this, "ERROR", "Something went wrong. Please try again!");
        return;
    }
    real_users = users_proc.readAllStandardOutput();
    QMessageBox::information(this, "Real System Users", real_users);
    return;
}

void Manager::on_human_users_checkBox_clicked(bool checked)
{
    if(checked)
    {
        cat_users();
    }
}

// List human system groups
void Manager::cat_groups()
{
    QProcess groups_proc;
    QString real_users;
    groups_proc.start("bash", QStringList() << "-c" << "cut -d: -f1,3 /etc/group | egrep ':[0-9]{4}$' | cut -d: -f1");
    if(!groups_proc.waitForFinished(3000) || groups_proc.exitCode()!=0)
    {
        QMessageBox::critical(this, "ERROR", "Something went wrong. Please try again!");
        return;
    }
    real_users = groups_proc.readAllStandardOutput();
    QMessageBox::information(this, "Real System Groups", real_users);
    return;
}

void Manager::on_human_groups_checkBox_clicked(bool checked)
{
    if(checked)
    {
        cat_groups();
    }
}

// Clear the Credentials Section Fields
void Manager::on_clear_credentials_checkBox_clicked(bool checked)
{
    if(checked)
    {
        ui->username_lineEdit->setText("");
        ui->password_lineEdit->setText("");
        ui->retype_password_lineEdit->setText("");
    }
}

// Clear User Management fields
void Manager::on_clear_user_fields_checkBox_clicked(bool checked)
{
    if(checked)
    {
        ui->new_username_lineEdit->setText("");
        ui->new_user_real_name_lineEdit->setText("");
        ui->new_user_group_lineEdit->setText("");
        ui->new_user_id_lineEdit->setText("");
        ui->new_user_home_lineEdit->setText("");
        ui->new_user_shell_lineEdit->setText("");
        ui->new_user_password_lineEdit->setText("");
    }
}

// Clear Group Management fields
void Manager::on_clear_group_fields_checkBox_clicked(bool checked)
{
    if(checked)
    {
        ui->group_name_lineEdit->setText("");
        ui->group_id_lineEdit->setText("");
        ui->new_group_name_lineEdit->setText("");
    }
}

// Exit the application helper function to call whenever I wish!
void Manager::exit_app()
{
    int checked_action = QMessageBox::question(this, "Quit and close Application ?",
                                               "You are about to exit the Application. Are you sure you want to continue?", QMessageBox::No | QMessageBox::Yes);
    if(checked_action==QMessageBox::Yes) {
        close();
    } else  if(checked_action==QMessageBox::No) {
        return;
    }
}

void Manager::on_exit_manager_checkBox_clicked(bool checked)
{
    if(checked)
    {
        exit_app();
    }
}

/* ============================================================================================================ */
/*                                           TOOLBAR ACTIONS SECTION                                            */
/* ============================================================================================================ */
void Manager::on_actionExit_Manager_triggered()
{
    exit_app();
}

void Manager::on_actionAbout_Author_triggered()
{
    QString author("Author: Dimos Katsimardos\nManager Version: v1.0\nDate: August 18, 2019");
    QMessageBox::information(this, "Author Information", author);
}

void Manager::on_actionAbout_Manager_triggered()
{
    QString manager("Manager for Linux systems regarding Users, Groups and Networking activities.");
    QMessageBox::information(this, "Manager Information", manager);
}

/* ============================================================================================================ */
/*                                      SECTION III : Group Management                                          */
/* ============================================================================================================ */

