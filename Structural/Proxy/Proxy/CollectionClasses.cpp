#include "CollectionClasses.h"
#include "Employee.h"

#include <format>
#include <map>
#include <string>


/*!
 * @brief Фабрика объектов
 * Требуется для создания объектов классов
 */
template<class ID, class Base, class ...Args>
class ObjectFactory final
{
    ObjectFactory() {};
    ~ObjectFactory() {};
    ObjectFactory(const ObjectFactory&) = delete;
    ObjectFactory& operator = (const ObjectFactory&) = delete;

    using ClassSharedPtr = std::shared_ptr<Base>;
    typedef ClassSharedPtr (*TBase)(const Args&...);
    using FactoryMap = std::map<ID, TBase>;

public:

    /*!
     * @brief Инициализация фабрики объектов
     * ID - Идентификатор  класса
     * Base - Базовый класс
     * iArgs - Аргументы базового класса
     */
    static ObjectFactory<ID, Base, Args...>& Instance()
    {
        static ObjectFactory<ID, Base, Args...> instance;
        return instance;
    }

    /*!
     * @brief Добавление класса в карту классов
     * Derived - Класс, который фабрике необходимо зарегистрировать
     * @param iID - Идентификатор  класса
     */
    template<class Derived>
    void Add(const ID& iID) noexcept
    {
        static_assert(std::is_base_of<Base, Derived>::value, "Factory::registerType doesn't accept this type because doesn't derive from base class"); // Проверка Derived на принадлежность к Base
        _classes[iID] = &Register<Derived>; // Сохраняем адрес метода Register
    }

    /*!
     * @brief Удаление класса
     * @param iID - Идентификатор  класса
     */
    void Remove(const ID& iID) noexcept
    {
        typename FactoryMap::iterator it = _classes.find(iID);
        if (it != _classes.end())
            _classes.erase(it);
    }

    /*!
     * @brief Проверка на наличие класса по Id
     * @param iID - Идентификатор  зарегистрированного класса
     * @return true - зарегистрированный класс найден, иначе - false
     */
    bool IsRegistered(const ID& iID) const noexcept
    {
        return _classes.find(iID) != _classes.end();
    }

    /*!
     * @brief Создание объекта зарегистрированного класса
     * @param iID - Идентификатор зарегистрированного класса
     * @param iArgs - Аргументы зарегистрированного класса
     * @return Созданный объект зарегистрированного класса
     */
    ClassSharedPtr Create(const ID& iID, const Args&... iArgs) const
    {
        if (auto it = _classes.find(iID); it != _classes.end())
        {
            auto& [_id, _class] = *it;
            return _class(iArgs...); // Вызывается метод Register
        }

        //throw std::range_error(std::format("key [{}] is not registered", iID)); // XCode не хочет подхватывать
        return nullptr;
    }

private:
    /*!
     * @brief Создание объекта  класса
     * Derived - Класс, который фабрике необходимо зарегистрировать
     * @return Созданный объект класса
     */
    template<class Derived>
    static ClassSharedPtr Register(const Args&... iArgs)
    {
        return std::make_shared<Derived>(iArgs...);
//        return std::make_shared<Derived>(std::forward<Args>(iArgs)...);
    }

private:
    FactoryMap _classes; // Хранится id и адрес метода Register с аргументами
};


template<typename ID, class Base, typename ...Args>
CollectionClassesProxy<ID, Base, Args...>& CollectionClassesProxy<ID, Base, Args...>::Instance()
{
    ObjectFactory<ID, Base, Args...>::Instance();
    static CollectionClassesProxy<ID, Base, Args...> instance;
    return instance;
}

template<typename ID, class Base, typename ...Args>
template<class Derived>
void CollectionClassesProxy<ID, Base, Args...>::Add(const ID& iID)
{
    ObjectFactory<ID, Base, Args...>::Instance().template Add<Derived>(iID);
    //ObjectFactory<ID, Base, Args...>::Instance().Add<Derived>(iID); // XCode не хочет подхватывать
}

template<typename ID, class Base, typename ...Args>
void CollectionClassesProxy<ID, Base, Args...>::Remove(const ID& iID) noexcept
{
    return ObjectFactory<ID, Base, Args...>::Instance().Remove(iID);
}

template<typename ID, class Base, typename ...Args>
bool CollectionClassesProxy<ID, Base, Args...>::IsRegistered(const ID& iID) const noexcept
{
    return ObjectFactory<ID, Base, Args...>::Instance().IsRegistered(iID);
}

template <typename ID, class Base, typename ...Args>
std::shared_ptr<Base> CollectionClassesProxy<ID, Base, Args...>::Create(const ID& iID, const Args&... iArgs) const
{
    return ObjectFactory<ID, Base, Args...>::Instance().Create(iID, iArgs...);
}

/*
 Явное инстанцирование (explicit instantiatian) с помощью резервированного слова template (обычно в .cpp файлах). Каждая единица трансляции транслируется отдельно, а потом всё линкуется. Инстанцирование шаблонов (подстановка) происходит до линковки. Компилятор при генерировании кода должен видеть определение шаблона (а не только объявление), так и определения типов, которые подставляются в шаблон. Поэтому если класс/функция инициализированы в .h файле, а реализация в .cpp файле, то при одновременной комплияции двух .cpp файлов компилятор не запомнит подставляемый тип в одном .cpp файла, где вызывается шаблон, во время компиляции другого .cpp файла, где находится реализация метода/класса. В .cpp файле, где реализация метода/класса нужно сделать явное инстанцирование.
 Плюсы:
   - Сокрытие явной реализации класса/метода в .cpp файле
   - Явное создание экземпляров позволяет сократить время компиляции
   - Предотвращение переопределение объекта (экономит время и размер)
 Минусы:
 - Если не создать явное инстанцирование всех типов в .cpp файле, то будет ошибка: undefined reference.
 */

// std::string
template class CollectionClassesProxy<std::string, Employee, int, std::string>;
template void CollectionClassesProxy<std::string, Employee, int, std::string>::Add<Employee1>(const std::string&);
template void CollectionClassesProxy<std::string, Employee, int, std::string>::Add<Employee2>(const std::string&);
template void CollectionClassesProxy<std::string, Employee, int, std::string>::Add<Employee3>(const std::string&);
template void CollectionClassesProxy<std::string, Employee, int, std::string>::Add<Employee4>(const std::string&);


// int
//template class CollectionClassesProxy<int, Employee, int, std::string>;
//template void CollectionClassesProxy<int, Employee, int, std::string>::Add<Employee1>(const std::string&);
//template void CollectionClassesProxy<int, Employee, int, std::string>::Add<Employee2>(const std::string&);
//template void CollectionClassesProxy<int, Employee, int, std::string>::Add<Employee3>(const std::string&);
//template void CollectionClassesProxy<int, Employee, int, std::string>::Add<Employee4>(const std::string&);
