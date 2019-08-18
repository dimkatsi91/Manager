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
}

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
