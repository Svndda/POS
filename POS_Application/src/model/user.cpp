#include "user.h"

#include <iostream>

User::User(const size_t userId, const std::string userName
    , const std::vector<PageAccess> userPermissions)
    : id(userId)
    , name(userName)
    , permissions(userPermissions) {
}

User::PageAccess::PageAccess(const size_t pageIndexed, const size_t pageAccess)
    : pageIndex(pageIndexed)
    , access(pageAccess) {
}

const size_t& User::getID() const {
  // Returns the user's id.
  return this->id;
}

void User::setUserID(const size_t id) {
  // Sets the user's id.
  this->id = id;
}

const std::string& User::getUsername() const {
  // Returns the user's name.
  return this->name;
}

void User::setUsername(const std::string name) {
  // Sets the user's name.
  this->name = name;
}

void User::setPassword(const std::string& newPassword) {
  // Creates a std hasher for the given password.
  std::hash<std::string> hasher;
  // Sets the hashed result as the user's password.
  this->password = hasher(newPassword);
}

bool User::verifyPassword(const std::string& passwordToCheck) const {
  // Creates a std hasher for the given password.  
  std::hash<std::string> hasher;
  // Checks if the hashed result equals the user's password.
  return this->password == hasher(passwordToCheck);
}

const std::vector<User::PageAccess>& User::getUserPermissions() const {
  // Returns the user's permissions.
  return this->permissions;
}

void User::setUserPermissions(const std::vector<User::PageAccess> permissions) {
  // Sets the user's permissions.
  this->permissions = permissions;
}

bool User::operator==(const User& other) const {
  // Checks if the users's attributes matches.
  return (this->name == other.name)
      && (this->password == other.password);
}

bool User::operator!=(const User& other) const {
  // Checks if the users's attributes does not matches.
  return !(*this == other);
}

User& User::operator=(const User& other) {
  // Avoid reasignation.
  if (this == &other) {
    // Avoids autoassignation.
    return *this;
  }
  
  qDebug() << "operador = User utilizado";
  // Copy the other object attributes.  
  this->id = other.id;
  this->name = other.name;
  this->permissions = other.permissions;
  this->password = other.password;
  
  // Returns the actual object.
  return *this;
}

bool User::PageAccess::operator==(const PageAccess& other) const {
  // Checks if the page's accesses aattributes matches.
  return (this->pageIndex == other.pageIndex)
      && (this->access == other.access);;  
}

User::PageAccess& User::PageAccess::operator=(const PageAccess& other) {
  // Avoid reasignation.
  if (*this == other) {
    return *this;
  }
  // Copy the other object attributes.    
  this->pageIndex = other.pageIndex;
  this->access = other.access;
  // Returns the current object.
  return *this;
}

void User::saveToBinary(std::ofstream& outFile) const {
  // Checks if the given output file is already open.
  if (outFile.is_open()) {
    // Writes out the user's id information.
    outFile.write(reinterpret_cast<const char*>(&this->id),
        sizeof(this->id));
    
    // Writes out the user's name information.
    size_t nameLength = this->name.size();
    outFile.write(reinterpret_cast<const char*>(&nameLength),
        sizeof(nameLength));
    outFile.write(this->name.c_str(), nameLength);
    
    // Writes out the user's password information.
    outFile.write(reinterpret_cast<const char*>(&this->password),
        sizeof(this->password));
    
    // Writes out the number of the user's permissions information.
    size_t numPermissions = this->permissions.size();
    outFile.write(reinterpret_cast<const char*>(&numPermissions),
        sizeof(numPermissions));
    
    // Writes out each user's permission information.
    for (const auto& perm : this->permissions) {
      perm.saveToBinary(outFile);
    }
  } else {
    std::cerr << "Error: El archivo no se pudo abrir para escritura.\n";
  }
}

void User::loadFromBinary(std::ifstream& inFile) {
  // Checks if the given input file is already open.
  if (inFile.is_open()) {
    // Reads the user's id information.
    size_t tmpId;
    inFile.read(reinterpret_cast<char*>(&tmpId), sizeof(tmpId));
    this->id = tmpId;
    
    // Reads the user's name information.
    size_t nameLength;
    inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    inFile.read(&name[0], nameLength);
    
    // Reads the user's password information.
    inFile.read(reinterpret_cast<char*>(&password),
        sizeof(password));
    
    // Reads the number of user's permissions.
    size_t numPermissions;
    inFile.read(reinterpret_cast<char*>(&numPermissions),
        sizeof(numPermissions));
    
    // Prepares the user's permissions vector.
    permissions.clear();
    permissions.resize(numPermissions);
    
    // Reads each user's permission.
    for (size_t i = 0; i < numPermissions; ++i) {
      permissions[i].loadFromBinary(inFile);
    }
  } else {
    std::cerr << "Error: El archivo no se pudo abrir para lectura.\n";
  }
}

void User::PageAccess::saveToBinary(std::ofstream& outFile) const {
  // Checks that the output file is already open.
  if (outFile.is_open()) {
    // Writes out the page access index.
    outFile.write(reinterpret_cast<const char*>(&this->pageIndex)
        , sizeof(this->pageIndex));
    // Writes out the page access user's access state information.    
    outFile.write(reinterpret_cast<const char*>(&this->access)
        , sizeof(this->access));
  } else {
    std::cerr << "Error: El archivo no se pudo abrir para escritura.\n";
  }
}

void User::PageAccess::loadFromBinary(std::ifstream& inFile) {
  if (inFile.is_open()) {
    // Reads out the page access index.  
    inFile.read(reinterpret_cast<char*>(&pageIndex)
        , sizeof(pageIndex));
    // Reads out the page access user's access state information.
    inFile.read(reinterpret_cast<char*>(&access)
        , sizeof(access));
  } else {
    std::cerr << "Error: El archivo no se pudo abrir para lectura.\n";
  }
}
