#include "ObjectPool.h"

#include <iostream>

int main()
{
    ObjectPool<std::string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    try 
    {
        pool.Deallocate(p2);
        pool.Deallocate(p3);
        pool.Deallocate(p1);
        pool.Deallocate(p1);
    } 
    catch (const std::invalid_argument& exception)
    {
        std::cout << exception.what() << std::endl;
    }
    return 0;
}
