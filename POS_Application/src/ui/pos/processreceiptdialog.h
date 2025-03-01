#ifndef PROCESSRECEIPTDIALOG_H
#define PROCESSRECEIPTDIALOG_H

#include <QDialog>

namespace Ui {
class ProcessReceiptDialog;
}

class ProcessReceiptDialog : public QDialog {
  Q_OBJECT
  
private:
  Ui::ProcessReceiptDialog *ui;
  const double receiptAmount = 0;
  double receivedAmount = 0;
  
public:
  explicit ProcessReceiptDialog(QWidget* parent = nullptr
      , const double amount = 0);
  ~ProcessReceiptDialog();
  
private slots:
  void updateChangeLabel(double value);
};

#endif // PROCESSRECEIPTDIALOG_H
