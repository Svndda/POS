#ifndef USERS_H
#define USERS_H

#include <QWidget>

#include "posmodel.h"
#include "catalog.h"

namespace Ui {
class Users;
}

class Users : public Catalog {
  Q_OBJECT
  
private:
  Ui::Users* ui;
  POS_Model& model = POS_Model::getInstance();

public:
  explicit Users(QWidget* parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance());
  ~Users();
  
protected:
  /**
   * @brief Refreshes the category display with the current page of categories.
   * @param pageItems The number of categories to display per page.
   */
  virtual void refreshDisplay(const size_t pageItems) override;
  
  /**
   * @brief Sets up signal-slot connections for various UI components.
   */
  virtual void setupConnections() override;
  
protected slots:
  /**
   * @brief Slot triggered when the "Add User" button is clicked.
   */
  void addUser_button_clicked();
  
  void saveRequestedUser(const User user);
  void editRequestedUser(const User oldUser, const User newUser);
  
protected slots:
  /**
   * @brief Slot triggered when the "Next Page" button is clicked.
   */
  virtual void nextPage_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Previous Page" button is clicked.
   */
  virtual void previousPage_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Delete User" button is clicked.
   */
  virtual void delete_button_clicked() override;

  /**
   * @brief Slot triggered when the "Edit User" button is clicked.
   */
  virtual void edit_button_clicked() override;
};

#endif // USERS_H
