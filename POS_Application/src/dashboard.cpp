#include <iostream>

#include "dashboard.h"
#include "ui_dashboard.h"
#include "product.h"
#include "filehandler.h"

Dashboard::Dashboard(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DashboardPage) {
  ui->setupUi(this);
  std::cout << "creando un plato" << std::endl;
  std::vector<SupplyItem> ingredients;
  ingredients.push_back(SupplyItem(std::string("arroz"), 300));
  ingredients.push_back(SupplyItem(std::string("carne"), 300));
  ingredients.push_back(SupplyItem(std::string("ensalada"), 300));
  std::string dishName("Casado");
  Product food(dishName, ingredients, 3500);
  std::cout << "plato creado: " << food << std::endl;
  
  try {
    // Obtener instancia del Singleton
    std::vector<std::string> backupFiles;
    backupFiles.push_back("C:\\Users\\aaron\\Dev\\Repositories\\POS\\POS_Application\\backup\\products\\Bebidas.txt");
    FileHandler& fileHandler = FileHandler::getInstance(backupFiles);
    // Cargar los productos del archivo
    auto categorias = fileHandler.readProductsBackup(backupFiles[0]);
    // Mostrar los productos organizados
    std::cout << "Productos disponibles en el restaurante:" << std::endl;
    for (const auto& [categoria, productos] : categorias) {
      std::cout << categoria << ":" << std::endl;
      for (const auto& producto : productos) {
        std::cout << "  - " << producto.getName() << ": $" << producto.getPrice() << std::endl;
        std::cout << "    Ingredientes:" << std::endl;
        for (const auto& ingrediente : producto.getIngredients()) {
          std::cout << "      " << ingrediente.getName()
                    << " (Cantidad: " << ingrediente.getQuantity() << ")" << std::endl;
        }
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

Dashboard::~Dashboard()
{
  delete ui;
}
