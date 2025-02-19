#ifndef USERFORMDIALOG_H
#define USERFORMDIALOG_H

#include <QDialog>
#include <QComboBox>

#include "user.h"

namespace Ui {
class UserFormDialog;
}

class UserFormDialog : public QDialog {
  Q_OBJECT
  
private:
  Ui::UserFormDialog *ui;
  
  /**
   * @brief A vector containing the existing users.
   * Used to validate the uniqueness of the new user.
   */
  const std::vector<User>& existingUsers;
  
  /**
   * @brief Stores the data of the new user created.
   */
  User newUser;

public:
  explicit UserFormDialog(QWidget *parent = nullptr
      , const std::vector<User>& users = std::vector<User>()
      , const User userAccount = User());
  ~UserFormDialog();
  
private:
  void refreshDisplay();
  void refreshPermissionsOptions(const size_t page
      , QComboBox* permissionOptions);
  User::PageAccess requestedUserPermissions(size_t page
      , QComboBox* permissionOptions);
  void createUser(const size_t& id, const std::string& name
      , const std::string& password
      , const std::vector<User::PageAccess> permissions);
  
signals:
  void newUserRequested(const User user);
  void editUserRequested(const User oldUser, const User newUser);
  
private slots:
  void on_acceptUser_button_clicked();
  void on_cancelUser_button_clicked();
};

#endif // USERFORMDIALOG_H
