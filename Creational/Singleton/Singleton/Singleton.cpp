#include "Singleton.h"

#include <iostream>

int main(int argc, const char * argv[]) 
{
    auto& singleton1 = Singleton1::Instance();
    auto& singleton2 = Singleton2::Instance();
    return 0;
}
