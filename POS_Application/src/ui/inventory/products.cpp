// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include <string>
#include <vector>
#include <utility>
#include <QMessageBox>

#include "productformdialog.h"
#include "products.h"
#include "ui_products.h"

#include "posmodel.h"
#include "product.h"
#include "util.h"

Products::Products(QWidget *parent, POS_Model& model)
    : Inventory(parent)
    , ui(new Ui::Products) {
  this->ui->setupUi(this);
  
  // Updates for the first time the diaply to show the first index page.
  this->refreshDisplay(this->itemsPerPage);
  this->setupConnections();
}

Products::~Products() {
  delete ui;
}

void Products::setupConnections() {
  // Connect the slot function to the next page button.
  this->connect(this->ui->nextProductPage_button
          , &QPushButton::clicked
          , this
          , &Products::on_nextPage_button_clicked);
  
  // Connect the slot function to the previous page button.
  this->connect(this->ui->previousProductPage_button
          , &QPushButton::clicked
          , this
          , &Products::on_previousPage_button_clicked);
  
  // Connect the slot function to the previous page button.
  this->connect(this->ui->addProduct_button
          , &QPushButton::clicked
          , this
          , &Products::addProduct_button_clicked);
  
  // Temporal variables to hold the different delete and edit buttons for each
  // product's row.
  QString deleteButtonName("");
  QString editButtonName("");
  // Iterates through each product's information row.
  for (size_t index = 0; index < this->itemsPerPage; ++index) {
    // Index the product's delete and edit button names.
    editButtonName = QString("editProduct_button_%1").arg(index);
    deleteButtonName = QString("deleteProduct_button_%1").arg(index);
    // Find the pointer to the product's delete and edit buttons.
    QPushButton* deleteButton = this->findChild<QPushButton *>(deleteButtonName);
    QPushButton* editButton = this->findChild<QPushButton *>(editButtonName);
    // Connect the delete button with the slot function.
    this->connect(deleteButton
            , &QPushButton::clicked
            , this
            , &Products::on_delete_button_clicked);
    // Connect the edit button with the slot function.    
    this->connect(editButton
            , &QPushButton::clicked
            , this
            , &Products::on_edit_button_clicked);
  }
  
  // Connect the categories button with the slot function.  
  this->connect(this->ui->categories_button
          , &QPushButton::clicked
          , this
          , &Products::on_categories_button_clicked);
}

void Products::refreshDisplay(const size_t pageItems) {
  // Refresh the products display for the given items.
  this->refreshProductDisplay(
      this->model.getProductsForPage(this->currentPageIndex, pageItems)
      , pageItems);
}

void Products::refreshProductDisplay(
    std::vector<std::pair<std::string, Product>> visibleProducts
    , const size_t items) {
  // Initiazates the label index iterator.
  size_t labelIt = 0;
  // Tranverse the different rows of products information.
  for (size_t index = 0; index < items; index++) {
    // Place holder labels text.
    QString productID("---");
    QString productName("------");
    QString productCategory("------");
    QString productIngredients("------");
    QString productPrice("------");
    
    // If theres remaining products information to be displayed.
    if (index < visibleProducts.size()) {
      // Extracts all the product information to display it on the screen.
      productID = 
          std::to_string(visibleProducts[index].second.getID()).data();
      productName = visibleProducts[index].second.getName().data();
      productCategory = visibleProducts[index].first.data();
      productPrice = QString::number(visibleProducts[index].second.getPrice(),
                                     'f', 2);
      productIngredients = this->model.formatProductIngredients(
          visibleProducts[index].second.getIngredients());
    }
    // Updates the the different labels for each product information that is
    // displayed in the UI.
    // Updates the product text contained in the productCode of the matching
    // label.
    Util::updatePrefixedLabel(this, "productCode", labelIt, productID);
    // Updates the product text contained in the productName of the matching
    // label.
    Util::updatePrefixedLabel(this, "productName", labelIt, productName);
    // Updates the product text contained in the productCategory of the matching
    // label.
    Util::updatePrefixedLabel(this, "productCategory", labelIt
                              , productCategory);
    // Updates the product text contained in the productIngredients of the
    // matching label.
    Util::updatePrefixedLabel(this, "productIngredients", labelIt
                              , productIngredients);
    // Updates the product text contained in the productPrice of the matching
    // label.
    Util::updatePrefixedLabel(this, "productPrice", labelIt, productPrice);
    // Increse the label iterator.
    ++labelIt;
  }
  
  size_t offset = this->currentPageIndex * 9;
  // Generates the page label text tha indicates the indexes of the products
  // displayed in the actual page.
  QString pageLabelText = QString("Mostrando productos %1 hasta %2 de %3"
    " productos").arg(offset + 1).arg(std::min(offset + 9,
              this->model.getNumberOfProducts())).arg(
    this->model.getNumberOfProducts());
  this->ui->pageProductsNumber_label->setText(pageLabelText);
}

void Products::addProduct_button_clicked() {
  // Creates a dialog to manage the product creation.
  ProductFormDialog dialog(this, this->model.getRegisteredProducts()
      , Product(), QString());
  // Executes the dialog and check if were accepted.
  if (dialog.exec() == QDialog::Accepted) {
    qDebug() << "Se acepto el dialogo y se agrego un nuevo producto";
    // Obtain the product created in the dialog.
    Product product = dialog.getProduct();
    // Obtain the category of the product.
    std::string category = dialog.getProductCategory().toStdString();
    // Try to add the new product into the registered ones.
    if (this->model.addProduct(category, product)) {
      // Refresh the display with the updated data.
      this->refreshDisplay(this->itemsPerPage);
    } else {
      QMessageBox::information(this, "Informacion inválida"
          , "No se añadió el suministro.");
    }
  } else {
    qDebug() << "Se cancelo la creacion de un producto";
  }
}

void Products::on_nextPage_button_clicked() {
  // Calculates the products page start and end indexes for the next page.
  size_t productPageIt = (this->currentPageIndex + 1) * 9;
  size_t productPageIt2 = productPageIt + 9;
  // Checks if the indexes the number of registered products is greather or
  // between the next page indexes.
  if (this->model.getNumberOfProducts() >= productPageIt
      && this->model.getNumberOfProducts() <= productPageIt2) {
    ++this->currentPageIndex;
    // Updates the product information in the display to show the next product
    // page.
    this->refreshDisplay(this->itemsPerPage);
  }
  qDebug() << "Boton de avance: " << this->currentPageIndex;
}

void Products::on_previousPage_button_clicked() {
  // Checks that the actual page is not the first one.
  if (this->currentPageIndex > 0) {
    // Decrements the page index.
    --this->currentPageIndex;
    // Updates the product information in the display to show the previous
    // product page.
    this->refreshDisplay(this->itemsPerPage);
  }
  std::cout << "Boton de retroceso: " << this->currentPageIndex << std::endl;
}

void Products::on_categories_button_clicked() {
  emit this->categories_button_signal();
}

void Products::on_supplies_button_clicked() {
  qDebug() << "Supplies button clicked"; // Verificar si la función se ejecuta.
  
  // Emit the signal for the app controller to handle/receive it.
  emit this->supplies_button_signal();
}

void Products::on_products_button_clicked() {
  
}

void Products::on_delete_button_clicked() {
  // Catch the pointer to the button object that sended the signal.
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  // If there's a pointer, then.
  if (button) {
    // Obtain the index of the display button.
    size_t index = button->property("index").toUInt();
    qDebug() << "Button clicked, index:" << index;
    // Delete the registered product.
    this->deleteRegisteredProduct(index);
  }
}

void Products::on_edit_button_clicked() {
  // Catch the pointer to the button object that sended the signal.
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  // If there's a pointer, then.
  if (button) {
    // Obtain the index of the display button.    
    size_t index = button->property("index").toUInt();
    qDebug() << "Button clicked, index:" << index;
    // Edit the product.
    this->editProductInformation(index);
  }
}

void Products::deleteRegisteredProduct(size_t index) {
  // Temporal variables to store the name of the labels containing products name
  // category.
  QString labelName;
  QString labelCategory;
  
  // Obtain the name of the labels by adding their row indexes.
  labelName = QString("productName_label_%1").arg(index);
  labelCategory = QString("productCategory_label_%1").arg(index);
  
  // Find the label of the product's name.
  QLabel* label = this->findChild<QLabel*>(labelName);
  // Obtains the product's name.
  const QString productName = label->text();
  
  // Find the label of the product's category.
  label = this->findChild<QLabel*>(labelCategory);
  // Obtain the product's category name.
  const QString productCategory = label->text();
  
  // Find a reference to the product to delete.
  Product& productToDelete = this->model.findProduct(productName.toStdString());
  // Try to delete the product from the registers.
  if (this->model.removeProduct(productCategory.toStdString()
    , productToDelete)) {
    // Refresh the products display.
    this->refreshDisplay(this->itemsPerPage);
  } else {
    QMessageBox::warning(this, "Error"
        , "No se pudo eliminar el producto.");
  }
}

void Products::editProductInformation(size_t index) {
  // Temporal variables to store the name of the labels containing products name
  // category.
  QString labelName;
  QString labelCategory;
  
  // Obtain the name of the labels by adding their row indexes.
  labelName = QString("productName_label_%1").arg(index);
  labelCategory = QString("productCategory_label_%1").arg(index);
  
  // Find the label of the product's name.
  QLabel* label = this->findChild<QLabel*>(labelName);
  // Obtains the product's name.
  const QString productName = label->text();
  
  // Find the label of the product's category.
  label = this->findChild<QLabel*>(labelCategory);
  // Obtain the product's category name.
  const QString productCategory = label->text();
  
  // Find a reference to the product to edit.  
  Product& productToEdit =
      this->model.findProduct(productName.toStdString());
  
  // Checks that the product to edit aren't a blank one.
  if (!(productToEdit == Product())) {
    // Open the dialog for product editing.
    ProductFormDialog dialog(
        this,
        this->model.getRegisteredProducts(),
        Product(productToEdit),
        QString(productCategory));
    // Executes the dialog and checks if was accepted.
    if (dialog.exec() == QDialog::Accepted) {
      if (!(productToEdit == dialog.getProduct())) {
        // Remove the current product using references.
        this->model.editProduct(productCategory.toStdString(), productToEdit
            , dialog.getProductCategory().toStdString(), dialog.getProduct());
        // Updates the products display.
        this->refreshDisplay(this->itemsPerPage);
      }
    }
  }
}
