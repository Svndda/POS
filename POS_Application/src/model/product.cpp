// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "product.h"

// << Operator definition.
std::ostream& operator<<(std::ostream& os, const Product& product) {
  // Print the product name.
  os << "Product Name: " << product.getName() << std::endl;
  
  // Prints the product ingredients.
  os << "Ingredients:" << std::endl;
  for (const auto& ingredient : product.getIngredients()) {
    os << "  " << ingredient.getName() << std::endl;
  }
  
  // Prints the product's price.
  os << "Price: $" << product.getPrice() << std::endl;
  
  return os;
}
