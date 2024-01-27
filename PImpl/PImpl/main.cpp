#include "Employee.hpp"

#include <iostream>

/*
 PImpl (Pointer to private implementation) - указатель на частную реализацию, позволяющий избежать постоянной привязки абстракции к реализации и изменять их независимо друг от друга. Такая обертка хранится внутри уже существующего класса в виде указателя (forward declaration) в .h файле (хедере). Реализация находится в .cpp файле.
 Плюсы:
 - меньше тратится время на перекомпилирование .h файла. Ускорение сборки происходит из-за уменьшения количества директив #include в хедерах
 - доступ в pImpl осуществляется косвенно через указатель, пользователь не видит никаких деталей реализации класса
 Минусы:
 - тратится память на дополнительный класс
 - загромождение кода в .cpp файле
*/


int main()
{
    Director director;
    director.PrintName();
    auto& director_name = director.GetName();

    HRManager hrManager;
    hrManager.PrintName();
    auto& hrManager_name = hrManager.GetName();

    return 0;
}
