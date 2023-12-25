#ifndef Employee_h
#define Employee_h

#include <iostream>

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
    virtual std::string GetName() const noexcept = 0;
    
protected:
    std::string _name;
    
private:
    int _id = -1;
    int _number = -1;
};

class Employee1 : public Employee
{
public:
    using Employee::Employee; // Заменяет конструктор дочернего класса
//    Employee1(int number, const std::string& name) : Employee(number, name){}
    std::string GetName() const noexcept override
    {
        return _name;
    }
};

class Employee2 : public Employee
{
public:
    using Employee::Employee; // Заменяет конструктор дочернего класса
//    Employee2(int number, const std::string& name) : Employee(number, name){}
    
    std::string GetName() const noexcept override
    {
        return _name;
    }
    
private:
    int _number = 0;
};

class Employee3 : public Employee
{
public:
    using Employee::Employee; // Заменяет конструктор дочернего класса
//    Employee3(int number, const std::string& name) : Employee(number, name){}
    
    std::string GetName() const noexcept override
    {
        return _name;
    }
    
private:
    std::string _str;
};

class Employee4 : public Employee
{
public:
    using Employee::Employee; // Заменяет конструктор дочернего класса
//    Employee4(int number, const std::string& name) : Employee(number, name){}
    
    std::string GetName() const noexcept override
    {
        return _name;
    }
    
private:
    int _number = 0;
    std::string _str;
};

#endif /* Employee_h */
