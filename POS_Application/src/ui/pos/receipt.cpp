#include "receipt.h"

#include <fstream>

#include <QDataStream>
#include <QDatetime>

Receipt::Receipt(const QString myBusinessName
    , const size_t myID
    , const QString myDateTime
    , const QString myUser
    , const std::vector<std::pair<Product, size_t>> myProducts
    , const QString myPaymentMethod
    , const double myReceivedAmount
    , const double myPrice)
    : businessName(myBusinessName)
    , ID(myID)
    , dateTime(myDateTime)
    , user(myUser)
    , products(myProducts)
    , paymentMethod(myPaymentMethod)
    , receivedAmount(myReceivedAmount)
    , price(myPrice) {
}

Receipt::Receipt(const Receipt& other)
    : businessName(other.businessName),
    ID(other.ID),
    dateTime(other.dateTime),
    user(other.user),
    products(other.products),
    paymentMethod(other.paymentMethod),
    receivedAmount(other.receivedAmount),
    price(other.price) {
}

Receipt::Receipt(const QString bussinessName
    , const size_t id
    , const QString username
    , const Order& order)
    : businessName(bussinessName),  // Puedes cambiarlo según corresponda
    ID(id),  // Asignar un ID único según la lógica de la aplicación
    dateTime(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")),
    user(username),  // Si el Order tiene usuario, puedes extraerlo
    products(order.getOrderProducts()),  // Copia los productos desde Order
    paymentMethod(order.getPaymentMethod()),
    receivedAmount(order.getReceivedMoney()),
    price(order.getOrderPrice()) {
}

Receipt::~Receipt() {

}

std::ofstream& operator<<(std::ofstream& out, const Receipt& receipt) {
  auto writeQString = [&](const QString& qstr) {
    std::string str = qstr.toUtf8().toStdString();  // Convertir QString a std::string en UTF-8
    size_t length = str.size();
    
    out.write(reinterpret_cast<const char*>(&length), sizeof(length));
    out.write(str.data(), length);
  };
  
  writeQString(receipt.getBusinessName());
  size_t id = receipt.getID();
  out.write(reinterpret_cast<const char*>(&id), sizeof(id));
  
  writeQString(receipt.getDateTime());
  writeQString(receipt.getUser());
  
  size_t productCount = receipt.getProducts().size();
  out.write(reinterpret_cast<const char*>(&productCount), sizeof(productCount));
  
  for (const auto& productPair : receipt.getProducts()) {
    writeQString(QString(productPair.first.getName().data()));
    out.write(reinterpret_cast<const char*>(&productPair.second), sizeof(productPair.second));
  }
  
  writeQString(receipt.getPaymentMethod());
  double receivedAmount = receipt.getReceivedAmount();
  double price = receipt.getPrice();
  
  out.write(reinterpret_cast<const char*>(&receivedAmount), sizeof(receivedAmount));
  out.write(reinterpret_cast<const char*>(&price), sizeof(price));
  
  return out;
}

std::ifstream& operator>>(std::ifstream& in, Receipt& receipt) {
  size_t id;
  size_t productCount;
  double receivedAmount, price;
  std::string businessName, dateTime, user, paymentMethod;
  std::vector<std::pair<Product, size_t>> products;
  
  // Leer cadenas con su tamaño
  auto readString = [&](std::string& str) {
    size_t length;
    in.read(reinterpret_cast<char*>(&length), sizeof(length));
    
    str.resize(length);
    in.read(&str[0], length);
  };
  
  readString(businessName);
  in.read(reinterpret_cast<char*>(&id), sizeof(id));
  
  readString(dateTime);
  readString(user);
  
  in.read(reinterpret_cast<char*>(&productCount), sizeof(productCount));  
  // Leer productos
  for (size_t i = 0; i < productCount; ++i) {
    std::string productName;
    size_t quantity;
    readString(productName);
    in.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));
    
    Product blankProduct;
    blankProduct.setName(productName);
    products.emplace_back(blankProduct, quantity);
  }
  
  readString(paymentMethod);
  in.read(reinterpret_cast<char*>(&receivedAmount), sizeof(receivedAmount));
  in.read(reinterpret_cast<char*>(&price), sizeof(price));
  
  receipt = Receipt(businessName.data(), id
                    , dateTime.data(), user.data()
                    , products, paymentMethod.data()
                    , receivedAmount, price
                    );
  
  return in;
}

// Operador de salida (serialización)
QDataStream& operator<<(QDataStream& out, const Receipt& receipt) {
  out << receipt.getBusinessName()
      << static_cast<quint64>(receipt.getID())  // size_t no es garantizado igual en todas las plataformas, mejor cast a tipo fijo
      << receipt.getDateTime()
      << receipt.getUser()
      << static_cast<quint32>(receipt.getProducts().size());  // Guardamos primero el tamaño de la lista
  
  for (const auto& productPair : receipt.getProducts()) {
    out << QString(productPair.first.getName().data())  // Product también debe implementar << y >>
        << static_cast<quint64>(productPair.second);  // cantidad
  }
  
  out << receipt.getPaymentMethod()
      << receipt.getReceivedAmount()
      << receipt.getPrice();
  
  return out;
}

// Sobrecarga del operador << para qDebug()
QDebug operator<<(QDebug dbg, const Receipt& receipt) {
  dbg.nospace() << "Business Name: " << receipt.getBusinessName()
  << "\nReceipt ID: " << receipt.getID()
  << "\nDate/Time: " << receipt.getDateTime()
  << "\nUser: " << receipt.getUser()
  << "\nPayment Method: " << receipt.getPaymentMethod()
  << "\nReceived Amount: " << receipt.getReceivedAmount()
  << "\nTotal Price: " << receipt.getPrice()
  << "\nProducts: \n";
  
  // Mostrar los productos del recibo
  for (const auto& productPair : receipt.getProducts()) {
    dbg.nospace() << "  Product: "
  	    << QString(productPair.first.getName().data())
    << " Quantity: " << productPair.second << "\n";
  }
  
  return dbg;
}

// Operador de entrada (deserialización)
QDataStream& operator>>(QDataStream& in, Receipt& receipt) {
  QString businessName;
  quint64 id;
  QString dateTime;
  QString user;
  quint32 productCount;
  std::vector<std::pair<Product, size_t>> products;
  QString paymentMethod;
  double receivedAmount;
  double price;
  
  in >> businessName
      >> id
      >> dateTime
      >> user
      >> productCount;
  
  for (quint32 i = 0; i < productCount; ++i) {
    QString productName;
    quint64 quantity;
    
    in >> productName >> quantity;
    Product blankProduct;
    blankProduct.setName(productName.toStdString());
    products.emplace_back(blankProduct, static_cast<size_t>(quantity));
  }
  
  in >> paymentMethod
      >> receivedAmount
      >> price;
  
  // Crear un nuevo Receipt usando un constructor adecuado (recomendado) o setters.
  receipt = Receipt(businessName, static_cast<size_t>(id), dateTime, user,
                    products, paymentMethod, receivedAmount, price);
  
  return in;
}

QString Receipt::formatProductList() {
  QString formattedList;
  
  for (const auto& productPair : this->products) {
    const Product& product = productPair.first;
    size_t quantity = productPair.second;
    double totalPrice = quantity * product.getPrice();
    
    formattedList += QString("%1 x %2 : %3\n")
        .arg(quantity)
        .arg(QString::fromStdString(product.getName()))
        .arg(totalPrice, 0, 'f', 2); // Formato con 2 decimales
  }
  
  return formattedList.trimmed();  // Elimina el último salto de línea extra
}
