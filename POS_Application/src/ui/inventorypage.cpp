#include <string>
#include <vector>
#include <utility>

#include "posmodel.h"
#include "product.h"
#include "util.h"

#include "productformdialog.h"
#include "inventorypage.h"
#include "ui_inventorypage.h"

InventoryPage::InventoryPage(QWidget *parent, POS_Model& model)
    : QWidget(parent)
    , ui(new Ui::InventoryPage)
    , appModel(model)
    , productPageIndex(0) {
  ui->setupUi(this);
  
  // Updates for the first time the diaply to show the first index page.
  this->refreshProductDisplay(
      this->appModel.getProductsForPage(this->productPageIndex));
  this->connectSlots();
}

void InventoryPage::connectSlots() {
  // Connect the slot function to the next page button.
  connect(this->ui->nextProductPage_button
          , &QPushButton::clicked
          , this
          , []{});
  
  // Connect the slot function to the previous page button.
  connect(this->ui->previousProductPage_button
          , &QPushButton::clicked
          , this
          , []{});
  
  // Connect the slot function to the previous page button.
  connect(this->ui->addProduct_button
          , &QPushButton::clicked
          , this
          , []{});
  
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
            , &InventoryPage::on_deleteProduct_button_clicked);
    connect(editButton
            , &QPushButton::clicked
            , this
            , &InventoryPage::on_editProduct_button_clicked);
  }
  
  connect(this->ui->categories_button
          , &QPushButton::clicked
          , this
          , &InventoryPage::on_categories_button_clicked);
}

void InventoryPage::on_categories_button_clicked() {
  emit this->categories_button_signal();
}

void InventoryPage::on_nextProductPage_button_clicked() {
  size_t productPageIt = (this->productPageIndex + 1) * 9;
  size_t productPageIt2 = productPageIt + 9;
  if (this->appModel.getNumberOfProducts() >= productPageIt
      && this->appModel.getNumberOfProducts() <= productPageIt2) {
    ++this->productPageIndex;
    // Updates the product information in the display to show the next product
    // page.
    this->refreshProductDisplay(
        this->appModel.getProductsForPage(this->productPageIndex));
  }
  qDebug() << "Boton de avance: " << this->productPageIndex;
}

void InventoryPage::on_previousProductPage_button_clicked() {
  // Checks that the actual page is not the first one.
  if (this->productPageIndex > 0) {
    // Decrements the page index.
    --this->productPageIndex;
    // Updates the product information in the display to show the previous
    // product page.
    this->refreshProductDisplay(
        this->appModel.getProductsForPage(this->productPageIndex));
  }
  std::cout << "Boton de retroceso: " << this->productPageIndex << std::endl;
}

void InventoryPage::on_addProduct_button_clicked() {
  ProductFormDialog dialog(this
      , this->appModel.getRegisteredProducts()
      , Product()
      , QString());

  if (dialog.exec() == QDialog::Accepted) {
    qDebug() << "Se acepto el dialogo y se agrego un nuevo producto";
    Product product = dialog.getProduct();
    std::string category = dialog.getProductCategory().toStdString();
    this->appModel.addProduct(category, product);
    this->refreshProductDisplay(
        this->appModel.getProductsForPage(this->productPageIndex));
  } else {
    qDebug() << "Se cancelo la creacion de un producto";
  }
}

void InventoryPage::on_deleteProduct_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    size_t index = button->property("index").toUInt(); // Recuperar índice
    qDebug() << "Button clicked, index:" << index;
    this->deleteRegisteredProduct(index);
  }
}

void InventoryPage::deleteRegisteredProduct(size_t index) {
  QString labelName;
  QString labelCategory;

  labelName = QString("productName_label_%1").arg(index);
  labelCategory = QString("productCategory_label_%1").arg(index);

  QLabel* label = this->findChild<QLabel*>(labelName);
  const QString productName = label->text();

  label = this->findChild<QLabel*>(labelCategory);
  const QString productCategory = label->text();

  this->appModel.removeProduct(productCategory.toStdString()
      , this->appModel.findProduct(productName.toStdString()));
  this->refreshProductDisplay(
      this->appModel.getProductsForPage(this->productPageIndex));
}

void InventoryPage::on_editProduct_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    size_t index = button->property("index").toUInt();
    qDebug() << "Button clicked, index:" << index;

    this->editProductInformation(index);
  }
}

void InventoryPage::editProductInformation(size_t index) {
  QString labelName;
  QString labelCategory;

  labelName = QString("productName_label_%1").arg(index);
  labelCategory = QString("productCategory_label_%1").arg(index);

  QLabel* label = this->findChild<QLabel*>(labelName);
  const QString productName = label->text();

  label = this->findChild<QLabel*>(labelCategory);
  const QString productCategory = label->text();

  Product& productToEdit =
      this->appModel.findProduct(productName.toStdString());

  if (!(productToEdit == Product())) {
    // Open the dialog for product editing
    ProductFormDialog dialog(
        this,
        this->appModel.getRegisteredProducts(),
        Product(productToEdit),
        QString(productCategory));

    if (dialog.exec() == QDialog::Accepted) {
      if (!(productToEdit == dialog.getProduct())) {
        // Remove the current product using the iterator
        this->appModel.editProduct(productCategory.toStdString(), productToEdit
            , dialog.getProductCategory().toStdString(), dialog.getProduct());

        this->refreshProductDisplay(
            this->appModel.getProductsForPage(this->productPageIndex));
      }
    }
  }
}

void InventoryPage::refreshProductDisplay(
    std::vector<std::pair<std::string, Product>> visibleProducts) {
  // Initiazates the label index iterator.
  size_t labelIt = 0;
  // Tranverse the different rows of products information.
  for (size_t index = 0; index < 9; index++) {
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
      productIngredients = this->appModel.formatProductIngredients(
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
  
  size_t offset = this->productPageIndex * 9;
  // Generates the page label text tha indicates the indexes of the products
  // displayed in the actual page.
  QString pageLabelText = QString("Mostrando productos %1 hasta %2 de %3"
      " productos").arg(offset + 1).arg(offset + 9).arg(this->appModel.getNumberOfProducts());
  this->ui->pageProductsNumber_label->setText(pageLabelText);
}

InventoryPage::~InventoryPage() {
  delete ui;
}
