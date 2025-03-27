#include "expenselabel.h"
#include "ui_expenselabel.h"

ExpenseLabel::ExpenseLabel(QWidget *parent
    , const QString expenseName
    , const QString expenseTime
    , const double expenseAmount)
    : QWidget(parent)
    , ui(new Ui::ExpenseLabel)
    , amount(expenseAmount) {
  ui->setupUi(this);
  this->ui->name_label->setText(expenseName);
  this->ui->time_label->setText("Hora: " + expenseTime);
  this->ui->amount_label->setText("Monto: ₡" + QString::number(expenseAmount, 'f', 2));
  this->update();
}

ExpenseLabel::~ExpenseLabel() {
  delete ui;
}
