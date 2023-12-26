#include "Database.h"

#include <string_view>

#include <pqxx/pqxx>
#include <pqxx/notification>
#include <pqxx/transaction>
#include <pqxx/transactor>


#if 0
    #define DATABASE_HOSTNAME "localhost"    // Хост 127.0.0.1
    #define DATABASE_PORT      5432          // Порт
    #define DATABASE_POSTGRES "postgres"     // Название базы данных по-умолчанию
    #define DATABASE_USER     "agaltsevstas" // Имя пользователя
    #define DATABASE_PASSWORD ""             // Пароль
    #define DATABASE_NAME     "test"         // Название базы данных
    #define DATABASE_TABLE    "person"       // Название таблицы
#endif

constexpr std::string_view database_hostname = "localhost"; // Хост 127.0.0.1
constexpr unsigned database_port = 5432;                    // Порт
constexpr std::string_view database_postgres = "postgres";  // Название базы данных по-умолчанию
constexpr std::string_view database_user = "agaltsevstas";  // Имя пользователя
constexpr std::string_view database_password = "";          // Пароль
constexpr std::string_view database_name = "test";          // Название базы данных
constexpr std::string_view database_table = "person";       // Название таблицы

using Data = DatabaseFacade::Data;

Data::Data(unsigned iKey,
           const std::string& iSurname,
           const std::string& iName,
           const std::string& iPatronymic,
           const std::string& iSex,
           unsigned iAge)
{
    key = iKey;
    surname = iSurname;
    name = iName;
    patronymic = iPatronymic;
    sex = iSex;
    age = iAge;
}

Data::Data(const std::string& iSurname,
           const std::string& iName,
           const std::string& iPatronymic,
           const std::string& iSex,
           unsigned iAge)
{
    Data(0, iSurname, iName, iPatronymic, iSex, iAge);
}

Data::Data(const Data& other) noexcept
{
    *this = other;
}

Data::Data(Data&& other) noexcept
{
    *this = std::move(other);
}

Data& Data::operator=(Data&& other) noexcept
{
    key = std::exchange(other.key, 0);
    surname = std::move(other.surname);
    name = std::move(other.name);
    patronymic = std::move(other.patronymic);
    sex = std::move(other.sex);
    age = std::exchange(other.age, 0);
    return *this;
}

Data& Data::operator=(const Data& other) noexcept
{
    key = other.key;
    surname = other.surname;
    name = other.name;
    patronymic = other.patronymic;
    sex = other.sex;
    age = other.age;
    return *this;
}

DatabaseFacade::~DatabaseFacade() noexcept
{
    
}

DatabaseFacade::DatabaseFacade() noexcept
{
    
}

bool operator >> (pqxx::stream_from &iStream, Data& oObject)
{
    std::tuple<unsigned, std::string, std::string, std::string, std::string, unsigned> row;
    iStream >> row;
    if (!iStream)
        return false;
    
    oObject.key = std::get<0>(row);
    oObject.surname = std::get<1>(row);
    oObject.name = std::get<2>(row);
    oObject.patronymic = std::get<3>(row);
    oObject.sex = std::get<4>(row);
    oObject.age = std::get<5>(row);
    return true;
}

pqxx::stream_to& operator << (pqxx::stream_to& ioStream, const Data& iObject)
{
    ioStream << std::make_tuple
    (
         iObject.key,
         iObject.surname,
         iObject.name,
         iObject.patronymic,
         iObject.sex,
         iObject.age
     );
        
    return ioStream;
}

void operator >> (const pqxx::row& iRow, Data& oObject)
{
    oObject.key = iRow["id"].as<unsigned>();
    oObject.surname = iRow["surname"].as<std::string>();
    oObject.name = iRow["name"].as<std::string>();
    oObject.patronymic = iRow["patronymic"].as<std::string>();
    oObject.sex = iRow["sex"].as<std::string>();
    oObject.age = iRow["age"].as<unsigned>();
}

DatabaseFacade& DatabaseFacade::Instance()
{
    static DatabaseFacade instance;
    return instance;
}

bool DatabaseFacade::_Load(const std::string& iDatabaseName)
{
    const std::string query = "host=" + std::string(database_hostname) +
                              " port=" + std::to_string(database_port) +
                              " dbname=" + iDatabaseName +
                              " user=" + std::string(database_user) +
                              " password=" + std::string(database_password);
    try
    {
        _db = std::make_unique<pqxx::connection>(query); // Соединение с PostgresSQL
        std::cout << "База данных: " + iDatabaseName + " успешно загрузилась" << std::endl;
    }
    catch (pqxx::broken_connection &exception)
    {
        std::cerr << "Ошибка при подключении к базе данных " + iDatabaseName + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::sql_error& exception)
    {
        std::cerr << "Ошибка при подключении к базе данных" + iDatabaseName + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception &exception)
    {
        std::cerr << "Ошибка при подключении к базе данных" + iDatabaseName + ": " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "Ошибка при подключении к базе данных" + iDatabaseName << std::endl;
        return false;
    }

    return _db->is_open();
}

bool DatabaseFacade::Load()
{
    return _Load(database_postgres.data());
}

bool DatabaseFacade::Create()
{
    pqxx::nontransaction transaction(*_db.get());
    try
    {
        
        const std::string query = "CREATE DATABASE " + std::string(database_name) + " TEMPLATE=template0 ENCODING 'UTF-8' LC_COLLATE 'ru_RU.UTF-8' LC_CTYPE 'ru_RU.UTF-8'";
        pqxx::result res = transaction.exec(query);
        transaction.commit();
        std::cout << "База данных: " + std::string(database_name) + " успешно создалась" << std::endl;
        _db->close();
        if (_db.release())
            return Connection();
    }
    catch (const pqxx::sql_error& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании базы данных "  + std::string(database_name) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception &exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании базы данных "  + std::string(database_name) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    return true;
}

bool DatabaseFacade::Delete()
{
    pqxx::nontransaction transaction(*_db.get());
    try
    {
        const std::string query = "DROP DATABASE " + std::string(database_name);
        pqxx::result res = transaction.exec0(query.data());
        transaction.commit();
        std::cout << "База данных " + std::string(database_name) + " успешно удалилась" << std::endl;
        return true;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при удалении базы данных test: " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при удалении базы данных test: " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при удалении базы данных test: " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    return true;
}

bool DatabaseFacade::DeleteTable()
{
    pqxx::transaction<> transaction(*_db.get());
    pqxx::subtransaction subTransaction(transaction);
    try
    {
        const std::string query = "DROP TABLE " + std::string(database_table);
        subTransaction.exec0(query);
        subTransaction.commit();
        std::cout << "Таблица " + std::string(database_table) + " успешно удалилась" << std::endl;
        return true;
    }
    catch (const pqxx::undefined_table& exception)
    {
        subTransaction.abort();
        std::cerr << "Ошибка при удалении таблицы " + std::string(database_table) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        subTransaction.abort();
        std::cerr << "Ошибка при удалении таблицы " + std::string(database_table) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        subTransaction.abort();
        std::cerr << "Ошибка при удалении таблицы " + std::string(database_table) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        subTransaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    return true;
}

bool DatabaseFacade::ClearTable()
{
    pqxx::transaction<> transaction(*_db.get());
    try
    {
        const std::string query = "DELETE FROM " + std::string(database_table);
        transaction.exec(query);
        transaction.commit();
        std::cout << "Таблица " + std::string(database_table) + " успешно учистилась" << std::endl;
        return true;
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при очистки таблицы " + std::string(database_table) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при очистки таблицы " + std::string(database_table) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при очистки таблицы " + std::string(database_table) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    return true;
}

bool DatabaseFacade::CreateTable()
{
    pqxx::nontransaction transaction(*_db.get());
    try
    {
        const std::string query = "CREATE TABLE IF NOT EXISTS " + std::string(database_table) + "(id serial primary key, surname text NOT NULL, name text NOT NULL, patronymic text NOT NULL, sex text NOT NULL, age smallint NOT NULL)";
        transaction.exec(query);
        transaction.commit();
        std::cout << "Таблица " + std::string(database_table) + " успешно создалась" << std::endl;
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании таблицы " + std::string(database_table) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании таблицы " + std::string(database_table) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании таблицы " + std::string(database_table) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    return true;
}

bool DatabaseFacade::Connection()
{
    if (!_Load(std::string(database_name)))
        if (!Create())
            return false;
    
    if (!CreateTable())
        return true;
    
    std::cout << "Выполнено подключение к базе данных " << _db->dbname() << std::endl;
    return true;
}

void DatabaseFacade::Disconnect()
{
    std::cout << "Выполнено отключение к базе данных " << _db->dbname() << std::endl;
    _db->close();
    _db.reset();
}

template <typename T>
bool DatabaseFacade::ExecuteRequest(const std::string& iQuery, T& oResult)
{
    pqxx::transaction<> transaction(*_db.get());
    
    try
    {
        oResult = transaction.query_value<T>(iQuery);
        transaction.commit();
        std::cout << "Запрос: " + iQuery + ", успешно выполнился" << std::endl;
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    
    return true;
}

bool DatabaseFacade::ExecuteRequest(const std::string& iQuery)
{
    pqxx::transaction<> transaction(*_db.get());
    try
    {
        transaction.exec0(iQuery);
        transaction.commit();
        std::cout << "Запрос: " + iQuery + ", успешно выполнился" << std::endl;
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    return true;
}

bool DatabaseFacade::UpdateSurname(unsigned iKey, const std::string& iSurname)
{
    const std::string query = "UPDATE " + std::string(database_table) + " SET surname = '" + iSurname + "' WHERE id = " + std::to_string(iKey);
    return ExecuteRequest(query);
}

bool DatabaseFacade::UpdateName(unsigned iKey, const std::string& iName)
{
    const std::string query = "UPDATE " + std::string(database_table) + " SET name = '" + iName + "' WHERE id = " + std::to_string(iKey);
    return ExecuteRequest(query);
}

bool DatabaseFacade::UpdatePatronymic(unsigned iKey, const std::string& iPatronymic)
{
    const std::string query = "UPDATE " + std::string(database_table) + " SET patronymic = '" + iPatronymic + "' WHERE id = " + std::to_string(iKey);
    return ExecuteRequest(query);
}

bool DatabaseFacade::UpdateSex(unsigned iKey, const std::string& iSex)
{
    const std::string query = "UPDATE " + std::string(database_table) + " SET sex = '" + iSex +  + "' WHERE id = " + std::to_string(iKey);
    return ExecuteRequest(query);
}

bool DatabaseFacade::UpdateAge(unsigned iKey, unsigned iAge)
{
    const std::string query = "UPDATE " + std::string(database_table) + " SET age = " + std::to_string(iAge) +  + " WHERE id = " + std::to_string(iKey);
    return ExecuteRequest(query);
}

bool DatabaseFacade::UpdateUser(unsigned iKey, const Data& iUser)
{
    pqxx::transaction<> transaction(*_db.get());
    try
    {
        const std::string query = "UPDATE " + std::string(database_table) + " SET surname=$2, name=$3, patronymic=$4, sex=$5, age=$6 WHERE id = $1";
        _db->prepare("update", query);
        transaction.exec_prepared("update", iUser.key, iUser.surname, iUser.name, iUser.patronymic, iUser.sex.data(), iUser.age);
        transaction.commit();
        std::cout << "Пользователь успешно обвновлен" << std::endl;
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    return true;
}

bool DatabaseFacade::UpdateUser(unsigned iKey,
                                const std::string& iSurname,
                                const std::string& iName,
                                const std::string& iPatronymic,
                                const std::string& iSex,
                                unsigned iAge)
{
    auto user = Data(iKey, iSurname, iName, iPatronymic, iSex, iAge);
    return UpdateUser(iKey, user);
}

bool DatabaseFacade::AddUser(const Data& iUser)
{
    pqxx::transaction<> transaction(*_db.get());
    
    try
    {
        const std::string query = "insert into person (surname, name, patronymic, sex, age) values($1, $2, $3, $4, $5)";
        
        pqxx::params params;
        params.reserve(5);
        params.append(iUser.surname);
        params.append(iUser.name);
        params.append(iUser.patronymic);
        params.append(iUser.sex.data());
        params.append(iUser.age);
        
        transaction.exec_params(query, params);
        transaction.commit();
        std::cout << "Пользователь успешно добавлен" << std::endl;
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    return true;
}

bool DatabaseFacade::AddUser(const std::string& iSurname,
                             const std::string& iName,
                             const std::string& iPatronymic,
                             const std::string& iSex,
                             unsigned iAge)
{
    auto user = Data(iSurname, iName, iPatronymic, iSex, iAge);
    return AddUser(user);
}

bool DatabaseFacade::AddUsers(const std::vector<Data>& iUsers)
{
    unsigned key;
    const std::string query = "SELECT id FROM " + std::string(database_table) + " WHERE id = (SELECT max(id) FROM " + std::string(database_table) + ")";
    if (!ExecuteRequest<unsigned>(query, key))
        return false;
    
    pqxx::transaction<> transaction(*_db.get());
    try
    {
#if 1
        const std::string table("person");
        auto stream = pqxx::stream_to::raw_table(transaction, table);
        // 1 Способ
        {
//            auto stream = pqxx::stream_to::table(_db.get(), {table});
            for (auto& user : iUsers)
            {
                const_cast<unsigned&>(user.key) = ++key;
                stream << user;
            }
            stream.complete();
            transaction.commit();
        }
#endif
#if 0
        // 2 Способ
        {
            for (const auto& user : iUsers)
            {
                stream.write_values(user.key + (++key), user.surname, user.name, user.patronymic, user.sex.data(), user.age);
            }
        }
#endif
#if 0
        // 3 Способ
        {
            for (const auto& user : iUsers)
            {
                auto row = std::make_tuple
                (
                    const_cast<unsigned&>(user.key) = ++key,
                    user.surname,
                    user.name,
                    user.patronymic,
                    user.sex.data(),
                    user.age
                 );
                stream.write_row(row);
            }
        }
#endif
#if 0
        // 4 Способ
        {
            const std::string query = "insert into person (id, surname, name, patronymic, sex, age) values($1, $2, $3, $4, $5, $6)";
            pqxx::params params;
            params.append_multi(iUsers); // Добавление вектора
            transaction.exec_params(query, params);
            transaction.commit();
        }
#endif
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    return true;
}

bool DatabaseFacade::DeleteUser(unsigned iKey)
{
    const std::string query = "DELETE FROM " + std::string(database_table) + " WHERE id = " + std::to_string(iKey);
    return ExecuteRequest(query);
}

bool DatabaseFacade::DeleteUser(const std::string& iSurname,
                                const std::string& iName,
                                const std::string& iPatronymic)
{
    const std::string query = "DELETE FROM " + std::string(database_table) + " WHERE surname = '" + iSurname +
                                                                             "' AND name = '" + iName +
                                                                             "' AND patronymic = '" + iPatronymic + "'";
    return ExecuteRequest(query);
}

bool DatabaseFacade::FindUserID(const std::string& iSurname,
                                const std::string& iName,
                                const std::string& iPatronymic,
                                unsigned& oKey)
{
    const std::string query = "SELECT id FROM " +  std::string(database_table) + " WHERE surname = '" + iSurname +
                                                                                 "' AND name = '" + iName +
                                                                                 "' AND patronymic = '" + iPatronymic + "'";
    if (!ExecuteRequest(query, oKey))
        return false;
    
    return true;
}

bool DatabaseFacade::FindUser(const std::string& iQuery, Data& oUser)
{
    pqxx::transaction<> transaction(*_db.get());
    try
    {
        pqxx::row row = transaction.exec1(iQuery);
        if (row.empty())
            return false;
        
        transaction.commit();
        row >> oUser;
        std::cout << "Пользователь успешно найден" << std::endl;
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
    
    return true;
}

bool DatabaseFacade::FindUser(unsigned iKey, Data& oUser)
{
    const std::string query = "SELECT id FROM " + std::string(database_table) + " WHERE id = " + std::to_string(iKey);
    return FindUser(query, oUser);
}

bool DatabaseFacade::FindUser(const std::string& iSurname,
                              const std::string& iName,
                              const std::string& iPatronymic,
                              Data& oUser)
{
    const std::string query = "SELECT * FROM " + std::string(database_table) + " WHERE surname = '" + iSurname +
                                                                               "' AND name = '" + iName +
                                                                               "' AND patronymic = '" + iPatronymic + "'";
    return FindUser(query, oUser);
}

bool DatabaseFacade::FindUser(unsigned iKey,
                              std::string& oSurname,
                              std::string& oName,
                              std::string& oPatronymic,
                              std::string& oSex,
                              unsigned& oAge)
{
    const std::string query = "SELECT * FROM " + std::string(database_table) + " where id = " + std::to_string(iKey);
    Data user;
    if (!FindUser(query, user))
        return false;
    
    oSurname = std::move(user.surname);
    oName = std::move(user.name);
    oPatronymic = std::move(user.patronymic);
    oSex = std::move(user.sex);
    oAge = std::exchange(user.age, 0);
    
    return true;
}

bool DatabaseFacade::GetAll(std::vector<Data>& oUsers)
{
    pqxx::transaction<> transaction(*_db.get());
    
    try
    {
#if 1
        // 1 Способ
        {
            const std::string query = "SELECT * FROM " + std::string(database_table);
            pqxx::result result = transaction.exec(query);
            if (result.empty())
                throw query;

            for (const auto& row: result)
            {
                Data user;
                row >> user;
                oUsers.emplace_back(user);
                transaction.commit();
            }
        }
#endif
#if 0
        // 2 Способ
        {
            const std::string query = "SELECT * FROM " + std::string(database_table);
            auto stream = pqxx::stream_from(transaction, query);
            Data user;
            while (stream >> user)
                oUsers.emplace_back(user);
            stream.complete();
            transaction.commit();
        }
#endif
#if 0
        // 3 Способ
        {
            const std::string query = "SELECT id, surname, name, patronymic, sex, age FROM " + std::string(database_table);
            pqxx::result result = transaction.exec(query);
            if (result.empty())
                throw query;
            
            for (const auto& row: result)
            {
                Data user;
                row >> user;
                oUsers.emplace_back(user);
                transaction.commit();
            }
        }
#endif
#if 0
        // 4 Способ
        {
            const std::string query = "(SELECT id, surname::TEXT, name::TEXT, patronymic::TEXT, sex::TEXT, age FROM " + std::string(database_table) + ")";
            for (auto [id, surname, name, patronymic, sex, age] : transaction.stream<unsigned, std::string_view, std::string_view, std::string_view, std::string_view, unsigned>(query))
            {
                Data user;
                user.key = id;
                user.surname = surname;
                user.name = name;
                user.patronymic = patronymic;
                user.sex = sex;
                user.age = age;
                oUsers.emplace_back(user);
            }
        }
#endif
    }
    catch (const std::string &exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception << std::endl;
        return false;
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Неверный запрос: " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        transaction.abort();
        std::cerr << "Неизвестная ошибка!" << std::endl;
        return false;
    }
        
    return !oUsers.empty();
}

bool DatabaseFacade::IsTableEmpty()
{
    bool result;
    // Check exist table
    {
        const std::string query = std::string("SELECT EXISTS (SELECT * FROM INFORMATION_SCHEMA.TABLES") + " WHERE TABLE_NAME = '" + std::string(database_table) + "')";
        if (!ExecuteRequest(query, result))
            return false;
    }
    
    // Check table empty
    {
        const std::string query = "SELECT (SELECT count(*) FROM " + std::string(database_table) + ") = 0";
        if (!result || !ExecuteRequest<bool>(query, result))
            return false;
    }
    
    return result;
}
