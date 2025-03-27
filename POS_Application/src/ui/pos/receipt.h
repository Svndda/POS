#ifndef RECEIPT_H
#define RECEIPT_H

#include <QWidget>
#include <QString>

#include "product.h"
#include "order.h"

class Receipt {
  
private:
  QString businessName; ///< Nombre del negocio
  size_t ID;            ///< Identificador único del recibo
  QString dateTime;     ///< Fecha y hora
  QString user;         ///< Usuario
  std::vector<std::pair<Product, size_t>> products; ///< Productos en el recibo (producto, cantidad)
  QString paymentMethod; ///< Método de pago
  double receivedAmount; ///< Cantidad recibida
  double price;          ///< Precio total

public:  
  explicit Receipt(const QString myBusinessName = QString()
      , const size_t myID = 0
      , const QString myDateTime = QString()
      , const QString myUser = QString()
      , const std::vector<std::pair<Product, size_t>> myProducts
      = std::vector<std::pair<Product, size_t>>()
      , const QString myPaymentMethod = QString()
      , const double myReceivedAmount = 0
      , const double myPrice = 0);
  
  Receipt(const Receipt& other);
  
  Receipt(const QString bussinessName
      , const size_t id
      , const QString username
      , const Order& order);
  
  ~Receipt();
  
  Receipt& operator=(const Receipt&) = default;
  
  friend std::ifstream& operator>>(std::ifstream& in, Receipt& receipt);
  friend std::ofstream& operator<<(std::ifstream& in, Receipt& receipt);
public:
  // Getters públicos para cada atributo (necesarios para el operador de flujo)
  QString getBusinessName() const { return businessName; }
  size_t getID() const { return ID; }
  QString getDateTime() const { return dateTime; }
  QString getUser() const { return user; }
  std::vector<std::pair<Product, size_t>> getProducts() const { return products; }
  QString getPaymentMethod() const { return paymentMethod; }
  double getReceivedAmount() const { return receivedAmount; }
  double getPrice() const { return price; }
  
  QString formatProductList();
  
private:
  // Setters privados
  void setBusinessName(const QString& name) { businessName = name; }
  void setID(size_t id) { ID = id; }
  void setDateTime(const QString& date) { dateTime = date; }
  void setUser(const QString& userName) { user = userName; }
  void setProducts(const std::vector<std::pair<Product, size_t>>& productList) { products = productList; }
  void setPaymentMethod(const QString& method) { paymentMethod = method; }
  void setReceivedAmount(double amount) { receivedAmount = amount; }
  void setPrice(double totalPrice) { price = totalPrice; }
  
};

// Declaraciones externas para los operadores de flujo
QDataStream& operator<<(QDataStream& out, const Receipt& receipt);
std::ifstream& operator>>(std::ifstream& in, Receipt& receipt);
std::ofstream& operator<<(std::ofstream& out, const Receipt& receipt);
QDebug operator<<(QDebug dbg, const Receipt& receipt);

#endif // RECEIPT_H
