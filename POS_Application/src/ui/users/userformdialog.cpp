#include "userformdialog.h"
#include "ui_userformdialog.h"

#include <QMessageBox>

UserFormDialog::UserFormDialog(QWidget* parent
    , const std::vector<User>& users
    , const User userAccount)
    : QDialog(parent)
    , ui(new Ui::UserFormDialog)
    , existingUsers(users)
    , newUser(userAccount) {
  this->ui->setupUi(this);
  this->refreshDisplay();
  
  this->connect(this->ui->acceptUser_button, &QPushButton::clicked
      , this, &UserFormDialog::on_acceptUser_button_clicked);
  
  this->update();
}

UserFormDialog::~UserFormDialog() {
  delete ui;
}

void UserFormDialog::refreshDisplay() {
  
  if (!(this->newUser == User())) {
    this->ui->name_lineEdit->setText(this->newUser.getUsername().data());
  }
  
  this->refreshPermissionsOptions(1, this->ui->pointOfSelling_comboBox);
  this->refreshPermissionsOptions(2, this->ui->inventory_comboBox);
  this->refreshPermissionsOptions(3, this->ui->sells_comboBox);
  this->refreshPermissionsOptions(4, this->ui->users_comboBox);
  this->refreshPermissionsOptions(5, this->ui->settings_comboBox);
}

void UserFormDialog::refreshPermissionsOptions(const size_t page
    , QComboBox* permissionOptions) {
  permissionOptions->addItem("Edición");  
  permissionOptions->addItem("Visualizar");  
  permissionOptions->addItem("Denegado");
  
  const std::vector<User::PageAccess> userPermissions
      = this->newUser.getUserPermissions();
  
  if (this->newUser != User()) {
    if (userPermissions[page].access == User::PageAccess::EDITABLE) {
      permissionOptions->setCurrentIndex(0);
    } else if (userPermissions[page].access == User::PageAccess::NON_EDITABLE) {
      permissionOptions->setCurrentIndex(1);
    } else {
      permissionOptions->setCurrentIndex(2);      
    }
  }
}

void UserFormDialog::on_acceptUser_button_clicked() {
  size_t id = 400 + this->existingUsers.size();
  QString name = this->ui->name_lineEdit->text();
  QString password = this->ui->password_lineEdit->text();
  
  if (!(name.isEmpty())) {
    std::vector<User::PageAccess> permissions;
    permissions.emplace_back(User::PageAccess(0, User::PageAccess::EDITABLE));
    
    permissions.emplace_back(this->requestedUserPermissions(1
        , this->ui->pointOfSelling_comboBox));
    permissions.emplace_back(this->requestedUserPermissions(2
        , this->ui->inventory_comboBox));
    permissions.emplace_back(this->requestedUserPermissions(3
        , this->ui->sells_comboBox));
    permissions.emplace_back(this->requestedUserPermissions(4
        , this->ui->users_comboBox));
    permissions.emplace_back(this->requestedUserPermissions(5
        , this->ui->settings_comboBox));
    
    this->createUser(id, name.toStdString()
        , password.toStdString(), permissions);
    this->accept();
  } else {
    QMessageBox::warning(this, "Error en los parámetros de usuario.",
        "Por favor, rellene correctamente el espacios de nombre");
  }
}

void UserFormDialog::on_cancelUser_button_clicked() {
  this->reject();
}

void UserFormDialog::createUser(const size_t& id, const std::string& name
    , const std::string& password
    , const std::vector<User::PageAccess> permissions) {
  if (this->newUser == User()) {
    if (!password.empty()) {
      this->newUser = User(id, name, permissions);
      this->newUser.setPassword(password);
      
      emit this->newUserRequested(this->newUser); 
    }
  } else {
    User oldUser = this->newUser;
    this->newUser.setUsername(name);
    this->newUser.setUserPermissions(permissions);
    
    if (!password.empty()) {
      this->newUser.setPassword(password);
    }
    
    emit this->editUserRequested(oldUser, this->newUser);
  }
}

User::PageAccess UserFormDialog::requestedUserPermissions(size_t page
    , QComboBox* permissionOptions) {
  if (permissionOptions->currentIndex() == 0) {
    return User::PageAccess(page, User::PageAccess::EDITABLE);
  } else if (permissionOptions->currentIndex() == 1) {
    return User::PageAccess(page, User::PageAccess::NON_EDITABLE);
  } else {
    return User::PageAccess(page, User::PageAccess::DENIED);
  }
}
