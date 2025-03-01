#include "receipt.h"
#include "ui_receipt.h"

#include <QDateTime>

#include "receiptelement.h"

Receipt::Receipt(QWidget *parent, const size_t myID)
    : QWidget(parent)
    , ui(new Ui::Receipt)
    , ID(myID) {
  ui->setupUi(this);
  this->setupReceiptDisplay();
}

Receipt::~Receipt() {
  delete ui;
}

void Receipt::setupReceiptDisplay() {
  // Creates a layot to contain the receipt scroll view's elements.
  QVBoxLayout* layout = new QVBoxLayout(this->ui->receipt_WidgetContents);
  // Sets the layout name.
  layout->setObjectName("receiptElementsLayout");
  // Sets the layout aligment to the top.
  layout->setAlignment(Qt::AlignTop);
  // Sets the layut content margin to zero.
  layout->setContentsMargins(0, 0, 0, 0);
  // Sets the layout's widgets space to zero.
  layout->setSpacing(0);
  // Sets layout into the scroll view content's widget.
  this->ui->receipt_WidgetContents->setLayout(layout);
}

void Receipt::addProduct(const Product &product) {
  this->products.emplace_back(product);
  // Creates a new ui object to store and display the new product information
  // on screen.
  ReceiptElement* productOnReceipt = new ReceiptElement(this, product);
  
  // Connects the object reduce and add quantity signals to update the receipt
  // total price.
  this->connect(productOnReceipt, &ReceiptElement::quantityIncreased
      , this, &Receipt::increaseReceiptPrice);
  this->connect(productOnReceipt, &ReceiptElement::quantityReduced
      , this, &Receipt::reduceReceiptPrice);
  
  // Update the total receipt price, adding the price of the new product
  //  product.
  this->totalPrice += product.getPrice();
  qDebug() << "anadiendo el precio del producto: " << this->totalPrice;
  this->ui->totalReceiptPrice_label->setText(QString::number(this->totalPrice));
  // Insert the object into the receipt ui.
  this->ui->receipt_WidgetContents->layout()->addWidget(productOnReceipt);
  // Force the update of the ui.
  this->update();
}

void Receipt::increaseReceiptPrice(const Product &product) {
  // Adds up the product price.
  this->totalPrice += product.getPrice();
  // Udates the receipt ui price.
  this->ui->totalReceiptPrice_label->setText(QString::number(this->totalPrice));
  this->update();
}

void Receipt::reduceReceiptPrice(const Product &product) {
  // Substract the product price;
  this->totalPrice -= product.getPrice();
  // Update the receipt ui price.
  this->ui->totalReceiptPrice_label->setText(QString::number(this->totalPrice));
  this->update();
}

QString Receipt::htmlContent() {
  QString htmlContent;
  
  // Header Info Variables
  QString businessName = "Macana's Place";
  QString receiptID = QString::number(this->ID);
  QString user = "Aaron Sanchez";
  QString orderNumber = QString::number(0);
  QString formattedDateTime = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
  
  // Header + Styles
  htmlContent = R"(
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta charset="UTF-8">
      <title>Restaurant Bill - Macana's Place</title>
      <style>
        body { font-family: Arial, sans-serif; width: 100mm; margin: 0 auto;
        padding: 5mm; background-color: #f9f9f9; border: 1px solid #ddd; }
        .header { text-align: center; font-weight: bold; font-size: 40px;
        margin-bottom: 10px; }
        .section, .line { font-size: 30px; }
        .line { display: flex; justify-content: space-between; }
        .dashed { border-top: 10px dashed #000; margin: 5px 10; }
        .total { font-weight: bold; font-size: 16px; margin-top: 5px; }
      </style>
    </head>
    <body>
  )";
  
  // Header Content
  htmlContent.append(QString(R"(
    <div class='header'>%1</div>
    <div class='section'>N° Recibo: %2</div>
    <div class='section'>Fecha: %3</div>
    <div class='section'>Usuario: %4</div>
    <div class='section'>Orden: %5</div>
    <div class='dashed'></div>
  )").arg(businessName, receiptID, formattedDateTime, user, orderNumber));
  
  // Products Table
  htmlContent.append("<table style='width:100%; font-size:30px;'>");
  
  QLayout* receiptLayout = this->ui->receipt_WidgetContents->layout();
  if (receiptLayout) {
    for (int i = 0; i < receiptLayout->count(); ++i) {
      QWidget* widget = receiptLayout->itemAt(i)->widget();
      if (widget) {
        ReceiptElement* element = qobject_cast<ReceiptElement*>(widget);
        if (element) {
          htmlContent.append(element->content()); // Assume element->content() returns proper HTML row <tr>...</tr>
        }
      }
    }
  }
  
  htmlContent.append("</table>"
      "<hr style=border: 1px solid black; width: 100%; margin: 5px 0;>");
  
  // Total Section
  QString totalReceiptPrice = QString::number(this->totalPrice, 'f', 2);
  htmlContent.append(QString(R"(
    <div class='total line'>
      <span>Total:</span>
      <span>₡%1</span>
    </div>
    <div class='dashed'></div>
  )").arg(totalReceiptPrice));
  
  // Footer Closure
  htmlContent.append("</body></html>");
  
  return htmlContent;
}
