#include "processorderdialog.h"
#include "ui_processorderdialog.h"

ProcessOrderDialog::ProcessOrderDialog(QWidget* parent
    , Order& orderToProcess)
    : QDialog(parent)
    , ui(new Ui::ProcessOrderDialog)
    , order(orderToProcess) {
  this->ui->setupUi(this);
  // SetUp tthe combo box options.
  this->ui->paymentMethod_comboBox->addItem("Efectivo");
  this->ui->paymentMethod_comboBox->addItem("Tarjeta");
  this->ui->paymentMethod_comboBox->addItem("Sinpe");
  // Connects the ui elements to the slot functions.
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
  // Obtains the order price.
  const double orderPrice = this->order.getPrice();
  // Gets the received amount introduced by the user.
  const double receivedAmount = this->order.getReceivedMoney();
  // Checks if the received amount is higher than the order price.
  if (receivedAmount >= orderPrice) {
    // Stablish the order payment method.
    this->order.setPaymentMethod(
        this->ui->paymentMethod_comboBox->currentText());
    this->accept(); 
  }
}

void ProcessOrderDialog::updateChangeLabel(double value) {
  // Checks if the payment method selected is cash.  
  if (this->ui->paymentMethod_comboBox->currentIndex() == 0) {
    // Checks if the value passed is higher than the order price.
    if (value < this->order.getPrice()) {
      this->ui->changeAmount_label->setText("Error en monto recibido");
    } else {
      // Sets the value as the received money for the order.
      this->order.setReceivedMoney(value);
      // Computes the change to return to the client.
      const double change = value - this->order.getPrice();
      this->ui->changeAmount_label->setText(QString::number(change, 'f', 2));
    }
  } else {
    // Updates the received money ui element with the order price for digital
    // payments.
    this->ui->receivedAmount_doubleSpinBox->setValue(
        this->order.getPrice());
    this->ui->changeAmount_label->setText("Pago exacto (sin cambio)");
    // Stablish the received money as the order price for digital payments.    
    this->order.setReceivedMoney(order.getPrice());
  }
}
