#include "logout.h"
#include "ui_logout.h"

Logout::Logout(QWidget *parent, POS_Model& appmodel)
    : QWidget(parent)
    , ui(new Ui::Logout)
    , model(appmodel) {
  ui->setupUi(this);
  this->ui->contentLayout->setAlignment(Qt::AlignTop);
}

Logout::~Logout() {
  delete ui;
}
