// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef POSMODEL_H
#define POSMODEL_H

#include <QString>
#include <vector>
#include <map>
#include <string>

#include "user.h"
#include "backupmodule.h"
#include "product.h"

/**
 * @class POS_Model
 * @brief Core singleton class managing the POS system.
 *
 * POS_Model is responsible for handling products, categories, supplies and user data.
 * It interacts with the BackupModule to load and persist data, and provides functions
 * for adding, editing, and removing items in the system.
 */
class POS_Model {
  // Deleted copy constructor and assignment operator to prevent copying.
  POS_Model(const POS_Model&) = delete;
  POS_Model operator=(const POS_Model) = delete;

private:
  User user = User(); ///< Currently logged in user.
  std::vector<User> registeredUsers;   ///< Registered users loaded from backup.
  BackupModule& backupModule; ///< Reference to the backup module for data persistence.
  std::map<std::string, std::vector<Product>> categories; ///< Map of product categories to their products.
  std::vector<std::pair<std::string, Product>> products;   ///< Vector of products for interface display.
  std::vector<Supply> supplies; ///< Inventory of supplies.
  size_t closedReceipts = 0;  ///< Counter of closed receipts.
  bool started = false;       ///< Flag indicating if the model has been started.
  
public:
  /**
   * @brief Checks if the POS model has been started.
   * @return True if the model is started.
   */
  inline bool isStarted() { return this->started; }
  
  size_t getPageAccess(const size_t page);
  
  /**
   * @brief Retrieves the currently logged in user.
   * @return Reference to the current User.
   */
  const User& getCurrentUser() { return this->user; } ;
  
  /**
   * @brief Retrieves the registered products categorized.
   * @return Reference to the map of registered products.
   */
  std::map<std::string, std::vector<Product>>& getRegisteredProductsMap() {
    return this->categories;
  }
  
  /**
   * @brief Retrieves the products vector for UI display.
   * @return Reference to the vector of registered products.
   */
  std::vector<std::pair<std::string, Product>>& getRegisteredProductsVector() {
    return this->products;
  }
  
  /**
   * @brief Retrieves the list of registered category names.
   * @return Vector of category names.
   */
  std::vector<std::string> getRegisteredCategories();
  
  /**
   * @brief Retrieves the registered supplies.
   * @return Reference to the vector of supplies.
   */
  std::vector<Supply>& getRegisteredSupplies() { return this->supplies; }
  
  /**
   * @brief Retrieves the registered users.
   * @return Reference to the vector of users.
   */
  std::vector<User>& getRegisteredUsers() { return this->registeredUsers; }
  
  /**
   * @brief Retrieves the total number of registered products.
   * @return Number of products.
   */
  size_t getNumberOfProducts() { return this->products.size(); }
  
  /**
   * @brief Retrieves the total number of registered categories.
   * @return Number of categories.
   */
  size_t getNumberOfCategories() { return this->categories.size(); }
  
  /**
   * @brief Retrieves the total number of registered supplies.
   * @return Number of supplies.
   */
  size_t getNumberOfSupplies() { return this->supplies.size(); }
  
  /**
   * @brief Retrieves the total number of registered users.
   * @return Number of users.
   */
  size_t getNumberOfUsers() { return this->registeredUsers.size(); }
  
  /**
   * @brief Retrieves the size (number of products) of a specific category.
   *
   * @param category The category name.
   * @return Number of products in the category, or std::numeric_limits<size_t>::max() if not found.
   */
  size_t getSizeOfCategory(std::string category);
  
  /**
   * @brief Retrieves products for a given page.
   *
   * Paginates the products vector based on the specified page index and items per page.
   *
   * @param pageIndex The page index.
   * @param itemsPerPage Number of products per page.
   * @return Vector of products for the specified page.
   */
  std::vector<std::pair<std::string, Product>> getProductsForPage(
      const size_t pageIndex, const size_t itemsPerPage);
  
  /**
   * @brief Retrieves product categories for a given page.
   *
   * Paginates the category names based on the specified page index and items per page.
   *
   * @param pageIndex The page index.
   * @param itemsPerPage Number of items per page.
   * @return Vector of category names for the page.
   */
  std::vector<std::string> getCategoriesForPage(
      const size_t pageIndex, const size_t itemsPerPage);
  
  /**
   * @brief Retrieves supplies for a given page.
   *
   * Paginates the supplies vector based on the specified page index and items per page.
   *
   * @param pageIndex The page index.
   * @param itemsPerPage Number of supplies per page.
   * @return Vector of supplies for the specified page.
   */
  std::vector<Supply> getSuppliesForPage(
      const size_t pageIndex, const size_t itemsPerPage);
  
  /**
   * @brief Retrieves users for a given page.
   *
   * Paginates the users vector based on the specified page index and items per page.
   *
   * @param pageIndex The page index.
   * @param itemsPerPage Number of users per page.
   * @return Vector of users for the specified page.
   */
  std::vector<User> getUsersForPage(
      const size_t pageIndex, const size_t itemsPerPage);
  
public:
  /**
   * @brief Retrieves the singleton instance of POS_Model.
   * @return Reference to the single instance of POS_Model.
   */
  static POS_Model& getInstance();
  
  /**
   * @brief Starts the POS model.
   *
   * Loads user data from backup and, if the provided user is registered, loads
   * product and supply data into memory.
   *
   * @param user The user to start the model with.
   * @return True if the model has been successfully started.
   */
  bool start(const User& user);
  
  /**
   * @brief Shuts down the POS model.
   *
   * Saves the current product and supply data to backup files and clears internal data.
   */
  void shutdown();
  
  /**
   * @brief Finds a product by its name.
   *
   * Searches all product categories for a product matching the provided name.
   *
   * @param productName The name of the product to search for.
   * @return Reference to the found Product.
   * @throws std::runtime_error if the product is not found.
   */
  Product& findProduct(const std::string& productName);
  
  /**
   * @brief Increments the closed receipts counter.
   *
   * Call this method each time a transaction is completed successfully.
   */
  void increaseClosedReceiptCount() { ++this->closedReceipts; }
  
  /**
   * @brief Adds a product to a specified category.
   *
   * Inserts the product into the given category if no duplicate exists.
   *
   * @param category The category name.
   * @param product The Product to add.
   * @return True if the product was added successfully.
   */
  bool addProduct(const std::string& category, const Product& product);
  
  /**
   * @brief Adds a new product category.
   *
   * Creates a new category in the system.
   *
   * @param newCategory The name of the new category.
   * @return True if the category was added successfully.
   */
  bool addCategory(const std::string newCategory);
  
  /**
   * @brief Adds a new supply to the inventory.
   *
   * Inserts a new supply item if it does not already exist.
   *
   * @param newSupply The Supply to add.
   * @return True if the supply was added successfully.
   */
  bool addSupply(const Supply newSupply);
  
  /**
   * @brief Adds a new user to the pos system.
   *
   * Inserts a new user into the pos system, if it does not already exist.
   *
   * @param newUser The User to add.
   * @return True if the user was added successfully.
   */
  bool addUser(const User newUser);
  
  /**
   * @brief Removes a product from a specified category.
   *
   * Erases the given product from its category.
   *
   * @param category The category from which to remove the product.
   * @param product The Product to remove.
   * @return True if the product was removed successfully.
   */
  bool removeProduct(const std::string& category, const Product& product);
  
  /**
   * @brief Removes a product category.
   *
   * Erases a category and all its associated products.
   *
   * @param category The name of the category to remove.
   * @return True if the category was removed successfully.
   */
  bool removeCategory(const std::string category);
  
  /**
   * @brief Removes a supply from the inventory.
   *
   * Erases the specified supply from the inventory.
   *
   * @param supply The Supply to remove.
   * @return True if the supply was removed successfully.
   */
  bool removeSupply(const Supply& supply);
  
  /**
   * @brief Removes a user from the pos system.
   *
   * Erases the specified user from the pos system.
   *
   * @param user The User to remove.
   * @return True if the user was removed successfully.
   */
  bool removeUser(const User& user);
  
  /**
   * @brief Edits an existing product.
   *
   * Replaces an old product with a new one, potentially changing its category.
   *
   * @param oldCategory The original category.
   * @param oldProduct The product to replace.
   * @param newCategory The new category for the product.
   * @param newProduct The new product data.
   * @return True if the product was edited successfully.
   */
  bool editProduct(const std::string& oldCategory, const Product& oldProduct,
      const std::string& newCategory, const Product& newProduct);
  
  /**
   * @brief Edits the name of an existing product category.
   *
   * Changes the category name while retaining its associated products.
   *
   * @param oldCategory The original category name.
   * @param newCategory The new category name.
   * @return True if the category name was updated successfully.
   */
  bool editCategory(const std::string oldCategory
      , const std::string newCategory);
  
  /**
   * @brief Edits an existing supply.
   *
   * Updates an existing supply with new information.
   *
   * @param oldSupply The original supply.
   * @param newSupply The new supply data.
   * @return True if the supply was edited successfully.
   */
  bool editSupply(const Supply& oldSupply, const Supply& newSupply);
  
  /**
   * @brief Edits an existing user information in the pos system.
   *
   * Updates an existing user with new information.
   *
   * @param oldUser The original user.
   * @param newUser The new user data.
   * @return True if the user was edited successfully.
   */
  bool editUser(const User& oldUser, const User& newUser);
  
  /**
   * @brief Formats product ingredients for display.
   *
   * Converts a list of supplies (ingredients) into a formatted QString for UI display.
   *
   * @param ingredients Vector of Supply objects.
   * @return QString containing the formatted ingredients.
   */
  QString formatProductIngredients(const std::vector<Supply>& ingredients);
  
  /**
   * @brief Formats user's permissions to be displayed as QString.
   *
   * Converts a list of user's permissions into a formatted QString for UI display.
   *
   * @param user Constant user object containing the information of the user's permissions.
   * @return QString containing the formatted user's permissions.
   */
  QString formatUserPermissions(const User user);
  
private:
  /**
   * @brief Private constructor for POS_Model.
   *
   * Initializes the model with a reference to the backup module.
   *
   * @param module Reference to the BackupModule.
   */
  POS_Model(BackupModule& module);
  
  /**
   * @brief Checks if a user is registered.
   *
   * Searches through the registered users to see if the provided user exists.
   *
   * @param user The User to check.
   * @return True if the user is found; otherwise, false.
   */
  bool isUserRegistered(const User& user);
  
  /**
   * @brief Loads product and supply data from backups.
   *
   * Reads products and supplies data from the backup module and stores them in memory.
   */
  void loadProductsBackups();
  
  /**
   * @brief Populates the products vector from category registers.
   *
   * Iterates over the categories map and extracts products into a vector for UI display.
   *
   * @param existingProducts Vector to store the extracted products.
   * @param categoryRegisters Map of categories to products.
   */
  void obtainProducts(
      std::vector<std::pair<std::string, Product>>& existingProducts
      , const std::map<std::string, std::vector<Product>>& categoryRegisters);
  
  /**
   * @brief Emplaces a product into the specified category.
   *
   * Adds a product to the category register if it does not already exist.
   *
   * @param productCategory The target category.
   * @param product The Product to insert.
   * @param categoriesRegister The map of categories to products.
   * @return True if the product was successfully inserted.
   */
  bool emplaceProduct(const std::string productCategory, const Product& product
      , std::map<std::string, std::vector<Product>>& categoriesRegister);
  
  /**
   * @brief Erases a product from the specified category.
   *
   * Removes a product from the category register.
   *
   * @param productCategory The category from which to remove the product.
   * @param product The Product to remove.
   * @param categoriesRegister The map of categories to products.
   * @return True if the product was successfully removed.
   */
  bool eraseProduct(const std::string productCategory, const Product& product,
      std::map<std::string, std::vector<Product>>& categoriesRegister);
};

#endif // APPMODEL_H
