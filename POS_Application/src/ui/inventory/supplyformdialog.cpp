// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
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
  
  this->ui->measureUnit_comboBox->addItem("Kilogramos");
  this->ui->measureUnit_comboBox->addItem("Gramos");
  this->ui->measureUnit_comboBox->addItem("Unidades");
  this->ui->measureUnit_comboBox->addItem("Porciones");
    
  // Sets the category line edit text with the given string in the constructor.
  this->ui->name_lineEdit->setText(this->newSupply.getName().data());
  this->ui->quantity_spinBox->setValue(this->newSupply.getQuantity());
  this->ui->measureUnit_comboBox->setCurrentText(
      this->newSupply.getMeasure().data());
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
  
  const SupplyItem supply(name, quantity, measureUnit);
  this->newSupply = supply;
  this->accept();
}

void SupplyFormDialog::on_cancelCategory_button_clicked() {
  this->reject();
}
