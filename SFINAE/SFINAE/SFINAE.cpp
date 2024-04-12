#include "SFINAE.h"

#include <iostream>
#include <vector>


// Лекция: https://www.youtube.com/watch?v=v49lAJXnnPM&t=1s&ab_channel=ComputerScienceCenter

/*
 Сайты: https://stackoverflow.com/questions/74263416/question-about-has-const-iterator-has-begin-end
        https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time
 */

/*
 SFINAE (substitution failure is not an error) - при определении перегрузок функции ошибочные подстановки в шаблоны не вызывают ошибку компиляции, а отбрасываются из списка кандидатов на наиболее подходящую перегрузку.
 Правила:
 - SFINAE это обязательно связано с перегрузкой функций
 - это работает при автоматическом выводе типов шаблона (type deduction) по аргументам функции
 - некоторые перегрузки могут отбрасываться в том случае, когда их невозможно определить из-за возникающей синтаксической ошибки, компиляция при этом продолжается - без ошибок
 - отбросить могут только шаблон
 - SFINAE рассматривает только заголовок функции, ошибки в теле функции не будут пропущены
*/

template <typename T>
struct Number
{
	Number(const T& number) : value(number) {}

	T value;
};


int main()
{
    using namespace SFINAE;
    // Square
    {
        int integer_num = 5;
        float floating_num = 5.0;
        bool boolean = true;
        Number<int> number_int(5);
        Number<float> number_float(5.0f);
        Number<float> number_boolean(true);
        
        {
            [[maybe_unused]] auto square1 = Square(integer_num); // Вызов int Square(int);
            [[maybe_unused]] auto square2 = Square(floating_num); // Вызов float Square(float);
            [[maybe_unused]] auto square3 = Square(boolean);  // Вызов bool Square(bool);
            //auto square4 = SFINAE::Square(number_int); // Ошибка: не найден operator*
        }

        // std::is_arithmetic
        {
            //auto square1 = IS_ARITHMETIC::Square(integer_num); // Ошибка: выражение слева от ".value" должно представлять класс, структуру или объединение
            //auto square2 = IS_ARITHMETIC::Square(floating_num); // Ошибка: выражение слева от ".value" должно представлять класс, структуру или объединение
            //auto square3 = IS_ARITHMETIC::Square(boolean);  // Ошибка: выражение слева от ".value" должно представлять класс, структуру или объединение
            //auto square4 = IS_ARITHMETIC::Square(number_int); // Ошибка: бинарный "*": "const T" не определяет этот оператор или преобразование к типу приемлемо к встроенному оператору
        }

        // C++14: std::enable_if
        {
            using namespace SFINAE;
            
            [[maybe_unused]] auto square1 = ENABLE_IF::Square(integer_num); // Вызов int Square(int);
            [[maybe_unused]] auto square2 = ENABLE_IF::Square(floating_num); // Вызов float Square(float);
            [[maybe_unused]] auto square3 = ENABLE_IF::Square(boolean);  // Вызов bool Square(bool);
            [[maybe_unused]] auto square4 = ENABLE_IF::Square(number_int); // Вызов Number<int> Square(Number<int>);
            [[maybe_unused]] auto square5 = ENABLE_IF::Square(number_float); // Вызов Number<float> Square(Number<float>);
            [[maybe_unused]] auto square6 = ENABLE_IF::Square(number_boolean); // Вызов Number<bool> Square(Number<bool>);
        }

        // C++17: constexpr
        { 
            [[maybe_unused]] auto square1 = CONSTEXPR::Square(integer_num); // Вызов int Square(int);
            [[maybe_unused]] auto square2 = CONSTEXPR::Square(floating_num); // Вызов float Square(float);
            [[maybe_unused]] auto square3 = CONSTEXPR::Square(boolean);  // Вызов bool Square(bool);
            [[maybe_unused]] auto square4 = CONSTEXPR::Square(number_int); // Вызов Number<int> Square(Number<int>);
            [[maybe_unused]] auto square5 = CONSTEXPR::Square(number_float); // Вызов Number<float> Square(Number<float>);
            [[maybe_unused]] auto square6 = CONSTEXPR::Square(number_boolean); // Вызов Number<bool> Square(Number<bool>);
            
            // TODO:
            CONSTEXPR::convertible<1>();
        }
        
        // C++20: concept
        {
            [[maybe_unused]] auto square1 = SFINAE::CONCEPT::Square(integer_num); // Вызов int Square(int);
            [[maybe_unused]] auto square2 = SFINAE::CONCEPT::Square(floating_num); // Вызов float Square(float);
            [[maybe_unused]] auto square3 = SFINAE::CONCEPT::Square(boolean);  // Вызов bool Square(bool);
            [[maybe_unused]] auto square4 = SFINAE::CONCEPT::Square(number_int); // Вызов Number<int>
            [[maybe_unused]] auto square5 = SFINAE::CONCEPT::Square(number_float); // Вызов Number<float> Square(Number<float>);
            [[maybe_unused]] auto square6 = SFINAE::CONCEPT::Square(number_boolean); // Вызов Number<bool>Square(Number<bool>);
        }
    }
    
    // CONTAINER
    {
        constexpr const int number = 10;
        constexpr std::string_view value = "10";
        std::vector<int> numbers { 1,2,3,4,5 };
        [[maybe_unused]] auto has_const_iterator1 = CONTAINER::has_const_iterator<std::vector<int>>::value;
        [[maybe_unused]] auto has_const_iterator2 = CONTAINER::has_const_iterator<int>::value;
        
        [[maybe_unused]] auto has_begin_value1 = CONTAINER::has_begin_end<std::vector<int>>::begin_value;
        [[maybe_unused]] auto has_end_value1 = CONTAINER::has_begin_end<std::vector<int>>::end_value;
        [[maybe_unused]] auto has_begin_value2 = CONTAINER::has_begin_end<int>::end_value;
        [[maybe_unused]] auto has_end_value2 = CONTAINER::has_begin_end<int>::begin_value;
        
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
