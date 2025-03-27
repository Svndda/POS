#include "users.h"
#include "ui_users.h"

#include <QMessageBox>

#include "userformdialog.h"
#include "util.h"

Users::Users(QWidget *parent, POS_Model& appModel)
    : Catalog(parent, appModel)
    , ui(new Ui::Users) {
  ui->setupUi(this);
  this->refreshDisplay(this->itemsPerPage);
  this->setupConnections();
}

Users::~Users() {
  delete ui;
}

void Users::refreshDisplay(const size_t items) {
  std::vector<User> visibleUsers
      = this->model.getUsersForPage(this->currentPageIndex, items);
  // Initiazates the label index iterator.
  size_t labelIt = 0;
  // Tranverse the different rows of user's information.
  for (size_t index = 0; index < items; index++) {
    // Place holder labels text.
    QString name("-----");
    QString permissions("------");
    
    // If theres remaining user's information to be displayed.
    if (index < visibleUsers.size()) {
      // Extracts all the user's information to display it on the screen.
      name = visibleUsers[index].getUsername().data();
      permissions = this->model.formatUserPermissions(visibleUsers[index]);
    }
    // Updates the the different labels for each user's information that is
    // displayed in the UI.
    // Updates the user's text contained in the user name of the matching
    // label.
    Util::updatePrefixedLabel(this, "userName", labelIt, name);
    // Updates the label containing the number of users registered in the
    // system.
    Util::updatePrefixedLabel(this, "userPermissions", labelIt, permissions);  
    // Increse the label iterator.
    ++labelIt;
  }
  const size_t offset = this->currentPageIndex * 9;
  // Generates the page label text tha indicates the indexes of the products
  // displayed in the actual page.
  const QString pageLabelText = QString("Mostrando usuarios %1 hasta %2 de %3"
      " usuarios").arg(offset + 1).arg(std::min(offset + 9,
                this->model.getNumberOfUsers())).arg(
      this->model.getNumberOfUsers());
  this->ui->pageDisplayingNumber_label->setText(pageLabelText);
  
  this->update();
}


void Users::setupConnections() {
  // String variables that will contain the different delete and edit buttons
  // name for each registered category.
  QString deleteButtonName("");
  QString editButtonName("");
  // Go through the rows of the page connecting their deletes and edit buttons
  //  with the funtions that handles that event.
  for (size_t index = 0; index < 9; ++index) {
    // Parse and index the correponding buttons name.
    editButtonName = QString("editProduct_button_%1").arg(index);
    deleteButtonName = QString("deleteProduct_button_%1").arg(index);
    // Find the buttons and store their pointers.
    QPushButton* deleteButton = this->findChild<QPushButton *>(deleteButtonName);
    QPushButton* editButton = this->findChild<QPushButton *>(editButtonName);
    // Connects the buttons with their functions.
    this->connect(deleteButton , &QPushButton::clicked
        , this, &Users::delete_button_clicked);
    this->connect(editButton , &QPushButton::clicked
        , this, &Users::edit_button_clicked);
  }
  // Connects the funtions that handles the next and previous page of registered
  // categories.
  this->connect(this->ui->nextPage_button, &QPushButton::clicked
      , this, &Users::nextPage_button_clicked);
  this->connect(this->ui->previousPage_button, &QPushButton::clicked
      , this, &Users::previousPage_button_clicked);
  this->connect(this->ui->addCategory_button, &QPushButton::clicked
      , this, &Users::addUser_button_clicked);
}


void Users::addUser_button_clicked() {
  if (this->model.getPageAccess(4) == User::PageAccess::EDITABLE) {
    // Creates a dialog to manage the category creation.
    UserFormDialog dialog(this, this->model.getRegisteredUsers()
        , User());
    
    this->connect(&dialog, &UserFormDialog::newUserRequested
        , this, &Users::saveRequestedUser);
    
    dialog.exec();
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void Users::saveRequestedUser(const User user) {
  if (this->model.addUser(user)) {
    this->refreshDisplay(this->itemsPerPage);
  }
}

void Users::nextPage_button_clicked() {
  
}

void Users::previousPage_button_clicked() {
  
}

void Users::delete_button_clicked() {
  if (this->model.getPageAccess(4) == User::PageAccess::EDITABLE) {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
      // Search for the property index in the button to see their index.
      const size_t buttonIndex = button->property("index").toUInt();
      // Gets the categories vector for the actual page.
      const auto usersForPage = this->model.getUsersForPage(
          this->currentPageIndex, this->itemsPerPage);
      // Checks that that the button index is lower than the categories for this
      // page to avoid an empty row.
      if (buttonIndex < usersForPage.size()) {
        // Gets the row category.
        const User user = usersForPage[buttonIndex];
        // Try to remove the category from the registered ones.
        if (this->model.removeUser(user)) {
          // Refresh the categories display with the updated data.
          this->refreshDisplay(this->itemsPerPage);
        } else {
          QMessageBox::warning(this, "Error de registros"
              , "No se añadió el usuario.");
        }
      }
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void Users::edit_button_clicked() {
  if (this->model.getPageAccess(4) == User::PageAccess::EDITABLE) {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    // Checks if the pointer is valid.
    if (button) {
      // Search for the property index in the button to see their index.
      size_t buttonIndex = button->property("index").toUInt();
      // Gets the categories vector for the actual page.
      const auto usersForPage = this->model.getUsersForPage(
          this->currentPageIndex, this->itemsPerPage);
      // Checks that that the button index is lower than the categories for this
      // page to avoid an empty row.
      if (buttonIndex < usersForPage.size()) {
        // Gets the row category.
        const User oldUser = usersForPage[buttonIndex];
        // Creates a dialog to manage the existing category editing.
        UserFormDialog dialog(this, this->model.getRegisteredUsers()
            , oldUser);
        
        this->connect(&dialog, &UserFormDialog::editUserRequested
            , this, &Users::editRequestedUser);
        
        // Executes the dialog to manage the category creation.
        dialog.exec();
      }
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void Users::editRequestedUser(const User oldUser, const User newUser) {
  // Try to update the category name to the name given by the user.
  if (this->model.editUser(oldUser, newUser)) {
    // Updates the display with the new category.
    this->refreshDisplay(this->itemsPerPage);
    qDebug() << "Se ha modificado un usuario exitosamente";    
  } else {
    QMessageBox::information(this, "Informacion inválida"
        , "No se añadió el usuario.");
  }
}
