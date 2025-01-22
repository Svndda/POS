#include <string>
#include <vector>
#include <utility>

#include "posmodel.h"
#include "product.h"
#include "util.h"

#include "productformdialog.h"
#include "products.h"
#include "ui_products.h"

Products::Products(QWidget *parent, POS_Model& model)
    : Inventory(parent)
    , ui(new Ui::Products) {
  ui->setupUi(this);
  
  // Updates for the first time the diaply to show the first index page.
  this->refreshDisplay(this->itemsPerPage);
  this->setupConnections();
}

Products::~Products() {
  delete ui;
}

void Products::refreshDisplay(const size_t items) {
  this->refreshProductDisplay(
      this->model.getProductsForPage(this->currentPageIndex
      , this->itemsPerPage)
      , this->itemsPerPage);
}

void Products::setupConnections() {
  // Connect the slot function to the next page button.
  connect(this->ui->nextProductPage_button
          , &QPushButton::clicked
          , this
          , &Products::on_nextPage_button_clicked);
  
  // Connect the slot function to the previous page button.
  connect(this->ui->previousProductPage_button
          , &QPushButton::clicked
          , this
          , &Products::on_previousPage_button_clicked);
  
  // Connect the slot function to the previous page button.
  connect(this->ui->addProduct_button
          , &QPushButton::clicked
          , this
          , &Products::addProduct_button_clicked);
  
  QString deleteButtonName("");
  QString editButtonName("");
  for (size_t index = 0; index < 9; ++index) {
    editButtonName = QString("editProduct_button_%1").arg(index);
    deleteButtonName = QString("deleteProduct_button_%1").arg(index);
    QPushButton* deleteButton = this->findChild<QPushButton *>(deleteButtonName);
    QPushButton* editButton = this->findChild<QPushButton *>(editButtonName);
    connect(deleteButton
            , &QPushButton::clicked
            , this
            , &Products::on_delete_button_clicked);
    connect(editButton
            , &QPushButton::clicked
            , this
            , &Products::on_edit_button_clicked);
  }
  
  connect(this->ui->categories_button
          , &QPushButton::clicked
          , this
          , &Products::on_categories_button_clicked);
}

void Products::addProduct_button_clicked() {
  ProductFormDialog dialog(this
                           , this->model.getRegisteredProducts()
                           , Product()
                           , QString());
  
  if (dialog.exec() == QDialog::Accepted) {
    qDebug() << "Se acepto el dialogo y se agrego un nuevo producto";
    Product product = dialog.getProduct();
    std::string category = dialog.getProductCategory().toStdString();
    this->model.addProduct(category, product);
    this->refreshProductDisplay(
        this->model.getProductsForPage(this->currentPageIndex
        , this->itemsPerPage)
        , this->itemsPerPage);
  } else {
    qDebug() << "Se cancelo la creacion de un producto";
  }
}

void Products::on_nextPage_button_clicked() {
  size_t productPageIt = (this->currentPageIndex + 1) * 9;
  size_t productPageIt2 = productPageIt + 9;
  if (this->model.getNumberOfProducts() >= productPageIt
      && this->model.getNumberOfProducts() <= productPageIt2) {
    ++this->currentPageIndex;
    // Updates the product information in the display to show the next product
    // page.
    this->refreshProductDisplay(
        this->model.getProductsForPage(this->currentPageIndex
        , this->itemsPerPage)
        , this->itemsPerPage);
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
    this->refreshProductDisplay(
        this->model.getProductsForPage(this->currentPageIndex
        , this->itemsPerPage)
        , this->itemsPerPage);
  }
  std::cout << "Boton de retroceso: " << this->currentPageIndex << std::endl;
}

void Products::on_delete_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    size_t index = button->property("index").toUInt(); // Recuperar índice
    qDebug() << "Button clicked, index:" << index;
    this->deleteRegisteredProduct(index);
  }
}

void Products::on_edit_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    size_t index = button->property("index").toUInt();
    qDebug() << "Button clicked, index:" << index;

    this->editProductInformation(index);
  }
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

void Products::deleteRegisteredProduct(size_t index) {
  QString labelName;
  QString labelCategory;
  
  labelName = QString("productName_label_%1").arg(index);
  labelCategory = QString("productCategory_label_%1").arg(index);
  
  QLabel* label = this->findChild<QLabel*>(labelName);
  const QString productName = label->text();
  
  label = this->findChild<QLabel*>(labelCategory);
  const QString productCategory = label->text();
  
  this->model.removeProduct(productCategory.toStdString()
      , this->model.findProduct(productName.toStdString()));
  
  this->refreshProductDisplay(
      this->model.getProductsForPage(this->currentPageIndex
      , this->itemsPerPage)
      , this->itemsPerPage);
}

void Products::editProductInformation(size_t index) {
  QString labelName;
  QString labelCategory;
  
  labelName = QString("productName_label_%1").arg(index);
  labelCategory = QString("productCategory_label_%1").arg(index);
  
  QLabel* label = this->findChild<QLabel*>(labelName);
  const QString productName = label->text();
  
  label = this->findChild<QLabel*>(labelCategory);
  const QString productCategory = label->text();
  
  Product& productToEdit =
      this->model.findProduct(productName.toStdString());
  
  if (!(productToEdit == Product())) {
    // Open the dialog for product editing
    ProductFormDialog dialog(
        this,
        this->model.getRegisteredProducts(),
        Product(productToEdit),
        QString(productCategory));
    
    if (dialog.exec() == QDialog::Accepted) {
      if (!(productToEdit == dialog.getProduct())) {
        // Remove the current product using the iterator
        this->model.editProduct(productCategory.toStdString(), productToEdit
            , dialog.getProductCategory().toStdString(), dialog.getProduct());
        
        this->refreshProductDisplay(
            this->model.getProductsForPage(this->currentPageIndex
            , this->itemsPerPage)
            , this->itemsPerPage);
      }
    }
  }
}

