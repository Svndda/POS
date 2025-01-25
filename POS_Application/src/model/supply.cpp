// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "supply.h"

// << Operator definition.
std::ostream& operator<<(std::ostream& os, const Supply& supply) {
  os << supply.getName() << " " << supply.getQuantity() << " "
     << supply.getMeasure();
  return os;
}
