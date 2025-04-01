// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "supply.h"
#include <istream>

/**
 * @brief Overloads the output stream operator to serialize a Supply object.
 * 
 * @param os The output stream.
 * @param supply The Supply object to serialize.
 * @return The modified output stream.
 */
std::ostream& operator<<(std::ostream& os, const Supply& supply) {
  os << supply.getName()
     << " " << supply.getQuantity()
     << " " << supply.getMeasure();
  return os;
}

/**
 * @brief Overloads the input stream operator to deserialize a Supply object.
 * 
 * @param is The input stream.
 * @param supply The Supply object to deserialize.
 * @return The modified input stream.
 */
std::istream& operator>>(std::istream& is, Supply& supply) {
  std::string name, measure;
  uint64_t quantity = 0;
  
  is >> std::ws; // Clean leading spaces
  std::getline(is, name, ' ');
  is >> quantity;
  is >> std::ws;
  std::getline(is, measure);
  
  if (is) {
    supply.setName(name);
    supply.setQuantity(quantity);
    supply.setMeasure(measure);
  }
  return is;
}
