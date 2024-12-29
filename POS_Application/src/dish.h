#ifndef DISH_H
#define DISH_H

#include <vector>
#include <iostream>

#include "supplyitem.h"

class Dish {
  // Class Attributes.
private:
  std::string name;
  std::vector<SupplyItem> ingredients;
  double price;

  // Class constructor.
public:
  Dish(const std::string& myName, const std::vector<SupplyItem> myIngredients,
      double myPrice)
      : name(myName)
      , ingredients(myIngredients)
      , price(myPrice) {
  };

  // Class Getters.
public:
  inline const std::string getName() const {return this->name;}
  inline const std::vector<SupplyItem>& getIngredients() const {
    return this->ingredients;
  }
  inline const double getPrice() const {return this->price;}
  
  // Class Setters.
public:
  inline void setName(const std::string& newName) {
    this->name = newName;
  }
  inline void setIngredients(const std::vector<SupplyItem>& newIngredients) {
    this->ingredients = newIngredients;
  }
  inline void setPrice(const double newPrice) {this->price = newPrice;}
  
// Class Operators.
public:
  friend std::ostream& operator<<(std::ostream& os, const Dish& dish);

};

#endif // DISH_H
