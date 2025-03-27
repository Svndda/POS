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
  // Creates the widget stack to store the current orders.
  this->pagesStack = new QStackedWidget();
  
  BillingPage* billingPage = new BillingPage(this, this->model);
  CashierPage* cashierPage = new CashierPage(this, this->model);
  
  this->pagesStack->addWidget(billingPage);
  this->pagesStack->addWidget(cashierPage);
  this->switchPage(1);
  
  this->ui->pagesContentLayout->addWidget(this->pagesStack);
  
  this->connect(this->ui->billing_button, &QPushButton::clicked
      , this, &Pos::on_billing_button_clicked);
  
  this->connect(this->ui->cashier_button, &QPushButton::clicked
      , this, &Pos::on_cashier_button_clicked);
  
  this->connect(billingPage, &BillingPage::orderProcessed
      , cashierPage, &CashierPage::addProcessedReceipt);
}

void Pos::on_billing_button_clicked() {
  if (this->pagesStack->currentIndex() != 0) {
    this->switchPage(0);
  }
}

void Pos::on_cashier_button_clicked() {
  if (this->pagesStack->currentIndex() != 1) {
    this->switchPage(1);
  }
}

void Pos::switchPage(const size_t index) {
  // Set the current catalog page
  this->pagesStack->setCurrentIndex(index);
  
  // Create lists for buttons and corresponding widgets.
  QVector<QPushButton*> buttons = {
      this->ui->billing_button,
      this->ui->cashier_button,
  };
  
  QVector<QWidget*> widgets = {
      this->ui->billing_widget,
      this->ui->cashier_widget,
  };
  
  // Iterate over the buttons and widgets to update their states
  for (int i = 0; i < buttons.size(); ++i) {
    const bool isSelected = (i == index);
    buttons[i]->setChecked(isSelected);
    // Use a conditional to set the style sheet: green for selected, transparent otherwise.
    widgets[i]->setStyleSheet(
        QString("QWidget { background-color: %1; }")
            .arg(isSelected ? "rgb(0, 153, 73)" : "transparent")
    );
  }
}
