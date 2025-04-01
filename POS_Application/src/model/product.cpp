// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "product.h"

/**
 * @brief Overloads the output stream operator to serialize a Product object.
 * 
 * @param os The output stream.
 * @param product The Product object to serialize.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const Product& product) {
  os << product.getID() << " " << product.getName() << " "
     << product.getPrice() << " " << product.getIngredients().size() << " ";
  for (const auto& ingredient : product.getIngredients()) {
    os << ingredient << " ";
  }
  return os;
}
