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
  this->ui->setupUi(this);
  
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
                , &SupplyFormDialog::on_acceptSupply_button_clicked);
  // Connects the slot of the cancel button of the QDialog.
  this->connect(ui->cancelSupply_button
                , &QPushButton::clicked
                , this
                , &SupplyFormDialog::on_cancelSupply_button_clicked);
}

SupplyFormDialog::~SupplyFormDialog() {
  delete this->ui;
}

SupplyItem SupplyFormDialog::getNewSupply() {
  // Returns the supply created by the user.
  return this->newSupply;
}

void SupplyFormDialog::on_acceptSupply_button_clicked() {
  // Gets the provided name for the supply.
  const std::string name = this->ui->name_lineEdit->text().toStdString();
  // Gets the quantity for the supply.
  const uint64_t quantity = this->ui->quantity_spinBox->value();
  // Gets the measure unit for the supply.
  const std::string measureUnit =
      this->ui->measureUnit_comboBox->currentText().toStdString();
  // Creates a new supply object with the given information.
  this->newSupply = SupplyItem(name, quantity, measureUnit);
  // Indicates that the dialog has ended accepted.
  this->accept();
}

void SupplyFormDialog::on_cancelSupply_button_clicked() {
  // Indicates that the dialog has ended rejected.
  this->reject();
}
