// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "appcontroller.h"
#include "posmodel.h"

#include "user.h"
#include "ui_mainwindow.h"
#include "loginpage.h"
#include "inventory.h"
#include "users.h"
#include "settings.h"
#include "pos.h"

AppController::AppController(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , pageStack(new QStackedWidget(this))
  , model(POS_Model::getInstance()) {
  // Define the controller ui as the mainWindow.
  ui->setupUi(this);
  // Adds the pages stack in his corresponding area of the program ui.
  this->ui->mainLayout->addWidget(this->pageStack, 1, 0);
  // Creates the pos loggin page to manage the user's loggin.
  LoginPage* loginPage = new LoginPage(this, this->model);
  // Add the login page to the stack view.
  this->pageStack->addWidget(loginPage);
  this->pageStack->setCurrentIndex(0);
  // Connects the login signal to the controller function to try
  //  start the system.
  this->connect(loginPage, &LoginPage::sendCredentials
      , this, &AppController::userAccepted);
  // Connects all the ui elements to their slot functions.
  this->setupConnections();
  
  // Mostrar en pantalla completa
  this->showMaximized();
}

AppController::~AppController() {
  // Primero, eliminamos el pageStack y su contenido
  // qDeleteAll(this->pageStack.);  // Elimina todos los widgets de pageStack
  
  delete this->pageStack;  // Después eliminamos el pageStack en sí mismo
  
  delete this->ui;  // Elimina el UI de la ventana principal
  
  // Apaga el modelo POS de manera segura
  this->model.shutdown();
}


void AppController::setupConnections() {
  // Connects all the pages systen pages buttons to thei slot functions.
  this->connect(this->ui->pos_button, &QPushButton::clicked
                , this, &AppController::on_pos_button_clicked);
  this->connect(this->ui->inventory_button, &QPushButton::clicked
                , this, &AppController::on_inventory_button_clicked);
  this->connect(this->ui->sells_button, &QPushButton::clicked
                , this, &AppController::on_sells_button_clicked);
  this->connect(this->ui->users_button, &QPushButton::clicked
                , this, &AppController::on_users_button_clicked);
  this->connect(this->ui->settings_button, &QPushButton::clicked
                , this, &AppController::on_settings_button_clicked);
  // Disables the pages buttons.
  this->disableButtons();
}

void AppController::enableButtons() {
  // Enables all the system pages buttons.
  this->ui->pos_button->setEnabled(true);
  this->ui->inventory_button->setEnabled(true);
  this->ui->sells_button->setEnabled(true);
  this->ui->users_button->setEnabled(true);
  this->ui->settings_button->setEnabled(true);
}

void AppController::disableButtons() {
  // Disables all the system pages buttons.  
  this->ui->pos_button->setDisabled(true);
  this->ui->inventory_button->setDisabled(true);
  this->ui->sells_button->setDisabled(true);
  this->ui->users_button->setDisabled(true);
  this->ui->settings_button->setDisabled(true);
}

void AppController::prepareSystemPages() {
  // Creates the different program pages.
  Pos* posPage = new Pos(this, this->model);
  Inventory* inventoryPage = new Inventory(this, this->model);
  Users* usersPage = new Users(this, this->model);
  Settings* settingsPage = new Settings(this, this->model);
  
  // Adds the program pages to the stack of pages.
  this->pageStack->addWidget(posPage);
  this->pageStack->addWidget(inventoryPage);
  this->pageStack->addWidget(usersPage);
  this->pageStack->addWidget(settingsPage);
  
  this->connect(settingsPage, &Settings::logoutCurrentUser
      , this, &AppController::resetApplicationState);
  
  // Sets the stack page to the pos.
  this->refreshPageStack(1);
}

void AppController::refreshPageStack(const size_t stackIndex) {
  // Checks if the model is started.
  if (this->model.isStarted()) {
    qDebug() << "boton de pos seleccionado";
    // Obtains the current user permissions.
    const auto userPermissions
        = this->model.getCurrentUser().getUserPermissions();
    qDebug() << "permission state: " << userPermissions[stackIndex].access;
    // Checks if the user has allowed access to the clicked button's page.
    if (userPermissions[stackIndex].access != User::PageAccess::DENIED) {
      // Switch the page.      
      this->switchPages(stackIndex);
    } 
  }
}

void AppController::switchPages(const size_t pageIndex) {
  // Sets the indexed page of the stack to the requested one.
  this->pageStack->setCurrentIndex(pageIndex);
  
  // Buttons offset.
  const size_t buttonsOffset = pageIndex - 1;
  
  // Vector of the application buttons to move through the pages.
  QVector<QPushButton*> buttons = {
      this->ui->pos_button,
      this->ui->inventory_button,
      this->ui->sells_button,
      this->ui->users_button,
      this->ui->settings_button
  };
  
  // Vector of the rectangle widgets that are sync with the current button page.
  QVector<QWidget*> widgets = {
      this->ui->pos_widget,
      this->ui->inventory_widget,
      this->ui->sells_widget,
      this->ui->users_widget,
      this->ui->settings_widget
  };
  
  // Iterate over the buttons and widgets to update their states
  for (size_t i = 0; i < buttons.size(); ++i) {
    // Boolean that indicates if the current index is the requested page.
    const bool isSelected = (i == buttonsOffset);
    buttons[i]->setChecked(isSelected);
    // Use a conditional to set the style sheet: green for selected,
    // transparent otherwise.
    widgets[i]->setStyleSheet(
        QString("QWidget { background-color: %1; }")
            .arg(isSelected ? "rgb(0, 153, 73)" : "transparent")
        );
  }
}

void AppController::on_pos_button_clicked() {
  this->refreshPageStack(1);
}

void AppController::on_inventory_button_clicked() {
  this->refreshPageStack(2);
}

void AppController::on_sells_button_clicked() {
  
}

void AppController::on_users_button_clicked() {
  this->refreshPageStack(3);
}

void AppController::on_settings_button_clicked() {
  this->refreshPageStack(4);
}

void AppController::userAccepted(const User user) {
  // Start, tell the model to prepare his information.  
  if (this->model.start(user)) {
    // Creates 
    this->prepareSystemPages();
    // Enables the page buttons.
    this->enableButtons();
    qDebug() << "credenciales aceptadas";
  } else {
    qDebug() << "Las credenciales de usuario no coinciden.";
  }
}

void AppController::resetApplicationState() {
  this->model.shutdown();
  this->pageStack->setCurrentIndex(0);
}
