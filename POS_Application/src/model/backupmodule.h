// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef BACKUPMODULE_H
#define BACKUPMODULE_H

#include <map>
#include <string>

#include "product.h"
#include "user.h"

/**
 * @class BackupModule
 * @brief Manages backup and restoration of product, supply, and user data.
 *
 * The BackupModule class facilitates reading and writing product, supply, and user data
 * from backup files, ensuring persistent storage of data for the application.
 *
 * This class implements the singleton pattern, ensuring that only one instance is used
 * throughout the application.
 */
class BackupModule {
private:
  const std::string PRODUCTS_BACKUP_FILE = "C:\\Users\\aaron\\Dev\\Repositories" \
      "\\POS\\POS_Application\\backup\\products\\Products.txt";
  const std::string SUPPLIES_BACKUP_FILE = "C:\\Users\\aaron\\Dev\\Repositories" \
      "\\POS\\POS_Application\\backup\\inventory\\primeMaterial.txt";
const std::string USERS_BACKUP_FILE = "C:\\Users\\aaron\\Dev\\Repositories" \
      "\\POS\\POS_Application\\backup\\users\\users_data.bin";
public:
  /**
   * @brief Gets the singleton instance of the BackupModule.
   *
   * Ensures that only one instance of BackupModule exists.
   *
   * @return Reference to the static BackupModule instance.
   */
  static BackupModule& getInstance();
  
  /**
   * @brief Retrieves the products backup.
   *
   * Reads product data from the backup file and returns a map
   * where each key is a product category and the corresponding value is
   * a vector of Product objects.
   *
   * @return Map of product categories to Product vectors.
   *
   * @throws std::runtime_error If the backup file cannot be opened.
   */
  std::map<std::string, std::vector<Product>> getProductsBackup();
  
  /**
   * @brief Retrieves the supplies backup.
   *
   * Reads supply data from the backup file and returns a vector of Supply objects.
   *
   * @return Vector of Supply objects.
   *
   * @throws std::runtime_error If the backup file cannot be opened.
   */
  std::vector<Supply> getSuppliesBackup();
  
  /**
   * @brief Retrieves the users backup.
   *
   * Reads user data from the backup file (in binary format) and returns a vector of User objects.
   *
   * @return Vector of User objects.
   *
   * @throws std::runtime_error If the backup file cannot be opened.
   */
  std::vector<User> getUsersBackup();
  
  /**
   * @brief Updates the products backup.
   *
   * Writes the provided product data to the products backup file.
   *
   * @param products Map of product categories to vectors of Product objects.
   *
   * @throws std::runtime_error If the backup file cannot be opened for writing.
   */
  void updateProductsBackup(const std::map<std::string, std::vector<Product>>& products);
  
  /**
   * @brief Updates the supplies backup.
   *
   * Writes the provided supply data to the supplies backup file.
   *
   * @param supplies Vector of Supply objects to be saved.
   *
   * @throws std::runtime_error If the backup file cannot be opened for writing.
   */
  void updateSuppliesBackup(const std::vector<Supply>& supplies);
  
  /**
   * @brief Updates the users backup.
   *
   * Writes the provided user data to the users backup file in binary format.
   *
   * @param users Vector of User objects to be saved.
   *
   * @throws std::runtime_error If the backup file cannot be opened for writing.
   */
  void updateUsersBackup(const std::vector<User>& users);
private:
  /**
   * @brief Private constructor to enforce the singleton pattern.
   */
  BackupModule();
  
  /**
   * @brief Reads product data from the backup file.
   *
   * Parses the given backup file to extract product categories, names, ingredients, prices, and image paths.
   *
   * @param filename Path to the products backup file.
   * @param registeredProducts Map to store the parsed product data.
   *
   * @throws std::runtime_error If the file cannot be opened or read.
   */
  void readProductsBackup(const std::string& filename
      , std::map<std::string, std::vector<Product>>& registeredProducts);
  
  /**
   * @brief Reads supply data from the backup file.
   *
   * Parses the supplies backup file and extracts supply details.
   *
   * @param supplies Vector to store the parsed Supply objects.
   *
   * @throws std::runtime_error If the file cannot be opened or read.
   */
  void readSupplyItemsBackup(std::vector<Supply>& supplies);
  
  /**
   * @brief Reads user data from the backup file.
   *
   * Parses the users backup file (binary format) and extracts user details.
   *
   * @param registeredUsers Vector to store the parsed User objects.
   *
   * @throws std::runtime_error If the file cannot be opened or read.
   */
  void readUsersBackup(std::vector<User>& registeredUsers);
  
  /**
   * @brief Writes product data to the backup file.
   *
   * Saves the provided product data in the appropriate format to the backup file.
   *
   * @param filename Path to the backup file.
   * @param registeredProducts Map of product categories to vectors of Product objects.
   *
   * @throws std::runtime_error If the file cannot be opened for writing.
   */
  void writeProductsBackup(const std::string& filename, const std::map<std::string, std::vector<Product>>& registeredProducts);
  
  /**
   * @brief Writes supply data to the backup file.
   *
   * Saves the provided supply data to the supplies backup file.
   *
   * @param supplies Vector of Supply objects to be written.
   *
   * @throws std::runtime_error If the file cannot be opened for writing.
   */
  void writeSuppliesBackup(const std::vector<Supply>& supplies);
  
  /**
   * @brief Writes user data to the backup file in binary format.
   *
   * Saves the provided user data to the users backup file.
   *
   * @param users Vector of User objects to be written.
   *
   * @throws std::runtime_error If the file cannot be opened for writing.
   */
  void writeUsersBackup(const std::vector<User>& users);
  
  // Copy and assignment constructors are disabled.
  BackupModule(const BackupModule&) = delete;
  BackupModule& operator=(const BackupModule&) = delete;
};

#endif // BACKUPMODULE_H
