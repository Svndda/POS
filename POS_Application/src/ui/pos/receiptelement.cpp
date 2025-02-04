#include "receiptelement.h"
#include "ui_receiptelement.h"

ReceiptElement::ReceiptElement(QWidget *parent, const Product& myProduct)
    : QWidget(parent)
    , ui(new Ui::ReceiptElement)
    , product(myProduct)
    , quantity(1)
    , totalPrice(myProduct.getPrice()) {
  ui->setupUi(this);
  this->ui->productName_label->setText(this->product.getName().data());
  
  QString productPrice = QString::number(this->product.getPrice());
  this->ui->productPrice_label->setText(productPrice);
  
  this->ui->totalPrice_label->setText(productPrice);
  
  this->connect(this->ui->incrementQuantity_button, &QPushButton::clicked
      , this, &ReceiptElement::incrementQuantity_button_clicked);
  this->connect(this->ui->reduceQuantity_button, &QPushButton::clicked
      , this, &ReceiptElement::reduceQuantity_button_clicked);
}

ReceiptElement::~ReceiptElement() {
  delete ui;
}

void ReceiptElement::incrementQuantity_button_clicked() {
  ++this->quantity;
  this->ui->quantity_label->setText(QString::number(this->quantity));
  this->totalPrice = this->quantity * this->product.getPrice();
  this->ui->totalPrice_label->setText(QString::number(this->totalPrice));
  this->update();
}

void ReceiptElement::reduceQuantity_button_clicked() {
  --this->quantity;
  if (this->quantity > 0) {
    this->ui->quantity_label->setText(QString::number(this->quantity));
    this->totalPrice = this->quantity * this->product.getPrice();
    this->ui->totalPrice_label->setText(QString::number(this->totalPrice));
  } else {
    this->deleteSelf();
    this->update();
  }
}

void ReceiptElement::deleteSelf() {
  // Verifica si tiene un layout padre y lo remueve
  if (this->parentWidget()) {
    QLayout *layout = this->parentWidget()->layout();
    if (layout) {
      layout->removeWidget(this);
    }
  }
  
  this->deleteLater();  // Marca el widget para ser eliminado de manera segura
}
