#include <iostream>
#include <string>

#include "ObjectFactory.h"

namespace ObjectFactorySpace
{
    class Employee
    {
    public:
        Employee(int number, const std::string& name):
        _number(number),
        _name(name)
        {
            std::cout << "Employee" << std::endl;
        }
        
        virtual ~Employee() {}
        virtual std::string GetName() const = 0;
        
    protected:
        std::string _name;
        
    private:
        int _number = -1;
    };

    class Employee1 : public Employee
    {
    public:
        Employee1() :
        Employee(1, "Employee1")
        {
            std::cout << "Employee1" << std::endl;
        }
        
        std::string GetName() const override
        {
            return _name;
        }
    };

    class Employee2 : public Employee
    {
    public:
        Employee2(int number) :
        Employee(2, "Employee2"),
        _number(number)
        {
            std::cout << "Employee2" << std::endl;
        }
        
        std::string GetName() const override
        {
            return _name;
        }
        
    private:
        int _number = 0;
    };

    class Employee3 : public Employee
    {
    public:
        Employee3(const std::string& str) :
        Employee(3, "Employee3"),
        _str(str)
        {
            std::cout << "Employee3" << std::endl;
        }
        
        std::string GetName() const override
        {
            return _name;
        }
        
    private:
        std::string _str;
    };

    class Employee4 : public Employee
    {
    public:
        Employee4(int number, const std::string& str) :
        Employee(4, "Employee4"),
        _number(number),
        _str(str)
        {
            std::cout << "Employee4" << std::endl;
        }
        
        std::string GetName() const override
        {
            return _name;
        }
        
    private:
        int _number = 0;
        std::string _str;
    };
}

namespace ClassFactorySpace
{
    class Animal
    {
    public:
        enum Gender
        {
            Male = 1,
            Female = 2
        };
        Animal(Gender gender, std::string name) :
        _gender(gender),
        _name(name)
        {}
        virtual std::string GetName() const = 0;
        
    protected:
        Gender _gender;
        std::string _name;
    };

    class Dog : public Animal
    {
    public:
        using Animal::Animal; // Заменяет конструктор дочернего класса
//        Dog(bool isAlive, std::string name) : Animal(isAlive, name){}
        std::string GetName() const override
        {
            return _name;
        }
    };

    class Cat : public Animal
    {
    public:
        using Animal::Animal; // Заменяет конструктор дочернего класса
//        Cat(bool isAlive, std::string name) : Animal(isAlive, name){}
        std::string GetName() const override
        {
            return _name;
        }
    };
}


int main(int argc, const char * argv[])
{
    // ObjectFactory
    {
        using namespace ObjectFactorySpace;
        auto& objectFactory = ObjectFactory<std::string, Employee>::Instance(); // Фабрика объектов производных классов от Employee
        objectFactory.Add<Employee1>("Employee1");
        objectFactory.Add<Employee2>("Employee2", 2);
        objectFactory.Add<Employee3>("Employee3", "3");
        objectFactory.Add<Employee4>("Employee4", 4, "4");
        
        if (objectFactory.IsRegistered("Employee1"))
        {
            if (auto employee1 = objectFactory.Create<Employee1>("Employee1"))
                employee1->GetName();
        }
        if (objectFactory.IsRegistered("Employee2"))
        {
            if (auto employee2 = objectFactory.Create<Employee2>("Employee4", 2))
                employee2->GetName();
        }
        if (objectFactory.IsRegistered("Employee3"))
        {
            if (auto employee3 = objectFactory.Create<Employee3>("Employee3", "3Copy"))
                employee3->GetName();
        }
        if (objectFactory.IsRegistered("Employee4"))
        {
            if (auto employee4 = objectFactory.Create<Employee4>("Employee4", 4, "4"))
                employee4->GetName();
        }
        
        if (auto employee1 = objectFactory.Get("Employee1")) // Получить сырой указатель, на существующий класс
            std::cout << "Класс: " << typeid(*employee1).name() << " существует" << std::endl;
        else
            std::cout << "Класс: " << "Employee1 НЕ существует" << std::endl;
        
        if (auto employee5 = objectFactory.Get("Employee5")) // Получить сырой указатель, на существующий класс
            std::cout << "Класс: " << typeid(*employee5).name() << " существует" << std::endl;
        else
            std::cout << "Класс: " << "Employee5 НЕ существует" << std::endl;
        
        if (objectFactory.IsRegistered("Employee1"))
        {
            objectFactory.Remove("Employee1");
            if (auto employee1 = objectFactory.Get("Employee1")) // Получить сырой указатель, на существующий класс
                std::cout << "Класс: " << typeid(*employee1).name() << " существует" << std::endl;
            else
                std::cout << "Класс: " << "Employee1 НЕ существует" << std::endl;
        }
    }
    // ClassFactory
    {
        using namespace ClassFactorySpace;
        
        auto& classFactory = ClassFactory<std::string, Animal, Animal::Gender, std::string>::Instance();
        classFactory.Add<Dog>(std::string("Dog"));
        classFactory.Add<Cat>(std::string("Cat"));
        
        // 1 Способ создания объекта
        {
            if (auto dog1 = classFactory.Create("Dog", Animal::Gender::Male, "Dog1"))
                std::cout << dog1->GetName() << std::endl;
            if (auto dog2 = classFactory.Create("Dog", Animal::Gender::Female, "Dog2"))
                std::cout << dog2->GetName() << std::endl;
            if (auto cat1 = classFactory.Create("Cat", Animal::Gender::Female, "Cat1"))
                std::cout << cat1->GetName() << std::endl;
            if (auto cat2 = classFactory.Create("Cat", Animal::Gender::Male, "Cat2"))
                std::cout << cat2->GetName() << std::endl;
        }
        
        // 2 Способ создания объекта
        {
            if (auto dog1 = classFactory.Get("Dog")(Animal::Gender::Male, "Dog1"))
                std::cout << dog1->GetName() << std::endl;
            if (auto dogClass = classFactory.Get("Dog"))
            {
                if (auto dog2 = dogClass(Animal::Gender::Male, "Dog2"))
                    std::cout << dog2->GetName() << std::endl;
            }
            if (auto dog2 = classFactory.Get("Dog")(Animal::Gender::Female, "Dog2"))
                std::cout << dog2->GetName() << std::endl;
            if (auto cat1 = classFactory.Get("Cat")(Animal::Gender::Female, "Cat1"))
                std::cout << cat1->GetName() << std::endl;
            if (auto catClass = classFactory.Get("Cat"))
            {
                if (auto cat2 = catClass(Animal::Gender::Male, "Cat2"))
                    std::cout << cat2->GetName() << std::endl;
            }
        }
    }
    
    return 0;
}
