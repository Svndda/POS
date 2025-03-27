#include "expensedialog.h"
#include "ui_expensedialog.h"

ExpenseDialog::ExpenseDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExpenseDialog) {
  ui->setupUi(this);
}

ExpenseDialog::~ExpenseDialog() {
  delete ui;
}

void ExpenseDialog::on_acceptExpense_button_clicked() {
  const QString expenseName = this->ui->expenseName_lineEdit->text();
  const double expenseCost = this->ui->expensePrice_doubleSpinBox->value();
  emit this->expenseCreated(expenseName, expenseCost);
  this->accept();
}

void ExpenseDialog::on_cancelExpense_button_clicked() {
  this->reject();
}
