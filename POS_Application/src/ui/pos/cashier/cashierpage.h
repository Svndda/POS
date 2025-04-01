#ifndef CASHIERPAGE_H
#define CASHIERPAGE_H

#include <QWidget>

#include "expenselabel.h"
#include "incomelabel.h"
#include "model.h"

namespace Ui {
class CashierPage;
}

class CashierPage : public QWidget {
  Q_OBJECT
  
private:
  Ui::CashierPage *ui;
  Model& model;       ///< Reference to the POS_Model singleton.
  QVector<ExpenseLabel*> expenses;
  QVector<IncomeLabel*> incomes;
  
public:
  explicit CashierPage(QWidget *parent = nullptr
      , Model &appModel = Model::getInstance());
  
  ~CashierPage();
private:
  double obtainTotalAmount();
  void reloadCashierElements();
  
private slots:  
  void on_openCashier_button_clicked();
  void on_closeCashier_button_clicked();
  void on_addExpense_button_clicked();
  void handleCreatedExpense(const QString expenseName
      , const double expensePrice);
  
public slots:
  void addProcessedReceipt();
};

#endif // CASHIERPAGE_H
