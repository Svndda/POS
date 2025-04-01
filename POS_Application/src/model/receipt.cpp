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
    : businessName(bussinessName),
    ID(id),
    dateTime(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")),
    user(username),
    products(order.getProducts()),
    paymentMethod(order.getPaymentMethod()),
    receivedAmount(order.getReceivedMoney()),
    price(order.getPrice()) {
}

Receipt::~Receipt() {

}

std::ofstream& operator<<(std::ofstream& out, const Receipt& receipt) {
  // Helper lambda to write QString as UTF-8 string with length prefix  
  auto writeQString = [&](const QString& qstr) {
    // Converts from QString to std::string in UFT-8
    std::string str = qstr.toUtf8().toStdString();
    // Obtains the string size.
    size_t length = str.size();
    // Writes out the string length in binary mode.
    out.write(reinterpret_cast<const char*>(&length), sizeof(length));
    // Writes out the string in binary mode.
    out.write(str.data(), length);
  };
  
  // Serialize all receipt fields.
  writeQString(receipt.getBusinessName());
  size_t id = receipt.getID();
  out.write(reinterpret_cast<const char*>(&id), sizeof(id));
  
  writeQString(receipt.getDateTime());
  writeQString(receipt.getUser());
  
  // Serialize products list  
  size_t productCount = receipt.getProducts().size();
  out.write(reinterpret_cast<const char*>(&productCount), sizeof(productCount));  
  for (const auto& productPair : receipt.getProducts()) {
    writeQString(QString(productPair.first.getName().data()));
    out.write(reinterpret_cast<const char*>(&productPair.second)
        , sizeof(productPair.second));
  }
  
  // Serialize payment information  
  writeQString(receipt.getPaymentMethod());
  double receivedAmount = receipt.getReceivedAmount();
  double price = receipt.getPrice();
  out.write(reinterpret_cast<const char*>(&receivedAmount)
      , sizeof(receivedAmount));
  out.write(reinterpret_cast<const char*>(&price), sizeof(price));
  
  return out;
}

std::ifstream& operator>>(std::ifstream& in, Receipt& receipt) {
  size_t id;
  size_t productCount;
  double receivedAmount, price;
  std::string businessName, dateTime, user, paymentMethod;
  std::vector<std::pair<Product, size_t>> products;
  
  // Helper lambda to read length-prefixed strings
  auto readString = [&](std::string& str) {
    size_t length;
    in.read(reinterpret_cast<char*>(&length), sizeof(length));
    
    str.resize(length);
    in.read(&str[0], length);
  };
  
  // Deserialize all receipt fields  
  readString(businessName);
  in.read(reinterpret_cast<char*>(&id), sizeof(id));
  readString(dateTime);
  readString(user);
  
  // Deserialize products list  
  in.read(reinterpret_cast<char*>(&productCount), sizeof(productCount));  
  for (size_t i = 0; i < productCount; ++i) {
    std::string productName;
    size_t quantity;
    readString(productName);
    in.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));
    
    Product blankProduct;
    blankProduct.setName(productName);
    products.emplace_back(blankProduct, quantity);
  }
  
  // Deserialize the receipt's payment method.
  readString(paymentMethod);
  in.read(reinterpret_cast<char*>(&receivedAmount), sizeof(receivedAmount));
  // Deserialize the receipt's price.  
  in.read(reinterpret_cast<char*>(&price), sizeof(price));
  
  // Creates a new instance of a receipt to store the readed data.
  receipt = Receipt(businessName.data(), id
                    , dateTime.data(), user.data()
                    , products, paymentMethod.data()
                    , receivedAmount, price
                    );
  
  return in;
}

// Operador de salida (serialización)
QDataStream& operator<<(QDataStream& out, const Receipt& receipt) {
  // Serialize receipt's information.
  out << receipt.getBusinessName()
      << static_cast<quint64>(receipt.getID())
      << receipt.getDateTime()
      << receipt.getUser()
      << static_cast<quint32>(receipt.getProducts().size());
  
  // Serialize products list  
  for (const auto& productPair : receipt.getProducts()) {
    out << QString(productPair.first.getName().data())
        << static_cast<quint64>(productPair.second);
  }
  
  // Serialize payment information  
  out << receipt.getPaymentMethod()
      << receipt.getReceivedAmount()
      << receipt.getPrice();
  
  return out;
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
  
  // Deserialize receipt's information. 
  in >> businessName
      >> id
      >> dateTime
      >> user
      >> productCount;
  
  // Deserialize products list.
  for (quint32 i = 0; i < productCount; ++i) {
    QString productName;
    quint64 quantity;
    
    in >> productName >> quantity;
    Product blankProduct;
    blankProduct.setName(productName.toStdString());
    products.emplace_back(blankProduct, static_cast<size_t>(quantity));
  }
  
  // Deserialize payment information.  
  in >> paymentMethod
      >> receivedAmount
      >> price;
  
  // Creates a new receipt's instance to store the receipt's information.
  receipt = Receipt(businessName, static_cast<size_t>(id), dateTime, user,
                    products, paymentMethod, receivedAmount, price);
  
  return in;
}

QDebug operator<<(QDebug dbg, const Receipt& receipt) {
  // Shows the receipts information.
  dbg.nospace() << "Business Name: " << receipt.getBusinessName()
  << "\nReceipt ID: " << receipt.getID()
  << "\nDate/Time: " << receipt.getDateTime()
  << "\nUser: " << receipt.getUser()
  << "\nPayment Method: " << receipt.getPaymentMethod()
  << "\nReceived Amount: " << receipt.getReceivedAmount()
  << "\nTotal Price: " << receipt.getPrice()
  << "\nProducts: \n";
  
  // Show the receipt's products.
  for (const auto& productPair : receipt.getProducts()) {
    dbg.nospace() << "  Product: "
  	    << QString(productPair.first.getName().data())
    << " Quantity: " << productPair.second << "\n";
  }
  
  return dbg;
}
