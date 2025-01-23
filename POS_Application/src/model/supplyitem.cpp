// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "supplyitem.h"

// << Operator definition.
std::ostream& operator<<(std::ostream& os, const SupplyItem& supply) {
  os << supply.getName() << " " << supply.getQuantity() << " "
     << supply.getMeasure();
  return os;
}
