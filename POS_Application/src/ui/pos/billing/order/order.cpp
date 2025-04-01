#include "order.h"
#include "ui_order.h"

#include <QDateTime>

#include "orderelement.h"

Order::Order(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Order) {
  ui->setupUi(this);
  this->setupOrderDisplay();
}

Order::~Order() {
  delete ui;
}

void Order::setupOrderDisplay() {
  // Creates a layot to contain the order scroll view's elements.
  QVBoxLayout* layout = new QVBoxLayout(this->ui->order_WidgetContents);
  // Sets the layout name.
  layout->setObjectName("orderElementsLayout");
  // Sets the layout aligment to the top.
  layout->setAlignment(Qt::AlignTop);
  // Sets the layut content margin to zero.
  layout->setContentsMargins(0, 0, 0, 0);
  // Sets the layout's widgets space to zero.
  layout->setSpacing(0);
  // Sets layout into the scroll view content's widget.
  this->ui->order_WidgetContents->setLayout(layout);
}

void Order::addProduct(const Product &product) {
  // Creates a new ui object to store and display the new product information
  // on screen.
  OrderElement* productOnOrder = new OrderElement(this, product);
  
  // Connects the object reduce and add quantity signals to update the order
  // total price.
  this->connect(productOnOrder, &OrderElement::quantityIncreased
      , this, &Order::increaseOrderPrice);
  this->connect(productOnOrder, &OrderElement::quantityReduced
      , this, &Order::reduceOrderPrice);
  
  // Update the total order price, adding the price of the new product
  //  product.
  this->totalPrice += product.getPrice();
  qDebug() << "anadiendo el precio del producto: " << this->totalPrice;
  this->ui->totalOrderPrice_label->setText(
      QString::number(this->totalPrice, 'f', 2));
  // Insert the object into the order ui.
  this->ui->order_WidgetContents->layout()->addWidget(productOnOrder);
  // Force the update of the ui.
  this->update();
}

void Order::increaseOrderPrice(const Product &product) {
  // Adds up the product price.
  this->totalPrice += product.getPrice();
  // Updates the order ui price.
  this->ui->totalOrderPrice_label->setText(
      QString::number(this->totalPrice, 'f', 2));
  this->update();
}

void Order::reduceOrderPrice(const Product &product) {
  // Substract the product price;
  this->totalPrice -= product.getPrice();
  // Update the order ui price.
  this->ui->totalOrderPrice_label->setText(
      QString::number(this->totalPrice, 'f', 2));
  this->update();
}

const std::vector<std::pair<Product, size_t>> Order::getProducts() const {
  // Vector of pair to store the products and their quantity.
  std::vector<std::pair<Product, size_t>> orderProducts;
  // Obtains the layout of the order's elements.
  QLayout* orderLayout = this->ui->order_WidgetContents->layout();
  
  if (orderLayout) {
    // Transverse all the widgets contained in the layout.
    for (int i = 0; i < orderLayout->count(); ++i) {
      QWidget* widget = orderLayout->itemAt(i)->widget();
      if (widget) {
        OrderElement* element = qobject_cast<OrderElement*>(widget);
        if (element) {
          // Emplace the information of the order's element.
          orderProducts.emplace_back(element->information());
        }
      }
    }
  }
  
  return orderProducts;
}
