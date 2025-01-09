#ifndef BACKUPMODULE_H
#define BACKUPMODULE_H

#include <map>
#include <string>
#include "product.h"

class BackupModule {
private:
  // Vector coantaining the filename of each file where the data is stored.
  std::vector<std::string> filenames;
public:
  // Method that returns a static instance of the class.
  static BackupModule& getInstance(const std::vector<std::string>&
                                   backupsVector);
  std::map<std::string, std::vector<Product>> readDrinksBackup();
  std::map<std::string, std::vector<Product>> readDishesBackup();
  void writeDrinksBackUp(const std::map<std::string, std::vector<Product>>&
      registeredDrinks);
  void writeDishesBackUp(const std::map<std::string, std::vector<Product>>&
      registeredDishes);
private:
  // Private constructor.
  BackupModule(const std::vector<std::string>& backupsVector);
  std::map<std::string, std::vector<Product>> readProductsBackup(
      const std::string& filename);
  void writeProductsBackup(
      const std::string& filename,
      const std::map<std::string, std::vector<Product>>& registeredProducts);
  // Copy and assignation constructors disabled.
  BackupModule(const BackupModule&) = delete;
  BackupModule& operator=(const BackupModule&) = delete;
};

#endif // BACKUPMODULE_H
