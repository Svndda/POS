#include "dish.h"

// Definición del operador << fuera de la clase.
std::ostream& operator<<(std::ostream& os, const Dish& dish) {
  // Imprimir nombre del plato
  os << "Dish Name: " << dish.getName() << std::endl;
  
  // Imprimir ingredientes del plato
  os << "Ingredients:" << std::endl;
  for (const auto& ingredient : dish.getIngredients()) {
    os << "  " << ingredient.getName() << std::endl;
  }
  
  // Imprimir precio del plato
  os << "Price: $" << dish.getPrice() << std::endl;
  
  return os;
}
