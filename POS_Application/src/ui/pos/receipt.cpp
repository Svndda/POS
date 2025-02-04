#include "receipt.h"
#include "ui_receipt.h"

#include "receiptelement.h"

Receipt::Receipt(QWidget *parent, const size_t myID)
    : QWidget(parent)
    , ui(new Ui::Receipt)
    , ID(myID) {
  ui->setupUi(this);
  
  QVBoxLayout* layout = new QVBoxLayout(this->ui->receipt_WidgetContents);
  layout->setObjectName("receiptElementsLayout");
  layout->setAlignment(Qt::AlignTop);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  this->ui->receipt_WidgetContents->setLayout(layout);
}

Receipt::~Receipt() {
  delete ui;
}

void Receipt::addProduct(const Product &product) {
  ReceiptElement* productOnReceipt = new ReceiptElement(this, product);
  this->ui->receipt_WidgetContents->layout()->addWidget(productOnReceipt);
  this->update();
}
