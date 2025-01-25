// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef SUPPLY_H
#define SUPPLY_H

#include <cstdint>
#include <string>
#include <ostream>

/**
 * @class Supply
 * @brief Represents an item in the inventory used for products.
 * 
 * The `Supply` class encapsulates the attributes of an inventory item, 
 * including its name and quantity. It provides getter and setter methods 
 * for these attributes, and an overloaded equality operator for comparison.
 */
class Supply {

// Class Attributes.
private:
  std::string name = ""; ///< Name of the supply item.
  uint64_t quantity = 0; ///< Quantity of the supply item in inventory.
  std::string measureUnit = "";

// Class Constructor.
public:
  /**
   * @brief Constructs a new Supply object.
   * 
   * Initializes the supply item with a given name and quantity. Default values 
   * are provided if not specified.
   * 
   * @param myName The name of the supply item (default is an empty string).
   * @param myQuantity The quantity of the supply item (default is 0).
   */
  Supply(const std::string& myName = ""
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
  bool operator==(const Supply& other) const {
    return this->name == other.name
        && this->quantity == other.quantity
        && this->measureUnit == other.measureUnit;
  }
  
  /**
   * @brief Overloads the unequally operator to compare two supply items.
   * 
   * Compares the supply items based on their name and quantity.
   * 
   * @param other The other supply item to compare.
   * @return True if the supply items are unequal, false otherwise.
   */
  bool operator!=(const Supply& other) const {
    return !(this == &other);
  }
  
  /**
   * @brief Overloads the assignment operator to copy the values of another supply.
   * 
   * This operator ensures that all attributes of the supply are copied.
   * It prevents self-assignment.
   * 
   * @param other The other supply to be copied.
   * @return A reference to the current supply object.
   */
  Supply& operator=(const Supply& other) {
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
  
  /**
   * @brief Overloads the addition operator to add the values of another supply.
   * 
   * This operator ensures that the quantity attribute of the supply are added to the current.
   * It prevents self-assignment.
   * 
   * @param other The other supply to be added.
   * @return A reference to the current supply object.
   */
  Supply& operator+(const Supply& other) {
    if (this == &other) {
      // Avoid self-assignment      
      return *this;
    }
    
    // if (this->measureUnit == other.measureUnit) {
    // Adds up the the supplies quantity.
    this->quantity = this->quantity + other.quantity;
    // }
    
    // Returns the actual object.
    return *this;
  }
  
  /**
   * @brief Overloads the substraction operator to substract the values of another supply.
   * 
   * This operator ensures that the quantity attribute of the supply are substracted to the current.
   * It prevents self-assignment.
   * 
   * @param other The other supply to be substracted.
   * @return A reference to the current supply object.
   */
  Supply& operator-(const Supply& other) {
    if (this == &other) {
      // Avoid self-assignment      
      return *this;
    }
    
    // if (this->measureUnit == other.measureUnit) {
    // Adds up the the supplies quantity.
    this->quantity = this->quantity - other.quantity;
    // }
    
    // Returns the actual object.
    return *this;
  }
  
  friend std::ostream& operator<<(std::ostream& os, const Supply& supply);

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

#endif // SUPPLY_H
