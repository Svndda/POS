#include "personalization.h"
#include "ui_personalization.h"

Personalization::Personalization(QWidget *parent, Model& appmodel)
    : QWidget(parent)
    , ui(new Ui::Personalization)
    , model(appmodel) {
  ui->setupUi(this);
}

Personalization::~Personalization() {
  delete ui;
}
