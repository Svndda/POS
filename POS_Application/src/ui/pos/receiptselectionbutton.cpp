#include "receiptselectionbutton.h"
#include "ui_receiptselectionbutton.h"

ReceiptSelectionButton::ReceiptSelectionButton(QWidget* parent
    , const size_t myNumber)
    : QWidget(parent)
    , ui(new Ui::ReceiptSelectionButton)
    , receiptNumber(myNumber) {
  ui->setupUi(this);
  this->ui->button->setText(QString::number(this->receiptNumber));
  this->connect(this->ui->button, &QPushButton::clicked
      , this, &ReceiptSelectionButton::on_button_clicked);
}

ReceiptSelectionButton::~ReceiptSelectionButton() {
  delete ui;
}

void ReceiptSelectionButton::on_button_clicked() {
  emit this->on_ReceiptSelectionButton_clicked();
}
