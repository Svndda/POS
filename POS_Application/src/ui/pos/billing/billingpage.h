#ifndef BILLINGPAGE_H
#define BILLINGPAGE_H

#include <QWidget>
#include <QStackedWidget>
#include <QPrinter>

#include "model.h"

namespace Ui {
class BillingPage;
}

/**
 * @class BillingPage
 * @brief Manages the BillingPage interface for creating, canceling, and paying orders.
 *
 * The BillingPage class is responsible for handling the interactions within the BillingPage interface,
 * including product selection, order management, and printing previews. It interacts with
 * the POS_Model to update the current order and manage the products added to a order.
 */
class BillingPage : public QWidget {
  Q_OBJECT
  
private:
  Ui::BillingPage *ui = nullptr;            ///< Pointer to the UI elements of the POS interface.
  Model& model;                         ///< Reference to the POS_Model singleton.
  QStackedWidget* ordersStack = nullptr;    ///< Stack widget to manage the orders.
  size_t openedOrders = 0;                ///< Counter for opened orders.
  
public:
  /**
   * @brief Constructs a BillingPage object.
   *
   * Initializes the BillingPage interface, sets up the UI, creates the order stack and
   * the product selection buttons.
   *
   * @param parent Pointer to the parent widget.
   * @param appmodel Reference to the POS_Model instance (defaults to the singleton instance).
   */
  explicit BillingPage(QWidget *parent = nullptr
      , Model &appModel = Model::getInstance());
  
  /**
   * @brief Destroys the Pos object and cleans up allocated resources.
   */
  ~BillingPage();
  
private:
  /**
   * @brief Creates and prepare the necesary initial state of some ui elements.
   *
   * Creates the dynamic ui elements layout and intial elements state.
   *
   */
  void setupDisplay();
  
  /**
   * @brief Creates product selection buttons.
   *
   * Dynamically generates buttons for product selection.
   *
   */
  void createSelectProductButtons();
  
  /**
   * @brief Adds a new order to the system
   */
  void addOrder();
  
  /**
   * @brief Removes an order from the system
   * @param orderWidget Widget containing the order to remove
   * @param index Index of the order to remove
   */
  void removeOrder(QWidget* orderWidget, const size_t index);
  
  /**
   * @brief Prints a receipt for an order
   * @param order Order to print receipt for
   */
  void printReceipt(const Order& order);
  
  /**
   * @brief Paints receipt content on a printer
   * @param printer Printer to use
   * @param order Order to print
   */
  void paintReceipt(QPrinter& printer, const Order& order);
  
private slots:
  /**
   * @brief Slot invoked when the "Create Order" button is clicked.
   *
   * Creates a new order selection button if there is no active order pending.
   */
  void createOrder_button_clicked();
  
  /**
   * @brief Slot invoked when the "Cancel Order" button is clicked.
   *
   * Cancels the current order by removing it from the order stack and updating the UI.
   */
  void cancelOrder_button_clicked();
  
  /**
   * @brief Slot invoked when the "Pay Order" button is clicked.
   *
   * Initiates the print preview process for the current order, and upon successful printing,
   * removes the order from the stack.
   */
  void payOrder_button_clicked();
  
  /**
   * @brief Adds a product to the current order.
   *
   * Adds the specified product to the currently active order widget.
   *
   * @param product The Product to add to the order.
   */
  void addProductToOrder(const Product& product);
  
signals:
  void orderProcessed();
};

#endif // BILLINGPAGE_H
