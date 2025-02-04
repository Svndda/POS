// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "appcontroller.h"
#include "posmodel.h"

#include "ui_mainwindow.h"
#include "loginpage.h"
#include "inventory.h"
#include "pos.h"

// Esta deberia de ser la clase controladora de la aplicacion.

AppController::AppController(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , pageStack(new QStackedWidget(this))
  , model(POS_Model::getInstance()) {
  
  // Define the controller ui as the mainWindow.
  ui->setupUi(this);
  
  // Obtain a reference to the model instance.
  POS_Model& model = POS_Model::getInstance();
  
  // Start, tell the model to prepare his information.
  model.start();
  
  // Creates the different program pages.
  LoginPage* loginPage = new LoginPage(this, model);
  qDebug() << "Entrado a crear products display";
  Pos* posSystem = new Pos(this, model);  
  Inventory* inventory = new Inventory(this, model);

  // Adds the program pages to the stack of pages.
  this->pageStack->addWidget(loginPage);
  this->pageStack->addWidget(posSystem);  
  this->pageStack->addWidget(inventory);
  
  // Adds the pages stack in his corresponding area of the program ui.
  this->ui->mainLayout->addWidget(this->pageStack, 1, 0);
  // Sets the stack page to the pos.
  this->pageStack->setCurrentIndex(1);
}

AppController::~AppController() {
  delete ui;
  this->model.shutdown();
}

void AppController::setupConnections() {
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
  this->switchPages(1);
}

void AppController::on_inventory_button_clicked() {
  this->switchPages(2);
}

void AppController::on_sells_button_clicked() {
  
}

void AppController::on_users_button_clicked() {
  
}

void AppController::on_settings_button_clicked() {
  
}
