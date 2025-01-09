#include <string>
#include <vector>
#include <map>
#include <utility>
#include "inventorypage.h"
#include "ui_inventorypage.h"
#include "posmodel.h"
#include "product.h"

QLabel* cloneLabel(QLabel* original) {
  QLabel* newLabel = new QLabel(original->parentWidget());
  
  // Copy the important attributes.
  newLabel->setText(original->text());
  newLabel->setAlignment(original->alignment());
  newLabel->setStyleSheet(original->styleSheet());
  newLabel->setFont(original->font());
  newLabel->setPixmap(original->pixmap(Qt::ReturnByValue)); // Si usa pixmap
  
  // Other settings if needed.
  newLabel->setSizePolicy(original->sizePolicy());
  newLabel->setGeometry(original->geometry());
  
  return newLabel;
}

QString InventoryPage::formatProductIngredients(
    const std::vector<SupplyItem>& ingredients) {
  QString formattedProductIngredients = "";
  
  for (auto ingredient : ingredients) {
    QString ingredientName(ingredient.getName().data());
    QString ingredientQuantity = QString::number(ingredient.getQuantity());
    formattedProductIngredients = QString(formattedProductIngredients
      + " " +ingredientName.trimmed() + " : " + ingredientQuantity);
  }
  
  return formattedProductIngredients;
}

InventoryPage::InventoryPage(QWidget *parent, POS_Model& model)
    : QWidget(parent)
    , ui(new Ui::InventoryPage)
    , appModel(model)
    , registeredProducts(std::vector<std::pair<std::string, Product>>())
    , productPageIndex(0) {
  ui->setupUi(this); 
  
  // Gets the registered products.
  std::map<std::string, std::vector<Product>> registeredDrinks
      = this->appModel.getRegisteredDrinks();
  std::map<std::string, std::vector<Product>> registeredDishes
      = this->appModel.getRegisteredDishes();
  
  // Creates a vector for the products to be displayed in screen.
  std::vector<std::pair<std::string, Product>> mergedProducts;
  
  this->registerProductType(mergedProducts, registeredDrinks);
  this->registerProductType(mergedProducts, registeredDishes);
  this->registeredProducts = mergedProducts;
  std::cout << "Registered Dishes's Category: " << registeredDishes.size()
      << std::endl;
  std::cout << "Registered Drink's Category: " << registeredDrinks.size()
            << std::endl;
  
  // Updates for the first time the diaply to show the first index page.
  this->updateProductsInformation(this->registeredProducts);
  
  // Connect the slot function to the next page button.
  connect(this->ui->nextProductPage_button,
          &QPushButton::clicked, this, []() {});
  
  // Connect the slot function to the previous page button.
  connect(this->ui->previousProductPage_button,
      &QPushButton::clicked, this, []() {});
}
    
void InventoryPage::on_nextProductPage_button_clicked() {
  // Calculates the indexes of the first and last product to be fopr the
  // nect page displayed.
  size_t productPageIt = (this->productPageIndex + 1) * 9;
  size_t productPageIt2 = productPageIt + 9;
  // Checks if the size of the registered products vector is lower than the
  // indexes of the next page.
  if (this->registeredProducts.size() >= productPageIt
      && this->registeredProducts.size() <= productPageIt2) {
    ++this->productPageIndex;
    // Updates the product information in the display to show the next product
    // page.
    this->updateProductsInformation(this->registeredProducts); 
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
    this->updateProductsInformation(this->registeredProducts);    
  }
  std::cout << "Boton de retroceso: " << this->productPageIndex << std::endl;
}

void InventoryPage::registerProductType(
    std::vector<std::pair<std::string, Product>>& registeredProducts
    , const std::map<std::string, std::vector<Product>>& productTypeRegister) {
  // Transverse all the product type categories and their products.
  for (const auto& [category, products] : productTypeRegister) {
    // Transverse all the products of the category and added them to the vector.
    for (const auto& product : products) {
      // Emplace the product in the the product register.
      registeredProducts.emplace_back(category, product);
    }
  }
}

void InventoryPage::updateProductsInformation(
    std::vector<std::pair<std::string, Product>> visibleProducts) {
  // Calculates an offset to calculate the indexes of the products to display on
  // the screen.
  const size_t offset = 0 + (9 * this->productPageIndex);
  // Calculates the last index of the actual product information page.
  size_t finalOffset = offset + 9;
  // Initiazates the label index iterator.
  size_t labelIt = 0;
  // Tranverse the different rows of products information.
  for (size_t index = offset; index < finalOffset; index++) {
    // Place holder labels text.
    QString productID("------");
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
      productIngredients = this->formatProductIngredients(
          visibleProducts[index].second.getIngredients());
    }
    // Updates the the different labels for each product information that is
    // displayed in the UI.
    
    // Updates the product text contained in the productCode of the matching
    // label.    
    this->updateProductLabel(labelIt, "productCode", productID);
    // Updates the product text contained in the productName of the matching
    // label.    
    this->updateProductLabel(labelIt, "productName", productName);
    // Updates the product text contained in the productCategory of the matching
    // label.    
    this->updateProductLabel(labelIt, "productCategory", productCategory);
    // Updates the product text contained in the productIngredients of the
    // matching label.    
    this->updateProductLabel(labelIt, "productIngredients", productIngredients);
    // Updates the product text contained in the productPrice of the matching
    // label.    
    this->updateProductLabel(labelIt, "productPrice", productPrice);    
    // Increse the label iterator.
    ++labelIt;
  }
  
  // Checks if the final offset is lower than the visible items.
  if (finalOffset > visibleProducts.size()) {
    finalOffset = visibleProducts.size();
  }
  
  // Generates the page label text tha indicates the indexes of the products
  // displayed in the actual page.
  QString pageLabelText = QString("Mostrando productos %1 hasta %2 de %3"
      " productos").arg(offset).arg(finalOffset).arg(visibleProducts.size());
  this->ui->pageProductsNumber_label->setText(pageLabelText);
}

void InventoryPage::updateProductLabel(size_t labelIt,
    const QString& labelSuffix, const QString& value) {
  
  // Generates the label name based on the suffix and label index.
  QString labelName = QString("%1_label_%2").arg(labelSuffix).arg(labelIt);
  QLabel* label = this->findChild<QLabel*>(labelName);
  
  // Checks if catched a label with that name.
  if (label != nullptr) {
    // Updates the label text with the given text value.
    label->setText(value.trimmed());
    label->setAlignment(Qt::AlignCenter);
  } else {
    qDebug() << "No se pudo encontrar el puntero del label con sufijo: "
             << labelSuffix;
  }
}


InventoryPage::~InventoryPage() {
  delete ui;
}
