#include <sstream>
#include <string>

#include "productformdialog.h"
#include "ui_productformdialog.h"

ProductFormDialog::ProductFormDialog(QWidget *parent
    , std::map<std::string, std::vector<Product>> drinks
    , std::map<std::string, std::vector<Product>> dishes)
    : QDialog(parent)
    , registeredDrinks(drinks)
    , registeredDishes(dishes)
    , ui(new Ui::ProductFormDialog) {
  ui->setupUi(this);
  std::vector<QString> productCategories;
  
  for (const auto& category : this->registeredDrinks) {
    productCategories.push_back(category.first.data());
  }
  
  for (const auto& category : this->registeredDishes) {
    productCategories.push_back(category.first.data());
  }
  
  for (const auto& category : productCategories) {
    this->ui->productCategory_comboBox->addItem(category);
  }
  
  this->createdProduct = Product();
  
  connect(ui->acceptProduct_button
          , &QPushButton::clicked
          , this
          , &ProductFormDialog::on_acceptProduct_button_clicked);
  connect(ui->cancelProduct_button
          , &QPushButton::clicked
          , this
          , &ProductFormDialog::on_cancelProduct_button_clicked);
}

void ProductFormDialog::on_cancelProduct_button_clicked() {
  this->reject();
}

void ProductFormDialog::on_acceptProduct_button_clicked() {
  QString productCategory = this->ui->productCategory_comboBox->currentText();
  QString productName = this->ui->productName_lineEdit->text();
  QString productIngredients = this->ui->productIngredients_lineEdit->text();
  double productPrice = this->ui->productPrice_doubleSpinBox->value();
  
  if (!productName.isEmpty()) {
    std::vector<SupplyItem> ingredients;
    std::istringstream buffer(productIngredients.toStdString());
    std::string item;
    
    while(std::getline(buffer, item, ',')) {
      std::istringstream itemBuffer(item);
      std::string name = "";
      uint64_t quantity = 0;
      itemBuffer >> name >> quantity;
      ingredients.emplace_back(name, quantity);
    }
    qDebug() << productName << productCategory << productPrice << productIngredients;
    this->createdProduct = Product(1,
        productName.toStdString(), ingredients, productPrice);
    this->accept();
  }
}

QString ProductFormDialog::getProductCategory() {
  return this->ui->productCategory_comboBox->currentText();
}

Product ProductFormDialog::getProduct() {
  return this->createdProduct;
}

ProductFormDialog::~ProductFormDialog() {
  delete ui;
}
