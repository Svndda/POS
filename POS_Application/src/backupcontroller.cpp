#include <vector>

#include "backupcontroller.h"
#include "filehandler.h"

BackUpController::BackUpController() {}

BackUpController& BackUpController::getInstance() {
  static BackUpController instance;
  return instance;
}

void BackUpController::start() {
  // Loads the data contained in the backup program files.
  std::vector<std::string> backUpFiles = {"C:\\Users\\aaron\\Dev\\Repositories"
      "\\POS\\POS_Application\\backup\\products\\Bebidas.txt",
      "C:\\Users\\aaron\\Dev\\Repositories\\POS\\POS_Application\\backup\\"
      "products\\Comidas.txt"
      , ""};
  // Obtains an instance of the program Memory File Manager.
  FileHandler& fileHandler = FileHandler::getInstance(backUpFiles);
  // Reads and store the backup to the program memory to use them in the
  // program execution.
  this->Drinks = fileHandler.readDrinksBackup();
  this->Dishes = fileHandler.readDishesBackup();
  fileHandler.writeDishesBackUp(this->Dishes);
  fileHandler.writeDrinksBackUp(this->Drinks);
}
