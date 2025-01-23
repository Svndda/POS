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
 * @brief The main controller class for managing the application workflow.
 *
 * This class acts as the primary controller for the application, managing 
 * page transitions and interactions with the POS model.
 */
class AppController : public QMainWindow {
  Q_OBJECT

public:
  /**
   * @brief Constructs the main application controller.
   * @param parent Pointer to the parent widget.
   */
  AppController(QWidget* parent = nullptr);
  
  /**
   * @brief Destroys the application controller and cleans up resources.
   */
  ~AppController();

private:
  Ui::MainWindow* ui;                  ///< Pointer to the main UI layout.
  QStackedWidget* pageStack;           ///< Stack widget managing application pages.
  POS_Model& model;                    ///< Reference to the POS model instance.
  
private slots:
  /**
   * @brief Displays the categories page in the application.
   */
  void inventory_categoriesDisplay();
  
  /**
   * @brief Displays the products page in the application.
   */
  void inventory_productsDisplay();
  
  /**
   * @brief Displays the supplies page in the application.
   */
  void inventory_primeDisplay();
};
#endif // APPCONTROLLER_H
