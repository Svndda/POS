#include "dish.h"

// << Operator definition.
std::ostream& operator<<(std::ostream& os, const Dish& dish) {
  // Print the dish name.
  os << "Dish Name: " << dish.getName() << std::endl;
  
  // Prints the plate ingredients.
  os << "Ingredients:" << std::endl;
  for (const auto& ingredient : dish.getIngredients()) {
    os << "  " << ingredient.getName() << std::endl;
  }
  
  // Prints the plate's price.
  os << "Price: $" << dish.getPrice() << std::endl;
  
  return os;
}
