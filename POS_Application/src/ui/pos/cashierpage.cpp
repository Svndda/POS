#include "cashierpage.h"
#include "ui_cashierpage.h"

CashierPage::CashierPage(QWidget *parent, POS_Model& appmodel)
    : QWidget(parent)
    , ui(new Ui::CashierPage)
    , model(appmodel) {
  ui->setupUi(this);
  
  QVBoxLayout* layout = new QVBoxLayout();
  this->ui->scrollAreaWidgetContents->setLayout(layout);
}

CashierPage::~CashierPage() {
  delete ui;
}
