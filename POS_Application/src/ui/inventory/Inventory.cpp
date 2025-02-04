// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include <QMessageBox>
#include <QStackedWidget>

#include "inventory.h"
#include "ui_inventory.h"

#include "categoriescatalog.h"
#include "productscatalog.h"
#include "suppliescatalog.h"
#include "posmodel.h"

Inventory::Inventory(QWidget *parent, POS_Model& appModel)
    : QWidget(parent)
    , ui(new Ui::Inventory)
    , model(appModel) {
  this->ui->setupUi(this);
  this->catalogStack = new QStackedWidget();
  this->catalogStack->setStyleSheet(
      "QStackedWidget {"
      " background-color: white;}");
  
  ProductsCatalog* products = new ProductsCatalog(this, this->model);  
  CategoriesCatalog* categories = new CategoriesCatalog(this, this->model);
  SuppliesCatalog* supplies = new SuppliesCatalog(this, this->model);
  
  this->catalogStack->addWidget(products);
  this->catalogStack->addWidget(categories);
  this->catalogStack->addWidget(supplies);

  this->catalogStack->setCurrentIndex(0);
  this->ui->mainLayout->addWidget(this->catalogStack, 0, 1);
  
  // // Updates for the first time the diaply to show the first index page.
  // this->refreshDisplay(this->itemsPerPage);
  this->setupConnections();
}

Inventory::~Inventory() {
  delete ui;
}

void Inventory::setupConnections() {
  // Connects the function that handles the products catalog button.
  this->connect(this->ui->productsCatalog_button, &QPushButton::clicked
                , this, &Inventory::on_productsCatalog_button_clicked);
  // Connects the function that handles the categories catalog button.
  this->connect(this->ui->categoriesCatalog_button, &QPushButton::clicked
                , this, &Inventory::on_categoriesCatalog_button_clicked);
  // Connects the function that handles the supplies catalog button.
  this->connect(this->ui->suppliesCatalog_button, &QPushButton::clicked
                , this, &Inventory::on_suppliesCatalog_button_clicked);
}

void Inventory::refreshDisplay(const size_t pageItems) {
}

void Inventory::on_productsCatalog_button_clicked() {
  this->switchCatalog(0);
}

void Inventory::on_categoriesCatalog_button_clicked() {
  this->switchCatalog(1);
}

void Inventory::on_suppliesCatalog_button_clicked() {
  this->switchCatalog(2);
}

void Inventory::switchCatalog(const size_t index) {
  // Set the current catalog page
  this->catalogStack->setCurrentIndex(index);
  
  // Create lists for buttons and corresponding widgets.
  QVector<QPushButton*> buttons = {
      this->ui->productsCatalog_button,
      this->ui->categoriesCatalog_button,
      this->ui->suppliesCatalog_button
  };
  
  QVector<QWidget*> widgets = {
      this->ui->productsCatalog_widget,
      this->ui->categoriesCatalog_widget,
      this->ui->suppliesCatalog_widget
  };
  
  // Iterate over the buttons and widgets to update their states
  for (int i = 0; i < buttons.size(); ++i) {
    const bool isSelected = (i == index);
    buttons[i]->setChecked(isSelected);
    // Use a conditional to set the style sheet: green for selected, transparent otherwise.
    widgets[i]->setStyleSheet(
        QString("QWidget { background-color: %1; }")
            .arg(isSelected ? "rgb(0, 153, 73)" : "transparent")
        );
  }
}

