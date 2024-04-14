#include <iostream>
#include <mutex>
#include <thread>


/*
 Сайты: https://stackoverflow.com/questions/6481260/non-virtual-interface-design-pattern-in-c-c
        https://xakep.ru/2010/11/01/53649/
 */

/*
 NVI (non virtual interface) - шаблона невиртуального интерфейса использует public невиртуальные методы в качестве обертки над вызовами private/protected виртуальных методов. NVI - частный случай паттерна — «Шаблонный метод».
 Плюсы:
 - базовый класс (интерфейс) имеет больше контроля над своим поведением, чем производные классы (например, вывод в файл или экран не нужно дублировать в каждой реализации производного класса: достаточно вынести вызов виртуального метода в невиртуальный метод + дополнив его).
 Минусы:
 - лишние методы.
 */

class Figure
{
public:
    void GetPerimeter()
    {
        std::lock_guard lock(_mutex);
        std::cout << "Perimeter: " << Perimeter() << std::endl;
    }
    
    void GetArea()
    {
        std::lock_guard lock(_mutex);
        std::cout << "Area: " << Area() << std::endl;
    }
    
    void GetVolume()
    {
        std::lock_guard lock(_mutex);
        std::cout << "Volume: " << Volume() << std::endl;
    }
private:
    virtual size_t Perimeter() const noexcept = 0;
    virtual size_t Area() const noexcept = 0;
    virtual size_t Volume() const noexcept = 0;
    
private:
    std::mutex _mutex;
};

class Square : public Figure
{
public:
    Square(size_t iSide) noexcept :
    _side(iSide)
    {}
    
private:
    size_t Perimeter() const noexcept override
    {
        return 4 * _side;
    }
    
    size_t Area() const noexcept override
    {
        return static_cast<size_t>(std::pow(_side, 2u));
    }
    
    size_t Volume() const noexcept override
    {
        return static_cast<size_t>(std::pow(_side, 3u));
    }
 
private:
    size_t _side = 0;
};


class Rectangle : public Figure
{
public:
    Rectangle(size_t iA, size_t iB, size_t iC) noexcept :
    _a(iA),
    _b(iB),
    _c(iC)
    {}
    
private:
    size_t Perimeter() const noexcept override
    {
        return 2 * (_a + _b);
    }
    
    size_t Area() const noexcept override
    {
        return _a * _b;
    }
    
    size_t Volume() const noexcept override
    {
        return _a * _b * _c;
    }
 
private:
    size_t _a = 0, _b = 0, _c = 0;
};


int main()
{
    Square square(4u);
    Rectangle rectangle(1u, 2u, 3u);

    std::thread thread1([&square]() 
    {
        square.GetPerimeter();
        square.GetArea();
        square.GetVolume();
    });
    
    std::thread thread2([&rectangle]()
    {
        rectangle.GetPerimeter();
        rectangle.GetArea();
        rectangle.GetVolume();
    });

    thread1.join();
    thread2.join();
    return 0;
}
