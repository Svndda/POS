#include <sstream>
#include <string>

#include <QMessageBox>
#include "productformdialog.h"
#include "ui_productformdialog.h"

ProductFormDialog::ProductFormDialog(QWidget *parent
    , std::map<std::string, std::vector<Product>>& products
    , Product product
    , QString category)
    : QDialog(parent)
    , registeredProducts(products)
    , createdProduct(product)
    , productCategory(category)
    , ui(new Ui::ProductFormDialog) {
  ui->setupUi(this);
  std::vector<QString> productCategories;
  
  for (const auto& category : this->registeredProducts) {
    productCategories.push_back(category.first.data());
  }
  
  
  for (const auto& category : productCategories) {
    this->ui->productCategory_comboBox->addItem(category);
  }
  
  // Checks if product information were provided, if so, displays it.
  if (!(this->createdProduct == Product())) {
    this->setProductInfo(this->createdProduct, this->productCategory);
  }
  
  // Connects the slot of the accept button of the QDialog.
  connect(ui->acceptProduct_button
          , &QPushButton::clicked
          , this
          , &ProductFormDialog::on_acceptProduct_button_clicked);
  // Connects the slot of the cancel button of the QDialog.  
  connect(ui->cancelProduct_button
          , &QPushButton::clicked
          , this
          , &ProductFormDialog::on_cancelProduct_button_clicked);
}

QString ProductFormDialog::formatProductIngredients(
    const std::vector<SupplyItem>& ingredients) {
  // String that will contain the ingredients information formated
  //  to be displayed.
  QString formattedProductIngredients = "";
  
  // Iterates through all the ingredients contained in the vector.
  for (auto it = ingredients.begin(); it != ingredients.end(); ++it) {
    // Saves the ingredient's information.
    QString ingredientName(it->getName().data());
    QString ingredientQuantity = QString::number(it->getQuantity());
    // Stablish a separator between ingredients.
    if (it == ingredients.end() - 1) {
      formattedProductIngredients = QString(formattedProductIngredients
          + ingredientName.trimmed() + " " + ingredientQuantity);
    } else {
      formattedProductIngredients = QString(formattedProductIngredients
          + ingredientName.trimmed() + " " + ingredientQuantity + ", ");
    }
  }
  // Returns a string containing all the ingredients information formated to be
  // displayed.
  return formattedProductIngredients;
}

void ProductFormDialog::on_cancelProduct_button_clicked() {
  // Stablish that the QDialog has finished incorrectly.
  this->reject();
}

void ProductFormDialog::on_acceptProduct_button_clicked() {
  // Product's information provided by the user.
  QString productCategory = this->ui->productCategory_comboBox->currentText();
  QString productName = this->ui->productName_lineEdit->text();
  QString productIngredients = this->ui->productIngredients_lineEdit->text();
  double productPrice = this->ui->productPrice_doubleSpinBox->value();
  
  QRegularExpression regex(R"(^\s*\p{L}+\s+\d+(\s*,\s*\p{L}+\s+\d+)*\s*$)");
  // Checks if product name were provided.
  if (!productName.isEmpty()) {
    // Checks that the product ingredients match the stablished format.
    if (regex.match(productIngredients).hasMatch()) {
      // vector of product ingredients.
      std::vector<SupplyItem> ingredients;
      // buffer containing all the ingredients information.
      std::istringstream buffer(productIngredients.toStdString());
      // String to almacenate each ingredient information.
      std::string item;
      
      // Separates the different ingredients data from the string.
      while(std::getline(buffer, item, ',')) {
        // Stream for each ingredient.
        std::istringstream itemBuffer(item);
        // Ingredients data.
        std::string name = "";
        uint64_t quantity = 0;
        // Reads the ingredients data from the buffer.
        itemBuffer >> name >> quantity;
        // Emplace a new ingredients to the product's vector ingredients.
        ingredients.emplace_back(name, quantity);
      }
      // Store a new product into with information given by the user.
      this->createdProduct = Product(1, productName.toStdString()
          , ingredients, productPrice);
      // Stablish that the Qdialog has finished correctly.
      this->accept();
    } else {
      // Creates a message box to indicate the user that the format in the
      // product ingredients is wrong.
      QMessageBox::warning(this, "Error en formato de ingredientes",
          "Por favor, escriba los ingredientes del producto en el formato"
          " solicitado");
    }
  }
}

void ProductFormDialog::setProductInfo(
    Product& productToEdit, QString productCategory) {
  // Index value for the category provided to the QDialog.
  size_t categoryIndex
      = this->ui->productCategory_comboBox->findText(productCategory
      , Qt::MatchContains);
  // Sets the category combo box to the index of the product's category.
  this->ui->productCategory_comboBox->setCurrentIndex(categoryIndex);
  // Sets the text of the lineEdit of the product name.
  this->ui->productName_lineEdit->setText(productToEdit.getName().data());
  // Creates a string containing all the ingredients information.
  QString productIngredients = this->formatProductIngredients(
      productToEdit.getIngredients());
  // Sets the text of the lineEdit of the product ingredients.
  this->ui->productIngredients_lineEdit->setText(productIngredients);
  // Sets the value of the double spin box of the product price.
  this->ui->productPrice_doubleSpinBox->setValue(productToEdit.getPrice());
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
