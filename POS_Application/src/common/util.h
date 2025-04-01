// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef UTIL_H
#define UTIL_H

#include <QLabel>
#include <qprinter.h>

/**
 * @class Util
 * @brief Non-instantiable utility class for UI helper functions.
 *
 * The Util class provides helper functions for updating labels and cloning QLabel objects.
 * It is designed to be used statically; hence, all constructors and assignment operators are deleted.
 */
class Util {
  // Delete all constructors and assignment operators to prevent instantiation.
  Util() = delete;
  ~Util() = delete;
  Util& operator=(Util&& other) = delete;
  Util& operator=(Util& other) = delete;
  Util operator=(const Util& other) = delete;
  Util operator=(const Util&& other) = delete;
  
  public: // QT functions.
  /**
   * @brief Updates a label with a prefixed text.
   *
   * This static function searches within the given view for a label identified by a prefix and index,
   * and updates its text with the provided value.
   *
   * @param view Pointer to the parent widget containing the label.
   * @param labelPrefix The prefix used to identify the label.
   * @param labelIt The index of the label.
   * @param value The new text value to set on the label.
   */
  static void updatePrefixedLabel(QWidget* view, const QString& labelPrefix
      , const size_t labelIt, const QString& value);
  
  /**
   * @brief Prints a line with word wrapping
   * @param painter Painter to use
   * @param text Text to print
   * @param y Current y position (updated by function)
   * @param bold Whether to print in bold
   * @param metrics Font metrics for calculations
   * @param margin Page margins
   * @param contentWidth Available content width
   * @param lineHeight Height of each line
   */
  static void printWrappedLine(QPainter &painter, const QString &text
      , int &y, const bool bold, const QFontMetrics &metrics, const int margin
      , const int contentWidth, const int lineHeight);
  
  /**
   * @brief Attempts to configure a printer for receipt printing
   * @param parent Parent widget for dialogs
   * @param preferredPrinterName Name of the preferred printer (default "POS-80C")
   * @return Configured QPrinter object if successful, nullptr otherwise
   * @note The caller takes ownership of the returned QPrinter object
   */
  static std::unique_ptr<QPrinter> systemPrinter(
      QWidget *parent, const QString &preferredPrinterName);
private:
  /**
  * @brief Gets the printable width of the currently selected printer in millimeters
  * @param printer Reference to the configured QPrinter object
  * @return Printable width in millimeters, or default 80mm if cannot be determined
  */
  static qreal getPrintableWidth(QPrinter &printer);
};

#endif // UTIL_H
