#ifndef USER_H
#define USER_H

#include <fstream>
#include <qdebug.h>
#include <qlogging.h>
#include <string>
#include <vector>

/**
 * @class User
 * @brief Represents a user in the POS system.
 *
 * The User class stores a user's unique identifier, name, hashed password,
 * and a set of page access permissions. It provides functionality to set and
 * verify the password as well as serialize/deserialize its data in binary format.
 */
class User {
public:
  /**
   * @class PageAccess
   * @brief Manages access permissions for a specific page.
   *
   * The PageAccess class holds the index of a page and its associated
   * access and data editing permissions. The permissions are represented as
   * size_t constants.
   */
  class PageAccess {
  public:
    static const size_t EDITABLE = 1;       ///< Editable data indicator.
    static const size_t NON_EDITABLE = 0;   ///< Non-editable data indicator.
    
    static const size_t ALLOWED = 3;        ///< Access allowed indicator.
    static const size_t DENEGATED = 2;      ///< Access denied indicator.
    
  public:
    size_t pageIndex = 0;   ///< Index of the page.
    size_t access = this->DENEGATED;    ///< Access permission (allowed/denegated).
    size_t dataAccess = this->NON_EDITABLE; ///< Data access permission (editable/non-editable).
    
  public:
    /**
     * @brief Constructs a PageAccess object.
     * @param pageIndexed The index of the page.
     * @param pageAccess The access permission for the page.
     * @param pageDataAccess The data access permission for the page.
     */
    PageAccess(const size_t pageIndexed = 0
        , const size_t pageAccess = PageAccess::DENEGATED
        , const size_t pageDataAccess = PageAccess::NON_EDITABLE);
    
    /**
     * @brief Destructor.
     */
    ~PageAccess() = default;
  public:
    /**
     * @brief Equality operator.
     *
     * Compares two PageAccess objects for equality based on pageIndex,
     * access, and dataAccess.
     *
     * @param other The other PageAccess object to compare.
     * @return True if all attributes match; otherwise, false.
     */
    bool operator==(const PageAccess& other) const;
    
    /**
     * @brief Assignment operator.
     *
     * Copies the values from the other PageAccess into this instance.
     *
     * @param other The other PageAccess object.
     * @return Reference to the current object.
     */
    PageAccess& operator=(const PageAccess& other);
    
  public:
    /**
     * @brief Saves the PageAccess data to a binary file.
     *
     * Serializes the pageIndex, access, and dataAccess values to the output stream.
     *
     * @param outFile Output file stream (must be open).
     */
    void saveToBinary(std::ofstream& outFile) const;
    
    /**
     * @brief Loads the PageAccess data from a binary file.
     *
     * Deserializes the pageIndex, access, and dataAccess values from the input stream.
     *
     * @param inFile Input file stream (must be open).
     */
    void loadFromBinary(std::ifstream& inFile);
  };
  
private:
public:
  size_t id = 0;                    ///< Unique identifier of the user.
  std::string name = "";            ///< Name of the user.
  std::vector<PageAccess> permissions; ///< Vector of page access permissions.
  size_t password = 0;              ///< Hashed password.
  
public:
  /**
   * @brief Constructs a User object.
   *
   * Initializes the User with a given id, name, and an optional set of page access permissions.
   *
   * @param userId Unique identifier for the user.
   * @param userName The user's name.
   * @param userPermissions A vector of PageAccess objects representing the user's permissions.
   */
  User(const size_t userId = 0
      , const std::string userName = ""
      , const std::vector<PageAccess> userPermissions
      = std::vector<PageAccess>());
  
  /**
   * @brief Destructor.
   */
  ~User() = default;
public:
  /**
   * @brief Equality operator.
   *
   * Compares two User objects based on their name and hashed password.
   *
   * @param other The other User object to compare.
   * @return True if the names and passwords match; otherwise, false.
   */
  bool operator==(const User& other) const;
  
  /**
   * @brief Assignment operator.
   *
   * Copies the attributes from another User into this instance.
   *
   * @param other The User object to copy from.
   * @return Reference to the current object.
   */
  User& operator=(const User& other);
  
  /**
   * @brief Sets the user's password.
   *
   * Hashes the provided password string and stores it.
   *
   * @param newPassword The new password to set.
   */
  void setPassword(const std::string& newPassword);
  
  /**
   * @brief Verifies the provided password against the stored hash.
   *
   * @param passwordToCheck The password to verify.
   * @return True if the provided password matches the stored hash; otherwise, false.
   */
  bool verifyPassword(const std::string& passwordToCheck) const;
  
  /**
   * @brief Retrieves the user's name.
   *
   * @return Constant reference to the user's name string.
   */
  const std::string& getUsername() const;
  
  /**
   * @brief Saves the user's data to a binary file.
   *
   * Serializes the user's id, name, password, and permissions to the output stream.
   *
   * @param outFile Output file stream (must be open).
   */
  void saveToBinary(std::ofstream& outFile) const;
  
  /**
   * @brief Loads the user's data from a binary file.
   *
   * Deserializes the user's id, name, password, and permissions from the input stream.
   *
   * @param inFile Input file stream (must be open).
   */
  void loadFromBinary(std::ifstream& inFile);
};

#endif // USER_H
