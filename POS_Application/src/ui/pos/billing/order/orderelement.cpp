#include "orderelement.h"
#include "ui_orderelement.h"

OrderElement::OrderElement(QWidget *parent, const Product& myProduct)
    : QWidget(parent)
    , ui(new Ui::OrderElement)
    , product(myProduct)
    , quantity(1)
    , totalPrice(myProduct.getPrice()) {
  ui->setupUi(this);
  this->refreshDisplay();
  this->setupConnections();
}

OrderElement::~OrderElement() {
  delete ui;
}

void OrderElement::refreshDisplay() {
  // Updates/sets the ui labels to contain the product's information.
  this->ui->productName_label->setText(this->product.getName().data());
  QString productPrice = QString::number(this->product.getPrice(), 'f', 2);
  this->ui->productPrice_label->setText(productPrice);
  this->ui->totalPrice_label->setText(productPrice);
}

void OrderElement::setupConnections() {
  // Connects the ui elements to their slot functions.
  this->connect(this->ui->incrementQuantity_button, &QPushButton::clicked
      , this, &OrderElement::incrementQuantity_button_clicked);
  this->connect(this->ui->reduceQuantity_button, &QPushButton::clicked
      , this, &OrderElement::reduceQuantity_button_clicked);
}

void OrderElement::deleteSelf() {
  // Checks if the order ui has a parent widget.
  if (this->parentWidget()) {
    // Obtains the layout of the parent container.
    QLayout *layout = this->parentWidget()->layout();
    // Checks if the layout pointer isn't null.
    if (layout) {
      // Remove the order widget ui from the layout.
      layout->removeWidget(this);
    }
  }
  // Force the deletion of the order ui elements.
  this->deleteLater();
}

void OrderElement::incrementQuantity_button_clicked() {
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
  // Force the update of the order element ui.
  this->update();
}

void OrderElement::reduceQuantity_button_clicked() {
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
    // Deletes the order element.
    this->deleteSelf();
  }
  // Emits the signal indicating that the product's quantity has changed.  
  emit this->quantityReduced(this->product);
  // Force the update of the order element ui.
  this->update();
}

std::pair<Product, size_t> OrderElement::information() {
  // Returns the formated order element information.
  return std::pair(this->product, this->quantity);
}
