# Patterns

[Все паттерны](https://ru.wikipedia.org/wiki/%D0%A8%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F#%D0%A8%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD%D1%8B_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F_%D0%BF%D0%BE%D1%82%D0%BE%D0%BA%D0%BE%D0%B2%D0%BE%D0%B9_%D0%BE%D0%B1%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%BA%D0%B8)

[Хабр](https://habr.com/ru/companies/vk/articles/325492/)

<br>

|[Patterns](#patterns)|[Creational Design Patterns](#creational-design-patterns)|[Structural Design Patterns](#structural-design-patterns)|[Behavioral Design Patterns](#behavioral-design-patterns)|
|:-|:-|:-|:-|
|[CRTP](#crtp)|[Singleton](#-singleton)|[Adapter](#-adapter)|[Chain of Responsibility](#-chain-of-responsibility)|
|[SFINAE](#sfinae)|[Simple Factory](#-simple-factory)|[Bridge](#-bridge)|[Command](#-command)|
|[Matching](#matching)|[Factory Method](#-factory-method)|[Composite](#-composite)|[Iterator](#-iterator)|
|[PImpl](#pimpl)|[Abstract Factory](#-abstract-factory)|[Decorator](#-decorator)|[Mediator](#-mediator)|
|[NVI](#nvi)|[Builder](#-builder)|[Facade](#-facade)|[Memento](#-memento)|
|[ThreadPool](#threadpool)|[Prototype](#-prototype)|[Flyweight](#-flyweight)|[Observer](#-observer)|
||[Object Pool](#object-pool)|[Proxy](#-proxy)|[Visitor](#-visitor)|
||||[Strategy](#-strategy)|
||||[State](#-state)|
||||[Template Method](#-template-method)|

<br>

Types of Design Patterns
------------------------

* [Patterns](#patterns)
* [Creational](#creational-design-patterns)
* [Structural](#structural-design-patterns)
* [Behavioral](#behavioral-design-patterns)
  
Patterns
--------

 * [CRTP](#crtp)
 * [SFINAE](#sfinae)
 * [Matching](#matching)
 * [PImpl](#pimpl)
 * [NVI](#nvi)
 * [ThreadPool](#threadpool)

CRTP
----
Сuriously Recurring Template Pattern - рекурсивный шаблон: класс Derived наследуется от шаблонного класса Base, использующего Derived как шаблонный параметр T. Через метод базового класса Base можно вызвать метод наследуемого класса Derived, используя статический полиморфизм вместо динамического полиморфизма (без таблицы виртуальных функций): <br>
```
template <typename T>
class Base
{
public:
    void interface1() // 1 Cпособ
    {
        static_cast<T*>(this)->implementation1();
    };
    
    static void interface2()
    {
        T::implementation2(); // 2 Способ: static
    };
};

class Derived : public Base<Derived>
{
public:
    void implementation1()
    {
        std::cout << "interface1 Derived" << std::endl;
    }
    
    static void implementation2()
    {
        std::cout << "interface2 Derived" << std::endl;
    }
};
```

*Плюсы:*
- делается во время компиляции (compile), а не во время исполнения (runtime)
- нет накладных расходов на создание таблиц виртуальных функций
- время, затрачиваемое на выбор метода в таблице виртуальных функций

*Минусы:*
- у всех классов-наследников должны определены методы, которые вызываются в базовом классе

SFINAE
------
Substitution Failure Is Not An Error - при определении перегрузок функции ошибочные подстановки в шаблоны не вызывают ошибку компиляции, а отбрасываются из списка кандидатов на наиболее подходящую перегрузку.

*Правила:*
- SFINAE это обязательно связано с перегрузкой функций
- это работает при автоматическом выводе типов шаблона (type deduction) по аргументам функции
- некоторые перегрузки могут отбрасываться в том случае, когда их невозможно определить из-за возникающей синтаксической ошибки, компиляция при этом продолжается - без ошибок
- отбросить могут только шаблон
- SFINAE рассматривает только заголовок функции, ошибки в теле функции не будут пропущены

Matching
--------
Паттерн сопоставляющий типы, который можно рассматривать как обобщение оператора switch-case.

PImpl
-----
Pointer to private implementation - указатель на частную реализацию, позволяющий избежать постоянной привязки абстракции к реализации и изменять их независимо друг от друга.
Такая обертка хранится внутри уже существующего класса в виде указателя (forward declaration) в .h файле (хедере). Реализация находится в .cpp файле.

*Плюсы:*
- меньше тратится время на перекомпилирование .h файла. Ускорение сборки происходит из-за уменьшения количества директив #include в хедерах
- доступ в pImpl осуществляется косвенно через указатель, пользователь не видит никаких деталей реализации класса

*Минусы:*
- тратится память на дополнительный класс
- загромождение кода в .cpp файле

NVI
---
Non virtual interface - шаблона невиртуального интерфейса использует public невиртуальные методы в качестве обертки над вызовами private/protected виртуальных методов. NVI - частный случай паттерна — «Шаблонный метод». Преимущество идиомы NVI - выполнение предварительных действий перед вызовов виртуального метода и выполнение завершающих действий после вызова виртуального метода (например, захват mutex).

*Плюсы:*
 - базовый класс (интерфейс) имеет больше контроля над своим поведением, чем производные классы (например, вывод в файл или экран не нужно дублировать в каждой реализации производного класса: достаточно вынести вызов виртуального метода в невиртуальный метод + дополнив его).

*Минусы:*
 - дополнительные методы.

ThreadPool
---
Пул потоков - позволяет выполнять множество задач параллельно, используя набор потоков. ThreadPool состоит обычно из очереди задач и нескольких потоков, которые достают задачи из очереди и выполняют их параллельно. Кол-во потоков можно задать вручную, так и по умолчанию с помощью thread::hardware_concurrency() - кол-во возможных потоков на компьютере.

*Плюсы:*
- помогает увеличить производительность программы, благодаря созданию потоков один раз (создание потока считается достаточно затратной операцией), поэтому для каждой новой задачи не требуется заново создавать поток.
- предоставляет удобный интерфейс для работы с многопоточностью.

Creational Design Patterns
==========================
Порождающие паттерны проектирования - позволяют сделать систему независимой от способа создания, композиции и представления объектов:

 * [Singleton](#-singleton)
 * [Simple Factory](#-simple-factory)
 * [Factory Method](#-factory-method)
 * [Abstract Factory](#-abstract-factory)
 * [Builder](#-builder)
 * [Prototype](#-prototype)
 * [Object Pool](#object-pool)
 
💍 Singleton
------------
Одиночка - класс, имеющий единственный эксземпляр, и доступ к нему осуществляется с помощью static метода, в котором создается static объект данного класса и возвращается. Конструктор = delete, Конструктор копирования = delete, Оператор копирования = delete; либо выносятся в private секцию.

*Плюсы:*
- ограничение кол-во экземпляров класса.

*Минусы:*
- отсутствие полиморфизма и наследования (исключение CRTP) -> сложно покрыть unit-тестами.

🏠 Simple Factory
-----------------
Простая фабрика/Фабрика объектов - создает объекты, определенные в фабрике с помощью единого интерфейса. Фабрика позволяет зарегистрировать класс, узнать наличие зарегистрированного класса, узнать количество зарегистрированных классов, а также ограничивать набор возможных классов. Работа фабрики объектов напоминает функционирование «виртуального конструктора» - можно создавать объекты нужных классов с помощью уникальных идентификаторов без указывания напрямую их типов.
«Виртуальный конструктор» - создание виртуального метода в классе, который будет возвращать копию объекта: ```virtual MyClass* clone() const { return new MyClass(*this); }```.

🏭 Factory Method
-----------------
Описание

🔨 Abstract Factory
-------------------
Абстрактная фабрика - предоставляет готовый интерфейс для создания объектов, не имея представление что это за объекты.

*Плюсы:*
- скрытие создания объектов в производных классах - это делает фабрику независимой от типов создаваемых объектов.
- можно абстрактную фабрику написать в .h файле (хедере), а реализацию производных классов вынести в .cpp файл. Таким образом будет меньше тратится время на перекомпилирование .h файла. Ускорение сборки происходит из-за уменьшения количества директив #include в хедерах.

*Минусы:*
- трудно изменяемый интерфейс из-за производных классов.

Пример: <br>
**IAbstractFactory** - класс синглтон, который хранит в себе карту классов: ```map<ID, TypeClass*>``` (id добавленного класса + тип добавленного класса), с помощью виртуальных методов можно создавать объекты классов и проводить над ними различные действия. С помощью protected метода ```void Register(ID id, TypeClass* iClass)``` (идентификатор добавляемого класса + тип добавляемого класса) - можно добавлять новые типы классов в карту классов. <br>
**FactoryClass** - производный класс от IAbstractFactory, который переопределяет виртуальные методы + добавляет в карту классов нужный тип класса - в конструкторе вызывается protected метод ```Register(id, MyClass)``` (id добавляемого класса + тип добавляемого класса). <br>

👷 Builder
----------
Описание

🐑 Prototype
------------
Описание

Object Pool
-----------
Пул объектов - основная идея состоит в том, чтобы избежать создания новых экземпляров класса в случае возможности их повторного использования. Такой подход повышает производительность при создание экземпляров некоторого класса, которые требует больших затрат и создаются часто, но число создаваемых объектов в единицу времени ограничено. Пул объектов, который может избежать создания новых объектов, запрашивая в пуле уже созданный экземпляр. Когда объекты больше не нужны, они возвращаются в пул для дальнейшего повторного использования. Если при очередном запросе все объекты пула заняты, то процесс будет ожидать освобождения объекта. Для исключетельной ситуации пул объектов должен уметь создавать новые объекты по мере необходимости, но при этом он также должен иметь механизм периодической очистки неиспользуемых объектов.

Structural Design Patterns
==========================
Структурные паттерны проектирования - используют наследование для составления композиций из интерфейсов и реализаций:

 * [Adapter](#-adapter) (Адаптер) - не меняет функционал, но адаптирует интерфейс.
 * [Bridge](#-bridge) (Мост) - обеспечивает отделение абстракции объекта от его реализации.
 * [Composite](#-composite)
 * [Decorator](#-decorator) (Декоратор) -  расширяет функционал, не трогая исходный 
 * [Facade](#-facade) (Фасад) - простой интерфейс к сложному интерфейсу.
 * [Flyweight](#-flyweight)
 * [Proxy](#-proxy) (Прокси) - не меняет интерфейс, но адаптирует функционал.

🔌 Adapter
----------
Адаптер - имеет нужный интерфейс, который сопрягается с другим классом, интерфейс которого не подходит и менять его проблематично.

🚡 Bridge
----------
 Мост - обеспечивает отделение абстракции объекта от его реализации (кросс-платформенные приложения, поддержание разных баз данных): <br>
 Абстракция: abstract class Figure от него наследуются class Rectangle, Circle. virtual method Draw(); <br>
 Реализация: По идее реализация должна быть внутри метода Draw() классов Rectangle, Circle. Однако, реализация выносится в отдельные классы RectangleRenderer, CircleRenderer, которые наследуются от abstract class Renderer с virtual method Render(). В конструктор Rectangle/Circle передается ссылка на экземпляр класса RectangleRenderer/CircleRenderer и внутри метода Draw() вызывается метод Render экземпляра класса RectangleRenderer/CircleRenderer. <br>
Такое разделение позволяет изменениям в одной части системы оказывать минимальное влияние на другую и код становится легче поддерживать.

🌿 Composite
------------
Описание

☕ Decorator
------------
Декоратор - расшириряет класс, в котором чего-то не хватает, дописав необходимые методы.

2 Способа декорирования:
  * Наследование. Наследовать от класса новый класс, и в нём дописать новые методы.
  * Композиция или делегация. Вместо наследования создается независимый класс с новыми методами, а для доступа к старым методам внутрь него помещаем (делегируем) экземпляр рабочего класса.

📦 Facade
---------
Фасад - определяет интерфейс более высокого уровня, который предоставить более простой интерфейс для всей подсистемы, состоящей из множества объектов.

🍃 Flyweight
------------
Описание

🎱 Proxy
--------
Прокси - имеет тот же интерфейс, что и реальный класс. Выступает прослойкой между пользователем и реальным классом. Он выполняет функции (контроль доступа, кеширование, изменение запроса и прочее), не изменяя при этом поведение класса.

Behavioral Design Patterns
==========================
Поведенческие паттерны проектирования - используется наследование, чтобы определить поведение для различных классов. В поведенческих шаблонах уровня объекта используется композиция:

* [Chain of Responsibility](#-chain-of-responsibility)
* [Command](#-command)
* [Iterator](#-iterator)
* [Mediator](#-mediator)
* [Memento](#-memento)
* [Observer](#-observer)
* [Visitor](#-visitor)
* [Strategy](#-strategy)
* [State](#-state)
* [Template Method](#-template-method)

🔗 Chain of Responsibility
--------------------------
Описание 

👮 Command
----------
Описание

➿ Iterator
-----------
 Итератор - позволяет получить последовательный доступ к элементам объекта, не раскрывая его внутреннего представления (list, forward_list). Итератор может хранить «сырой указатель», таким образом преобразуя опасный интерфейс указателя на элемент контейнера в безопасный. Итератор может также хранить ссылку на объект, к которому он принадлежит – это позволяет выполнять сравнение между итераторами разных объектов. С помощью функций поиска find, find_if можно перебирать итераторы. <br> 
 
*Имеет методы:*
 - получение следующего/предыдущего элемента c перегрузкой operator++/--(int).
 - доступ к значению элемента с перегрузкой operator*()/operator->().
 - сравнение итераторов c перегрузкой operator==/operator!=.

👽 Mediator
-----------
Описание

💾 Memento
----------
Описание

😎 Observer
-----------
Описание

🏃 Visitor
----------
Описание

💡 Strategy
-----------
Описание

💢 State
--------
Описание

📒 Template Method
------------------
Шаблонный метод - метод в абстрактном классе, определяющий структуру алгоритма и состоящего из нескольких шагов: вызов некоторых методов в абстрактном классе и вызовов чисто виртуальных методов, реализованных в производных классах. <br>
Пример: есть абстрактный класс Figure, от него наследуются производные классы Square, Rectangle. Шаблонный метод в Figure вызывает чисто виртуальные методы вычисления периметра, площади и объема, реализованные в производных классах Square, Rectangle. После вызова метода Template Method и всех расчетов в нем можно получить результаты.

# Сайты: 
[ПАТТЕРНЫ ПРОЕКТИРОВАНИЯ на C++](https://refactoringguru.cn/ru/design-patterns/cpp) <br/>
