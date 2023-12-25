#include <iostream>

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

// SFINAE
namespace SFINAE
{
	/*
	  При вызове функции компилятор должен выполнить проверку, чтобы узнать, какую из функций вызывать.
	  Нужно учитывать перегрузку функций (function overloading), позволяющую давать нескольким функциям одинаковые имена.
	  Шаги:
	  1. Name lookup (поиск по имени);
	  2. Template Argument Deduction (дедукция аргумента шаблона);
	  3. Template Argument Substitution (замена аргумента шаблона);
	  4. Overload Resolution (разрешение перегрузки).
	*/

	/*
	  Алгоритм SFINAE проявляется на шаге 3 (Template Argument Substitution), когда функция-кандидат проваливает тест Substitution (замены).
	  Ошибка компиляции не возникает, а функция просто удаляется из списка кандидатов.
	*/
	namespace
	{
		template<typename T>
		T Square(const T& number)
		{
			return number * number;
		}
	}

	/*
	  Для Number<int> не работает: при вызове. Эта функция пытается найти int.value, которого не существует. Ошибку: часть else не удалена из функции (error: request for member ‘value’ in ‘t’, which is of non-class type ‘const int’).
	*/
	namespace IS_ARITHMETIC
	{
		template<typename T>
		T Square(const T& number)
		{
			if (std::is_arithmetic<T>::value)
			{
				return number * number;
			}
			else
			{
				return number.value * number.value;
			}
		}
	}

	/*
	* Чтобы решить проблему Number<int>, нужны два шаблона функций, которые проверяют передаваемый тип на арифметическу.
	*/
	namespace ENABLE_IF
	{
		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value, T>::type Square(const T& number)
		{
			return number * number;
		}

		template<typename T>
		typename std::enable_if<!std::is_arithmetic<T>::value, T>::type Square(const T& number)
		{
			return number.value * number.value;
		}
	}

	/*
	  Здесь используется только один шаблон функции (что упрощает код вместо std::enable_if).
	  Компилятор берет только ветку с истинным условием (true) и отбрасывает другие.
	*/
	namespace CONSTEXPR
	{
		template<typename T>
		T Square(const T& number)
		{
			if constexpr (std::is_arithmetic<T>::value) // Проверка нужна для класса Number, иначе ошибка: не найден operator*
			{
				return number * number;
			}
			else
			{
				return number.value * number.value;
			}
		}
	}
}

int main()
{
	int integer_num = 5;
	float floating_num = 5.0;
	bool boolean = true;
	Number<int> number_int(5);

	{
		auto square1 = SFINAE::Square(integer_num); // Вызов int Square(int);
		auto square2 = SFINAE::Square(floating_num); // Вызов float Square(float);
		auto square3 = SFINAE::Square(boolean);  // Вызов bool Square(bool);
		//auto square4 = SFINAE::Square(number_int); // Ошибка: не найден operator*
	}

	// std::is_arithmetic
	{
		//auto square1 = SFINAE::IS_ARITHMETIC::Square(integer_num); // Ошибка: выражение слева от ".value" должно представлять класс, структуру или объединение
		//auto square2 = SFINAE::IS_ARITHMETIC::Square(floating_num); // Ошибка: выражение слева от ".value" должно представлять класс, структуру или объединение
		//auto square3 = SFINAE::IS_ARITHMETIC::Square(boolean);  // Ошибка: выражение слева от ".value" должно представлять класс, структуру или объединение
		//auto square4 = SFINAE::IS_ARITHMETIC::Square(number_int); // Ошибка: бинарный "*": "const T" не определяет этот оператор или преобразование к типу приемлемо к встроенному оператору
	}

	// std::enable_if
	{
		auto square1 = SFINAE::ENABLE_IF::Square(integer_num); // Вызов int Square(int);
		auto square2 = SFINAE::ENABLE_IF::Square(floating_num); // Вызов float Square(float);
		auto square3 = SFINAE::ENABLE_IF::Square(boolean);  // Вызов bool Square(bool);
		auto square4 = SFINAE::ENABLE_IF::Square(number_int); // Вызов Number<int> Square(Number<int>);
	}

	// constexpr
	{
		auto square1 = SFINAE::CONSTEXPR::Square(integer_num); // Вызов int Square(int);
		auto square2 = SFINAE::CONSTEXPR::Square(floating_num); // Вызов float Square(float);
		auto square3 = SFINAE::CONSTEXPR::Square(boolean);  // Вызов bool Square(bool);
		auto square4 = SFINAE::CONSTEXPR::Square(number_int); // Вызов Number<int> Square(Number<int>);
	}
    return 0;
}
