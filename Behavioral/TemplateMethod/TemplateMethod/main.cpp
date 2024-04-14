#include <iostream>

/*
 Шаблонный метод - метод в абстрактном классе, определяющий структуру алгоритма и состоящего из нескольких шагов: вызов некоторых методов в абстрактном классе и вызовов чисто виртуальных методов, реализованных в производных классах.
 Пример: есть абстрактный класс Figure, от него наследуются производные классы Square, Rectangle. Шаблонный метод в Figure вызывает чисто виртуальные методы вычисления периметра, площади и объема, реализованные в производных классах Square, Rectangle. После вызова метода Template Method и всех расчетов в нем можно получить результаты.
 */

/*
 Абстрактный класс определяет шаблонный метод, содержащий скелет алгоритма, состоящего из вызовов абстрактных методов, переопределенных в производных классах
 */
class Figure
{
public:
    // Шаблонный метод определяет скелет алгоритма
    void TemplateMethod()
    {
        Perimeter();
        Area();
        Volume();
    }
    virtual size_t GetPerimeter() const noexcept = 0;
    virtual size_t GetArea() const noexcept = 0;
    virtual size_t GetVolume() const noexcept = 0;
protected:
    // Операции должны быть реализованы в производных классах
    virtual void Perimeter() = 0;
    virtual void Area() = 0;
    virtual void Volume() = 0;
};

/*
 Производный класс Square реализует виртуальные методы, использующиеся в Template Method
 */
class Square : public Figure
{
public:
    Square(size_t iSide) noexcept :
    _side(iSide)
    {}
    
    size_t GetPerimeter() const noexcept override
    {
        return _perimeter;
    }
    
    size_t GetArea() const noexcept override
    {
        return _area;
    }
    
    size_t GetVolume() const noexcept override
    {
        return _volume;
    }
    
protected:
    void Perimeter() override
    {
        _perimeter = 4 * _side;
    }
    
    void Area() override
    {
        _area = static_cast<size_t>(std::pow(_side, 2u));
    }
    
    void Volume() override
    {
        _volume = static_cast<size_t>(std::pow(_side, 3u));
    }
 
private:
    size_t _side = 0;
    size_t _perimeter = 0;
    size_t _area = 0;
    size_t _volume = 0;
};

/*
 Производный класс Rectangle реализует виртуальные методы, использующиеся в Template Method
 */
class Rectangle : public Figure
{
public:
    Rectangle(size_t iA, size_t iB, size_t iC) noexcept :
    _a(iA),
    _b(iB),
    _c(iC)
    {}
    
    size_t GetPerimeter() const noexcept override
    {
        return _perimeter;
    }
    
    size_t GetArea() const noexcept override
    {
        return _area;
    }
    
    size_t GetVolume() const noexcept override
    {
        return _volume;
    }
    
protected:
    void Perimeter() override
    {
        _perimeter = 2 * (_a + _b);
    }
    
    void Area() override
    {
        _area = _a * _b;
    }
    
    void Volume() override
    {
        _volume = _a * _b * _c;
    }
 
private:
    size_t _a = 0, _b = 0, _c = 0;
    size_t _perimeter = 0;
    size_t _area = 0;
    size_t _volume = 0;
};

/*
 Создаются производные классы, вызывается метод Template Method, в котором вычисляются периметр, площадь и объем и получаем эти значения.
 */
int main()
{
    Square square(4u);
    square.TemplateMethod();
    auto perimeterSquare = square.GetPerimeter();
    auto areaSquare = square.GetArea();
    auto volumeSquare = square.GetVolume();
 
    Rectangle rectangle(1u, 2u, 3u);
    rectangle.TemplateMethod();
    auto perimeterRectangle = rectangle.GetPerimeter();
    auto areaRectangle = rectangle.GetArea();
    auto volumeRectangle = rectangle.GetVolume();
    return 0;
}
