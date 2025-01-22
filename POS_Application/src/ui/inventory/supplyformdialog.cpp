#include "supplyformdialog.h"
#include "ui_supplyformdialog.h"

#include <vector>
#include <supplyitem.h>

SupplyFormDialog::SupplyFormDialog(QWidget *parent
    , std::vector<SupplyItem> supplies
    , SupplyItem supply)
    : QDialog(parent)
    , ui(new Ui::SupplyFormDialog)
    , existingSupplies(supplies)
    , newSupply(supply) {
  ui->setupUi(this);
  
  // Sets the category line edit text with the given string in the constructor.
  this->ui->name_lineEdit->setText(this->newSupply.getName().data());
  this->ui->quantity_spinBox->setValue(this->newSupply.getQuantity());
  // Connects the slot of the accept button of the QDialog.
  this->connect(ui->acceptSupply_button
                , &QPushButton::clicked
                , this
                , &SupplyFormDialog::on_acceptCategory_button_clicked);
  // Connects the slot of the cancel button of the QDialog.
  this->connect(ui->cancelSupply_button
                , &QPushButton::clicked
                , this
                , &SupplyFormDialog::on_cancelCategory_button_clicked);
}

SupplyFormDialog::~SupplyFormDialog() {
  delete ui;
}

SupplyItem SupplyFormDialog::getNewSupply() {
  return this->newSupply;
}

void SupplyFormDialog::on_acceptCategory_button_clicked() {
  const std::string name = this->ui->name_lineEdit->text().toStdString();
  const uint64_t quantity = this->ui->quantity_spinBox->value();
  const std::string measureUnit =
      this->ui->measureUnit_comboBox->currentText().toStdString();
  
  const SupplyItem supply(name, quantity);
  auto it = std::find(this->existingSupplies.begin()
      , this->existingSupplies.end()
      , supply);
  
  if (it == this->existingSupplies.end()) {
    this->newSupply = supply;
    this->accept();
  } else {
    qDebug() << "Ya se encuentra registrado el suministro.";
  }
}

void SupplyFormDialog::on_cancelCategory_button_clicked() {
  this->reject();
}
