#include <vector>

#include "backupcontroller.h"
#include "filehandler.h"

BackUpController::BackUpController() {}

void BackUpController::start() {
  // Loads the data contained in the backup program files.
  std::vector<std::string> backUpFiles = {"C:\\Users\\aaron\\Dev\\Repositories"
      "\\POS\\POS_Application\\backup\\products\\Bebidas.txt", "C:\\Users\\aaron"
      "\\Dev\\Repositories\\POS\\POS_Application\\backup\\products\\Comidas.txt"
      , ""};
  // Obtains an instance of the program Memory File Manager.
  FileHandler& fileHandler = FileHandler::getInstance(backUpFiles);
  // Reads and store the backup to the program memory to use them in the
  // program execution.
  this->Drinks = fileHandler.readProductsBackup(backUpFiles[0]);
  this->Dishes = fileHandler.readProductsBackup(backUpFiles[1]);
}
