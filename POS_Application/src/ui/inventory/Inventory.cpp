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
  this->ui->mainLayout->addWidget(this->catalogStack, 1, 1);
  
  // // Updates for the first time the diaply to show the first index page.
  // this->refreshDisplay(this->itemsPerPage);
  // this->setupConnections();
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
  this->catalogStack->setCurrentIndex(0);
  this->ui->productsCatalog_button->setChecked(true);
  this->ui->categoriesCatalog_button->setChecked(false);
  this->ui->suppliesCatalog_button->setChecked(false);
  
  this->ui->productsCatalog_widget->setStyleSheet(
      "QWidget { background-color: rgb(0, 153, 73);}");
  this->ui->categoriesCatalog_widget->setStyleSheet(
      "QWidget { background-color: transparent;}");
  this->ui->suppliesCatalog_widget->setStyleSheet(
      "QWidget { background-color: transparent};");
}

void Inventory::on_categoriesCatalog_button_clicked() {
  this->catalogStack->setCurrentIndex(1);
  
  this->ui->categoriesCatalog_button->setChecked(true);
  this->ui->productsCatalog_button->setChecked(false);
  this->ui->suppliesCatalog_button->setChecked(false);
  
  
  this->ui->categoriesCatalog_widget->setStyleSheet(
      "QWidget { background-color: rgb(0, 153, 73);}");
  this->ui->productsCatalog_widget->setStyleSheet(
      "QWidget { background-color: transparent;}");
  this->ui->suppliesCatalog_widget->setStyleSheet(
      "QWidget { background-color: transparent};");
}

void Inventory::on_suppliesCatalog_button_clicked() {
  this->catalogStack->setCurrentIndex(2);
  this->ui->productsCatalog_button->setChecked(false);
  this->ui->categoriesCatalog_button->setChecked(false);
  this->ui->suppliesCatalog_button->setChecked(true);
  
  this->ui->productsCatalog_widget->setStyleSheet(
      "QWidget { background-color: transparent;}");
  this->ui->categoriesCatalog_widget->setStyleSheet(
      "QWidget { background-color: transparent;}");
  this->ui->suppliesCatalog_widget->setStyleSheet(
      "QWidget { background-color: rgb(0, 153, 73);}");
}

