// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QMainWindow>
#include <QStackedWidget>
#include "model.h"

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
  Model& model = Model::getInstance(); ///< Reference to the POS model singleton.
  
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
  
  /**
   * @brief Refreshes the page stack based on the given index.
   * 
   * This function checks if the application model is started and retrieves 
   * the current user's permissions. If the user has access to the requested 
   * page, it switches to the corresponding page.
   * 
   * @param stackIndex The index of the page stack to switch to.
   */
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
  
private slots:
  /**
   * @brief Processes the accepted user credentials.
   * @param user The authenticated user.
   */
  void userAccepted(const User user);
  
  /**
   * @brief Resets the application state to its initial configuration.
   * 
   * This function shuts down the model and resets the page stack to its 
   * initial state by setting the current index to 0, which corresponds
   * to the login page.
   */
  void resetApplicationState();
};
#endif // APPCONTROLLER_H
