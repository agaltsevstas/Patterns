#include "Database.h"

#include <string>

#include <pqxx/pqxx>
#include <pqxx/notification>
#include <pqxx/transaction>
#include <pqxx/transactor>

#define DATABASE_HOSTNAME "localhost"    // Хост 127.0.0.1
#define DATABASE_PORT      5432          // Порт
#define DATABASE_POSTGRES "postgres"     // Название базы данных по-умолчанию
#define DATABASE_USER     "agaltsevstas" // Имя пользователя
#define DATABASE_PASSWORD ""             // Пароль
#define DATABASE_NAME     "test"         // Название базы данных
#define DATABASE_TABLE    "person"       // Название таблицы


using Data = DatabaseFacade::Data;

DatabaseFacade::~DatabaseFacade()
{
    
}

DatabaseFacade::DatabaseFacade()
{
    
}

void operator >> (pqxx::stream_from &iStream, Data& oObject)
{
    std::tuple<unsigned, std::string, std::string, std::string, std::string, unsigned> row;
    iStream >> row;
    if (!iStream)
        return;
    
    oObject.key = std::get<0>(row);
    oObject.surname = std::get<1>(row);
    oObject.name = std::get<2>(row);
    oObject.patronymic = std::get<3>(row);
    std::copy(std::get<3>(row).begin(), std::get<3>(row).end(), oObject.sex.data());
    oObject.age = std::get<5>(row);
}

pqxx::stream_to& operator << (pqxx::stream_to& ioStream, const Data& iObject)
{
    ioStream << std::make_tuple
    (
         iObject.key,
         iObject.surname,
         iObject.name,
         iObject.patronymic,
         iObject.sex.data(),
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
    std::copy(iRow["sex"].as<std::string>().begin(), iRow["sex"].as<std::string>().end(), oObject.sex.data());
    oObject.age = iRow["age"].as<unsigned>();
}

DatabaseFacade& DatabaseFacade::Instance()
{
    static DatabaseFacade instance;
    return instance;
}

bool DatabaseFacade::_Load(const std::string& iDatabaseName)
{
    std::cout << "Подключение к базе данных: " + iDatabaseName << std::endl;
    const std::string query = "host=" + std::string(DATABASE_HOSTNAME) +
                              " port=" + std::to_string(DATABASE_PORT) +
                              " dbname=" + iDatabaseName +
                              " user=" + DATABASE_USER +
                              " password=" + DATABASE_PASSWORD;
    try
    {
        _db = std::make_unique<pqxx::connection>(query); // Соединение с PostgresSQL
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
    catch(const std::exception &exception)
    {
        std::cerr << "Ошибка при подключении к базе данных" + iDatabaseName + ": " << exception.what() << std::endl;
        return false;
    }
    catch(...)
    {
        std::cerr << "Ошибка при подключении к базе данных" + iDatabaseName << std::endl;
        return false;
    }

    return _db->is_open();
}

bool DatabaseFacade::Load()
{
    return _Load(std::string(DATABASE_POSTGRES));
}

bool DatabaseFacade::Create()
{
    pqxx::nontransaction transaction(*_db.get());
    try
    {
        std::cerr << "Попытка создать базу данных: " + std::string(DATABASE_NAME) << std::endl;
        const std::string query = "CREATE DATABASE " + std::string(DATABASE_NAME) + " TEMPLATE=template0 ENCODING 'UTF-8' LC_COLLATE 'ru_RU.UTF-8' LC_CTYPE 'ru_RU.UTF-8'";
        pqxx::result res = transaction.exec(query);
        transaction.commit();
        _db->close();
        if (_db.release())
            return Connection();
    }
    catch (const pqxx::sql_error& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании базы данных "  + std::string(DATABASE_NAME) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception &exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании базы данных "  + std::string(DATABASE_NAME) + ": " << exception.what() << std::endl;
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
    pqxx::transaction<> transaction(*_db.get(), "delete: " + std::string(DATABASE_NAME));
    pqxx::subtransaction subTransaction(transaction);
    try
    {
        pqxx::result res = subTransaction.exec0("DROP TABLE " + std::string(DATABASE_NAME));
        if (res.empty())
            return false;
        
        subTransaction.commit();
        return true;
    }
    catch (const pqxx::data_exception& exception)
    {
        subTransaction.abort();
        std::cerr << "Ошибка при удалении базы данных test: " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::undefined_table& exception)
    {
        subTransaction.abort();
        std::cerr << "Ошибка при удалении базы данных test: " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        subTransaction.abort();
        std::cerr << "Ошибка при удалении базы данных test: " << exception.what() << std::endl;
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

bool DatabaseFacade::DeleteTable()
{
    pqxx::transaction<> transaction(*_db.get(), "delete: " + std::string(DATABASE_TABLE));
    pqxx::subtransaction subTransaction(transaction);
    try
    {
        pqxx::result res = subTransaction.exec0("DROP TABLE " + std::string(DATABASE_TABLE));
        if (res.empty())
            return false;
        
        subTransaction.commit();
        return true;
    }
    catch (const pqxx::undefined_table& exception)
    {
        subTransaction.abort();
        std::cerr << "Ошибка при удалении таблицы " + std::string(DATABASE_TABLE) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        subTransaction.abort();
        std::cerr << "Ошибка при удалении таблицы " + std::string(DATABASE_TABLE) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const std::exception& exception)
    {
        subTransaction.abort();
        std::cerr << "Ошибка при удалении таблицы " + std::string(DATABASE_TABLE) + ": " << exception.what() << std::endl;
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

bool DatabaseFacade::CreateTable()
{
    pqxx::nontransaction transaction(*_db.get());
    try
    {
        const std::string query = "CREATE TABLE IF NOT EXISTS " + std::string(DATABASE_TABLE) + "(id serial primary key, surname text NOT NULL, name text NOT NULL, patronymic text NOT NULL, sex varchar(3) NOT NULL, age smallint[3] NOT NULL)";
        transaction.exec(query);
        transaction.commit();
    }
    catch (const pqxx::undefined_table& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании таблицы " + std::string(DATABASE_TABLE) + ": " << exception.what() << std::endl;
        return false;
    }
    catch (const pqxx::data_exception& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании таблицы " + std::string(DATABASE_TABLE) + ": " << exception.what() << std::endl;
        return false;
    }
    catch(const std::exception& exception)
    {
        transaction.abort();
        std::cerr << "Ошибка при создании таблицы " + std::string(DATABASE_TABLE) + ": " << exception.what() << std::endl;
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
    if (!_Load(std::string(DATABASE_NAME)))
        if (!Create())
            return false;
    
    if (!CreateTable())
        return true;
    
    std::cout << "Выполнено подключение к базе данных " << _db->dbname() << std::endl;
    return true;
}

void DatabaseFacade::Disconnect()
{
    _db->close();
    _db.reset();
}

bool DatabaseFacade::IsOpen()
{
    return _db && _db->is_open();
}

bool DatabaseFacade::FindUserID(const std::string& iSurname,
                const std::string& iName,
                const std::string& iPatronymic,
                unsigned& OKey)
{
    return true;
}

bool DatabaseFacade::FindUser(unsigned iKey,
              Data& oData)
{
    return true;
}

bool DatabaseFacade::FindUser(const std::string& iSurname,
              const std::string& iName,
              const std::string& iPatronymic,
              Data& oData)
{
    return true;
}

bool DatabaseFacade::FindUser(unsigned iKey,
                              std::string& oSurname,
                              std::string& oName,
                              std::string& oPatronymic,
                              std::array<char, 3>& oSex,
                              std::array<char, 3>& oAge)
{
    return true;
}

bool DatabaseFacade::UpdateUser(unsigned iKey, const Data& iData)
{
    return true;
}

bool DatabaseFacade::UpdateSurname(unsigned iKey, const std::string& iSurname)
{
    return true;
}

bool DatabaseFacade::UpdateName(unsigned iKey, const std::string& iName)
{
    return true;
}

bool DatabaseFacade::UpdatePatronymic(unsigned iKey, const std::string& iPatronymic)
{
    return true;
}

bool DatabaseFacade::UpdateSex(unsigned iKey, const std::array<char, 3>& iSex)
{
    return true;
}

bool DatabaseFacade::UpdateAge(unsigned iKey, const std::array<char, 3>& iAge)
{
    return true;
}

bool DatabaseFacade::AddUsers(const std::vector<Data>& iUsers)
{
    pqxx::transaction<> transaction(*_db.get());
    try
    {
#if 1
        const std::string table("employee");
        auto stream = pqxx::stream_to::raw_table(transaction, table);
        // 1 Способ
        {
//            auto stream = pqxx::stream_to::table(_db.get(), {table});
            for (const auto& user : iUsers)
                stream << user;
            stream.complete();
            transaction.commit();
        }
#endif
#if 0
        // 2 Способ
        {
            for (const auto& user : iUsers)
            {
//                auto row = std::make_tuple
//                (
//                    user.key,
//                    user.surname,
//                    user.name,
//                    user.patronymic,
//                    user.sex.data(),
//                    user.age
//                 );
//                stream.write_row(row);
                stream.write_values(user.key, user.surname, user.name, user.patronymic, user.sex.data(), user.age);
            }
        }
#endif
#if 0
        // 3 Способ
        {
//            const std::string query = "insert into person (id, position, surname, name, patronymic, sex, age) values($1, $2, $3, $4, $5, $6)";
//            pqxx::params params;
//            params.append_multi(iUsers); // Добавление вектора
//            transaction.exec_params(query, params);
//            transaction.commit();
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
    catch(const std::exception& exception)
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

bool DatabaseFacade::AddUser(const Data& iUser)
{
    pqxx::transaction<> transaction(*_db.get());
    
    try
    {
        const std::string query = "insert into person (id, position, surname, name, patronymic, sex, age) values($1, $2, $3, $4, $5, $6)";
        
        pqxx::params params;
        params.reserve(6);
        params.append(iUser.key);
        params.append(iUser.surname);
        params.append(iUser.name);
        params.append(iUser.patronymic);
        params.append(iUser.sex.data());
        params.append(iUser.age);
        
        transaction.exec_params(query, params);
        transaction.commit();
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
    catch(const std::exception& exception)
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

bool DatabaseFacade::AddUser(unsigned iKey,
                             const std::string& iSurname,
                             const std::string& iName,
                             const std::string& iPatronymic,
                             const std::array<char, 3>& iSex,
                             const std::array<char, 3>& iAge)
{
    return true;
}

bool DatabaseFacade::DeleteUser(unsigned iKey)
{
    return true;
}

bool DatabaseFacade::DeleteUser(const std::string& iSurname,
                                const std::string& iName,
                                const std::string& iPatronymic)
{
    return true;
}

bool DatabaseFacade::GetAll(std::vector<Data>& oUsers)
{
    return true;
}

bool Add()
{
    return false;
}

bool DatabaseFacade::IsEmpty()
{
    return false;
}
