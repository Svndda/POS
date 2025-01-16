#ifndef BACKUPMODULE_H
#define BACKUPMODULE_H

#include <map>
#include <string>
#include "product.h"

  /**
   * @class BackupModule
   * @brief Handles reading and writing of product backup data from files.
   * 
   * The `BackupModule` class provides functionality for loading and saving product data 
   * such as drinks and dishes to and from backup files. It supports reading product categories, 
   * their ingredients, and prices from files, as well as writing the modified data back to files.
   */
class BackupModule {
private:
  const std::string PRODUCTS_BACKUP_FILE = "C:\\Users\\aaron\\Dev\\Repositories" \
      "\\POS\\POS_Application\\backup\\products\\Products.txt"; ///< Represents an update operation.

public:
  /**
   * @brief Gets the singleton instance of the BackupModule class.
   * 
   * This function ensures that there is only one instance of `BackupModule` used 
   * in the application.
   * 
   * @return A reference to the static singleton instance of the `BackupModule` class.
   */
  static BackupModule& getInstance();

  /**
   * @brief Reads the backup data for drinks.
   * 
   * This function reads the drink-related backup file specified in the backup filenames 
   * and returns a map of registered products categorized by type.
   * 
   * @return A map where the key is a product category and the value is a vector of Product objects.
   */
  std::map<std::string, std::vector<Product>> getProductsBackup();
  
  void writeRegistersBackUp(
      const std::map<std::string, std::vector<Product>>& products);
private:
  BackupModule();
  
  /**
   * @brief Reads the backup data for products from a specified file.
   * 
   * This function reads the backup data from the provided file. It parses the file to extract product 
   * categories, product names, ingredients, and prices, and stores the information in a map.
   * 
   * @param filename The name of the backup file to read from.
   * @throws std::runtime_error If there is an issue opening or reading from the file.
   */
  void readProductsBackup(
      const std::string& filename
      , std::map<std::string, std::vector<Product>>& registeredProducts);
  
  /**
   * @brief Writes the backup data for drinks to a file.
   * 
   * This function writes the current state of registered drinks to the backup file.
   * 
   * @param registeredDrinks A map where the key is a product category and the value is a vector of Product objects.
   */
  void writeProductsBackup(
      const std::string& filename,
      const std::map<std::string, std::vector<Product>>& registeredProducts);
  // Copy and assignation constructors disabled.
  BackupModule(const BackupModule&) = delete;
  BackupModule& operator=(const BackupModule&) = delete;
};

#endif // BACKUPMODULE_H
