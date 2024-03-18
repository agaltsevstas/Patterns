#include "Singleton.h"

#include <iostream>

int main(int argc, const char * argv[]) 
{
    [[maybe_unused]] auto& singleton1 = Singleton1::Instance();
    [[maybe_unused]] auto& singleton2 = Singleton2::Instance();
    return 0;
}
