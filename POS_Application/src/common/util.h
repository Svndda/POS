// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef UTIL_H
#define UTIL_H

#include <QLabel>

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
   * @brief Clones a QLabel.
   *
   * Creates a new QLabel that is a clone of the original label by copying its key attributes
   * such as text, alignment, style, font, pixmap, size policy, and geometry.
   *
   * @param original Pointer to the original QLabel to clone.
   * @return A pointer to the newly created QLabel clone.
   */
  static QLabel* cloneLabel(QLabel* original);
};

#endif // UTIL_H
