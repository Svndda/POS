#ifndef RECEIPT_H
#define RECEIPT_H

#include <QWidget>

#include "product.h"

namespace Ui {
class Receipt;
}

/**
 * @class Receipt
 * @brief Widget that represents a receipt in the POS system.
 *
 * The Receipt class manages the display and update of a receipt,
 * including adding products, calculating the total price, and generating
 * an HTML representation of the receipt for printing purposes.
 */
class Receipt : public QWidget {
  Q_OBJECT
  
private:
  Ui::Receipt* ui = nullptr;   ///< Pointer to the UI elements for the receipt.
  const size_t ID = 0;         ///< Unique identifier for the receipt.
  std::vector<Product> products;
  double totalPrice = 0;       ///< Accumulated total price of the receipt.
  
public:
  /**
   * @brief Constructs a Receipt widget.
   *
   * Initializes the receipt UI, sets the receipt ID and prepares the display.
   *
   * @param parent Pointer to the parent widget.
   * @param myID Unique identifier for the receipt (default is 0).
   */
  explicit Receipt(QWidget *parent = nullptr, const size_t myID = 0);
  
  /**
   * @brief Destroys the Receipt widget.
   */
  ~Receipt();
  
public:
  /**
   * @brief Adds a product to the receipt.
   *
   * Creates a ReceiptElement for the given product, adds it to the receipt UI,
   * updates the total price, and connects signals to update the price when the
   * quantity changes.
   *
   * @param product The Product to add.
   */
  void addProduct(const Product& product);
  
  /**
   * @brief Generates the HTML content of the receipt.
   *
   * Assembles the receipt header, the list of products (obtained from each
   * ReceiptElement in the layout), and the total price into an HTML-formatted string.
   *
   * @return A QString containing the HTML representation of the receipt.
   */
  QString htmlContent();
  
  const double getReceiptPrice() const  {return this->totalPrice;}
protected:
  /**
   * @brief Sets up the receipt display.
   *
   * Creates and configures the layout that will contain the receipt elements.
   */
  void setupReceiptDisplay();
  
private:
  /**
   * @brief Increases the receipt total price.
   *
   * Adds the price of the given product to the total price and updates the UI.
   *
   * @param product The Product whose price is to be added.
   */
  void increaseReceiptPrice(const Product &product);
  
  /**
   * @brief Reduces the receipt total price.
   *
   * Subtracts the price of the given product from the total price and updates the UI.
   *
   * @param product The Product whose price is to be subtracted.
   */
  void reduceReceiptPrice(const Product &product);
};

#endif // RECEIPT_H
