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
  // String to store the recipt content in html format.
  QString htmlContent;
  // String containing the business's name.
  QString businessName = "Macana's Place";
  // String containing the receipt's id.
  QString receiptID = QString::number(this->ID);
  // String containing the user that created the receipt.
  QString user = "Aaron Sanchez";
  // String that containg the order's number of the cage.
  QString orderNumber = QString::number(0);
  
  // Obtains the current formated date.
  QDateTime currentDateTime = QDateTime::currentDateTime();
  QString formattedDateTime = currentDateTime.toString("dd/MM/yyyy hh:mm:ss");
  
  // Format the receipt header information.
  QString receiptInfo = "";
  receiptInfo += "<div class='header'>" + businessName + "</div>\n"
      + "<div class='info'>N° Recibo: " + receiptID + "</div>\n"
      + "<div class='info'>" + formattedDateTime + "</div>\n"
      + "<div class='info'>Usuario: " + user + "</div>\n"
      + "<div class='info'>Orden: " + orderNumber + "</div>\n"
      + "<div class='line'></div>\n";
  
  // String to store the products information of the receipt content.
  QString receiptContent = "\n<table>\n";
  
  // Obtains the receipt elements layout.
  QLayout* receiptLayout = this->ui->receipt_WidgetContents->layout();
  // Iterates through the layout's elements.
  for (size_t index = 0;  index < receiptLayout->count(); ++index) {
    // Obtains the indexed layout element.
    QLayoutItem* productSpace = receiptLayout->itemAt(index);
    // Checks if the layout element is a widget.
    if (productSpace->widget()) {
      // Obtains the widget pointer and cast it into a receipt element.
      QWidget* widget = productSpace->widget();
      ReceiptElement* element = qobject_cast<ReceiptElement*>(widget);
      // Obtains the receipt element content in html format.
      receiptContent += element->content();
    }
  }
  
  // String to store the total receipt price value.
  QString totalReceiptPrice(QString::number(this->totalPrice));
  // Adds up the total receipt price informatio into the receipt content string.
  receiptContent += "<div class='total'> TOTAL:\t\t₡"
      + totalReceiptPrice + "\n"
      + "<div class='line'></div>\n";
  
  // Adds up the two parts of the receipt content.
  htmlContent = receiptInfo + receiptContent;
  

  std::cout << htmlContent.toStdString();
  // Returns the receipt content in a html formatted string.
  return htmlContent;
}
