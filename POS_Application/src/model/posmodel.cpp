#include <vector>
#include <limits>

#include <QDebug>

#include "posmodel.h"
#include "backupmodule.h"

const size_t POS_Model::INSERT = 100;
const size_t POS_Model::UPDATE = 200;
const size_t POS_Model::DELETE = 0;

POS_Model::POS_Model(BackupModule& module)
    : backupModule(module) {
}

POS_Model& POS_Model::getInstance() {
  // Creates an static instance of the POS MODEL.
  static POS_Model instance(BackupModule::getInstance());
  return instance;
}

void POS_Model::start() {
  // Reads and store the backup to the program memory to use them in the
  // program execution.
  this->products = this->backupModule.getProductsBackup();
  this->productsVector.clear();
  this->extractProducts(this->productsVector, this->products);
  // Changes the started value to true.
  this->started = true;
}

Product& POS_Model::findProduct(const std::string& productName) {
  for (auto& [category, products] : this->products) {
    // Iterar sobre los productos en la categoría actual
    for (auto& product : products) {
      // Comparar el nombre del producto con el buscado
      if (product.getName() == productName) {
        return product; // Platillo encontrado
      }
    }
  }
  throw std::runtime_error("Product not found: " + productName);  
}

void POS_Model::addProduct(const std::string& category
    , const Product& product) {
  if (insertOnRegister(category, product, this->products)) {
    qDebug() << "Producto anadido correctamente";
    backupModule.writeRegistersBackUp(this->products);
  }
}

void POS_Model::removeProduct(const std::string& category
    , const Product& product) {
  if (eraseOnRegister(category, product, this->products)) {
    qDebug() << "producto elimnado correctamente";
    backupModule.writeRegistersBackUp(this->products);
  }
}

void POS_Model::editProduct(const std::string& oldCategory
    , const Product& oldProduct, const std::string& newCategory
    , const Product& newProduct) {
  if (eraseOnRegister(oldCategory, oldProduct, this->products)) {
    insertOnRegister(newCategory, newProduct, this->products);
    backupModule.writeRegistersBackUp(this->products);
  }
}

std::vector<std::string> POS_Model::getCategoriesForPage(size_t pageIndex) {
  std::vector<std::string> registeredCategories;
  size_t startIdx = pageIndex * 9;
  size_t endIdx = startIdx + 9;
  
  endIdx = std::min(endIdx, this->products.size());
  
  size_t currentIdx = 0;
  
  for (const auto& [category, product] : this->products) {
    if (currentIdx >= startIdx && currentIdx < endIdx) {
      registeredCategories.push_back(category);
    }
    ++currentIdx;
    if (currentIdx >= endIdx) {
      break;
    }
  }
  
  return registeredCategories;
}

size_t POS_Model::getSizeOfCategory(std::string category) {
  size_t no_value = std::numeric_limits<size_t>::max();
  for (const auto& element : this->products) {
    if (element.first == category) {
      return element.second.size();
    }
  }
  return no_value;
}

std::vector<std::pair<std::string, Product>> POS_Model::getProductsForPage(
    size_t pageIndex) {
  size_t startIdx = pageIndex * 9;
  size_t endIdx = std::min(startIdx + 9, this->productsVector.size());
  
  std::vector<std::pair<std::string, Product>> pageProducts;
  for (size_t i = startIdx; i < endIdx; ++i) {
    pageProducts.push_back(this->productsVector[i]);
  }
  return pageProducts;
}

bool POS_Model::insertOnRegister(const std::string productCategory
    , const Product& product
    , std::map<std::string, std::vector<Product>>& productTypeRegister) {
  // Transverse all the registered product categories.
  for (auto& category : productTypeRegister) {
    // Checks if the category matches.
    if (category.first == productCategory) {
      // Transverse all the registered products in the category.
      for (auto& registeredProduct : category.second) {
        // check if there's a registered product in the category.
        if (registeredProduct == product) {
          qDebug() << "Ya existe un elemento igual: "
              << registeredProduct.getName() << "," << product.getName();
          return false;
        }
      }
      qDebug() << "Categoria anadiendo producto a la categoria: "
               << category.first;
      // Adds the created product into the vector of registered products.
      category.second.emplace_back(product);
      this->productsVector.clear();      
      this->extractProducts(this->productsVector, this->products);      
      return true;
    }
  }
  return false;
}

bool POS_Model::eraseOnRegister(const std::string productCategory
    , const Product& product
    , std::map<std::string, std::vector<Product>>& productTypeRegister) {
  // Transverse all the registered product categories.
  for (auto& category : productTypeRegister) {
    // Checks if the category matches.
    qDebug() << category.first << ":" << productCategory;
    if (category.first == productCategory) {
      // Transverse all the registered products in the category.
      for (size_t registerIt = 0; registerIt < category.second.size();
           ++registerIt) {
        // Check if there's a registered product in the category that matches
        // the product to delete.
        std::cout << category.second[registerIt] <<  ":" << product;   
        if (category.second[registerIt] == product) {
          category.second.erase(category.second.begin() + registerIt);
          this->productsVector.clear();
          this->extractProducts(this->productsVector, this->products);
          return true;
        }
      }
    }
  }
  return false;
}

void POS_Model::extractProducts(
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

QString POS_Model::formatProductIngredients(
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

void POS_Model::shutdown() {
  // Writes out the registers of the dishes and drinks back to the backup files.
  this->backupModule.writeRegistersBackUp(this->products);
  // Clears the vector memory.
  this->products.clear();
  this->started = false;
}
