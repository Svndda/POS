#include "receiptelement.h"
#include "ui_receiptelement.h"

ReceiptElement::ReceiptElement(QWidget *parent, const Product& myProduct)
    : QWidget(parent)
    , ui(new Ui::ReceiptElement)
    , product(myProduct)
    , quantity(1)
    , totalPrice(myProduct.getPrice()) {
  ui->setupUi(this);
  this->refreshDisplay();
  this->setupConnections();
}

ReceiptElement::~ReceiptElement() {
  delete ui;
}

void ReceiptElement::refreshDisplay() {
  // Updates/sets the ui labels to contain the product's information.
  this->ui->productName_label->setText(this->product.getName().data());
  QString productPrice = QString::number(this->product.getPrice());
  this->ui->productPrice_label->setText(productPrice);
  this->ui->totalPrice_label->setText(productPrice);
}

void ReceiptElement::setupConnections() {
  // Connects the ui elements to their slot functions.
  this->connect(this->ui->incrementQuantity_button, &QPushButton::clicked
      , this, &ReceiptElement::incrementQuantity_button_clicked);
  this->connect(this->ui->reduceQuantity_button, &QPushButton::clicked
      , this, &ReceiptElement::reduceQuantity_button_clicked);
}

void ReceiptElement::deleteSelf() {
  // Checks if the receipt ui has a parent widget.
  if (this->parentWidget()) {
    // Obtains the layout of the parent container.
    QLayout *layout = this->parentWidget()->layout();
    // Checks if the layout pointer isn't null.
    if (layout) {
      // Remove the receipt widget ui from the layout.
      layout->removeWidget(this);
    }
  }
  // Force the deletion of the receipt ui elements.
  this->deleteLater();
}

void ReceiptElement::incrementQuantity_button_clicked() {
  // Increments the product's quantity counter.
  ++this->quantity;
  // Updates the quantity label text to show the new value.
  this->ui->quantity_label->setText(QString::number(this->quantity));
  // Calculates the new product's total price.
  this->totalPrice = this->quantity * this->product.getPrice();
  // Updates the product's total price label to show the new value.
  this->ui->totalPrice_label->setText(QString::number(this->totalPrice));
  // Emits the signal indicating that the product's quantity has changed.
  emit this->quantityIncreased(this->product);
  // Force the update of the receipt element ui.
  this->update();
}

void ReceiptElement::reduceQuantity_button_clicked() {
  // Decrements the quantity counter value.
  --this->quantity;
  // Checks if the quantity value is higher than 0.
  if (this->quantity > 0) {
    // Updates/sets the product's quantity number label to show the new value.
    this->ui->quantity_label->setText(QString::number(this->quantity));
    // Calculates the new product's total price.
    this->totalPrice = this->quantity * this->product.getPrice();
    // Updates the product's total price label to show the new value.    
    this->ui->totalPrice_label->setText(QString::number(this->totalPrice));
  } else {
    // Deletes the receipt element.
    this->deleteSelf();
  }
  // Emits the signal indicating that the product's quantity has changed.  
  emit this->quantityReduced(this->product);
  // Force the update of the receipt element ui.
  this->update();
}

QString ReceiptElement::content() {
  // Obtains the current receipt labels information.
  QString productName(this->product.getName().data());
  QString productPrice(QString::number(this->product.getPrice()));
  QString productQuantity(QString::number(this->quantity));
  QString totalPrice(QString::number(
      this->quantity * this->product.getPrice()));
  // String to containg the receipt element information in html format.
  QString formatedContent = "<table style='width: 100%; font-size: 12px;'>"
      "<tr>"
      "<td style='text-align: left;'>" + productName + "</td>"
      "<td style='text-align: right;'>" + productQuantity +
      " x " + productPrice + " ₡" + totalPrice + "</td>"
      "</tr>"
      "</table>\n";

  // Returns the formated receipt element information.
  return formatedContent;
}
