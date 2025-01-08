#include <iostream>

#include "dashboard.h"
#include "ui_dashboard.h"
#include "backupcontroller.h"

Dashboard::Dashboard(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DashboardPage) {
  ui->setupUi(this);
  
  BackUpController backupMemory = BackUpController::getInstance();
  backupMemory.start();
}

Dashboard::~Dashboard()
{
  delete ui;
}
