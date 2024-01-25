#ifndef Employee_hpp
#define Employee_hpp

#include <iostream>


class Employee
{
public:
    virtual void PrintName() const noexcept = 0;
    virtual ~Employee() = default;
};

class Director : public Employee
{
public:
    Director() noexcept;
    void PrintName() const noexcept override;
    const std::string& GetName() const noexcept;

private:
    std::string _name;
};

class HRManager : public Employee
{
public:
    HRManager() noexcept;
    void PrintName() const noexcept override;
    const std::string& GetName() const noexcept;

private:
    class Impl;
    /*
      std::unique_ptr - является полным типом а Impl - нет, поэтому при удалении необходимо явно определить деструктор, для этого используется пользовательский deleter
    */
    class Deleter
    {
    public:
        void operator()(Impl*);
    };
    std::unique_ptr<Impl, Deleter> _impl;
};


#endif /* Employee_hpp */
