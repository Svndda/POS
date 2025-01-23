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
  this->categories = this->backupModule.getProductsBackup();
  this->obtainProducts(this->products, this->categories);
  this->supplies = this->backupModule.getSuppliesBackup();
  // Changes the started value to true.
  this->started = true;
}

Product& POS_Model::findProduct(const std::string& productName) {
  // Iterates through the categories register.
  for (auto& [category, products] : this->categories) {
    // Iterates through the category's products.
    for (auto& product : products) {
      // Compares the product name with the names in the registers.
      if (product.getName() == productName) {
        // Returns the matching product.
        return product;
      }
    }
  }
  throw std::runtime_error("Product not found: " + productName);  
}

bool POS_Model::addProduct(const std::string& category
    , const Product& product) {
  // Checks that the given product and category isn't empty.
  if (!category.empty() && !(product == Product())) {
    // Try to emplace/add the product in the specifiec category.
    if (this->emplaceProduct(category, product, this->categories)) {
      qDebug() << "Producto anadido correctamente";
      // Updates the files containing teh products information backup.
      backupModule.updateProductsBackup(this->categories);
      return true;
    } 
  }
  return false;
}

bool POS_Model::addCategory(const std::string newCategory) {
  // Checks that the given new category isn't empty.
  if (!newCategory.empty()) {
    // Try to emplace the new category into the categories register.
    auto result = this->categories.try_emplace(
        newCategory, std::vector<Product>());
    // If the emplacement was successful, then.
    if (result.second) {
      // Updates the file that contains the products backup.
      backupModule.updateProductsBackup(this->categories);
      return true;
    } 
  }
  return false;
}

bool POS_Model::addSupply(const SupplyItem newSupply) {
  // Checks that the given supply to add isn't empty.
  if (!(newSupply == SupplyItem())) {
    // Try to find the given supply in the registered supplies of the pos.
    auto it = std::find(this->supplies.begin(), this->supplies.end()
        , newSupply);
    // If the given supply aren't registed, then.
    if (it == this->supplies.end()) {
      // Adds the new supply into the supplies registered.
      this->supplies.emplace_back(newSupply);
      // Updates the file containing the supplies backup information.
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
  // Checks that the given category and product aren't empty.
  if (!category.empty() && !(product == Product())) {
    // Try to erase the product from the specific category register.
    if (this->eraseProduct(category, product, this->categories)) {
      qDebug() << "producto elimnado correctamente";
      // Update the registered products.
      backupModule.updateProductsBackup(this->categories);
      return true;
    } 
  }
  return false;
}

bool POS_Model::removeCategory(const std::string category) {
  // Checks that the category isn't empty.
  if (!category.empty()) {
    // Try to erase the category key and related data from the
    // category registers.
    size_t remove = this->categories.erase(category);
    // If the deletion was successful
    if (remove) {
      // Update the registered products.
      this->obtainProducts(this->products, this->categories);
      qDebug() << "Categoria eliminada";
      return true;
    }
  }
  return false;
}

bool POS_Model::removeSupply(const SupplyItem &supply) {
  // Checks that the supply contain or not information.
  if (!supply.empty()) {
    // Try to find the supply in the exitisting supplies in the pos.
    auto it = std::find(this->supplies.begin(), this->supplies.end(), supply);
    // If there's a supply with the same name and measure in the pos, then.
    if (it != this->supplies.end()) {
      // Erase the supply from the existing supplies of the pos.
      this->supplies.erase(it);
      // Updates the long-term memory.
      this->backupModule.updateSuppliesBackup(this->supplies);
      qDebug() << "Se eliminó el suministro, correctamente.";
      // Indicates that the supply was removed correctly.
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
  // Try to erase the old product information from the category registers, then.
  if (this->eraseProduct(oldCategory, oldProduct, this->categories)) {
    // Try emplace the new product into the specific category.
    this->emplaceProduct(newCategory, newProduct, this->categories);
    // Updates the files that contains the pos products backup.
    backupModule.updateProductsBackup(this->categories);
    return true;
  }
  return false;
}

bool POS_Model::editCategory(const std::string oldCategory
    , const std::string newCategory) {
  // Checks that the categories names aren't empty.
  if (!oldCategory.empty() && !newCategory.empty()) {
    // Try to find the existing category with the old category references.
    auto existingCategory = this->categories.find(oldCategory);
    // If there's a eisting category in  the pos system, then.
    if (existingCategory != this->categories.end()) {
      // Saves the reference of the category's products.
      std::vector<Product> categoryProducts = existingCategory->second;      
      // Erase the existing category data contained in the map.
      this->categories.erase(existingCategory);
      qDebug() << "Categoria eliminada: " << this->categories.size();
      // Emplace/Add the a new space in the map with the new category name and
      // the old category's products.
      this->categories.emplace(newCategory, categoryProducts);
      qDebug() << "Mapa actualizado: " << this->categories.size();
      this->obtainProducts(this->products, this->categories);
      return true;
    } 
  }
  return false;
}

bool POS_Model::editSupply(const SupplyItem& oldSupply
    , const SupplyItem& newSupply) {
  // Checks that the provided supplies aren't equal.
  if (!(oldSupply == newSupply)) {
    // Try to find the provided old supply on the registered supplies.
    auto existingSupply = std::find(this->supplies.begin(), this->supplies.end()
                                    , oldSupply);
    // If there's a supply that matches, then.
    if (existingSupply != this->supplies.end()) {
      // Update the supply propperties.
      *existingSupply = newSupply;
      // Update the supplies backup.
      this->backupModule.updateSuppliesBackup(this->supplies);
      qDebug() << "Suministro editado correctamente.";
      return true;
    }
    qDebug() << "No se encontró un suministro con este nombre.";
  }
  qDebug() << "No se cambiaron los datos del suministro.";   
  return false;
}

std::vector<std::pair<std::string, Product>> POS_Model::getProductsForPage(
    const size_t pageIndex, const size_t itemsPerPage) {
  // Calculate the starting and end index for the products of this page.
  size_t startIdx = pageIndex * itemsPerPage;
  size_t endIdx = std::min(startIdx + itemsPerPage, this->products.size());
  
  // Temporal vector to store and return the products to display in this page.
  std::vector<std::pair<std::string, Product>> pageProducts;
  // Iterates between the start and ending indexes to access the products for
  // this page.
  for (size_t i = startIdx; i < endIdx; ++i) {
    // Adds the indexed product into the page products vector.
    pageProducts.push_back(this->products[i]);
  }
  return pageProducts;
}

std::vector<std::string> POS_Model::getCategoriesForPage(const size_t pageIndex
    , const size_t itemsPerPage) {
  // Calculate the starting and end index for the products of this page.  
  size_t startIdx = pageIndex * itemsPerPage;  
  size_t endIdx = std::min(startIdx + itemsPerPage, this->categories.size());
  
  // Temporal counter to manage the categories addition.
  size_t currentIdx = 0;
  // Temporal vector to store and return the categories to display in this page.  
  std::vector<std::string> registeredCategories;
  // Iterates through the categories map.
  for (const auto& [category, product] : this->categories) {
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
  // Calculate the starting and end index for the products of this page.  
  size_t startIdx = pageIndex * itemsPerPage;  
  size_t endIdx = std::min(startIdx + itemsPerPage, this->supplies.size());
  
  // Temporal counter to manage the categories addition.
  size_t currentIdx = 0;
  // Temporal vector to store and return the supplies to display in this page.    
  std::vector<SupplyItem> pageSupplies;
  // Iterates between the start and ending indexes to access the supplies for
  // this page.
  for (size_t index = startIdx; index < endIdx; ++index) {
    // Adds the indexed product into the page supplies vector.
    pageSupplies.emplace_back(this->supplies[index]);
  }
  
  return pageSupplies;
}

size_t POS_Model::getSizeOfCategory(std::string category) {
  // Temporal value to retunr in case the categories registers is zero.
  size_t no_value = std::numeric_limits<size_t>::max();
  // Checks that the category register name isn't empaty.
  if (!category.empty()) {
    // Iterates through the category registers.
    for (const auto& categoryRegister : this->categories) {
      // Checks the category name matches.
      if (categoryRegister.first == category) {
        // Returns the number of products contained in the category.
        return categoryRegister.second.size();
      }
    }
  }
  return no_value;
}

std::vector<std::string> POS_Model::getRegisteredCategories() {
  // Temporal vector to store the names of the registered categories.
  std::vector<std::string> categories;
  // Iterates through the categories register.
  for (const auto& element : this->categories) {
    // Emplace/Add the category's name to the vector.
    categories.emplace_back(element.first);
  }
  // Returns the names's vector.
  return categories;
}

bool POS_Model::emplaceProduct(const std::string productCategory
    , const Product& product
    , std::map<std::string, std::vector<Product>>& categoriesRegister) {
  // Transverse all the registered product categories.
  for (auto& category : categoriesRegister) {
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
      this->products.clear();     
      this->obtainProducts(this->products, this->categories);      
      return true;
    }
  }
  return false;
}

bool POS_Model::eraseProduct(const std::string productCategory
    , const Product& product
    , std::map<std::string, std::vector<Product>>& categoriesRegister) {
  // Transverse all the registered product categories.
  for (auto& category : categoriesRegister) {
    // Checks if the category matches.
    qDebug() << category.first << ":" << productCategory;
    if (category.first == productCategory) {
      // Transverse all the registered products in the category.
      for (size_t i = 0; i < category.second.size(); ++i) {
        // Check if there's a registered product in the category that matches
        // the product to delete.
        if (category.second[i] == product) {
          // Erase the product from the category.
          category.second.erase(category.second.begin() + i);
          this->products.clear();
          this->obtainProducts(this->products, this->categories);
          return true;
        }
      }
    }
  }
  return false;
}

void POS_Model::obtainProducts(
    std::vector<std::pair<std::string, Product>>& existingProducts
    , const std::map<std::string, std::vector<Product>>& categoryRegisters) {
  existingProducts.clear();
  // Transverse all the product type categories and their products.
  for (const auto& [category, products] : categoryRegisters) {
    // Transverse all the products of the category and added them to the vector.
    for (const auto& product : products) {
      // Emplace the product in the the product register.
      existingProducts.emplace_back(category, product);
    }
  }
}

QString POS_Model::formatProductIngredients(
    const std::vector<SupplyItem>& ingredients) {
  // Qstring temporal to contain all the product's ingredients information.
  QString formattedProductIngredients = "";
  
  // Iterates through all the ingredients vector.
  for (auto ingredient : ingredients) {
    // Obtain the name of the ingredient.
    QString ingredientName(ingredient.getName().data());
    // Obtain the ingredient's quantity.
    QString ingredientQuantity = QString::number(ingredient.getQuantity());
    // Obtain the ingredients measure unit.
    QString ingredientMeasure(ingredient.getMeasure().data());
    // Appends the ingredient's information into the formatted string.
    formattedProductIngredients = QString(formattedProductIngredients
        + " " + ingredientName.trimmed() + " : " + ingredientQuantity);
  }
  
  return formattedProductIngredients;
}

void POS_Model::shutdown() {
  // Writes out the registers of the dishes and drinks back to the backup files.
  this->backupModule.updateProductsBackup(this->categories);
  this->backupModule.updateSuppliesBackup(this->supplies);
  // Clears the vector memory.
  this->categories.clear();
  this->started = false;
}
