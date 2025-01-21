#include "supplies.h"

#include <vector>
#include <string>
#include <supplyitem.h>

#include "posmodel.h"

#include "ui_supplies.h"

Supplies::Supplies(QWidget *parent, POS_Model& model)
    : Inventory(parent, model)
    , ui(new Ui::Supplies) {
  ui->setupUi(this);
}

Supplies::~Supplies() {
  delete ui;
}

void Supplies::refreshDisplay(const size_t pageItems) {
  
}

void Supplies::setupConnections() {
  
}

void Supplies::refreshCategoriesDisplay(std::vector<std::string> visibleCategories
    , const size_t items) {
  
}

void Supplies::on_addSupply_button_clicked() {
  
}

void Supplies::on_nextProductPage_button_clicked() {
  
}

void Supplies::on_previousProductPage_button_clicked() {
  
}

void Supplies::on_deleteProduct_button_clicked() {
  
}

void Supplies::on_editProduct_button_clicked() {
  
}

void Supplies::on_categories_button_clicked() {
  // Emit the signal for the app controller to handle/receive it.  
  emit this->categories_button_signal();
}

void Supplies::on_supplies_button_clicked() {
  
}

void Supplies::on_products_button_clicked() {
  // Emit the signal for the app controller to handle/receive it.  
  emit this->products_button_signal();
}


