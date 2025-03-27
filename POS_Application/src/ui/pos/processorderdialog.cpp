#include "processorderdialog.h"
#include "ui_processorderdialog.h"

ProcessOrderDialog::ProcessOrderDialog(QWidget* parent
    , Order& orderToProcess)
    : QDialog(parent)
    , ui(new Ui::ProcessOrderDialog)
    , order(orderToProcess) {
  ui->setupUi(this);
  this->ui->paymentMethod_comboBox->addItem("Efectivo");
  this->ui->paymentMethod_comboBox->addItem("Tarjeta");
  this->ui->paymentMethod_comboBox->addItem("Sinpe");
  this->connect(this->ui->receivedAmount_doubleSpinBox
    , &QDoubleSpinBox::valueChanged
    , this
    , &ProcessOrderDialog::updateChangeLabel);
  this->connect(ui->paymentMethod_comboBox, &QComboBox::currentIndexChanged,
      this, [this]() {
        updateChangeLabel(this->ui->receivedAmount_doubleSpinBox->value());
      });
  
  this->connect(this->ui->accept_button, &QPushButton::clicked
      , this, &ProcessOrderDialog::on_accept_button_clicked);
  this->connect(this->ui->cancel_button, &QPushButton::clicked
      , this, [this]() {this->reject();});
}

ProcessOrderDialog::~ProcessOrderDialog() {
  delete ui;
}

void ProcessOrderDialog::on_accept_button_clicked() {
  const double orderPrice = this->order.getOrderPrice();
  const double receivedAmount = this->ui->receivedAmount_doubleSpinBox->value();
  if (receivedAmount >= orderPrice) {
    this->order.setPaymentMethod(
        this->ui->paymentMethod_comboBox->currentText());
    this->accept(); 
  }
}

// void ProcessOrderDialog::on_paymentMethod_changed(int index) {
//   // if (index != 0) {
//   //   const double orderPrice = this->order.getOrderPrice();
//   //   this->ui->receivedAmount_doubleSpinBox->setValue(orderPrice);
//   // }
// }

void ProcessOrderDialog::updateChangeLabel(double value) {
  this->order.setReceivedMoney(value);
  if (this->ui->paymentMethod_comboBox->currentIndex() == 0) {
    if (value < this->order.getOrderPrice()) {
      this->ui->changeAmount_label->setText("Error en monto recibido");
    } else {
      const double change = value - this->order.getOrderPrice();
      this->ui->changeAmount_label->setText(QString::number(change, 'f', 2));
    }
  } else {
    this->ui->receivedAmount_doubleSpinBox->setValue(
        this->order.getOrderPrice());
    this->ui->changeAmount_label->setText("Pago exacto (sin cambio)");
    this->order.setReceivedMoney(order.getOrderPrice());
  }
}
