#include <vector>

#include "appmodel.h"
#include "backupmodule.h"

#define DRINKS_BACKUP_FILE "C:\\Users\\aaron\\Dev\\Repositories" \
"\\POS\\POS_Application\\backup\\products\\Bebidas.txt"

#define DISHES_BACKUP_FILE "C:\\Users\\aaron\\Dev\\Repositories" \
"\\POS\\POS_Application\\backup\\products\\Comidas.txt"

#define PRIME_BACKUP_FILE ""

AppModel::AppModel(BackupModule& module)
    : backupModule(module) {
}

AppModel& AppModel::getInstance() {
  // Loads the data contained in the backup program files.
  std::vector<std::string> backUpFiles = {
      DRINKS_BACKUP_FILE,
      DISHES_BACKUP_FILE,
      ""};
  static AppModel instance(BackupModule::getInstance(backUpFiles));
  return instance;
}

void AppModel::start() {
  // Reads and store the backup to the program memory to use them in the
  // program execution.
  this->Drinks = this->backupModule.readDrinksBackup();
  this->Dishes = this->backupModule.readDishesBackup();
  
  // Changes the started value to true.
  this->started = true;
}

void AppModel::shutdown() {
  // Writes out the registers of the dishes and drinks back to the backup files.
  this->backupModule.writeDishesBackUp(this->Dishes);
  this->backupModule.writeDrinksBackUp(this->Drinks);
  // Clears the vector memory.
  this->Dishes.clear();
  this->Drinks.clear();
  this->started = false;
}
