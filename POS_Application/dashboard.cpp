#include "dashboard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DashboardPage)
{
  ui->setupUi(this);
}

Dashboard::~Dashboard()
{
  delete ui;
}
