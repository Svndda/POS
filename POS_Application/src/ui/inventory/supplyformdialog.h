#ifndef SUPPLYFORMDIALOG_H
#define SUPPLYFORMDIALOG_H

#include <QDialog>
#include <vector>
#include "supplyitem.h"

namespace Ui {
class SupplyFormDialog;
}

class SupplyFormDialog : public QDialog {
  Q_OBJECT
  
private:
  Ui::SupplyFormDialog* ui;
  std::vector<SupplyItem> existingSupplies;
  SupplyItem newSupply;

public:
  explicit SupplyFormDialog(QWidget *parent
      , std::vector<SupplyItem> supplies, SupplyItem supply);
  ~SupplyFormDialog();
  
public:
  SupplyItem getNewSupply();
  
private slots:
  void on_acceptCategory_button_clicked();
  void on_cancelCategory_button_clicked();
};

#endif // SUPPLYFORMDIALOG_H
