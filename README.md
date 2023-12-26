# Patterns

[Все шаблоны](https://ru.wikipedia.org/wiki/%D0%A8%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F#%D0%A8%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD%D1%8B_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F_%D0%BF%D0%BE%D1%82%D0%BE%D0%BA%D0%BE%D0%B2%D0%BE%D0%B9_%D0%BE%D0%B1%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%BA%D0%B8)

<br>

|[Creational Design Patterns](#creational-design-patterns)|[Structural Design Patterns](#structural-design-patterns)|[Behavioral Design Patterns](#behavioral-design-patterns)|
|:-|:-|:-|
|[Simple Factory](#-simple-factory)|[Adapter](#-adapter)|[Chain of Responsibility](#-chain-of-responsibility)|
|[Factory Method](#-factory-method)|[Bridge](#-bridge)|[Command](#-command)|
|[Abstract Factory](#-abstract-factory)|[Composite](#-composite)|[Iterator](#-iterator)|
|[Builder](#-builder)|[Decorator](#-decorator)|[Mediator](#-mediator)|
|[Prototype](#-prototype)|[Facade](#-facade)|[Memento](#-memento)|
|[Singleton](#-singleton)|[Flyweight](#-flyweight)|[Observer](#-observer)|
||[Proxy](#-proxy)|[Visitor](#-visitor)|
|||[Strategy](#-strategy)|
|||[State](#-state)|
|||[Template Method](#-template-method)|

<br>

Types of Design Patterns
-----------------

* [Creational](#creational-design-patterns)
* [Structural](#structural-design-patterns)
* [Behavioral](#behavioral-design-patterns)

Creational Design Patterns
==========================
Порождающие паттерны проектирования:

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
Структурные паттерны проектирования:

 * [Adapter](#-adapter) (Адаптер) - не меняет функционал, но адаптирует интерфейс.
 * [Bridge](#-bridge)
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
Описание

🌿 Composite
-----------------
Описание

☕ Decorator
-------------
Декоратор - расшириряет класс, в котором чего-то не хватает, дописав необходимые методы Когда у нас есть класс с определённым функционалом, иногда в нём чего-то не хватает.
2 Способа декорирования:
  * Наследование. Наследовать от класса новый класс, и в нём дописать новые методы.
  * Композиция или делегация. Вместо наследования создается независимый класс с новыми методами, а для доступа к старым методам внутрь него помещаем (делегируем) экземпляр рабочего класса.

📦 Facade
----------------
  Фасад определяет интерфейс более высокого уровня, который предоставить более простой интерфейс для всей подсистемы, состоящей из множества объектов.

🍃 Flyweight
---------
Описание

🎱 Proxy
-------------------
 Прокси имеет тот же интерфейс, что и реальный класс. Выступает прослойкой между пользователем и реальным классом. Он выполняет функции (контроль доступа, кеширование, изменение запроса и прочее), не изменяя при этом поведение класса.

Behavioral Design Patterns
==========================
Поведенческие паттерны проектирования:

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
