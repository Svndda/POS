#ifndef POSMODEL_H
#define POSMODEL_H

#include <QString>
#include <vector>
#include <map>
#include <string>
#include "backupmodule.h"
#include "product.h"

class POS_Model {
  // Copy and assignation constructors disabled.
  POS_Model(const POS_Model&) = delete;
  POS_Model operator=(const POS_Model) = delete;
public:
  // Macros representing different types of register operations.
  static const size_t UPDATE; ///< Represents an update operation.
  static const size_t INSERT; ///< Represents an insert operation.
  static const size_t DELETE; ///< Represents a delete operation.
  
private:
  BackupModule& backupModule; ///< Reference to the BackupModule instance used for data persistence.
  std::map<std::string, std::vector<Product>> products; ///< Map of dish categories to their respective products.
  std::vector<std::pair<std::string, Product>> productsVector;
  std::vector<SupplyItem> itemsOnInventory; ///< Inventory of supply items.
  bool started = false; ///< Flag indicating whether the POS model has started.
  
public: ///< Class functions.
  /**
   * @brief Returns the singleton instance of the POS_Model class.
   * 
   * This function ensures that only one instance of the POS_Model class exists
   * and initializes the BackupModule with the necessary file paths.
   * 
   * @return A reference to the singleton POS_Model instance.
   */
  static POS_Model& getInstance();
  /**
   * @brief Starts the POS_Model, loading backup data into memory.
   * 
   * This function reads the backup data for drinks and dishes, storing them 
   * in the program's memory for later use during execution. It also sets the
   * 'started' flag to true.
   */
  void start();
  /**
   * @brief Shuts down the POS_Model, saving data and cleaning up.
   * 
   * This function writes the current product data for drinks and dishes to the 
   * backup files and clears the in-memory data.
   */
  void shutdown();
  
  Product& findProduct(const std::string& productName);
  /**
   * @brief Updates the product register based on the specified operation.
   * 
   * This function modifies the product register based on the operation type 
   * (INSERT or DELETE). It updates the backup files immediately after the 
   * changes are made.
   * 
   * @param productCategory The category of the product to be added or removed.
   * @param product The Product object to be added or removed.
   * @param operation The type of operation (INSERT, UPDATE, DELETE) to be performed.
   */
  void updateProductRegisters(const std::string productCategory,
      const Product& product, const size_t operation);
  
  void addProduct(const std::string& category, const Product& product);
  
  void addCategory(const std::string newCategory);
  
  void removeProduct(const std::string& category, const Product& product);
  
  void removeCategory(const std::string category);
  
  void editProduct(const std::string& oldCategory, const Product& oldProduct
      , const std::string& newCategory, const Product& newProduct);
  
  void editCategory(const std::string oldCategory
      , const std::string newCategory);
    
  QString formatProductIngredients(
      const std::vector<SupplyItem>& ingredients);
  
private: ///< Class functions.
  /**
   * @brief Constructor for the POS_Model class.
   * 
   * Initializes the POS_Model instance with a reference to a BackupModule object.
   * 
   * @param module The BackupModule instance to interact with backup files.
   */
  POS_Model(BackupModule& module);
  
  /**
   * @brief Extracts products from a product register and adds them to a vector.
   * 
   * This function transverses through a product register (a map of product categories) 
   * and extracts all the products, adding them to the provided vector.
   * 
   * @param registeredProducts A vector to store the extracted products.
   * @param productTypeRegister The map of registered products by category.
   */
  void extractProducts(
      std::vector<std::pair<std::string, Product>>& registeredProducts
      , const std::map<std::string, std::vector<Product>>& productTypeRegister);
  
  /**
   * @brief Inserts a product into the specified product register.
   * 
   * This function inserts a product into the product register if the product 
   * doesn't already exist in the specified category. It checks for duplicates 
   * and ensures that no identical product exists before adding it to the category.
   * 
   * @param productCategory The category of the product to be inserted.
   * @param product The Product object to be inserted.
   * @param productTypeRegister The register (map) of products.
   * @return True if the product was inserted successfully, false if the product 
   *         already exists in the category.
   */
  bool insertProduct(const std::string productCategory
      , const Product& product
      , std::map<std::string, std::vector<Product>>& productTypeRegister);
  
  /**
   * @brief Removes a product from the specified product register.
   * 
   * This function removes a product from the product register, given the product's 
   * category and the product itself. It searches through the product category and 
   * deletes the product if found.
   * 
   * @param productCategory The category of the product to be removed.
   * @param product The Product object to be removed.
   * @param productTypeRegister The register (map) of products.
   * @return True if the product was removed successfully, false if the product 
   *         was not found in the category.
   */
  bool eraseOnRegister(const std::string productCategory
      , const Product& product
      , std::map<std::string, std::vector<Product>>& productTypeRegister);
  
public: ///< Class getters.
  inline bool isStarted() {
    return this->started;
  }
  
  std::map<std::string, std::vector<Product>>& getRegisteredProducts() {
    return this->products;
  }
  
  size_t getNumberOfProducts() {
    return this->productsVector.size();
  }
  
  std::vector<std::string> getRegisteredCategories();
  
  size_t getNumberOfCategories() {
    return this->products.size();
  }
  
  std::vector<std::string> getCategoriesForPage(size_t pageIndex);
  
  size_t getSizeOfCategory(std::string category);
  
  std::vector<std::pair<std::string, Product>> getProductsForPage(
      size_t pageIndex);
  
};

#endif // APPMODEL_H
