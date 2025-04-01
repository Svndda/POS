#ifndef RECEIPT_H
#define RECEIPT_H

#include <QWidget>
#include <QString>

#include "product.h"
#include "order.h"

/**
 * @class Receipt
 * @brief Represents a sales receipt with all relevant transaction information
 * 
 * This class encapsulates all data needed for a sales receipt including
 * business information, sold products, payment method, and totals.
 */
class Receipt {
private:
  QString businessName;  ///< Name of the business
  size_t ID;            ///< Unique receipt identifier
  QString dateTime;     ///< Transaction date and time
  QString user;         ///< User who processed the sale
  std::vector<std::pair<Product, size_t>> products;  ///< Sold products with quantities
  QString paymentMethod;  ///< Payment method used
  double receivedAmount;  ///< Amount received from customer
  double price;          ///< Total price of the transaction
  
public:
  /**
     * @brief Primary constructor
     * @param myBusinessName Name of the business
     * @param myID Unique receipt identifier
     * @param myDateTime Transaction date and time
     * @param myUser User who processed the sale
     * @param myProducts List of sold products (product + quantity pairs)
     * @param myPaymentMethod Payment method used
     * @param myReceivedAmount Amount received from customer
     * @param myPrice Total price of the transaction
     */
  Receipt(const QString myBusinessName = ""
          , const size_t myID = 0
          , const QString myDateTime = ""
          , const QString myUser = ""
          , const std::vector<std::pair<Product, size_t>> myProducts
          = std::vector<std::pair<Product, size_t>>()
          , const QString myPaymentMethod = ""
          , const double myReceivedAmount = 0,
          const double myPrice = 0);
  
  /**
   * @brief Copy constructor
   * @param other Receipt to copy from
   */
  Receipt(const Receipt& other);
  
  /**
   * @brief Constructor from Order object
   * @param businessName Name of the business
   * @param id Unique receipt identifier
   * @param username User who processed the sale
   * @param order Order object to create receipt from
   */
  Receipt(const QString businessName, const size_t id,
          const QString username, const Order& order);
  
  /**
   * @brief Destructor
   */
  ~Receipt();
  
  // Getters
public:
  /**
   * @brief Retrieves the bussiness name on receipt.
   * @return Copy of the bussiness name on receipt.
   */
  QString getBusinessName() const {
    return this->businessName;
  };
  
  /**
   * @brief Retrieves the receipt's id.
   * @return Copy of the receipt's id.
   */
  size_t getID() const {
    return this->ID;
  };
  
  /**
   * @brief Retrieves the date time of the receipt.
   * @return Copy of the date time on format "yyyy-MM-dd HH:mm:ss"
   */
  QString getDateTime() const {
    return this->dateTime;
  };
  
  /**
   * @brief Retrieves the name of the user that closed the receipt.
   * @return Copy of the user's name on receipt.
   */
  QString getUser() const {
    return this->user;
  };
  
  /**
   * @brief Retrieves the product list linked to this receipt.
   * @return Copy of the receipt's product list.
   */
  std::vector<std::pair<Product, size_t>> getProducts() const {
    return this->products;
  };
  
  /**
   * @brief Retrieves the payment method used in the receipts.
   * @return Receipt's payment method.
   */
  QString getPaymentMethod() const {
    return this->paymentMethod;
  };
  
  /**
   * @brief Retrieves the received amount of money for the receipt.
   * @return Receipt's received money amount.
   */
  double getReceivedAmount() const {
    return this->receivedAmount;
  };
  
  /**
   * @brief Retrieves the receipt's total price.
   * @return Receipt's total price.
   */
  double getPrice() const {
    return this->price;
  };
  
public:
  // Serialization operators
  friend std::ofstream& operator<<(std::ofstream& out, const Receipt& receipt);
  friend std::ifstream& operator>>(std::ifstream& in, Receipt& receipt);
  friend QDataStream& operator<<(QDataStream& out, const Receipt& receipt);
  friend QDataStream& operator>>(QDataStream& in, Receipt& receipt);
  friend QDebug operator<<(QDebug dbg, const Receipt& receipt);
};

#endif // RECEIPT_H
