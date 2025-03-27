#ifndef PROCESSRECEIPTDIALOG_H
#define PROCESSRECEIPTDIALOG_H

#include <QDialog>

#include "order.h"

namespace Ui {
class ProcessOrderDialog;
}

class ProcessOrderDialog : public QDialog {
  Q_OBJECT
  
private:
  Ui::ProcessOrderDialog *ui;
  Order& order;
  
public:
  explicit ProcessOrderDialog(QWidget* parent, Order& orderToProcess);
  ~ProcessOrderDialog();
  
private slots:
  void on_accept_button_clicked();
  // void on_paymentMethod_changed(int index);
  void updateChangeLabel(double value);
};

#endif // PROCESSRECEIPTDIALOG_H
