#ifndef EXPENSEDIALOG_H
#define EXPENSEDIALOG_H

#include <QDialog>

namespace Ui {
class ExpenseDialog;
}

class ExpenseDialog : public QDialog {
  Q_OBJECT
  
private:
  Ui::ExpenseDialog *ui;
  
public:
  explicit ExpenseDialog(QWidget *parent = nullptr);
  ~ExpenseDialog();
  
private slots:
  void on_acceptExpense_button_clicked();
  void on_cancelExpense_button_clicked();
signals:
  void expenseCreated(const QString expenseName, const double expenseCost);
};

#endif // EXPENSEDIALOG_H
