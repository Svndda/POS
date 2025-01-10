#ifndef SUPPLYITEM_H
#define SUPPLYITEM_H

#include <cstdint>
#include <string>

class SupplyItem {

// Class Attributes.
private:
  std::string name = "";
  uint64_t quantity = 0;

// Class Constructor.
public:
  SupplyItem(const std::string& myName = "", uint64_t myQuantity = 0)
    : name(myName),
      quantity(myQuantity) {
  };
  
  bool operator==(const SupplyItem& other) const {
    return this->name == other.name && this->quantity == other.quantity;
  }

// Class Getters.
public:
  inline const std::string& getName() const {return this->name;};
  inline const uint64_t getQuantity() const {return this->quantity;}

// Class Setters.
public:
  inline void setName(const std::string& newName) {this->name = newName;}
  inline void setQuantity(const uint64_t newQuantity) {
    this->quantity = newQuantity;
  }
};
#endif // SUPPLYITEM_H
