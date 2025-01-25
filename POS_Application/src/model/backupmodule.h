// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef BACKUPMODULE_H
#define BACKUPMODULE_H

#include <map>
#include <string>
#include "product.h"

/**
 * @class BackupModule
 * @brief Handles the backup and restoration of product and supply data from files.
 *
 * The `BackupModule` class facilitates reading and writing product and supply data 
 * from backup files, ensuring data persistence for products (such as drinks and dishes) 
 * and supplies (such as ingredients).
 */
class BackupModule {
private:
  const std::string PRODUCTS_BACKUP_FILE = "C:\\Users\\aaron\\Dev\\Repositories" \
      "\\POS\\POS_Application\\backup\\products\\Products.txt";
  const std::string SUPPLIES_BACKUP_FILE = "C:\\Users\\aaron\\Dev\\Repositories" \
      "\\POS\\POS_Application\\backup\\inventory\\primeMaterial.txt";
public:
  /**
   * @brief Returns the singleton instance of the BackupModule.
   * 
   * Ensures only one instance of the BackupModule class is used across the application.
   * 
   * @return A reference to the static singleton instance of the BackupModule.
   */
  static BackupModule& getInstance();
  
  /**
   * @brief Reads and returns the backup data for products.
   * 
   * Reads product data from the backup file and returns a map categorized by product type.
   * 
   * @return A map where the key is a product category, and the value is a vector of Product objects.
   */
  std::map<std::string, std::vector<Product>> getProductsBackup();
  
  /**
   * @brief Reads and returns the backup data for supplies.
   * 
   * Reads supply data from the backup file and returns a vector of Supply objects.
   * 
   * @return A vector of Supply objects representing the supplies backup.
   */
  std::vector<Supply> getSuppliesBackup();
  
  /**
   * @brief Updates the backup data for products.
   * 
   * Writes the updated product data back to the backup file.
   * 
   * @param products A map of product categories and their associated Product objects.
   */
  void updateProductsBackup(
      const std::map<std::string, std::vector<Product>>& products);
  
  /**
   * @brief Updates the backup data for supplies.
   * 
   * Writes the updated supply data back to the backup file.
   * 
   * @param supplies A vector of SupplyItem objects representing the updated supplies.
   */
  void updateSuppliesBackup(const std::vector<Supply>& supplies);
private:
  BackupModule(); ///< Private constructor to enforce the singleton pattern.
  
  /**
   * @brief Reads product data from a backup file and stores it in a map.
   * 
   * Parses the product backup file, extracting product categories, names, ingredients, and prices.
   * 
   * @param filename The path to the backup file.
   * @param registeredProducts A map to store the parsed product data.
   * @throws std::runtime_error If the file cannot be opened or read.
   */
  void readProductsBackup(const std::string& filename, std::map<std::string
      , std::vector<Product>>& registeredProducts);
  
  /**
   * @brief Reads supply data from a backup file and stores it in a vector.
   * 
   * Parses the supplies backup file and extracts supply details.
   * 
   * @param supplies A vector to store the parsed supply data.
   * @throws std::runtime_error If the file cannot be opened or read.
   */
  void readSupplyItemsBackup(std::vector<Supply>& supplies);
  
  /**
   * @brief Writes product data to a backup file.
   * 
   * Saves the product data back to the backup file in the same format.
   * 
   * @param filename The path to the backup file.
   * @param registeredProducts A map of product categories and associated products.
   */
  void writeProductsBackup(const std::string& filename
      , const std::map<std::string, std::vector<Product>>& registeredProducts);
  
  /**
   * @brief Writes supply data to a backup file.
   * 
   * Saves the supply data back to the backup file.
   * 
   * @param supplies A vector of SupplyItem objects to be written to the backup file.
   */
  void writeSuppliesBackup(const std::vector<Supply>& supplies);
  
  // Copy and assignment constructors are disabled.
  BackupModule(const BackupModule&) = delete;
  BackupModule& operator=(const BackupModule&) = delete;
};

#endif // BACKUPMODULE_H
