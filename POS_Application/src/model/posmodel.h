// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef POSMODEL_H
#define POSMODEL_H

#include <QString>
#include <vector>
#include <map>
#include <string>
#include "backupmodule.h"
#include "product.h"

/**
 * @class POS_Model
 * 
 * @brief A singleton class responsible for managing the Point of Sale (POS) system's products,
 *        categories, and supplies, as well as performing backup operations.
 * 
 * The POS_Model class serves as the core of the POS system, interacting with the backup module
 * to persist and load product and supply data. It allows for the manipulation of products and
 * categories, as well as managing the supply inventory. The class ensures that data is backed
 * up whenever changes are made and allows easy retrieval and modification of products.
 */
class POS_Model {
  // Deleted copy constructor and assignment operator to prevent copying.
  POS_Model(const POS_Model&) = delete;
  POS_Model operator=(const POS_Model) = delete;

private:
  BackupModule& backupModule; ///< Reference to the backup module for data persistence.
  std::map<std::string, std::vector<Product>> categories; ///< Map of product categories to their products.
  std::vector<std::pair<std::string, Product>> products; ///< Vector of products used for interface display.
  std::vector<SupplyItem> supplies; ///< Inventory of supplies.
  bool started = false; ///< Flag indicating if the POS model has been started.
  
public: ///< Public methods.
  /**
   * @brief Retrieves the singleton instance of POS_Model.
   * 
   * This method ensures that only one instance of POS_Model exists, initializing it with the
   * necessary file paths for backup operations.
   * 
   * @return Reference to the single instance of POS_Model.
   */
  static POS_Model& getInstance();
  
  /**
   * @brief Starts the POS model by loading backup data into memory.
   * 
   * This function loads product (beverages and dishes) data from the backup system and stores it
   * in the program's memory, marking the model as started.
   */
  void start();
  
  /**
   * @brief Shuts down the POS model, saving data and clearing memory.
   * 
   * This function writes the current product and supply data to backup files and clears
   * the in-memory data.
   */
  void shutdown();
  
  /**
   * @brief Finds a product by its name.
   * 
   * Searches through product categories to locate a specific product by its name.
   * 
   * @param productName Name of the product to search for.
   * @return Reference to the found product.
   * @throws std::runtime_error if the product is not found.
   */
  Product& findProduct(const std::string& productName);
  
  /**
   * @brief Adds a product to the specified category.
   * 
   * Inserts a product into a specific category.
   * 
   * @param category Category to add the product to.
   * @param product Product object to add.
   * @return True if the product was successfully added.
   */
  bool addProduct(const std::string& category, const Product& product);
  
  /**
   * @brief Adds a new product category.
   * 
   * Creates a new product category in the register.
   * 
   * @param newCategory Name of the new category.
   * @return True if the category was successfully added.
   */
  bool addCategory(const std::string newCategory);
  
  /**
   * @brief Adds a new supply to the inventory.
   * 
   * Inserts a new supply into the supply inventory.
   * 
   * @param newSupply Supply item to add.
   * @return True if the supply was successfully added.
   */
  bool addSupply(const SupplyItem newSupply);
  
  /**
   * @brief Removes a product from the specified category.
   * 
   * Removes a specific product from its category.
   * 
   * @param category Category of the product to remove.
   * @param product Product object to remove.
   * @return True if the product was successfully removed.
   */
  bool removeProduct(const std::string& category, const Product& product);
  
  /**
   * @brief Removes a product category.
   * 
   * Removes a product category along with its associated products.
   * 
   * @param category Name of the category to remove.
   * @return True if the category was successfully removed.
   */
  bool removeCategory(const std::string category);
  
  /**
   * @brief Removes a supply from the inventory.
   * 
   * Removes a specific supply from the inventory.
   * 
   * @param newSupply Supply item to remove.
   * @return True if the supply was successfully removed.
   */
  bool removeSupply(const SupplyItem& newSupply);
  
  /**
   * @brief Edits an existing product.
   * 
   * Replaces an existing product in its category with a new one.
   * 
   * @param oldCategory Original category of the product.
   * @param oldProduct Original product to replace.
   * @param newCategory New category for the product.
   * @param newProduct New product to replace the old one.
   * @return True if the product was successfully edited.
   */
  bool editProduct(const std::string& oldCategory, const Product& oldProduct
      , const std::string& newCategory, const Product& newProduct);
  
  /**
   * @brief Edits the name of an existing product category.
   * 
   * Changes the name of an existing product category.
   * 
   * @param oldCategory Original category name.
   * @param newCategory New category name.
   * @return True if the category was successfully edited.
   */
  bool editCategory(const std::string oldCategory
      , const std::string newCategory);
  
  /**
   * @brief Edits an existing supply.
   * 
   * Replaces an old supply with a new one containing updated information.
   * 
   * @param oldSupply Original supply.
   * @param newSupply New supply with updated details.
   * @return True if the supply was successfully edited.
   */
  bool editSupply(const SupplyItem& oldSupply
      , const SupplyItem& newSupply);
  
  /**
   * @brief Formats the ingredients of a product for display.
   * 
   * Converts a list of ingredients into a string format suitable for displaying
   * in the user interface.
   * 
   * @param ingredients List of ingredients for the product.
   * @return Formatted string representing the product's ingredients.
   */
  QString formatProductIngredients(
      const std::vector<SupplyItem>& ingredients);
  
private: ///< Private methods.
  /**
   * @brief Constructor for POS_Model.
   * 
   * Initializes POS_Model with a reference to a backup module.
   * 
   * @param module Reference to the backup module for data persistence.
   */
  POS_Model(BackupModule& module);
  
  /**
   * @brief Extracts products from the category registers and adds them to a vector.
   * 
   * Iterates through the category registers (map of categories) and extracts all
   * the products, adding them to the provided vector.
   * 
   * @param existingProducts Vector to store the extracted products.
   * @param categoryRegisters Map of registered products by category.
   */
  void obtainProducts(
      std::vector<std::pair<std::string, Product>>& existingProducts
      , const std::map<std::string, std::vector<Product>>& categoryRegisters);
  
  /**
   * @brief Emplace a product into the product register.
   * 
   * Emplace a product into the specified category, ensuring no duplicates exist.
   * 
   * @param productCategory Category of the product.
   * @param product Product object to insert.
   * @param categoriesRegister Category registers that contains the products.
   * @return True if the product was successfully emplaced.
   */
  bool emplaceProduct(const std::string productCategory
      , const Product& product
      , std::map<std::string, std::vector<Product>>& categoriesRegister);
  
  /**
   * @brief Erase a product from the product register.
   * 
   * Erase a product from the specified category.
   * 
   * @param productCategory Category of the product to remove.
   * @param product Product object to remove.
   * @param categoriesRegister Category registers that contains the products.
   * @return True if the product was successfully erased.
   */
  bool eraseProduct(const std::string productCategory
      , const Product& product
      , std::map<std::string, std::vector<Product>>& categoriesRegister);
  
  void updateProductsCategory(
    std::vector<std::pair<std::string, Product>>& products
      , const std::string& category);
  
public: ///< Public getter methods.
  /**
   * @brief Checks if the POS model has been started.
   * 
   * @return True if the POS model is started.
   */
  inline bool isStarted() {
    return this->started;
  }
  
  /**
   * @brief Retrieves the registered categories in the system.
   * 
   * @return Reference to the map of registered categories by category.
   */
  std::map<std::string, std::vector<Product>>& getRegisteredProducts() {
    return this->categories;
  }
  
  /**
   * @brief Retrieves the categories of products registered in the system.
   * 
   * @return A vector containing the names of the registered categories.
   */
  std::vector<std::string> getRegisteredCategories();
  
  /**
   * @brief Retrieves the registered supplies in the system.
   * 
   * @return Reference to the vector of registered supplies.
   */
  std::vector<SupplyItem>& getRegisteredSupplies() {
    return this->supplies;
  }
  
  /**
   * @brief Retrieves the total number of products registered.
   * 
   * @return The total number of registered products.
   */
  size_t getNumberOfProducts() {
    return this->products.size();
  }
  
  /**
   * @brief Retrieves the total number of categories registered.
   * 
   * @return The total number of registered categories.
   */
  size_t getNumberOfCategories() {
    return this->categories.size();
  }
  
  /**
   * @brief Retrieves the total number of supplies registered.
   * 
   * @return The total number of registered supplies.
   */
  size_t getNumberOfSupplies() {
    return this->supplies.size();
  }
  
  /**
   * @brief Retrieves product categories for a specific page.
   * 
   * @param pageIndex Index of the page.
   * @param itemsPerPage Number of items per page.
   * @return Vector containing category names for the page.
   */
  std::vector<std::string> getCategoriesForPage(const size_t pageIndex
      , const size_t itemsPerPage);
  
  /**
   * @brief Retrieves the size of a specific category.
   * 
   * @param category Name of the category.
   * @return The size of the category or a maximum value if not found.
   */
  size_t getSizeOfCategory(std::string category);
  
  /**
   * @brief Retrieves products for a specific page.
   * 
   * @param pageIndex Index of the page.
   * @param itemsPerPage Number of products per page.
   * @return A vector of products for the specified page.
   */
  std::vector<std::pair<std::string, Product>> getProductsForPage(
      const size_t pageIndex , const size_t itemsPerPage);
  
  /**
   * @brief Retrieves supplies for a specific page.
   * 
   * @param pageIndex Index of the page.
   * @param itemsPerPage Number of supplies per page.
   * @return A vector of supplies for the specified page.
   */
  std::vector<SupplyItem> getSuppliesForPage(const size_t pageIndex
      , const size_t itemsPerPage);
  
};

#endif // APPMODEL_H
