#include "assistance.h"
#include "ui_assistance.h"

Assistance::Assistance(QWidget *parent, Model& appmodel)
    : QWidget(parent)
    , ui(new Ui::Assistance)
    , model(appmodel) {
  ui->setupUi(this);
}

Assistance::~Assistance() {
  delete ui;
}
