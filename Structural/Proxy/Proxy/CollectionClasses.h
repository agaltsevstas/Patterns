#ifndef CollectionClasses_hpp
#define CollectionClasses_hpp

#include <iostream>

/*!
 * @brief Прокси-класс (Proxy-class) к Фабрике объектов (ObjectFactory)
 * Proxy-class выступает прослойкой между пользователем и реальным классом
 * Proxy-class имеет тот же интерфейс, что и реальный класс, поэтому  нет разницы — работать через proxy или напрямую с классом. Он выполняет функцию (контроль доступа, кеширование, изменение запроса и прочее), не изменяя при этом поведение класса
 */
template<class ID, class Base, typename ...Args>
class CollectionClassesProxy final
{
    CollectionClassesProxy() {};
    ~CollectionClassesProxy() {};
    CollectionClassesProxy(const CollectionClassesProxy&) = delete;
    CollectionClassesProxy& operator = (const CollectionClassesProxy&) = delete;
    
public:
    /*!
     * @brief Инициализация фабрики объектов
     * ID - Идентификатор  класса
     * Base - Базовый класс
     * iArgs - Аргументы базового класса
     */
    static CollectionClassesProxy& Instance();

    /*!
     * @brief Добавление класса в карту классов
     * Derived - Класс, который фабрике необходимо зарегистрировать
     * @param iID - Идентификатор  класса
     */
    template<class Derived>
    void Add(const ID& iID);

    /*!
     * @brief Удаление класса
     * @param iID - Идентификатор  класса
     */
    void Remove(const ID& iID) noexcept;

    /*!
     * @brief Проверка на наличие класса по Id
     * @param iID - Идентификатор  зарегистрированного класса
     * @return true - зарегистрированный класс найден, иначе - false
     */
    bool IsRegistered(const ID& iID) const noexcept;

    /*!
     * @brief Создание объекта зарегистрированного класса
     * @param iID - Идентификатор зарегистрированного класса
     * @param iArgs - Аргументы зарегистрированного класса
     * @return Созданный объект зарегистрированного класса
     */
    std::shared_ptr<Base> Create(const ID& iID, const Args&... iArgs) const;
};

#endif /* CollectionClasses_hpp */