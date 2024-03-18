#include <iostream>

/*
 Сайты: https://www.geeksforgeeks.org/bridge-method-c-design-patterns/
 */

/*
 Мост (bridge) - обеспечивает отделение абстракции объекта от его реализации (кросс-платформенные приложения, поддержание разных баз данных):
 Абстракция: abstract class Figure от него наследуются class Rectangle, Circle. virtual method Draw();
 Реализация: По идее реализация должна быть внутри метода Draw() классов Rectangle, Circle. Однако, реализация выносится в отдельные классы RectangleRenderer, CircleRenderer, которые наследуются от abstract class Renderer с virtual method Render(). В конструктор Rectangle/Circle передается ссылка на экземпляр класса RectangleRenderer/CircleRenderer и внутри метода Draw() вызывается метод Render экземпляра класса RectangleRenderer/CircleRenderer.
Такое разделение позволяет изменениям в одной части системы оказывать минимальное влияние на другую и код становится легче поддерживать.
 */

class Figure
{
public:
    virtual void Draw() = 0;
};
 
class Renderer
{
public:
    virtual void Render() = 0;
};
 
class CircleRenderer : public Renderer
{
public:
    void Render() override
    {
        std::cout << "CircleRenderer" << std::endl;
    }
};
 
class SquareRenderer : public Renderer
{
public:
    void Render() override
    {
        std::cout << "SquareRenderer" << std::endl;
    }
};
 
class Circle : public Figure
{
public:
    Circle(Renderer& renderer) noexcept :
    _renderer(renderer)
    {
    }
 
    void Draw() override
    {
        std::cout << "Drawing: ";
        _renderer.Render();
    }
 
private:
    Renderer& _renderer;
};
 
class Square : public Figure
{
public:
    Square(Renderer& renderer) noexcept :
    _renderer(renderer)
    {
    }
 
    void Draw() override
    {
        std::cout << "Drawing: ";
        _renderer.Render();
    }
 
private:
    Renderer& _renderer;
};
 
int main()
{
    CircleRenderer circleRenderer;
    SquareRenderer squareRenderer;
 
    Circle circle(circleRenderer);
    Square square(squareRenderer);
 
    circle.Draw(); // Drawing: CircleRenderer
    square.Draw(); // Drawing: SquareRenderer
 
    return 0;
}
