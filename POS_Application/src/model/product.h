#ifndef PRODUCT_H
#define PRODUCT_H

#include <vector>
#include <iostream>

#include "supplyitem.h"

class Product {
  // Class Attributes.
private:
  uint64_t id = 0;
  std::string name = "";
  std::vector<SupplyItem> ingredients;
  double price = 0;

  // Class constructor.
public:
  Product(uint64_t myID = 0
          , const std::string &myName = ""
          , const std::vector<SupplyItem> myIngredients
              = std::vector<SupplyItem>()
          , double myPrice = 0)
      : id(myID)
      , name(myName)
      , ingredients(myIngredients)
      , price(myPrice) {
  }

  // Class Getters.
public:
  inline const uint64_t getID() const {return this->id;}
  
  inline const std::string getName() const {return this->name;}
  
  inline const std::vector<SupplyItem>& getIngredients() const {
    return this->ingredients;
  }
  
  inline const double getPrice() const {return this->price;}
  
  // Class Setters.
public:
  inline void setName(const std::string &newName) {
    this->name = newName;
  }
  
  inline void setIngredients(const std::vector<SupplyItem>
                                 &newIngredients) {
    this->ingredients = newIngredients;
  }
  
  inline void setPrice(const double newPrice) {this->price = newPrice;}
  
// Class Operators.
public:
  friend std::ostream& operator<<(std::ostream& os, const Product& product);
  
  Product &operator=(const Product &other) {
    if (this == &other) {
      // Evitar autoasignación
      return *this;
    }
    
    // Copiar los valores de los atributos
    this->id = other.id;
    this->name = other.name;
    this->ingredients = other.ingredients;
    this->price = other.price;
    
    return *this; // Devolver la referencia al objeto actual
  }
  
  bool operator==(const Product &other) const {
    return this->id == other.id &&
           this->name == other.name &&
           this->ingredients == other.ingredients &&
           this->price == other.price;
  }
  
};

#endif // PRODUCT_H
