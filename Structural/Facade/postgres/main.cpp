#include "Database.h"

#include <iostream>
#include <ranges>

/*!
 * @brief Фасад (Fasade) - позволяющий скрыть сложность системы путём сведения всех возможных внешних вызовов к одному объекту, делегирующему их соответствующим объектам системы
 * унифицированный интерфейс для набора интерфейсов в подсистеме. Фасад определяет интерфейс более высокого уровня, который упрощает использование подсистемы. Это можно использовать для упрощения ряда сложных взаимодействий с объектами в едином интерфейсе.
 * предоставить более простой интерфейс для всей подсистемы, состоящей из множества объектов
 */

namespace utils
{
    inline void tolower(std::string &iSource)
    {
        std::transform(iSource.begin(), iSource.end(), iSource.begin(), ::tolower);
    }

    inline unsigned constexpr Hash(const char *iSource)
    {
        return *iSource ? static_cast<unsigned int>(*iSource) + 33 * Hash(iSource + 1) : 5381;
    }
}

auto& db = DatabaseFacade::Instance();

namespace database
{
    bool AddUser()
    {
        while (true)
        {
            std::cout << "user - добавить пользователя, users - добавить пользователей, return/назад - вернуться: " << std::endl;
            std::string input;
            std::cin >> input;
            utils::tolower(input);
            switch (utils::Hash(input.c_str()))
            {
                case utils::Hash("user") :
                {
                    std::cout << "Введите фамилию: " <<  std::endl;
                    std::string surname;
                    std::cin >> surname;
                    std::cout << "Введите имя: " <<  std::endl;
                    std::string name;
                    std::cin >> name;
                    std::cout << "Введите отчество: " <<  std::endl;
                    std::string patronymic;
                    std::cin >> patronymic;
                    std::cout << "Введите пол: " <<  std::endl;
                    std::string sex;
                    std::cin >> sex;
                    std::cout << "Введите возраст: " <<  std::endl;
                    unsigned age;
                    std::cin >> age;
                    return db.AddUser(surname, name, patronymic, sex, age);
                }
                case utils::Hash("users") :
                {
                    std::cout << "Кол-во пользователей: " <<  std::endl;
                    unsigned count;
                    std::cin >> count;
                    std::vector<DatabaseFacade::Data> users;
                    for (const auto i : std::views::iota(0u, count))
                    {
                        std::cout << "Введите данные для " + std::to_string(i + 1) + " пользователя" << std::endl;
                        std::cout << "Введите фамилию: " <<  std::endl;
                        std::string surname;
                        std::cin >> surname;
                        std::cout << "Введите имя: " <<  std::endl;
                        std::string name;
                        std::cin >> name;
                        std::cout << "Введите отчество: " <<  std::endl;
                        std::string patronymic;
                        std::cin >> patronymic;
                        std::cout << "Введите пол: " <<  std::endl;
                        std::string sex;
                        std::cin >> sex;
                        std::cout << "Введите возраст: " <<  std::endl;
                        unsigned age;
                        std::cin >> age;
                        users.emplace_back(i, surname, name, patronymic, sex, age);
                    }
                    
                    return db.AddUsers(users);
                }
                case utils::Hash("return") :
                case utils::Hash("назад") :
                    return false;
                default:
                    throw input;
            }
        }
    }

    bool DeleteUser()
    {
        while (true)
        {
            std::cout << "id - удалить пользователя по id, "
            "anthroponym - удалить пользователя по фамилии, имени, отчеству, "
            "return/назад - вернуться: " << std::endl;
            std::string input;
            std::cin >> input;
            utils::tolower(input);
            switch (utils::Hash(input.c_str()))
            {
                case utils::Hash("id") :
                {
                    std::cout << "Введите id: " <<  std::endl;
                    unsigned key;
                    std::cin >> key;
                    return db.DeleteUser(key);
                }
                    
                case utils::Hash("anthroponym") :
                {
                    std::cout << "Введите фамилию: " <<  std::endl;
                    std::string surname;
                    std::cin >> surname;
                    std::cout << "Введите имя: " <<  std::endl;
                    std::string name;
                    std::cin >> name;
                    std::cout << "Введите отчество: " <<  std::endl;
                    std::string patronymic;
                    std::cin >> patronymic;
                    
                    return db.DeleteUser(surname, name, patronymic);
                }
                case utils::Hash("return") :
                case utils::Hash("назад") :
                    return false;
                default:
                    throw input;
            }
        }
    }

    bool UpdateUser()
    {
        while (true)
        {
            std::cout << "surname - обновить фамилию, "
            "name - обновить имя, "
            "patronymic - обновить отчество, "
            "sex - обновить пол, "
            "age - обновить возраст, "
            "all - обновить все данные, "
            "return/назад - вернуться: " << std::endl;
            std::string input;
            std::cin >> input;
            utils::tolower(input);
            switch (utils::Hash(input.c_str()))
            {
                case utils::Hash("surname") :
                {
                    std::cout << "Введите id: " <<  std::endl;
                    unsigned key;
                    std::cin >> key;
                    std::cout << "Введите фамилию: " <<  std::endl;
                    std::string surname;
                    std::cin >> surname;
                    return db.UpdateSurname(key, surname);
                }
                case utils::Hash("name") :
                {
                    std::cout << "Введите id: " <<  std::endl;
                    unsigned key;
                    std::cin >> key;
                    std::cout << "Введите имя: " <<  std::endl;
                    std::string name;
                    std::cin >> name;
                    return db.UpdateName(key, name);
                }
                case utils::Hash("patronymic") :
                {
                    std::cout << "Введите id: " <<  std::endl;
                    unsigned key;
                    std::cin >> key;
                    std::cout << "Введите отчество: " <<  std::endl;
                    std::string patronymic;
                    std::cin >> patronymic;
                    return db.UpdateName(key, patronymic);
                }
                case utils::Hash("sex") :
                {
                    std::cout << "Введите id: " <<  std::endl;
                    unsigned key;
                    std::cin >> key;
                    std::cout << "Введите пол: " <<  std::endl;
                    std::string sex;
                    std::cin >> sex;
                    return db.UpdateName(key, sex);
                }
                case utils::Hash("age") :
                {
                    std::cout << "Введите id: " <<  std::endl;
                    unsigned key;
                    std::cin >> key;
                    std::cout << "Введите возраст: " <<  std::endl;
                    unsigned age;
                    std::cin >> age;
                    return db.UpdateAge(key, age);
                }
                case utils::Hash("all") :
                {
                    std::cout << "Введите id: " <<  std::endl;
                    unsigned key;
                    std::cin >> key;
                    std::cout << "Введите фамилию: " <<  std::endl;
                    std::string surname;
                    std::cin >> surname;
                    std::cout << "Введите имя: " <<  std::endl;
                    std::string name;
                    std::cin >> name;
                    std::cout << "Введите отчество: " <<  std::endl;
                    std::string patronymic;
                    std::cin >> patronymic;
                    std::cout << "Введите пол: " <<  std::endl;
                    std::string sex;
                    std::cin >> sex;
                    std::cout << "Введите возраст: " <<  std::endl;
                    unsigned age;
                    std::cin >> age;
                    return db.UpdateUser(key, surname, name, patronymic, sex, age);
                }
                case utils::Hash("return") :
                case utils::Hash("назад") :
                    return false;
                default:
                    throw input;
            }
        }
    }

    bool FindUser()
    {
        while (true)
        {
            std::cout << "id - найти id пользователя по фамилии, имени и отчеству, "
                         "all1 - найти все данные по id, "
                         "all2 - найти все данные пользователя по фамилии, имени и отчеству, "
                         "return/назад - вернуться: " << std::endl;
            std::string input;
            std::cin >> input;
            utils::tolower(input);
            switch (utils::Hash(input.c_str()))
            {
                case utils::Hash("id") :
                {
                    std::cout << "Введите фамилию: " <<  std::endl;
                    std::string surname;
                    std::cin >> surname;
                    std::cout << "Введите имя: " <<  std::endl;
                    std::string name;
                    std::cin >> name;
                    std::cout << "Введите отчество: " <<  std::endl;
                    std::string patronymic;
                    std::cin >> patronymic;
                    
                    unsigned key;
                    auto result = db.FindUserID(surname, name, patronymic, key);
                    if (result)
                        std::cout << "id: " << key << std::endl;
                    return result;
                }
                case utils::Hash("all1") :
                {
                    std::cout << "Введите id: " <<  std::endl;
                    unsigned key;
                    std::cin >> key;
                    std::string surname, name, patronymic, sex;
                    unsigned age;
                    auto result = db.FindUser(key, surname, name, patronymic, sex, age);
                    if (result)
                    {
                        std::cout << "id: " << key
                                  << ", фамилия: " << surname
                                  << ", имя: " << name
                                  << ", отчество: " << patronymic
                                  << ", пол: " << sex
                                  << ", возраст: " << age
                                  << std::endl;
                    }
                    return result;
                }
                case utils::Hash("all2") :
                {
                    std::cout << "Введите фамилию: " <<  std::endl;
                    std::string surname;
                    std::cin >> surname;
                    std::cout << "Введите имя: " <<  std::endl;
                    std::string name;
                    std::cin >> name;
                    std::cout << "Введите отчество: " <<  std::endl;
                    std::string patronymic;
                    std::cin >> patronymic;
                    
                    DatabaseFacade::Data user;
                    auto result = db.FindUser(surname, name, patronymic, user);
                    if (result)
                    {
                        std::cout << "id: " << user.key
                                  << ", фамилия: " << user.surname
                                  << ", имя: " << user.name
                                  << ", отчество: " << user.patronymic
                                  << ", пол: " << user.sex
                                  << ", возраст: " << user.age
                                  << std::endl;
                    }
                    
                    return result;
                }
                case utils::Hash("return") :
                case utils::Hash("назад") :
                    return false;
                default:
                    throw input;
            }
        }
    }
}


int main(int argc, const char * argv[])
{
    auto& db = DatabaseFacade::Instance();
    
    if (!db.Load())
        return EXIT_FAILURE;
    
    if (!db.Connection())
        return EXIT_FAILURE;
    
    while (true)
    {
        try
        {
            std::cout << "Введите номер операции ("
            "deletedb - удалить базу данных, "
            "createtable - создать таблицу, "
            "deletetable - удалить таблицу, "
            "cleartable - очистить таблицу, "
            "checktableempty - проверить таблицу на пустоту, "
            "adduser - добавить пользователя(ей), "
            "deleteuser - добавить пользователя(ей), "
            "updateuser - обновить данные пользователя, "
            "finduser - найти пользователя, "
            "getall - получить все пользователей, "
            "esc/выход - выход из приложения): " << std::endl;
            std::string input;
            std::cin >> input;
            utils::tolower(input);
            switch (utils::Hash(input.c_str()))
            {
                case utils::Hash("deletedb") :
                {
                    if (db.Load())
                        db.Delete();
                    break;
                }
                case utils::Hash("createtable") :
                {
                    db.CreateTable();
                    break;
                }
                case utils::Hash("deletetable") :
                {
                    db.DeleteTable();
                    break;
                }
                case utils::Hash("cleartable") :
                {
                    db.ClearTable();
                    break;
                }
                case utils::Hash("checktableempty") :
                {
                    std::cout << (db.IsTableEmpty() ? "Таблица пуста" : "Таблица не пуста") << std::endl;
                    break;
                }
                case utils::Hash("adduser") :
                {
                    database::AddUser();
                    break;
                }
                case utils::Hash("deleteuser") :
                {
                    database::DeleteUser();
                    break;
                }
                case utils::Hash("updateuser") :
                {
                    database::UpdateUser();
                    break;
                }
                case utils::Hash("finduser") :
                {
                    database::FindUser();
                    break;
                }
                case utils::Hash("getall") :
                {
                    std::vector<DatabaseFacade::Data> users;
                    db.GetAll(users);
                    for (const auto& user: users)
                    {
                        std::cout << "id: " << user.key
                                  << ", фамилия: " << user.surname
                                  << ", имя: " << user.name
                                  << ", отчество: " << user.patronymic
                                  << ", пол: " << user.sex
                                  << ", возраст: " << user.age
                                  << std::endl;
                    }
                    break;
                }
                case utils::Hash("esc") :
                case utils::Hash("выход") :
                    db.Disconnect();
                    std::cout << "Вы вышли из программы" << std::endl;
                    exit(0);
                default:
                    throw input;
            }
        }
        catch (const std::string &exception)
        {
            std::cerr << "Вы ввели >> " << exception << " - неверная команда!" << std::endl;
        }
        catch (...)
        {
            std::cerr << "Неизвестная ошибка!" << std::endl;
        }
    }
    
    return EXIT_SUCCESS;
}
