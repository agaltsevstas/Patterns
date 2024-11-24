#ifndef ObjectFactory_h
#define ObjectFactory_h

#include <format>
#include <functional>
#include <iostream>
#include <map>


/*!
 * @brief Фабрика объектов
 * Требуется для создания объектов классов
 */
template<typename ID, class Base> class
ObjectFactory final
{
    ObjectFactory() {};
    ~ObjectFactory() {};
    ObjectFactory(const ObjectFactory&) = delete;
    ObjectFactory& operator = (const ObjectFactory&) = delete;
    
    using ClassUniquePtr = std::unique_ptr<Base>;
    using ClassSharedPtr = std::shared_ptr<Base>;
    using FactoryMap = std::map<ID, ClassUniquePtr>;

public:
    
    /*!
     * @brief Инициализация фабрики объектов
     * ID - Идентификатор  класса
     * Base - Базовый класс
     */
    static ObjectFactory<ID, Base>& Instance()
    {
        static ObjectFactory<ID, Base> instance;
        return instance;
    }

    /*!
     * @brief Добавление класса в карту классов
     * Derived - Класс, который фабрике необходимо зарегистрировать
     * @param iID - Идентификатор  класса
     * @param iArgs - Аргументы класса
     */
    template<class Derived, typename... Args>
    void Add(ID iID, const Args&... iArgs)
    {
        static_assert(std::is_base_of<Base, Derived>::value, "Factory::registerType doesn't accept this type because doesn't derive from base class"); // Проверка Derived на принадлежность к Base
        _classes[iID] = Register<Derived>(iArgs...);
//        _classes[iID] = std::make_unique<Derived>(iArgs...);
    }
    
    /*!
     * @brief Удаление класса
     * @param iID - Идентификатор  класса
     */
    void Remove(ID iID)
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
    bool IsRegistered(ID iID) const
    {
        return _classes.find(iID) != _classes.end();
    }

    /*!
     * @brief Получение объекта зарегистрированного класса
     * @param iID - Идентификатор зарегистрированного класса
     * @return Найденный объект зарегистрированного класса
     */
    Base* Get(ID iID)
    {
        return IsRegistered(iID) ? _classes[iID].get() : nullptr;
    }
    
    /*!
     * @brief Создание объекта зарегистрированного класса
     * Derived - Зарегистированный класс, объект которого нужно создать
     * @param iID - Идентификатор зарегистрированного класса
     * @param iArgs - Аргументы зарегистрированного класса
     * @return Созданный объект зарегистрированного класса
     */
    template <class Derived, typename... Args>
    std::shared_ptr<Derived> Create(ID iID, Args&&... iArgs) const
    {
        static_assert(std::is_base_of<Base, Derived>::value, "Factory::registerType doesn't accept this type because doesn't derive from base class"); // Проверка Derived на принадлежность к Base
        if (auto it = _classes.find(iID); it != _classes.end())
        {
//            auto& [_id, _class] = *it;
            return std::make_shared<Derived>(std::forward<Args>(iArgs)...);
        }
        
        throw std::range_error(std::format("key [{}] is not registered", iID));
        return nullptr;
    }
    
private:
    /*!
     * @brief Создание объекта  класса
     * Derived - Класс, который фабрике необходимо зарегистрировать
     * @return Созданный объект класса
     */
    template<class Derived, typename... Args>
    ClassUniquePtr Register(Args&&... iArgs)
    {
        return std::make_unique<Derived>(std::forward<Args>(iArgs)...);
    }
    
private:
    FactoryMap _classes; // Карта классов
};


/*!
 * @brief Фабрика Классов
 * Требуется для создания объектов классов
 */
template<class ID, class Base, typename ...Args>
class ClassFactory final
{
    ClassFactory() {};
    ~ClassFactory() {};
    ClassFactory(const ClassFactory&) = delete;
    ClassFactory& operator =(const ClassFactory&) = delete;

    using ClassUniquePtr = std::unique_ptr<Base>;
    using ClassSharedPtr = std::shared_ptr<Base>;
    using TBase = std::function<ClassSharedPtr(Args&&...)>;
//    using TBase = ClassSharedPtr (*)(Args&&...); // Function pointer
//    typedef ClassSharedPtr(*TBase)(Args&&...); // Function pointer
    using FactoryMap = std::map<ID, TBase>;

public:

    /*!
     * @brief Инициализация фабрики объектов
     * ID - Идентификатор  класса
     * Base - Базовый класс
     * iArgs - Аргументы базового класса
     */
    static ClassFactory<ID, Base, Args...>& Instance()
    {
        static ClassFactory<ID, Base, Args...> instance;
        return instance;
    }

    /*!
     * @brief Добавление класса в карту классов
     * Derived - Класс, который фабрике необходимо зарегистрировать
     * @param iID - Идентификатор  класса
     */
    template<class Derived>
    void Add(const ID& iID)
    {
        static_assert(std::is_base_of<Base, Derived>::value, "Factory::registerType doesn't accept this type because doesn't derive from base class"); // Проверка Derived на принадлежность к Base
        _classes[iID] = &Register<Derived>; // Сохраняем адрес метода Register
    }
    
    /*!
     * @brief Удаление класса
     * @param iID - Идентификатор  класса
     */
    void Remove(ID iID)
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
    bool IsRegistered(ID iID) const
    {
        return _classes.find(iID) != _classes.end();
    }

    /*!
     * @brief Создание объекта зарегистрированного класса
     * @param iID - Идентификатор зарегистрированного класса
     * @param iArgs - Аргументы зарегистрированного класса
     * @return Созданный объект зарегистрированного класса
     */
    std::shared_ptr<Base> Create(ID iID, Args&&... iArgs) const
    {
        if (auto it = _classes.find(iID); it != _classes.end())
        {
            auto& [_id, _class] = *it;
            return _class(std::forward<Args>(iArgs)...); // Вызывается метод Register
        }

        throw std::range_error(std::format("key [{}] is not registered", iID));
        return nullptr;
    }

    /*!
     * @brief Создание объекта зарегистрированного класса
     * @param iID - Идентификатор зарегистрированного класса
     * @return Созданный объект зарегистрированного класса
     */
    TBase Get(const ID& iID)
    {
        return _classes[iID];
    }

private:
    /*!
     * @brief Создание объекта  класса
     * Derived - Класс, который фабрике необходимо зарегистрировать
     * @return Созданный объект класса
     */
    template<class Derived>
    static ClassSharedPtr Register(Args&&... iArgs)
    {
        return std::make_shared<Derived>(std::forward<Args>(iArgs)...);
    }

private:
    FactoryMap _classes; // Хранится id и адрес метода Register с аргументами
};


#endif /* ObjectFactory_h */
