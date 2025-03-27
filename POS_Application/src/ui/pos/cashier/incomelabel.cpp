#include "incomelabel.h"
#include "ui_incomelabel.h"

IncomeLabel::IncomeLabel(QWidget *parent
    , const size_t receiptID
    , const QString time
    , const QString paymentMethod
    , const double amount)
    : QWidget(parent)
    , ui(new Ui::IncomeLabel) {
  ui->setupUi(this);
  this->ui->receiptID_label->setText("N Recibo: " + QString::number(receiptID));
  this->ui->time_label->setText("Hora: " + time);
  this->ui->paymentMethod_label->setText("Método de pago: " + paymentMethod);
  this->ui->amount_label->setText("Monto: ₡" + QString::number(amount, 'f', 2));
  this->update();
}

IncomeLabel::~IncomeLabel() {
  delete ui;
}
