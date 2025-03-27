#ifndef EXPENSELABEL_H
#define EXPENSELABEL_H

#include <QWidget>

namespace Ui {
class ExpenseLabel;
}

class ExpenseLabel : public QWidget {
  Q_OBJECT

public:
  explicit ExpenseLabel(QWidget *parent = nullptr
      , const QString expenseName = QString()
      , const QString expenseTime = QString()
      , const double expenseAmount = 0);
  ~ExpenseLabel();
  
public:
  double getAmount() {return this->amount;};

private:
  Ui::ExpenseLabel *ui;
  const double amount;
};

#endif // EXPENSELABEL_H
