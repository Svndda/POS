// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef PRODUCT_H
#define PRODUCT_H

#include <QPixmap>
#include <vector>
#include <iostream>

#include "supply.h"

/**
 * @class Product
 * @brief Represents a product, which can be a food item or a drink.
 * 
 * The `Product` class encapsulates the attributes of a product, including 
 * its unique ID, name, list of ingredients, and price. It provides getter and 
 * setter methods for these attributes, as well as overloaded operators for 
 * comparison and assignment.
 */
class Product {
  // Class Attributes.
private:
  uint64_t id = 0; ///< Unique identifier for the product.
  std::string name = ""; ///< Name of the product.
  std::vector<Supply> ingredients; ///< List of ingredients for the product.
  double price = 0; ///< Price of the product.
  QPixmap image;

  // Class constructor.
public:
  /**
   * @brief Constructs a new Product object.
   * 
   * Initializes the product with a given ID, name, list of ingredients, and price. 
   * Default values are provided for each attribute if not specified.
   * 
   * @param myID The unique identifier for the product (default is 0).
   * @param myName The name of the product (default is an empty string).
   * @param myIngredients The list of ingredients for the product (default is an empty vector).
   * @param myPrice The price of the product (default is 0).
   */
  Product(uint64_t myID = 0
      , const std::string &myName = ""
      , const std::vector<Supply> myIngredients
          = std::vector<Supply>()
      , double myPrice = 0
      , const QPixmap myImage = QPixmap())
      : id(myID)
      , name(myName)
      , ingredients(myIngredients)
      , price(myPrice)
      , image(myImage)  {
  }

  // Class Getters.
public:
  /**
   * @brief Gets the unique identifier of the product.
   * 
   * @return The product's ID.
   */
  inline const uint64_t getID() const {return this->id;}
  
  /**
   * @brief Gets the name of the product.
   * 
   * @return The product's name.
   */
  inline const std::string getName() const {return this->name;}
  
  /**
   * @brief Gets the list of ingredients for the product.
   * 
   * @return A constant reference to the vector of ingredients.
   */
  inline const std::vector<Supply>& getIngredients() const {
    return this->ingredients;
  }
  
  /**
   * @brief Gets the price of the product.
   * 
   * @return The product's price.
   */
  inline const double getPrice() const {return this->price;}
  
  
  inline const QPixmap& getImage() const {return this->image;}
  
  // Class Setters.
public:
  /**
   * @brief Sets the name of the product.
   * 
   * @param newName The new name of the product.
   */
  inline void setName(const std::string &newName) {
    this->name = newName;
  }
  
  /**
   * @brief Sets the ingredients for the product.
   * 
   * @param newIngredients The new list of ingredients.
   */
  inline void setIngredients(const std::vector<Supply>& newIngredients) {
    this->ingredients = newIngredients;
  }
  
  /**
   * @brief Sets the price of the product.
   * 
   * @param newPrice The new price of the product.
   */
  inline void setPrice(const double newPrice) {this->price = newPrice;}
  
// Class Operators.
public:
  /**
   * @brief Overloads the output stream operator to display the product details.
   * 
   * @param os The output stream object.
   * @param product The product object to be output.
   * @return The output stream with the product details.
   */
  friend std::ostream& operator<<(std::ostream& os, const Product& product);
  
  /**
   * @brief Overloads the assignment operator to copy the values of another product.
   * 
   * This operator ensures that all attributes of the product are copied.
   * It prevents self-assignment.
   * 
   * @param other The other product to be copied.
   * @return A reference to the current product object.
   */
  Product &operator=(const Product &other) {
    if (this == &other) {
      // Avoid self-assignment
      return *this;
    }
    
    // Copy the values of the attributes
    this->id = other.id;
    this->name = other.name;
    this->ingredients = other.ingredients;
    this->price = other.price;
    this->image = other.image;
    // Return the reference to the current object
    return *this;
  }
  
  /**
   * @brief Overloads the equality operator to compare two products.
   * 
   * Compares the products based on their ID, name, price, and ingredients.
   * 
   * @param other The other product to compare.
   * @return True if the products are equal, false otherwise.
   */
  bool operator==(const Product &other) const {
    return this->id == other.id &&
          this->name == other.name &&
          this->price == other.price &&
          std::equal(this->ingredients.begin(),
                    this->ingredients.end(),
                    other.ingredients.begin(),
                    other.ingredients.end());
  }
};

#endif // PRODUCT_H
