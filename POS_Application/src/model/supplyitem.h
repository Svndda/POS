// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef SUPPLYITEM_H
#define SUPPLYITEM_H

#include <cstdint>
#include <string>
#include <ostream>

/**
 * @class SupplyItem
 * @brief Represents an item in the inventory used for products.
 * 
 * The `SupplyItem` class encapsulates the attributes of an inventory item, 
 * including its name and quantity. It provides getter and setter methods 
 * for these attributes, and an overloaded equality operator for comparison.
 */
class SupplyItem {

// Class Attributes.
private:
  std::string name = ""; ///< Name of the supply item.
  uint64_t quantity = 0; ///< Quantity of the supply item in inventory.
  std::string measureUnit = "";

// Class Constructor.
public:
  /**
   * @brief Constructs a new SupplyItem object.
   * 
   * Initializes the supply item with a given name and quantity. Default values 
   * are provided if not specified.
   * 
   * @param myName The name of the supply item (default is an empty string).
   * @param myQuantity The quantity of the supply item (default is 0).
   */
  SupplyItem(const std::string& myName = ""
    , uint64_t myQuantity = 0
    , const std::string& myMeasure = "")
      : name(myName), quantity(myQuantity), measureUnit(myMeasure) {
  };
  
  /**
   * @brief Overloads the equality operator to compare two supply items.
   * 
   * Compares the supply items based on their name and quantity.
   * 
   * @param other The other supply item to compare.
   * @return True if the supply items are equal, false otherwise.
   */
  bool operator==(const SupplyItem& other) const {
    return this->name == other.name
        && this->quantity == other.quantity
        && this->measureUnit == other.measureUnit;
  }
  
  SupplyItem& operator=(const SupplyItem &other) {
    if (this == &other) {
      // Avoids autoassignation.
      return *this;
    }
    
    // Copy the other object attributes.
    this->name = other.name;
    this->quantity = other.quantity;
    this->measureUnit = other.measureUnit;
    
    // returns the actual object.
    return *this;
  }
  
  friend std::ostream& operator<<(std::ostream& os, const SupplyItem& supply);

// Class Getters.
public:
  /**
   * @brief Gets the name of the supply item.
   * 
   * @return A constant reference to the name of the supply item.
   */
  inline const std::string& getName() const {return this->name;};
  
  /**
   * @brief Gets the quantity of the supply item.
   * 
   * @return The quantity of the supply item.
   */
  inline const uint64_t getQuantity() const {return this->quantity;}
  
  /**
   * @brief Gets the measure of the supply item.
   * 
   * @return The measure of the supply item.
   */
  inline const std::string& getMeasure() const {return this->measureUnit;}
  
  inline const bool empty() const {
    return this->name.empty() && this->quantity == 0;
  }

// Class Setters.
public:
  /**
   * @brief Sets the name of the supply item.
   * 
   * @param newName The new name for the supply item.
   */
  inline void setName(const std::string& newName) {this->name = newName;}
  
  /**
   * @brief Sets the quantity of the supply item.
   * 
   * @param newQuantity The new quantity for the supply item.
   */
  inline void setQuantity(const uint64_t newQuantity) {
    this->quantity = newQuantity;
  }
};

#endif // SUPPLYITEM_H
