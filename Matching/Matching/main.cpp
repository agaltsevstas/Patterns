#include <iostream>

/*
 Matching - паттерн сопоставляющий типы, который можно рассматривать как обобщение оператора switch-case
 */

namespace matching
{
    namespace detail
    {
        template <class ...> struct Match;

        template <class Head, class ... Tail>
        struct Match<Head, Tail...> : Match<Tail...>, Head
        {
            template <class Head2, class ... Tail2>
            Match(Head2&& head, Tail2&& ... tail) : Match<Tail...>(std::forward<Tail2>(tail)...), Head(std::forward<Head2>(head)) {}
            using Head::operator();
            using Match<Tail...>::operator();
        };

        template <class T>
        struct Match<T> : T
        {
            template <class R>
            Match(R&& r) : T(std::forward<R>(r)) {}
            using T::operator();
        };
    }

    template <class ... Cases>
    auto Match(Cases&& ... cases)
    {
        return detail::Match<typename std::decay<Cases>::type...>{std::forward<Cases>(cases)...};
    }
}

int main(int argc, const char * argv[])
{
    using namespace matching;
    
    Match([](int number1, int number2) { std::cout << "int: " << number1 << " " << number2 << std::endl; },
          [](double number1, double number2) { std::cout << "double: " << number1 << " " << number2 << std::endl; },
          [](int number1, auto number2)  { std::cout << "float: " << number1 << " " << number2 << std::endl; },
          [](double number1, auto number2)  { std::cout << "float: " << number1 << " " << number2 << std::endl; },
          [&](auto... values) { std::cout << "other types: "; ((std::cout << values << " "), ...) << std::endl; }
          )(1, 1, 1);
    return 0;
}
