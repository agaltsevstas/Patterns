#ifndef Database_hpp
#define Database_hpp

#include <iostream>

namespace pqxx
{
    class connection;
    class row;
    class stream_from;
    class stream_to;
}

class DatabaseFacade
{
    DatabaseFacade();
    ~DatabaseFacade();
    DatabaseFacade(const DatabaseFacade&) = delete;
    DatabaseFacade& operator=(const DatabaseFacade&) = delete;
    
public:
    struct Data
    {
        unsigned key;
        std::string surname;
        std::string name;
        std::string patronymic;
        std::array<char, 3> sex;
        unsigned age;
        
        friend pqxx::stream_to& operator << (pqxx::stream_to& ioStream, const Data& object);
        friend void operator >> (pqxx::stream_from &iStream, Data& oObject);
        friend void operator >> (const pqxx::row& iRow, Data& oObject);
    };
    
public:
    static DatabaseFacade& Instance();
    bool Load();
    bool Create();
    bool Connection();
    void Disconnect();
    bool IsOpen();
    bool Delete();
    bool DeleteTable();
    bool FindUserID(const std::string& iSurname,
                    const std::string& iName,
                    const std::string& iPatronymic,
                    unsigned& OKey);
    bool FindUser(unsigned iKey, Data& oData);
    bool FindUser(const std::string& iSurname,
                  const std::string& iName,
                  const std::string& iPatronymic,
                  Data& oData);
    bool FindUser(unsigned iKey,
                  std::string& oSurname,
                  std::string& oName,
                  std::string& oPatronymic,
                  std::array<char, 3>& oSex,
                  std::array<char, 3>& oAge);
    bool UpdateUser(unsigned iKey, const Data& iData);
    bool UpdateSurname(unsigned iKey, const std::string& iSurname);
    bool UpdateName(unsigned iKey, const std::string& iName);
    bool UpdatePatronymic(unsigned iKey, const std::string& iPatronymic);
    bool UpdateSex(unsigned iKey, const std::array<char, 3>& iSex);
    bool UpdateAge(unsigned iKey, const std::array<char, 3>& iAge);
    bool AddUsers(const std::vector<Data>& iUsers);
    bool AddUser(const Data& iUser);
    bool AddUser(unsigned iKey,
                 const std::string& iSurname,
                 const std::string& iName,
                 const std::string& iPatronymic,
                 const std::array<char, 3>& iSex,
                 const std::array<char, 3>& iAge);
    bool DeleteUser(unsigned iKey);
    bool DeleteUser(const std::string& iSurname,
                    const std::string& iName,
                    const std::string& iPatronymic);
    bool GetAll(std::vector<Data>& oUsers);
    bool IsEmpty();
    
private:
    bool _Load(const std::string& iDatabaseName);
    bool CreateTable();
    
private:
    std::unique_ptr<pqxx::connection> _db;
};

#endif /* Database_hpp */
