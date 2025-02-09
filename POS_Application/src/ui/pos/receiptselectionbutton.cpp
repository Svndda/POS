#include "receiptselectionbutton.h"
#include "ui_receiptselectionbutton.h"

ReceiptSelectionButton::ReceiptSelectionButton(QWidget* parent
    , const size_t myNumber)
    : QWidget(parent)
    , ui(new Ui::ReceiptSelectionButton)
    , receiptNumber(myNumber) {
  ui->setupUi(this);
  // Updates the button text to show the receipt number.
  this->ui->button->setText(QString::number(this->receiptNumber));
  // Connects the button to his signal.
  this->connect(this->ui->button, &QPushButton::clicked
      , this, &ReceiptSelectionButton::on_button_clicked);
}

ReceiptSelectionButton::~ReceiptSelectionButton() {
  delete ui;
}

void ReceiptSelectionButton::on_button_clicked() {
  // Emits the signal indicating that the receipt button has beenm clicked.
  emit this->on_ReceiptSelectionButton_clicked();
}
