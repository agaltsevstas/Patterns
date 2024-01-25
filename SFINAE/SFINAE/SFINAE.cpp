#include "SFINAE.h"

#include <iostream>
#include <vector>


// Сайты: https://stackoverflow.com/questions/74263416/question-about-has-const-iterator-has-begin-end
//        https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time

// Лекция: https://www.youtube.com/watch?v=v49lAJXnnPM&t=1s&ab_channel=ComputerScienceCenter

/*
* SFINAE (substitution failure is not an error) - при определении перегрузок функции ошибочные подстановки в шаблоны не вызывают ошибку компиляции, а отбрасываются из списка кандидатов на наиболее подходящую перегрузку. 
  Правила:
  1. SFINAE это обязательно связано с перегрузкой функций.
  2. Это работает при автоматическом выводе типов шаблона (type deduction) по аргументам функции.
  3. Некоторые перегрузки могут отбрасываться в том случае, когда их невозможно определить из-за возникающей синтаксической ошибки; компиляция при этом продолжается, без ошибок.
  4. Отбросить могут только шаблон.
  5. SFINAE рассматривает только заголовок функции, ошибки в теле функции не будут пропущены.
*/

template <typename T>
struct Number
{
	Number(const T& number) : value(number) {}

	T value;
};


int main()
{
	int integer_num = 5;
	float floating_num = 5.0;
	bool boolean = true;
	Number<int> number_int(5);

    using namespace SFINAE;
    // Square
    {
        {
            auto square1 = Square(integer_num); // Вызов int Square(int);
            auto square2 = Square(floating_num); // Вызов float Square(float);
            auto square3 = Square(boolean);  // Вызов bool Square(bool);
            //auto square4 = SFINAE::Square(number_int); // Ошибка: не найден operator*
        }

        // std::is_arithmetic
        {
            //auto square1 = IS_ARITHMETIC::Square(integer_num); // Ошибка: выражение слева от ".value" должно представлять класс, структуру или объединение
            //auto square2 = IS_ARITHMETIC::Square(floating_num); // Ошибка: выражение слева от ".value" должно представлять класс, структуру или объединение
            //auto square3 = IS_ARITHMETIC::Square(boolean);  // Ошибка: выражение слева от ".value" должно представлять класс, структуру или объединение
            //auto square4 = IS_ARITHMETIC::Square(number_int); // Ошибка: бинарный "*": "const T" не определяет этот оператор или преобразование к типу приемлемо к встроенному оператору
        }

        // std::enable_if
        {
            using namespace SFINAE;
            
            auto square1 = ENABLE_IF::Square(integer_num); // Вызов int Square(int);
            auto square2 = ENABLE_IF::Square(floating_num); // Вызов float Square(float);
            auto square3 = ENABLE_IF::Square(boolean);  // Вызов bool Square(bool);
            auto square4 = ENABLE_IF::Square(number_int); // Вызов Number<int> Square(Number<int>);
        }

        // constexpr
        { 
            auto square1 = CONSTEXPR::Square(integer_num); // Вызов int Square(int);
            auto square2 = CONSTEXPR::Square(floating_num); // Вызов float Square(float);
            auto square3 = CONSTEXPR::Square(boolean);  // Вызов bool Square(bool);
            auto square4 = CONSTEXPR::Square(number_int); // Вызов Number<int> Square(Number<int>);
            
            // TODO:
            CONSTEXPR::convertible<1>();
        }
    }
    
    // CONTAINER
    {
        constexpr const int number = 10;
        constexpr std::string_view value = "10";
        std::vector<int> numbers { 1,2,3,4,5 };
        auto has_const_iterator1 = CONTAINER::has_const_iterator<std::vector<int>>::value;
        auto has_const_iterator2 = CONTAINER::has_const_iterator<int>::value;
        
        auto has_begin_value1 = CONTAINER::has_begin_end<std::vector<int>>::begin_value;
        auto has_end_value1 = CONTAINER::has_begin_end<std::vector<int>>::end_value;
        auto has_begin_value2 = CONTAINER::has_begin_end<int>::end_value;
        auto has_end_value2 = CONTAINER::has_begin_end<int>::begin_value;
        
        // std::enable_if
        {
            CONTAINER::ENABLE_IF::Print(numbers);
            CONTAINER::ENABLE_IF::Print(number);
            CONTAINER::ENABLE_IF::Print(value);
        }
        // constexpr
        {
            CONTAINER::CONSTEXPR::Print(numbers);
            CONTAINER::CONSTEXPR::Print(number);
            CONTAINER::CONSTEXPR::Print(value);
        }
        
        // CONCEPTS
        {
            CONTAINER::CONCEPTS::Print(numbers);
        }
    }
    
    return 0;
}
