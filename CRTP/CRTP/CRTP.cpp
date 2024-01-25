#include <iostream>

#include "CRTP.h"

/*
 CRTP (curiously recurring template pattern) - рекурсивный шаблон: класс Derived наследуется от шаблона класса Base, использующего Derived как шаблонный параметр. Вызов Base::interface вызывает Derived::interface. Используется статический полиморфизм вместо динамического полиморфизма (без виртуальных функций)
 Плюсы:
 - делается во время компиляции (compile), а не во время исполнения (runtime)
 - нет накладных расходов на создание таблиц виртуальных функций
 - время, затрачиваемое на выбор метода
 Минусы:
 - У всех классов-наследников должны определены методы, которые которые вызываются в базовом классе
*/


int main(int argc, const char * argv[])
{
    CRTP::Derived derived;
    derived.interface1();
    derived.interface2();
}
