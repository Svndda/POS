#include <vector>

#include "posmodel.h"
#include "backupmodule.h"

#define DRINKS_BACKUP_FILE "C:\\Users\\aaron\\Dev\\Repositories" \
"\\POS\\POS_Application\\backup\\products\\Bebidas.txt"

#define DISHES_BACKUP_FILE "C:\\Users\\aaron\\Dev\\Repositories" \
"\\POS\\POS_Application\\backup\\products\\Comidas.txt"

#define PRIME_BACKUP_FILE ""

POS_Model::POS_Model(BackupModule& module)
    : backupModule(module) {
}

POS_Model& POS_Model::getInstance() {
  // Loads the data contained in the backup program files.
  std::vector<std::string> backUpFiles = {
      DRINKS_BACKUP_FILE,
      DISHES_BACKUP_FILE,
      ""};
  static POS_Model instance(BackupModule::getInstance(backUpFiles));
  return instance;
}

void POS_Model::start() {
  // Reads and store the backup to the program memory to use them in the
  // program execution.
  this->Drinks = this->backupModule.readDrinksBackup();
  this->Dishes = this->backupModule.readDishesBackup();
  
  // Changes the started value to true.
  this->started = true;
}

void POS_Model::shutdown() {
  // Writes out the registers of the dishes and drinks back to the backup files.
  this->backupModule.writeDishesBackUp(this->Dishes);
  this->backupModule.writeDrinksBackUp(this->Drinks);
  // Clears the vector memory.
  this->Dishes.clear();
  this->Drinks.clear();
  this->started = false;
}
