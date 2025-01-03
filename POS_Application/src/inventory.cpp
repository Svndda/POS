#include "inventory.h"
#include "ui_inventory.h"

Inventory::Inventory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InventoryPage) {
  ui->setupUi(this);
}

Inventory::~Inventory() {
  delete ui;
}
