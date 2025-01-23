// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include <vector>
#include <limits>

#include <QDebug>

#include "posmodel.h"
#include "backupmodule.h"

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
  this->supplies = this->backupModule.getSuppliesBackup();
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

bool POS_Model::addProduct(const std::string& category
    , const Product& product) {
  if (this->insertProduct(category, product, this->products)) {
    qDebug() << "Producto anadido correctamente";
    backupModule.updateProductsBackup(this->products);
    return true;
  }
  return false;
}

bool POS_Model::addCategory(const std::string newCategory) {
  auto result = this->products.try_emplace(newCategory, std::vector<Product>());
  if (result.second) {
    backupModule.updateProductsBackup(this->products);
    return true;
  }
  return false;
}

bool POS_Model::addSupply(const SupplyItem newSupply) {
  if (!newSupply.empty()) {
    auto it = std::find(this->supplies.begin(), this->supplies.end()
        , newSupply);
    if (it == this->supplies.end()) {
      this->supplies.emplace_back(newSupply);
      this->backupModule.updateSuppliesBackup(this->supplies);
      qDebug() << "Se añadió el suministro, correctamente.";
      return true;
    }
    qDebug() << "No se añadió el suministro, ya existe uno con este nombre.";
  }
  qDebug() << "No se añadió el suministro, el recipiente estaba vacio.";
  return false;
}

bool POS_Model::removeProduct(const std::string& category
    , const Product& product) {
  if (eraseOnRegister(category, product, this->products)) {
    qDebug() << "producto elimnado correctamente";
    backupModule.updateProductsBackup(this->products);
    return true;
  }
  return false;
}

bool POS_Model::removeCategory(const std::string category) {
  size_t removed = this->products.erase(category);
  if (removed) {
    this->productsVector.clear();
    this->extractProducts(this->productsVector, this->products);
    qDebug() << "Categoria eliminada";
    return true;
  }
  return false;
}

bool POS_Model::removeSupply(const SupplyItem &supply) {
  auto it = std::find(this->supplies.begin(), this->supplies.end(), supply);
  if (!supply.getName().empty()) {
    if (it != this->supplies.end()) {
      this->supplies.erase(it);
      this->backupModule.updateSuppliesBackup(this->supplies);
      qDebug() << "Se eliminó el suministro, correctamente.";
      return true;
    }
    qDebug() << "No se eliminó el suministro, no existe uno con este nombre.";
  }
  qDebug() << "No se eliminó el suministro, el recipiente estaba vacio.";
  return false;
}

bool POS_Model::editProduct(const std::string& oldCategory
    , const Product& oldProduct, const std::string& newCategory
    , const Product& newProduct) {
  if (eraseOnRegister(oldCategory, oldProduct, this->products)) {
    this->insertProduct(newCategory, newProduct, this->products);
    backupModule.updateProductsBackup(this->products);
    return true;
  }
  return false;
}

bool POS_Model::editCategory(const std::string oldCategory
    , const std::string newCategory) {
  auto categoryIt = this->products.find(oldCategory);
  if (categoryIt != this->products.end()) {
    std::vector<Product> categoryProducts = categoryIt->second;
    this->products.erase(categoryIt);
    qDebug() << "Categoria eliminada: " << this->products.size();
    this->products.emplace(newCategory, categoryProducts);
    qDebug() << "Mapa actualizado: " << this->products.size();
    this->productsVector.clear();
    this->extractProducts(this->productsVector, this->products);
    return true;
  }
  return false;
}

bool POS_Model::editSupply(const SupplyItem& oldSupply
    , const SupplyItem& newSupply) {
  if (!newSupply.getName().empty()) {
    auto it = std::find(this->supplies.begin(), this->supplies.end()
        , oldSupply);
    if (it != this->supplies.end()) {
      if (oldSupply.getQuantity() != newSupply.getQuantity()
          || oldSupply.getMeasure() != newSupply.getMeasure()) {
        *it = newSupply;
        this->backupModule.updateSuppliesBackup(this->supplies);
        qDebug() << "Suministro editado correctamente.";
        return true;
      }
      qDebug() << "No se cambiaron los datos del suministro."; 
    }
    qDebug() << "No se encontró un suministro con este nombre.";
  }
  return false;
}

std::vector<std::pair<std::string, Product>> POS_Model::getProductsForPage(
    const size_t pageIndex, const size_t itemsPerPage) {
  size_t startIdx = pageIndex * itemsPerPage;
  size_t endIdx = std::min(startIdx + itemsPerPage, this->productsVector.size());
  
  std::vector<std::pair<std::string, Product>> pageProducts;
  for (size_t i = startIdx; i < endIdx; ++i) {
    pageProducts.push_back(this->productsVector[i]);
  }
  return pageProducts;
}

std::vector<std::string> POS_Model::getCategoriesForPage(const size_t pageIndex
    , const size_t itemsPerPage) {
  std::vector<std::string> registeredCategories;
  size_t startIdx = pageIndex * itemsPerPage;
  size_t endIdx = startIdx + itemsPerPage;
  
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

std::vector<SupplyItem> POS_Model::getSuppliesForPage(const size_t pageIndex
    , const size_t itemsPerPage) {
  size_t startIdx = pageIndex * itemsPerPage;
  size_t endIdx = startIdx + itemsPerPage;
  
  endIdx = std::min(endIdx, this->supplies.size());
  
  size_t currentIdx = 0;
  std::vector<SupplyItem> pageSupplies;
  for (size_t index = startIdx; index < endIdx; ++index) {
    pageSupplies.emplace_back(this->supplies[index]);
  }
  
  return pageSupplies;
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

std::vector<std::string> POS_Model::getRegisteredCategories() {
  std::vector<std::string> categories;
  for (const auto& element : this->products) {
    categories.emplace_back(element.first);
  }
  return categories;
}

bool POS_Model::insertProduct(const std::string productCategory
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
  this->backupModule.updateProductsBackup(this->products);
  this->backupModule.updateSuppliesBackup(this->supplies);
  // Clears the vector memory.
  this->products.clear();
  this->started = false;
}
