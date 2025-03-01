#include "pos.h"
#include "ui_pos.h"

#include <QMessageBox>

#include "billingpage.h"
#include "cashierpage.h"

Pos::Pos(QWidget *parent, POS_Model& appModel)
    : QWidget(parent)
    , ui(new Ui::Pos)
    , model(appModel) {
  ui->setupUi(this);
  this->setupDisplay();
}

Pos::~Pos() {
  delete ui;
}

void Pos::setupDisplay() {
  // Creates the widget stack to store the current receipts.
  this->pagesStack = new QStackedWidget();
  
  BillingPage* billingPage = new BillingPage(this, this->model);
  CashierPage* cashierPage = new CashierPage(this, this->model);
  
  this->pagesStack->addWidget(billingPage);
  this->pagesStack->addWidget(cashierPage);
  this->pagesStack->setCurrentIndex(0);
  
  this->ui->pagesContentLayout->addWidget(this->pagesStack);
  
  this->connect(this->ui->billing_button, &QPushButton::clicked
      , this, &Pos::on_billing_button_clicked);
  
  this->connect(this->ui->cashier_button, &QPushButton::clicked
      , this, &Pos::on_cashier_button_clicked);
  
  // // Creates the product selection buttons on the scroll view, with six elements
  // // per row.
  // this->createSelectProductButtons(5);
  // // Sets the receipts layout container alignment to the top.
  // this->ui->billingLayout->setAlignment(Qt::AlignTop);
  
  // // Creates a new receipt.
  // Receipt* receipt = new Receipt(this, 0);
  // // Adds the new receipt to the layout.
  // this->ui->billingLayout->addWidget(this->receipStack);
  // // Adds the nre receipt to the stak.
  // this->receipStack->addWidget(receipt);
  
  // // Creates a layout to store the receipt selection buttons of the pos.
  // QHBoxLayout* layout = new QHBoxLayout();
  // // Sets the layout aligment to the left.
  // layout->setAlignment(Qt::AlignLeft);
  // // Sets the layout content margins to zero.
  // layout->setContentsMargins(0, 0, 0, 0);
  // // Sets the layout elements spacing to one.
  // layout->setSpacing(1);
  // this->ui->openedReceiptsArea->setLayout(layout);
  
  // // Creates and adds up a receipt selection button into the layout.
  // ReceiptSelectionButton* receiptSelectionButton
  //     = new ReceiptSelectionButton(this, this->openedReceipts);
  // layout->addWidget(receiptSelectionButton);
}

void Pos::on_billing_button_clicked() {
  if (this->pagesStack->currentIndex() != 0) {
    this->pagesStack->setCurrentIndex(0);
  }
}

void Pos::on_cashier_button_clicked() {
  if (this->pagesStack->currentIndex() != 1) {
    this->pagesStack->setCurrentIndex(1);
  }
}
