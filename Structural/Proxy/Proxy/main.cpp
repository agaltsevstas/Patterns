#include "CollectionClasses.h"
#include "Employee.h"

/*!
 * @brief Прокси (Proxy) -  имеет тот же интерфейс, что и реальный класс. Выступает прослойкой между пользователем и реальным классом. Он выполняет функции (контроль доступа, кеширование, изменение запроса и прочее), не изменяя при этом поведение класса.
 */

int main(int argc, const char * argv[])
{
    auto& objectFactory = CollectionClassesProxy<std::string, Employee, int, std::string>::Instance(); /// Фабрика объектов производных классов от Employee
    objectFactory.Add<Employee1>(typeid(Employee1).name());
    objectFactory.Add<Employee2>(typeid(Employee2).name());
    objectFactory.Add<Employee3>(typeid(Employee3).name());
    objectFactory.Add<Employee4>(typeid(Employee4).name());
    
    if (objectFactory.IsRegistered(typeid(Employee1).name()))
    {
        if (auto employee1 = objectFactory.Create(typeid(Employee1).name(), 1, "Employee1"))
            std::cout << employee1->GetName() << std::endl;
    }
    if (objectFactory.IsRegistered(typeid(Employee2).name()))
    {
        if (auto employee2 = objectFactory.Create(typeid(Employee2).name(), 2, "Employee2"))
            std::cout << employee2->GetName() << std::endl;
    }
    if (objectFactory.IsRegistered(typeid(Employee3).name()))
    {
        if (auto employee3 = objectFactory.Create(typeid(Employee3).name(), 3, "Employee3"))
            std::cout << employee3->GetName() << std::endl;
    }
    if (objectFactory.IsRegistered(typeid(Employee4).name()))
    {
        objectFactory.Remove(typeid(Employee4).name());
        if (auto employee4 = objectFactory.Create(typeid(Employee4).name(), 4, "Employee4"))
            std::cout << employee4->GetName() << std::endl;
        else
            std::cout << "Класс: " << "Employee4 НЕ существует" << std::endl;
    }
    return 0;
}

// Явно инстанцированная шаблонная функция/класс могут иметь копии .cpp файлах, что может тормозить сам процесс компиляции и сборки. Можно указать компилятору не инстанцировать шаблон в данной единице трансляции, для этого перед инстанцированием указать extern - это уменьшает время компиляции.
extern template class CollectionClassesProxy<std::string, Employee, int, std::string>;
extern template void CollectionClassesProxy<std::string, Employee, int, std::string>::Add<Employee1>(const std::string&);
extern template void CollectionClassesProxy<std::string, Employee, int, std::string>::Add<Employee2>(const std::string&);
extern template void CollectionClassesProxy<std::string, Employee, int, std::string>::Add<Employee3>(const std::string&);
extern template void CollectionClassesProxy<std::string, Employee, int, std::string>::Add<Employee4>(const std::string&);
