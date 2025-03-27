#include "orderselectionbutton.h"
#include "ui_orderselectionbutton.h"

OrderSelectionButton::OrderSelectionButton(QWidget* parent
    , const size_t myNumber)
    : QWidget(parent)
    , ui(new Ui::OrderSelectionButton)
    , orderNumber(myNumber) {
  ui->setupUi(this);
  // Updates the button text to show the order number.
  this->ui->button->setText(QString::number(this->orderNumber));
  // Connects the button to his signal.
  this->connect(this->ui->button, &QPushButton::clicked
      , this, &OrderSelectionButton::on_button_clicked);
}

OrderSelectionButton::~OrderSelectionButton() {
  delete ui;
}

void OrderSelectionButton::on_button_clicked() {
  // Emits the signal indicating that the order button has beenm clicked.
  emit this->on_OrderSelectionButton_clicked();
}
