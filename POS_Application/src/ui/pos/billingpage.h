#ifndef BILLINGPAGE_H
#define BILLINGPAGE_H

#include <QWidget>
#include <QStackedWidget>
#include <QPrinter>

#include "posmodel.h"

namespace Ui {
class BillingPage;
}

/**
 * @class BillingPage
 * @brief Manages the BillingPage interface for creating, canceling, and paying receipts.
 *
 * The BillingPage class is responsible for handling the interactions within the BillingPage interface,
 * including product selection, receipt management, and printing previews. It interacts with
 * the POS_Model to update the current receipt and manage the products added to a receipt.
 */
class BillingPage : public QWidget {
  Q_OBJECT
  
private:
  Ui::BillingPage *ui = nullptr;            ///< Pointer to the UI elements of the POS interface.
  POS_Model& model;                         ///< Reference to the POS_Model singleton.
  QStackedWidget* receipStack = nullptr;    ///< Stack widget to manage the receipts.
  size_t openedReceipts = 1;                ///< Counter for opened receipts.
  
public:
  /**
   * @brief Constructs a BillingPage object.
   *
   * Initializes the BillingPage interface, sets up the UI, creates the receipt stack and
   * the product selection buttons.
   *
   * @param parent Pointer to the parent widget.
   * @param appmodel Reference to the POS_Model instance (defaults to the singleton instance).
   */
  explicit BillingPage(QWidget *parent = nullptr
      , POS_Model &appModel = POS_Model::getInstance());
  
  /**
   * @brief Destroys the Pos object and cleans up allocated resources.
   */
  ~BillingPage();
  
public:  
  /**
   * @brief Adds a product to the current receipt.
   *
   * Adds the specified product to the currently active receipt widget.
   *
   * @param product The Product to add to the receipt.
   */
  void addProductToReceipt(const Product& product);
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
   * Dynamically generates buttons for product selection based on the number of products.
   *
   * @param numberOfProducts The total number of product buttons to create.
   */
  void createSelectProductButtons(size_t numberOfProducts);
private slots:
  /**
   * @brief Slot invoked when the "Create Receipt" button is clicked.
   *
   * Creates a new receipt selection button if there is no active receipt pending.
   */
  void on_createReceipt_button_clicked();
  
  /**
   * @brief Slot invoked when the "Cancel Receipt" button is clicked.
   *
   * Cancels the current receipt by removing it from the receipt stack and updating the UI.
   */
  void on_cancelReceipt_button_clicked();
  
  /**
   * @brief Slot invoked when the "Pay Receipt" button is clicked.
   *
   * Initiates the print preview process for the current receipt, and upon successful printing,
   * removes the receipt from the stack.
   */
  void on_payReceipt_button_clicked();
  
  /**
   * @brief Slot for handling print preview.
   *
   * Renders the current receipt's content to the provided printer.
   *
   * @param printer Pointer to the QPrinter object used for printing.
   */
  void printPreview(QPrinter* printer);
};

#endif // BILLINGPAGE_H
