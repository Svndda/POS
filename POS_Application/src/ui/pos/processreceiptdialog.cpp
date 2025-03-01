#include "processreceiptdialog.h"
#include "ui_processreceiptdialog.h"

ProcessReceiptDialog::ProcessReceiptDialog(QWidget *parent
    , const double amount)
    : QDialog(parent)
    , ui(new Ui::ProcessReceiptDialog)
    , receiptAmount(amount) {
  ui->setupUi(this);
  this->ui->paymentMethod_comboBox->addItem("Efectivo");
  this->ui->paymentMethod_comboBox->addItem("Tarjeta");
  this->ui->paymentMethod_comboBox->addItem("Sinpe");
  this->connect(this->ui->receivedAmount_doubleSpinBox
    , &QDoubleSpinBox::valueChanged
    , this
    , &ProcessReceiptDialog::updateChangeLabel);
  this->connect(ui->paymentMethod_comboBox, &QComboBox::currentIndexChanged,
      this, [this]() {
        updateChangeLabel(this->ui->receivedAmount_doubleSpinBox->value());
      });
  
  this->connect(this->ui->accept_button, &QPushButton::clicked
      , this, [this]() {this->accept();});
  this->connect(this->ui->cancel_button, &QPushButton::clicked
      , this, [this]() {this->reject();});
}

ProcessReceiptDialog::~ProcessReceiptDialog() {
  delete ui;
}

void ProcessReceiptDialog::updateChangeLabel(double value) {
  this->receivedAmount = value;
  if (this->ui->paymentMethod_comboBox->currentIndex() == 0) {
    if (this->receivedAmount < this->receiptAmount) {
      this->ui->changeAmount_label->setText("Error en monto recibido");
    } else {
      const double change = this->receivedAmount - this->receiptAmount;
      this->ui->changeAmount_label->setText(QString::number(change, 'f', 2));
    }
  } else {
    this->ui->changeAmount_label->setText("Pago exacto (sin cambio)");
  }
}
