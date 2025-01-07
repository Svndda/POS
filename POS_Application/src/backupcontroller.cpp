#include <vector>

#include "backupcontroller.h"
#include "filehandler.h"

#define DRINKS_BACKUP_FILE "C:\\Users\\aaron\\Dev\\Repositories" \
"\\POS\\POS_Application\\backup\\products\\Bebidas.txt"

#define DISHES_BACKUP_FILE "C:\\Users\\aaron\\Dev\\Repositories" \
"\\POS\\POS_Application\\backup\\products\\Comidas.txt"

#define PRIME_BACKUP_FILE ""

BackUpController::BackUpController(FileHandler& myFileHandler)
    : fileHandler(myFileHandler) {
}

BackUpController& BackUpController::getInstance() {
  // Loads the data contained in the backup program files.
  std::vector<std::string> backUpFiles = {
      DRINKS_BACKUP_FILE,
      DISHES_BACKUP_FILE,
      ""};
  static BackUpController instance(FileHandler::getInstance(backUpFiles));
  return instance;
}

void BackUpController::start() {
  // Reads and store the backup to the program memory to use them in the
  // program execution.
  this->Drinks = this->fileHandler.readDrinksBackup();
  this->Dishes = this->fileHandler.readDishesBackup();
  this->fileHandler.writeDishesBackUp(this->Dishes);
  this->fileHandler.writeDrinksBackUp(this->Drinks);
}
