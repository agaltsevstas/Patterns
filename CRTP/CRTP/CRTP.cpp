#include <iostream>

#include "CRTP.h"

/*
 CRTP (curiously recurring template pattern) - рекурсивный шаблон: класс Derived наследуется от шаблонного класса Base, использующего Derived как шаблонный параметр T. Через метод базового класса Base можно вызвать метод наследуемого класса Derived, используя статический полиморфизм вместо динамического полиморфизма (без таблицы виртуальных функций): static_cast<T*>(this)->method()
 Плюсы:
 - делается во время компиляции (compile), а не во время исполнения (runtime)
 - нет накладных расходов на создание таблиц виртуальных функций
 - время, затрачиваемое на выбор метода в таблице виртуальных функций
 Минусы:
 - у всех классов-наследников должны определены методы, которые вызываются в базовом классе
*/


int main(int argc, const char * argv[])
{
    CRTP::Derived derived;
    derived.interface1();
    derived.interface2();
    
    auto& singleton1 = CRTP::SINGLETON::Singleton1::Instance();
    auto& singleton2 = CRTP::SINGLETON::Singleton2::Instance();
    
    return 0;
}
