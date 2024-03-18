# Patterns

[Все паттерны](https://ru.wikipedia.org/wiki/%D0%A8%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F#%D0%A8%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD%D1%8B_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F_%D0%BF%D0%BE%D1%82%D0%BE%D0%BA%D0%BE%D0%B2%D0%BE%D0%B9_%D0%BE%D0%B1%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%BA%D0%B8)

[Хабр](https://habr.com/ru/companies/vk/articles/325492/)

<br>

|[Patterns](#patterns)|[Creational Design Patterns](#creational-design-patterns)|[Structural Design Patterns](#structural-design-patterns)|[Behavioral Design Patterns](#behavioral-design-patterns)|
|:-|:-|:-|:-|
|[CRTP](#crtp)|[Simple Factory](#-simple-factory)|[Adapter](#-adapter)|[Chain of Responsibility](#-chain-of-responsibility)|
|[SFINAE](#sfinae)|[Factory Method](#-factory-method)|[Bridge](#-bridge)|[Command](#-command)|
|[Matching](#matching)|[Abstract Factory](#-abstract-factory)|[Composite](#-composite)|[Iterator](#-iterator)|
|[PImpl](#pimpl)|[Builder](#-builder)|[Decorator](#-decorator)|[Mediator](#-mediator)|
||[Prototype](#-prototype)|[Facade](#-facade)|[Memento](#-memento)|
||[Singleton](#-singleton)|[Flyweight](#-flyweight)|[Observer](#-observer)|
|||[Proxy](#-proxy)|[Visitor](#-visitor)|
||||[Strategy](#-strategy)|
||||[State](#-state)|
||||[Template Method](#-template-method)|

<br>

Types of Design Patterns
-----------------

* [Patterns](#patterns)
* [Creational](#creational-design-patterns)
* [Structural](#structural-design-patterns)
* [Behavioral](#behavioral-design-patterns)
  
Patterns
-----------------

 * [CRTP](#crtp)
 * [SFINAE](#sfinae)
 * [Matching](#matching)
 * [PImpl](#pimpl)

CRTP
--------------
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

Плюсы:
 - делается во время компиляции (compile), а не во время исполнения (runtime)
 - нет накладных расходов на создание таблиц виртуальных функций
 - время, затрачиваемое на выбор метода в таблице виртуальных функций

 Минусы:
 - у всех классов-наследников должны определены методы, которые вызываются в базовом классе

SFINAE
--------------
Substitution Failure Is Not An Error - при определении перегрузок функции ошибочные подстановки в шаблоны не вызывают ошибку компиляции, а отбрасываются из списка кандидатов на наиболее подходящую перегрузку.

Правила:
- SFINAE это обязательно связано с перегрузкой функций
- это работает при автоматическом выводе типов шаблона (type deduction) по аргументам функции
- некоторые перегрузки могут отбрасываться в том случае, когда их невозможно определить из-за возникающей синтаксической ошибки, компиляция при этом продолжается - без ошибок
- отбросить могут только шаблон
- SFINAE рассматривает только заголовок функции, ошибки в теле функции не будут пропущены

Matching
----------------
Паттерн сопоставляющий типы, который можно рассматривать как обобщение оператора switch-case.

PImpl
--------------------------------------------
Pointer to private implementation - указатель на частную реализацию, позволяющий избежать постоянной привязки абстракции к реализации и изменять их независимо друг от друга.
Такая обертка хранится внутри уже существующего класса в виде указателя (forward declaration) в .h файле (хедере). Реализация находится в .cpp файле.

Плюсы:
- меньше тратится время на перекомпилирование .h файла. Ускорение сборки происходит из-за уменьшения количества директив #include в хедерах
- доступ в pImpl осуществляется косвенно через указатель, пользователь не видит никаких деталей реализации класса

Минусы:
- тратится память на дополнительный класс
- загромождение кода в .cpp файле

Creational Design Patterns
==========================
Порождающие паттерны проектирования - позволяют сделать систему независимой от способа создания, композиции и представления объектов:

 * [Simple Factory](#-simple-factory)
 * [Factory Method](#-factory-method)
 * [Abstract Factory](#-abstract-factory)
 * [Builder](#-builder)
 * [Prototype](#-prototype)
 * [Singleton](#-singleton)

🏠 Simple Factory
--------------
Описание

🏭 Factory Method
--------------
Описание

🔨 Abstract Factory
----------------
Описание

👷 Builder
--------------------------------------------
Описание

🐑 Prototype
------------
Описание

💍 Singleton
------------
Описание

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
-------
Адаптер - имеет нужный интерфейс, который сопрягается с другим классом, интерфейс которого не подходит и менять его проблематично.

🚡 Bridge
------
 Мост - обеспечивает отделение абстракции объекта от его реализации (кросс-платформенные приложения, поддержание разных баз данных):
 Абстракция: abstract class Figure от него наследуются class Rectangle, Circle. virtual method Draw();
 Реализация: По идее реализация должна быть внутри метода Draw() классов Rectangle, Circle. Однако, реализация выносится в отдельные классы RectangleRenderer, CircleRenderer, которые наследуются от abstract class Renderer с virtual method Render(). В конструктор Rectangle/Circle передается ссылка на экземпляр класса RectangleRenderer/CircleRenderer и внутри метода Draw() вызывается метод Render экземпляра класса RectangleRenderer/CircleRenderer.
Такое разделение позволяет изменениям в одной части системы оказывать минимальное влияние на другую и код становится легче поддерживать.

🌿 Composite
-----------------
Описание

☕ Decorator
-------------
Декоратор - расшириряет класс, в котором чего-то не хватает, дописав необходимые методы.

2 Способа декорирования:
  * Наследование. Наследовать от класса новый класс, и в нём дописать новые методы.
  * Композиция или делегация. Вместо наследования создается независимый класс с новыми методами, а для доступа к старым методам внутрь него помещаем (делегируем) экземпляр рабочего класса.

📦 Facade
----------------
Фасад - определяет интерфейс более высокого уровня, который предоставить более простой интерфейс для всей подсистемы, состоящей из множества объектов.

🍃 Flyweight
---------
Описание

🎱 Proxy
-------------------
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
-----------------------
Описание 

👮 Command
-------
Описание

➿ Iterator
--------
Описание

👽 Mediator
========
Описание

💾 Memento
-------
Описание

😎 Observer
--------
Описание

🏃 Visitor
-------
Описание

💡 Strategy
--------
Описание

💢 State
-----
Описание

📒 Template Method
---------------
Описание
