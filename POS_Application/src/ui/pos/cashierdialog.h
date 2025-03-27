#ifndef CASHIERDIALOG_H
#define CASHIERDIALOG_H

#include <QDialog.h>

#include "expenselabel.h"
#include "incomelabel.h"
#include "receipt.h"

namespace Ui {
class CashierDialog;
}

class CashierDialog : public QDialog {
  Q_OBJECT
  
private:
  Ui::CashierDialog *ui;
  const QVector<IncomeLabel*>& incomes;
  const QVector<ExpenseLabel*>& expenses;
  const std::vector<Receipt>& receipts;
  
public:
  explicit CashierDialog(QWidget *parent
      , const QVector<IncomeLabel*>& cashierIncomes
      , const QVector<ExpenseLabel*>& cashierExpenses
      , const std::vector<Receipt>& cashierReceipts);
  ~CashierDialog();

private:
  
  void printCashierClosing();
};

#endif // CASHIERDIALOG_H
