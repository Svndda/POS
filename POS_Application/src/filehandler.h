#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <map>
#include <string>
#include "product.h"

class FileHandler {
private:
  // Vector coantaining the filename of each file where the data is stored.
  std::vector<std::string> filenames;
public:
  // Method that returns a static instance of the class.
  static FileHandler& getInstance(const std::vector<std::string>&
                                   backupsVector);
  std::map<std::string, std::vector<Product>> readDrinksBackup();
  std::map<std::string, std::vector<Product>> readDishesBackup();
  void writeDrinksBackUp(const std::map<std::string, std::vector<Product>>&
      registeredDrinks);
  void writeDishesBackUp(const std::map<std::string, std::vector<Product>>&
      registeredDishes);
private:
  // Private constructor.
  FileHandler(const std::vector<std::string>& backupsVector);
  std::map<std::string, std::vector<Product>> readProductsBackup(
      const std::string& filename);
  void writeProductsBackup(
      const std::string& filename,
      const std::map<std::string, std::vector<Product>>& registeredProducts);
  // Copy and assignation constructors disabled.
  FileHandler(const FileHandler&) = delete;
  FileHandler& operator=(const FileHandler&) = delete;
};

#endif // FILEHANDLER_H
