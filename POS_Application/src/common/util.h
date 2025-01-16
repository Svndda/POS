#ifndef UTIL_H
#define UTIL_H

#include <QLabel>

class Util {
  // Deletes all the operators and constructors to disable the class instantion.
  Util() = delete;
  ~Util() = delete;
  Util& operator=(Util&& other) = delete;
  Util& operator=(Util& other) = delete;
  Util operator=(const Util& other) = delete;
  Util operator=(const Util&& other) = delete;
  
  public: // QT functions.
  
  static void updatePrefixedLabel(QWidget* view, const QString& labelPrefix
      , const size_t labelIt, const QString& value);
    
  QLabel* cloneLabel(QLabel* original) {
    QLabel* newLabel = new QLabel(original->parentWidget());
    
    // Copy the important attributes.
    newLabel->setText(original->text());
    newLabel->setAlignment(original->alignment());
    newLabel->setStyleSheet(original->styleSheet());
    newLabel->setFont(original->font());
    newLabel->setPixmap(original->pixmap(Qt::ReturnByValue)); // Si usa pixmap
    
    // Other settings if needed.
    newLabel->setSizePolicy(original->sizePolicy());
    newLabel->setGeometry(original->geometry());
    
    return newLabel;
  }
};

#endif // UTIL_H
