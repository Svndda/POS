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
  Inventory* inventory = new Inventory(this, model);
  Pos* posSystem = new Pos(this, model);

  // Adds the program oages to the stack of pages.
  this->pageStack->addWidget(loginPage);
  this->pageStack->addWidget(inventory);
  this->pageStack->addWidget(posSystem);
  
  // Sets the page stack as the cental widget of the window.
  this->setCentralWidget(pageStack);
  // Sets the stack page to the main page index.
  this->pageStack->setCurrentIndex(1);
  qDebug() << "Controller completado";
}

AppController::~AppController() {
  delete ui;
  this->model.shutdown();
}
