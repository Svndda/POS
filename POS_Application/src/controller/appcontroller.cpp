#include "appcontroller.h"
#include "posmodel.h"

#include "ui_mainwindow.h"
#include "loginpage.h"
#include "inventorypage.h"
#include "categoriespage.h"

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
  InventoryPage* inventoryPage = new InventoryPage(this, model);
  CategoriesPage* categoriesPage = new CategoriesPage(this);

  // Adds the program oages to the stack of pages.
  this->pageStack->addWidget(loginPage);
  this->pageStack->addWidget(inventoryPage);
  this->pageStack->addWidget(categoriesPage);
  
  // Sets the page stack as the cental widget of the window.
  this->setCentralWidget(pageStack);
  // Sets the stack page to the main page index.
  this->pageStack->setCurrentIndex(1);
  
  connect(inventoryPage
          , &InventoryPage::registeredCategoriesSelected
          , this
          , &AppController::inventoryCategorySelected);
}

void AppController::inventoryCategorySelected() {
  this->pageStack->setCurrentIndex(2);
}

AppController::~AppController() {
  delete ui;
  this->model.shutdown();
}
