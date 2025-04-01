// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include <vector>
#include <limits>

#include <QDebug>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>

#include "model.h"
#include "backupmodule.h"
#include "order.h"

Model::Model(BackupModule& module)
    : backupModule(module) {
}

Model& Model::getInstance() {
  // Creates an static instance of the POS MODEL.
  static Model instance(BackupModule::getInstance());
  return instance;
}

bool Model::start(const User& user) {
  // Obtains the registrered users information.
  this->registeredUsers = this->backupModule.getUsersBackup();
  qDebug() << "usuarios registrados: " << this->registeredUsers.size();
  // Checks if the given user is registered.
  if (this->isUserRegistered(user)) {
    // Loads the products information.
    this->loadSystemBackups();
    qDebug() << "Model encendido.";
    // Change model state flag to true.
    this->started = true;
  }
  // Returns the model state flag.
  return this->started;
}

void Model::shutdown() {
  // Cheks if the model is started.
  if (this->isStarted()) {
    // Writes out the registers of the products information.
    this->backupModule.updateProductsBackup(this->categories);
    this->backupModule.updateSuppliesBackup(this->supplies);
    this->closeCashier();
    this->backupModule.updateReceiptsBackup(this->currentReceiptID
        , this->registeredReceipts);
    // Clears the model memory.
    this->categories.clear();
    this->products.clear();
    this->supplies.clear();
    this->registeredUsers.clear();
    this->ongoingReceipts.clear();
    this->user = User();
    // Sets the model state flag to false.
    this->started = false;
  }
}

void Model::openCashier() {
  this->ongoingReceipts.clear();
  this->cashierOpened = true;
}

void Model::closeCashier() {
  // Checks if the cashier has receipts.
  if (!this->ongoingReceipts.empty()) {
    // Inserts the cashier receipts into the backup ones.
    for (const auto& receipt : this->ongoingReceipts) {
      this->registeredReceipts.emplace_back(receipt);
    }
    // Updates the backup receipts.
    this->backupModule.updateReceiptsBackup(this->registeredReceipts.size()
        ,this->registeredReceipts);
    // Clears the cashier receipts.
    this->ongoingReceipts.clear();
  }
  // Sets the cashier state to false.
  this->cashierOpened = false;
}

std::vector<std::string> Model::getRegisteredCategories() {
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

size_t Model::getSizeOfCategory(std::string category) {
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

std::vector<std::pair<std::string, Product>> Model::getProductsForPage(
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

std::vector<std::string> Model::getCategoriesForPage(const size_t pageIndex
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

std::vector<Supply> Model::getSuppliesForPage(const size_t pageIndex
    , const size_t itemsPerPage) {
  // Calculate the starting and end index for the products of this page.  
  size_t startIdx = pageIndex * itemsPerPage;  
  size_t endIdx = std::min(startIdx + itemsPerPage, this->supplies.size());
  
  // Temporal counter to manage the categories addition.
  size_t currentIdx = 0;
  // Temporal vector to store and return the supplies to display in this page.    
  std::vector<Supply> pageSupplies;
  // Iterates between the start and ending indexes to access the supplies for
  // this page.
  for (size_t i = startIdx; i < endIdx; ++i) {
    // Adds the indexed product into the page supplies vector.
    pageSupplies.emplace_back(this->supplies[i]);
  }
  
  return pageSupplies;
}

std::vector<User> Model::getUsersForPage(const size_t pageIndex
    , const size_t itemsPerPage) {
  // Calculate the starting and end index for the user of this page.  
  size_t startIdx = pageIndex * itemsPerPage;  
  size_t endIdx = std::min(startIdx + itemsPerPage
    , this->registeredUsers.size());
  
  // Temporal counter to manage the user addition.
  size_t currentIdx = 0;
  // Temporal vector to store and return the users to display in this page.    
  std::vector<User> pageUsers;
  // Iterates between the start and ending indexes to access the user for
  // this page.
  for (size_t i = startIdx; i < endIdx; ++i) {
    // Adds the indexed user into the page users vector.
    pageUsers.emplace_back(this->registeredUsers[i]);
  }
  
  return pageUsers;
}

Product& Model::findProduct(const std::string& productName) {
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

bool Model::generateReceipt(const Order& order) {
  // Creates a new receipt from the given order.
  Receipt newReceipt("Macana's Place"
      , ++this->currentReceiptID, this->user.getUsername().data(), order
  );
  // Emplace the receipt into the cashier receipts.
  this->ongoingReceipts.emplace_back(newReceipt);
  
  // Obstains the products information from the order.
  std::vector<std::pair<Product, size_t>> productsInfo
      = order.getProducts();
  
  // Tranverse all the products from the order.
  for (const auto& element : productsInfo) {
    // Obtains the products quantity.
    const size_t quantity = element.second;
    // Obtains the product's ingredients.
    const std::vector<Supply> productSupplies = element.first.getIngredients();
    // Tranverse all the product's ingredients.
    for (const auto& supply : productSupplies) {
      // Obtains the supply's name.
      const std::string supplyName = supply.getName();
      // Try to find the supply into the registered supplies.
      auto it = std::find_if(this->supplies.begin(), this->supplies.end(),
          [supplyName](const Supply& registeredSupply) {
          return registeredSupply.getName() == supplyName;
      });
      
      // Checks if the supply was registered.
      if (it != this->supplies.end()) {
        const size_t registerIndex = std::distance(this->supplies.begin(), it);
        // Decrements the product's ingredients from the registered supplies.
        for (size_t i = 0; i < quantity; ++i) {
          if (this->supplies[registerIndex].getQuantity() > 0) {
            this->supplies[registerIndex] - supply;
          }
        }
      } else {
        qDebug() << "Suministro no registrado.";
      }
    }
  }
  qDebug() << "Recibo anadido correctamente, recibo numero: "
      << this->ongoingReceipts.size();
  return true;
}

bool Model::addProduct(const std::string& category
    , const Product& product) {
  // Checks that the given product and category isn't empty.
  if (!category.empty() && !(product == Product())) {
    // Try to emplace/add the product in the specifiec category.
    if (this->emplaceProduct(category, product, this->categories)) {
      qDebug() << "Producto anadido correctamente";
      // Updates the files containing teh products information backup.
      this->backupModule.updateProductsBackup(this->categories);
      return true;
    } 
  }
  return false;
}

bool Model::addCategory(const std::string newCategory) {
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

bool Model::addSupply(const Supply newSupply) {
  // Temporal to adapt to avoid checking the measure, cause the ui always give
  // it.
  const Supply baseSupply("", 0, newSupply.getMeasure());
  // Checks that the given supply to add isn't empty.
  if (!(newSupply == baseSupply)) {
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
    } else {
      qDebug() << "No se añadió el suministro, ya existe uno con este nombre.";
    }
  } else {
    qDebug() << "No se añadió el suministro, el recipiente estaba vacio.";    
  }
  return false;
}

bool Model::addUser(const User newUser) {
  // Temporal to adapt to avoid checking the measure, cause the ui always give
  // it.
  const User baseUser;
  // Checks that the given user to add isn't empty.
  if (!(newUser == baseUser)) {
    // Try to find the given user in the registered users of the pos.
    auto it = std::find(this->registeredUsers.begin()
        , this->registeredUsers.end(), newUser);
    // If the given user aren't registed, then.
    if (it == this->registeredUsers.end()) {
      // Adds the new user into the users registered.
      this->registeredUsers.emplace_back(newUser);
      // Updates the file containing the users backup information.
      this->backupModule.updateUsersBackup(this->registeredUsers);
      qDebug() << "Se añadió el usuario, correctamente.";
      return true;
    } else {
      qDebug() << "No se añadió el usuario, ya existe uno con este nombre.";
    }
  } else {
    qDebug() << "No se añadió el usuario, el recipiente estaba vacio.";    
  }
  return false;
}

bool Model::removeProduct(const std::string& category
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

bool Model::removeCategory(const std::string category) {
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

bool Model::removeSupply(const Supply &supply) {
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
    } else {
      qDebug() << "No se eliminó el suministro, no existe uno con este nombre.";      
    }
  } else {
    qDebug() << "No se eliminó el suministro, el recipiente estaba vacio.";
  }
  return false;
}

bool Model::removeUser(const User& user) {
  // Checks that the user contain or not information.
  if (!(user == User())) {
    // Try to find the user in the exitisting users in the pos.
    auto it = std::find(this->registeredUsers.begin()
        , this->registeredUsers.end(), user);
    // If there's a user with the same name and measure in the pos, then.
    if (it != this->registeredUsers.end()) {
      // Erase the user from the existing users of the pos.
      this->registeredUsers.erase(it);
      // Updates the long-term memory.
      this->backupModule.updateUsersBackup(this->registeredUsers);
      qDebug() << "Se eliminó el usuario, correctamente.";
      // Indicates that the user was removed correctly.
      return true;
    } else {
      qDebug() << "No se eliminó el usuario, no existe uno con este nombre.";      
    }
  } else {
    qDebug() << "No se eliminó el usuario, el recipiente estaba vacio.";    
  }
  return false;
}

bool Model::editProduct(const std::string& oldCategory
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

bool Model::editCategory(const std::string oldCategory
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

bool Model::editSupply(const Supply& oldSupply
    , const Supply& newSupply) {
  // Checks that the provided supplies aren't equal.
  if (!(oldSupply == newSupply) && !newSupply.empty()) {
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
    } else {
      qDebug() << "No se encontró un suministro con este nombre.";      
    }
  } else {
    qDebug() << "No se cambiaron los datos del suministro.";       
  }
  return false;
}

bool Model::editUser(const User& oldUser
    , const User& newUser) {
  qDebug() << "Modelo revisando la edicion de usuarios.";
  
  // Checks that the provided users aren't equal.
  if (newUser != User()) {
    // Try to find the provided old user on the registered users.
    auto existingUser = std::find(this->registeredUsers.begin()
        , this->registeredUsers.end(), oldUser);
    qDebug() << "Datos del usuario: " << oldUser.getUsername() << " " << oldUser.getID();
    
    // If there's a user that matches, then.
    if (existingUser != this->registeredUsers.end()) {
      // Update the user propperties.
      *existingUser = newUser;
      // Update the user backup.
      this->backupModule.updateUsersBackup(this->registeredUsers);
      qDebug() << "Usuario editado correctamente.";
      return true;
    } else {
      qDebug() << "No se encontró un usuario con estos datos.";      
    }
  } else {
    qDebug() << "No se cambiaron los datos del usuario.";       
  }
  return false;
}

QString Model::formatProductIngredients(
    const std::vector<Supply>& ingredients) {
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

QString Model::formatUserPermissions(const User user) {
  QString formattedPermissions = "";
  
  QString pageAccessAllowed("Visualizar. ");
  QString pageEditionAllowed("Edición. ");
  QString pageAccessDenied("Denegado. ");
  
  const std::vector<User::PageAccess> permissions = user.getUserPermissions();
  for (size_t i = 1; i < permissions.size(); ++i) {
    switch (i) {
      case 1: formattedPermissions += "Punto de Venta : "; 
        break;
      case 2: formattedPermissions += "Inventario : "; 
        break;
      case 3: formattedPermissions += "Ventas : "; 
        break;
      case 4: formattedPermissions += "\nUsuarios : "; 
        break;
      case 5: formattedPermissions += "Ajustes : "; 
        break;
      default:
        break;
    }
      
    if (permissions[i].access == User::PageAccess::EDITABLE) {
      formattedPermissions += pageEditionAllowed;
    } else if (permissions[i].access == User::PageAccess::NON_EDITABLE) {
      formattedPermissions += pageAccessAllowed;
    } else if (permissions[i].access == User::PageAccess::DENIED) {
      formattedPermissions += pageAccessDenied;
    }
  }
  
  return formattedPermissions;
}

bool Model::isUserRegistered(const User& user) {
  // Transverse all the registered users.
  for (size_t i = 0; i < this->registeredUsers.size(); ++i) {
    // Checks if the given user information matches.
    if (this->registeredUsers[i] == user) {
      // Save the user information into the current user holder.
      this->user = this->registeredUsers[i];
      return true;
    }
  }
  qDebug() << "el usuario no esta registrado";  
  return false;
}

void Model::loadSystemBackups() {
  // Reads and store the backup to the program memory to use them in the
  // program execution.
  this->categories = this->backupModule.getProductsBackup();
  this->obtainProducts(this->products, this->categories);
  this->supplies = this->backupModule.getSuppliesBackup();
  this->registeredReceipts = this->backupModule.getReceiptsBackup();
  this->currentReceiptID = this->registeredReceipts.size();
}

void Model::obtainProducts(
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

bool Model::emplaceProduct(const std::string productCategory
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

bool Model::eraseProduct(const std::string productCategory
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
