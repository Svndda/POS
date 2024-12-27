#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "dashboard.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , pageStack(new QStackedWidget(this))
{
  ui->setupUi(this);
  // Create the derivates pages of the main page.
  // Create the login page, in the case if it is need it.
  Login* loginPage = new Login(this);
  // Create the login dashboard page.
  Dashboard* dashboardPage = new Dashboard(this);

  // Agregar las páginas al stack
  this->pageStack->addWidget(loginPage);
  this->pageStack->addWidget(dashboardPage);

  this->setCentralWidget(pageStack);
  this->pageStack->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
  delete ui;
}
