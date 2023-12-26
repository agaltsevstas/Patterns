#ifndef Database_hpp
#define Database_hpp

#include <iostream>

namespace pqxx
{
    class connection;
    class result;
    class row;
    class stream_from;
    class stream_to;
}

/*!
 * @brief Фасад (Fasade) - определяет интерфейс более высокого уровня, который предоставить более простой интерфейс для всей подсистемы, состоящей из множества объектов.
 */
class DatabaseFacade
{
    DatabaseFacade() noexcept;
    ~DatabaseFacade() noexcept;
    DatabaseFacade(const DatabaseFacade&) = delete;
    DatabaseFacade& operator=(const DatabaseFacade&) = delete;
    
public:
    struct Data
    {
        Data() noexcept = default;
        Data(unsigned iKey,
             const std::string& iSurname,
             const std::string& iName,
             const std::string& iPatronymic,
             const std::string& iSex,
             unsigned iAge);
        Data(const std::string& iSurname,
             const std::string& iName,
             const std::string& iPatronymic,
             const std::string& iSex,
             unsigned iAge);
        ~Data() noexcept = default;

        Data(const Data& other) noexcept;
        explicit Data(Data&& other) noexcept;
        Data& operator=(Data&& other) noexcept;
        Data& operator=(const Data& other) noexcept;
        
        unsigned key = 0;
        std::string surname;
        std::string name;
        std::string patronymic;
        std::string sex;
        unsigned age = 0;
        
        friend pqxx::stream_to& operator << (pqxx::stream_to& ioStream, const Data& object);
        friend bool operator >> (pqxx::stream_from &iStream, Data& oObject);
        friend void operator >> (const pqxx::row& iRow, Data& oObject);
    };
    
public:
    static DatabaseFacade& Instance();
    bool Load();
    bool Connection();
    void Disconnect();
    bool CreateTable();
    bool Delete();
    bool DeleteTable();
    bool ClearTable();
    bool UpdateSurname(unsigned iKey, const std::string& iSurname);
    bool UpdateName(unsigned iKey, const std::string& iName);
    bool UpdatePatronymic(unsigned iKey, const std::string& iPatronymic);
    bool UpdateSex(unsigned iKey, const std::string& iSex);
    bool UpdateAge(unsigned iKey, unsigned iAge);
    bool UpdateUser(unsigned iKey, const Data& iUser);
    bool UpdateUser(unsigned iKey,
                    const std::string& iSurname,
                    const std::string& iName,
                    const std::string& iPatronymic,
                    const std::string& iSex,
                    unsigned iAge);
    bool AddUser(const Data& iUser);
    bool AddUser(const std::string& iSurname,
                 const std::string& iName,
                 const std::string& iPatronymic,
                 const std::string& iSex,
                 unsigned iAge);
    bool AddUsers(const std::vector<Data>& iUsers);
    bool DeleteUser(unsigned iKey);
    bool DeleteUser(const std::string& iSurname,
                    const std::string& iName,
                    const std::string& iPatronymic);
    bool FindUserID(const std::string& iSurname,
                    const std::string& iName,
                    const std::string& iPatronymic,
                    unsigned& oKey);
    bool FindUser(unsigned iKey, Data& oUser);
    bool FindUser(const std::string& iSurname,
                  const std::string& iName,
                  const std::string& iPatronymic,
                  Data& oUser);
    bool FindUser(unsigned iKey,
                  std::string& oSurname,
                  std::string& oName,
                  std::string& oPatronymic,
                  std::string& oSex,
                  unsigned& oAge);
    bool GetAll(std::vector<Data>& oUsers);
    bool IsTableEmpty();
    
private:
    bool Create();
    bool _Load(const std::string& iDatabaseName);
    bool ExecuteRequest(const std::string& iQuery);
    template <typename T>
    bool ExecuteRequest(const std::string& iQuery, T& oResult);
    bool FindUser(const std::string& iQuery, Data& oUser);
    
private:
    std::unique_ptr<pqxx::connection> _db;
};

#endif /* Database_hpp */
