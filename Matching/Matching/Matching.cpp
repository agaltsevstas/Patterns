#include "Matching.h"

#include <iostream>

/*
 Matching - паттерн сопоставляющий типы, который можно рассматривать как обобщение оператора switch-case
 */

int main()
{
    using namespace matching;

    Match([](int number1, int number2) { std::cout << "int: " << number1 << " " << number2 << std::endl; },
        [](double number1, double number2) { std::cout << "double: " << number1 << " " << number2 << std::endl; },
        [](int number1, auto number2) { std::cout << "float: " << number1 << " " << number2 << std::endl; },
        [](double number1, auto number2) { std::cout << "float: " << number1 << " " << number2 << std::endl; },
        [&](auto... values) { std::cout << "other types: "; ((std::cout << values << " "), ...) << std::endl; }
    )(1, 1, 1);
    return 0;
}
