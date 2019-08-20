#include "manager.h"
#include "ui_manager.h"
#include "netinfo.h"

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
    QMessageBox::information(this, "INFO", "Credentials entered. Continue procedure ...");
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

// List the available system shells [ aka bash, tcsh, ksh, etc... ]
void Manager::on_shells_checkBox_clicked(bool checked)
{
    if(checked)
    {
        QProcess proc;
        QString shells;
        proc.start("cat /etc/shells");
        proc.waitForFinished(-1);
        if(proc.exitCode()!=0)
        {
            QMessageBox::warning(this, "WARNING", "Could not find available shells in path /ec/shells !");
            return;
        }
        shells = proc.readAllStandardOutput();
        QMessageBox::information(this, "Available shells", shells);
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
QString Manager::getGid() const
{
    return gid;
}

void Manager::setGid(const QString &value)
{
    gid = value;
}

QString Manager::getNew_groupname() const
{
    return new_groupname;
}

void Manager::setNew_groupname(const QString &value)
{
    new_groupname = value;
}

QString Manager::getGroupname() const
{
    return groupname;
}

void Manager::setGroupname(const QString &value)
{
    groupname = value;
}

bool Manager::group_exists()
{
    QProcess proc;
    proc.start("getent group " + getGroupname());
    proc.waitForFinished(-1);
    if(proc.exitCode()!=0)
    {
        return false;
    }
    return true;
}

bool Manager::groupadd()
{
    QProcess pass, add;
    pass.setStandardOutputProcess(&add);
    pass.start("echo " + getPassword());
    if(gid.isEmpty())
    {
        add.start("sudo -S groupadd " + getGroupname());
    }
    else {
        add.start("sudo -S groupadd -g " + getGid() + " " + getGroupname());
    }
    pass.waitForFinished(-1);
    add.waitForFinished(-1);
    if(add.exitCode()!=0)
    {
        return false;
    }
    return true;
}

bool Manager::groupmod()
{
    QProcess pass, mod;
    pass.setStandardOutputProcess(&mod);
    pass.start("echo " + getPassword());
    mod.start("sudo -S groupmod -n " + getNew_groupname() + " " + getGroupname());
    pass.waitForFinished(-1);
    mod.waitForFinished(-1);
    if(mod.exitCode()!=0)
    {
        return false;
    }
    return true;
}

bool Manager::groupdel()
{
    QProcess pass, del;
    pass.setStandardOutputProcess(&del);
    pass.start("echo " + getPassword());
    del.start("sudo -S groupdel " + getGroupname());
    pass.waitForFinished(-1);
    del.waitForFinished(-1);
    if(del.exitCode()!=0)
    {
        return false;
    }
    return true;
}

void Manager::on_confirm_group_stuff_checkBox_clicked(bool checked)
{
    if(checked)
    {
        // If this checkbox is 'checked' then grab group related info
        setGroupname(ui->group_name_lineEdit->text());
        setNew_groupname(ui->new_group_name_lineEdit->text());
        setGid(ui->group_id_lineEdit->text());
        if(getGroupname().isEmpty())
        {
            QMessageBox::warning(this, "WARNING", "Please provide compulsory option groupname and try again!");
        }
    }
}

// Create the new group | Only if it does not already exist
void Manager::on_create_group_button_clicked()
{
    if(!submit_validation)
    {
        QMessageBox::critical(this, "Warning", "Please provide username & password and try again!");
        return;
    }
    if(group_exists()==true)
    {
        QMessageBox::warning(this, "WARNING", "This group already exists in the system. Please try with another one!");
        return;
    }
    // Else create the new group called 'groupname'
    if(groupadd())
    {
        QMessageBox::information(this, "SUCCESS", "The new group: " + getGroupname() + " successfully created!");
        return;
    }
    else {
        QMessageBox::critical(this, "FAILURE", "New group: " + getGroupname() + " failed to be created!");
        return;
    }
}

// Remove the group | Only if it exists in the system
void Manager::on_remove_group_button_clicked()
{
    if(!submit_validation)
    {
        QMessageBox::critical(this, "Warning", "Please provide username & password and try again!");
        return;
    }
    if(!group_exists())
    {
        QMessageBox::warning(this, "WARNING", "This group already does not exist in the system. No need to remove it!");
        return;
    }
    // Else remove it
    if(groupdel())
    {
        QMessageBox::information(this, "SUCCESS", "The group: " + getGroupname() + " successfully removed from the system!");
        return;
    }
    else {
        QMessageBox::critical(this, "FAILURE", "The group: " + getGroupname() + " failed to be removed from the system!");
        return;
    }
}

// Rename a group | If it exists of course
void Manager::on_rename_group_button_clicked()
{
    if(getNew_groupname().isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "Cannot rename group: " + getGroupname() + " to: []! Enter new groupname and try again!");
        return;
    }

    if(!submit_validation)
    {
        QMessageBox::critical(this, "Warning", "Please provide username & password and try again!");
        return;
    }
    if(!group_exists())
    {
        QMessageBox::warning(this, "WARNING", "This group already does not exist in the system!");
        return;
    }
    // Else try to rename this group named getGroupname()
    if(groupmod())
    {
        QMessageBox::information(this, "SUCCESS", "The group: " + getGroupname() + " successfully renamed to: " + getNew_groupname() + " !");
        return;
    }
    else {
        QMessageBox::critical(this, "FAILURE", "The group: " + getGroupname() + " failed to be renamed to: " + getNew_groupname() + " !");
        return;
    }
}

/* ============================================================================================================ */
/*                                       SECTION III : User Management                                          */
/* ============================================================================================================ */
QString Manager::getNew_user_shell() const
{
    return new_user_shell;
}

void Manager::setNew_user_shell(const QString &value)
{
    new_user_shell = value;
}

QString Manager::getNew_user_id() const
{
    return new_user_id;
}

void Manager::setNew_user_id(const QString &value)
{
    new_user_id = value;
}

QString Manager::getNew_user_group() const
{
    return new_user_group;
}

void Manager::setNew_user_group(const QString &value)
{
    new_user_group = value;
}

QString Manager::getNew_user_realname() const
{
    return new_user_realname;
}

void Manager::setNew_user_realname(const QString &value)
{
    new_user_realname = value;
}

QString Manager::getNew_username() const
{
    return new_username;
}

void Manager::setNew_username(const QString &value)
{
    new_username = value;
}

QString Manager::getNew_user_encr_password() const
{
    return new_user_encr_password;
}

void Manager::setNew_user_encr_password(const QString &value)
{
    new_user_encr_password = value;
}

void Manager::create_enc_password()
{
    // opnssl passwd <plain_password>  --> creates the encrypted password hash
    // to be used with the useradd -p <password_hash> option !
    QProcess openssl;
    openssl.start("openssl passwd " + ui->new_user_password_lineEdit->text());
    openssl.waitForFinished();
    QString hold(openssl.readAllStandardOutput());
    hold.remove("\n");
    setNew_user_encr_password(hold);
    if(openssl.exitCode()!=0)
    {
        new_user_encr_password = "";
    }
}


void Manager::on_show_new_user_password_checkBox_clicked(bool checked)
{
    if(checked)
    {
        // If user clicks this checkbox the password is shown
        ui->new_user_password_lineEdit->setEchoMode(QLineEdit::Normal);
    }
    else {
        // If this checkbox is un-checked, please hide the password
        ui->new_user_password_lineEdit->setEchoMode(QLineEdit::Password);
    }
}

// Add a new user in the system
//useradd command example :: useradd -c "Real User Name" -m -u <UID> -g <GROUP> -d /home/$username -s <shell> $username
bool Manager::is_username_valid()
{
    // Name of the new user should not start with a digit
    if(getNew_username() == getenv("USER") || getNew_username().at(0).isDigit())
    {
        //QMessageBox::warning(this, "ERROR", "Can not create the currently logged user!");
        return false;
    }
    QVector<QString> invalid_characters = {"`", "~", "@", "!", "#", "$", "%", "^", "&", "*", "(", ")", "-", "+", "<", ">", ",", ".", "=", "_", "/", ";", ":", "?"};
    QVector<QString>::iterator start = invalid_characters.begin();
    if(getNew_username()=="root")
    {
        return false;
    }
    while(start != invalid_characters.end())
    {
        if(getNew_username().at(0) == *start)
        {
            return false;
        }
        start++;
    }

    return true;
}

// Confirm box is checked | Set new user information fields / string variables
void Manager::on_confirm_user_stuff_checkBox_clicked(bool checked)
{
    if(checked)
    {
        // If this box is checked, then set all entered stuff from the user regarding the new user to be created
        setNew_username(ui->new_username_lineEdit->text());
        setNew_user_realname(ui->new_user_real_name_lineEdit->text());
        setNew_user_group(ui->new_user_group_lineEdit->text());
        setNew_user_id(ui->new_user_id_lineEdit->text());
        setNew_user_shell(ui->new_user_shell_lineEdit->text());
        setNew_groupname(ui->new_user_group_lineEdit->text());
        // This functions takes the password as plain text and sets the new_user_enc_password as an encrypted hash
        // of the entered plain text password using openssl
        if(!ui->new_user_password_lineEdit->text().isEmpty())
        {
            // If this check is missing, openssl process is running ... at the background ! Problem !
            create_enc_password();
        }
        // Next check is new_username || new_user_password are empty
        // The other fields can be empty | Doesn't matter so much
        if(getNew_username().isEmpty() || getNew_user_encr_password().isEmpty())
        {
            QMessageBox::information(this, "ERROR", "The username and password for the new user should not be empty!");
            return;
        }
        if(!is_username_valid())
        {
            QMessageBox::warning(this, "ERROR", "Please provide a valid username and try again!");
            return;
        }
    }
}

// Ownership & Permissions for the home directory of the new user
bool Manager::set_chown()
{
    QProcess passwd, chown;
    passwd.setStandardOutputProcess(&chown);
    passwd.start("echo " + getPassword());
    chown.start("sudo -S chown " + getNew_username() + " /home/" + getNew_username());
    chown.waitForFinished(6000);
    passwd.waitForFinished(6000);
    if(chown.exitCode()!=0)
    {
        return false;
    }
    return true;
}

bool Manager::set_chmod()
{
    QProcess pass, chmod;
    pass.setStandardOutputProcess(&chmod);
    pass.start("echo " + getPassword());
    chmod.start("sudo -S chmod -R u=rwx,g=rw,o=--- /home/" + getNew_username());
    chmod.waitForFinished(6000);
    pass.waitForFinished(6000);
    if(chmod.exitCode()!=0)
    {
        return false;
    }
    return true;
}

// Create tthe new user using a QProcess
bool Manager::adduser()
{
    QString options;
    QProcess pass, add;
    pass.setStandardOutputProcess(&add);
    pass.start("echo " + getPassword());
    if(!getNew_user_realname().isEmpty())
    {
        options += " -c \"" + getNew_user_realname() + "\"";
    }
    if(!getNew_user_group().isEmpty())
    {
        options += " -g " + getNew_groupname();
    }
    if(!getNew_user_id().isEmpty())
    {
        options += " -u " + getNew_user_id();
    }
    options += " -m -d /home/" + getNew_username();
    if(!getNew_user_shell().isEmpty())
    {
        options += " -s /bin/" + getNew_user_shell();
    }
    options += " -p " + getNew_user_encr_password();
    options += " " + getNew_username();
    add.start("sudo -S useradd " + options);
    add.waitForFinished(-1);
    if(add.exitCode()!=0)
    {
        return false;
    }
    set_chmod();
    set_chown();
    return true;
}

void Manager::on_create_new_user_button_clicked()
{
    if(!submit_validation)
    {
        QMessageBox::critical(this, "Warning", "Please provide username & password and try again!");
        return;
    }
    // Check if the user exists in the system
    if(user_exists())
    {
        QMessageBox::information(this, "INFO", "The user: " + getNew_username() + " already exists in the system. Please try with another name!");
        return;
    }

    // Remember to check here if the entered group for the new user who will be created
    // exists in the system, or the user should create this group from the GROUP Management Section

    //
    if(adduser())
    {
        QMessageBox::information(this, "SUCCESS", "The new user: " + getNew_username() + " created successfully!");
        return;
    }
    else {
        QMessageBox::critical(this, "FAILURE", "The user: " + getNew_username() + " Failed to be created!");
        return;
    }
}

// REMOVE A USER
bool Manager::deluser()
{
    QProcess pass, del;
    pass.setStandardOutputProcess(&del);
    pass.start("echo " + getPassword());
    del.start("sudo -S userdel " + getNew_username());
    pass.waitForFinished();
    del.waitForFinished(-1);
    if(del.exitCode()!=0)
    {
        return false;
    }
    return true;
}

// CHeck if a user exists in the system
bool Manager::user_exists()
{
    QProcess proc;
    proc.start("id " + getNew_username());
    proc.waitForFinished();
    if(proc.exitCode()!=0)
    {
        return false;
    }
    return true;
}

// Delete the user's home directory
bool Manager::del_user_home()
{
    QProcess pass, rm_dir;
    pass.setStandardOutputProcess(&rm_dir);
    pass.start("echo " + getPassword());
    rm_dir.start("sudo -S rm -r /home/" + getNew_username());
    pass.waitForFinished(6000);
    rm_dir.waitForFinished(6000);
    if(rm_dir.exitCode()!=0)
    {
        return false;
    }
    return true;
}

void Manager::on_remove_user_button_clicked()
{
    if(!submit_validation)
    {
        QMessageBox::critical(this, "Warning", "Please provide username & password and try again!");
        return;
    }
    if(!user_exists())
    {
        QMessageBox::information(this, "WARNING", "The user: " + getNew_username() + " does not exist in the system!");
        return;
    }
    if(deluser() && del_user_home())
    {
        QMessageBox::information(this, "SUCCESS", "The user: " + getNew_username() + " successfully removed from the system!");
        return;
    }
    else {
        QMessageBox::critical(this, "FAILURE", "The user: " + getNew_username() + " failed to be removed from the system! Please try again!");
        return;
    }
}

/* ============================================================================================================ */
/*                                           SECTION IV : Networking                                            */
/* ============================================================================================================ */
QString Manager::netstat()
{
    QProcess net;
    // r -> display routing table argument
    net.start("netstat -r");
    net.waitForFinished(6000);
    QString hold(net.readAllStandardOutput());
    if(net.exitCode()!=0)
    {
        return "[]";
    }
    return hold;
}

QString Manager::ifconfig()
{
    QProcess ifconf;
    ifconf.start("ifconfig");
    ifconf.waitForFinished(6000);
    QString hold(ifconf.readAllStandardOutput());
    if(ifconf.exitCode()!=0)
    {
        return "[]";
    }
    return hold;
}

QString Manager::iptables()
{
    QProcess pass, ip_proc;
    pass.setStandardOutputProcess(&ip_proc);
    pass.start("echo " + getPassword());
    ip_proc.start("sudo -S iptables -L --line-numbers");
    ip_proc.waitForFinished(6000);
    pass.waitForFinished(6000);
    QString hold(ip_proc.readAllStandardOutput());
    if(ip_proc.exitCode()!=0)
    {
        return "[]";
    }
    return hold;
}

void Manager::on_interfaces_checkBox_clicked(bool checked)
{
    if(checked)
    {
        if(!submit_validation)
        {
            QMessageBox::critical(this, "Warning", "Please provide username & password and try again!");
            return;
        }
        // Show the NetInfo Dialog
        NetInfo *NetInfoDialog = new NetInfo(this);
        NetInfoDialog->setWindowTitle("NETWORK INTERFACES");
        QString info = ifconfig();
        NetInfoDialog->catchText(info);
        NetInfoDialog->exec();
    }
}

void Manager::on_routing_table_checkBox_clicked(bool checked)
{
    if(checked)
    {
        if(!submit_validation)
        {
            QMessageBox::critical(this, "Warning", "Please provide username & password and try again!");
            return;
        }
        // Show the NetInfo Dialog
        NetInfo *NetInfoDialog = new NetInfo(this);
        NetInfoDialog->setWindowTitle("ROUTING TABLE");
        QString info = netstat();
        NetInfoDialog->catchText(info);
        NetInfoDialog->exec();
    }
}

void Manager::on_firewall_checkBox_clicked(bool checked)
{
    if(checked)
    {
        if(!submit_validation)
        {
            QMessageBox::critical(this, "Warning", "Please provide username & password and try again!");
            return;
        }
        // Show the NetInfo Dialog
        NetInfo *NetInfoDialog = new NetInfo(this);
        NetInfoDialog->setWindowTitle("NETFILTER FIREWALL SET-UP");
        QString info = iptables();
        NetInfoDialog->catchText(info);
        NetInfoDialog->exec();
    }
}
