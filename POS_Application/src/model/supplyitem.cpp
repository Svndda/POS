#include "supplyitem.h"

// << Operator definition.
std::ostream& operator<<(std::ostream& os, const SupplyItem& supply) {
  os << supply.getName() << " " << supply.getQuantity();
  return os;
}
