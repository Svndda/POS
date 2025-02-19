// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QMainWindow>
#include <QStackedWidget>
#include "posmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class AppController
 * @brief Main application controller managing UI and workflow.
 *
 * This class coordinates the application pages, user interactions,
 * and communication with the POS model. It manages page transitions,
 * enables/disables UI controls, and processes user authentication.
 */
class AppController : public QMainWindow {
  Q_OBJECT

public:
  /**
   * @brief Constructs the AppController.
   * @param parent Pointer to the parent widget.
   */
  explicit AppController(QWidget* parent = nullptr);
  
  /**
   * @brief Destroys the AppController and performs cleanup.
   */
  ~AppController();

private:
  Ui::MainWindow* ui;                      ///< Pointer to the main UI layout.
  QStackedWidget* pageStack;               ///< Stack widget managing application pages.
  POS_Model& model = POS_Model::getInstance(); ///< Reference to the POS model singleton.
  
private:
  /**
   * @brief Sets up the connections between UI signals and controller slots.
   */
  void setupConnections();
  
  /**
   * @brief Enables all navigation buttons in the UI.
   */
  void enableButtons();
  
  /**
   * @brief Disables all navigation buttons in the UI.
   */
  void disableButtons();
  
  /**
   * @brief Prepares and creates system pages such as POS and Inventory.
   */
  void prepareSystemPages();
  
  void refreshPageStack(const size_t pageIndex);
  /**
   * @brief Switches the displayed page in the stack widget.
   * @param pageIndex The index of the page to display.
   */
  void switchPages(const size_t pageIndex);
  
private slots:
  /**
   * @brief Handles the click event on the POS button.
   */
  void on_pos_button_clicked();
  
  /**
   * @brief Handles the click event on the Inventory button.
   */
  void on_inventory_button_clicked();
  
  /**
   * @brief Handles the click event on the Sells button.
   */
  void on_sells_button_clicked();
  
  /**
   * @brief Handles the click event on the Users button.
   */
  void on_users_button_clicked();
  
  /**
   * @brief Handles the click event on the Settings button.
   */
  void on_settings_button_clicked();
  
  /**
   * @brief Processes the accepted user credentials.
   * @param user The authenticated user.
   */
  void userAccepted(const User user);
};
#endif // APPCONTROLLER_H
