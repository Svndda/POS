#ifndef RECEIPTSELECTIONBUTTON_H
#define RECEIPTSELECTIONBUTTON_H

#include <QWidget>

namespace Ui {
class ReceiptSelectionButton;
}

class ReceiptSelectionButton : public QWidget {
  Q_OBJECT
  
private:
  Ui::ReceiptSelectionButton *ui;
  const size_t receiptNumber = 0;
  
public:
  explicit ReceiptSelectionButton(QWidget *parent = nullptr
      , const size_t receiptNumber = 0);
  ~ReceiptSelectionButton();
  
signals:
  void on_ReceiptSelectionButton_clicked();    
  
private slots:
  void on_button_clicked();
};

#endif // RECEIPTSELECTIONBUTTON_H
