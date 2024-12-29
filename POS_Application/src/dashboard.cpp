#include <iostream>

#include "dashboard.h"
#include "ui_dashboard.h"
#include "dish.h"

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
  Dish food(dishName, ingredients, 3500);
  std::cout << "plato creado: " << food << std::endl;
}

Dashboard::~Dashboard()
{
  delete ui;
}
