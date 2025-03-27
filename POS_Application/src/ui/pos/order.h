#ifndef ORDER_H
#define ORDER_H

#include <QWidget>

#include "product.h"

namespace Ui {
class Order;
}

/**
 * @class Order
 * @brief Widget that represents a order in the POS system.
 *
 * The Order class manages the display and update of a order,
 * including adding products, calculating the total price.
 */
class Order : public QWidget {
  Q_OBJECT
  
private:
  Ui::Order* ui = nullptr;   ///< Pointer to the UI elements for the order.
  QString paymentMethod = QString();
  double receivedMoney = 0;
  double totalPrice = 0;       ///< Accumulated total price of the order.
  
public:
  /**
   * @brief Constructs a Order widget.
   *
   * Initializes the order UI, sets the order ID and prepares the display.
   *
   * @param parent Pointer to the parent widget.
   * @param myID Unique identifier for the order (default is 0).
   */
  explicit Order(QWidget *parent = nullptr);
  
  /**
   * @brief Destroys the Order widget.
   */
  ~Order();
public:
  /**
   * @brief Adds a product to the order.
   *
   * Creates a OrderElement for the given product, adds it to the order UI,
   * updates the total price, and connects signals to update the price when the
   * quantity changes.
   *
   * @param product The Product to add.
   */
  void addProduct(const Product& product);
    
  const std::vector<std::pair<Product, size_t>> getOrderProducts() const;
  
  void setPaymentMethod(const QString orderPaymentMethod) {
    this->paymentMethod = orderPaymentMethod;
  }
  
  void setReceivedMoney(const double orderReceivedMoney) {
    this->receivedMoney = orderReceivedMoney;
  }
  
  const double getOrderPrice() const {return this->totalPrice;}
  
  const QString getPaymentMethod() const {return this->paymentMethod;} 
  
  const double getReceivedMoney() const {return this->receivedMoney;}
protected:
  /**
   * @brief Sets up the order display.
   *
   * Creates and configures the layout that will contain the order elements.
   */
  void setupOrderDisplay();
  
private:
  /**
   * @brief Increases the order total price.
   *
   * Adds the price of the given product to the total price and updates the UI.
   *
   * @param product The Product whose price is to be added.
   */
  void increaseOrderPrice(const Product &product);
  
  /**
   * @brief Reduces the order total price.
   *
   * Subtracts the price of the given product from the total price and updates the UI.
   *
   * @param product The Product whose price is to be subtracted.
   */
  void reduceOrderPrice(const Product &product);
};

#endif // ORDER_H
