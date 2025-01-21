#include "appcontroller.h"
#include "posmodel.h"

#include "ui_mainwindow.h"
#include "loginpage.h"
#include "products.h"
#include "categories.h"
#include "supplies.h"

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
  Products* products = new Products(this, model);
  Categories* categories = new Categories(this, model);
  Supplies* supplies = new Supplies(this, model);

  // Adds the program oages to the stack of pages.
  this->pageStack->addWidget(loginPage);
  this->pageStack->addWidget(products);
  this->pageStack->addWidget(categories);
  this->pageStack->addWidget(supplies);
  
  // Sets the page stack as the cental widget of the window.
  this->setCentralWidget(pageStack);
  // Sets the stack page to the main page index.
  this->pageStack->setCurrentIndex(1);
  
  connect(products
          , &Products::categories_button_signal
          , this
          , &AppController::inventory_categoriesDisplay);
  
  connect(products
          , &Products::supplies_button_signal
          , this
          , &AppController::inventory_primeDisplay);
  
  connect(categories
          , &Categories::products_button_signal
          , this
          , &AppController::inventory_productsDisplay);
  
  connect(categories
          , &Categories::supplies_button_signal
          , this
          , &AppController::inventory_primeDisplay);
  
  connect(supplies
          , &Supplies::products_button_signal
          , this
          , &AppController::inventory_productsDisplay);
  
  connect(supplies
          , &Supplies::categories_button_signal
          , this
          , &AppController::inventory_categoriesDisplay);
}

void AppController::inventory_categoriesDisplay() {
  this->pageStack->setCurrentIndex(2);
}

void AppController::inventory_productsDisplay() {
  this->pageStack->setCurrentIndex(1);
}

void AppController::inventory_primeDisplay() {
  this->pageStack->setCurrentIndex(3);
}

AppController::~AppController() {
  delete ui;
  this->model.shutdown();
}
