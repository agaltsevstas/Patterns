#include "Database.h"

#include <iostream>

/*!
 * @brief Фасад (Fasade) - позволяющий скрыть сложность системы путём сведения всех возможных внешних вызовов к одному объекту, делегирующему их соответствующим объектам системы
 * унифицированный интерфейс для набора интерфейсов в подсистеме. Фасад определяет интерфейс более высокого уровня, который упрощает использование подсистемы. Это можно использовать для упрощения ряда сложных взаимодействий с объектами в едином интерфейсе.
 * предоставить более простой интерфейс для всей подсистемы, состоящей из множества объектов
 */
int main(int argc, const char * argv[])
{
    auto& db = DatabaseFacade::Instance();
    if (!db.Load())
        return EXIT_FAILURE;
    
    if (!db.Connection())
        return EXIT_FAILURE;
    
    if (!db.IsOpen())
        return EXIT_FAILURE;
    
    
    db.Delete();
    db.Disconnect();
    
    
    return 0;
}
