#ifndef DISH_H
#define DISH_H

#include <vector>
#include <iostream>

#include "supplyitem.h"

class Product {
  // Class Attributes.
private:
  std::string name;
  std::vector<SupplyItem> ingredients;
  double price;

  // Class constructor.
public:
  Product(const std::string& myName,
      const std::vector<SupplyItem> myIngredients,
      double myPrice);

  // Class Getters.
public:
  const std::string getName() const;
  const std::vector<SupplyItem>& getIngredients() const;
  const double getPrice() const;
  
  // Class Setters.
public:
  void setName(const std::string& newName);
  void setIngredients(const std::vector<SupplyItem>& newIngredients);
  void setPrice(const double newPrice);
  
// Class Operators.
public:
  friend std::ostream& operator<<(std::ostream& os, const Product& product);

};

inline Product::Product(const std::string &myName,
    const std::vector<SupplyItem> myIngredients, double myPrice)
    : name(myName)
    , ingredients(myIngredients)
    , price(myPrice) {
}

inline const std::string Product::getName() const {return this->name;}

inline const std::vector<SupplyItem> &Product::getIngredients() const {
  return this->ingredients;
}

inline const double Product::getPrice() const {return this->price;}

inline void Product::setName(const std::string &newName) {
  this->name = newName;
}

inline void Product::setIngredients(const std::vector<SupplyItem>
    &newIngredients) {
  this->ingredients = newIngredients;
}

inline void Product::setPrice(const double newPrice) {this->price = newPrice;}

#endif // DISH_H
