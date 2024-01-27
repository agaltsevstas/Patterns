#include "Employee.hpp"

/*
 PImpl (Pointer to private implementation) - указатель на частную реализацию, позволяющий избежать постоянной привязки абстракции к реализации и изменять их независимо друг от друга. Такая обертка хранится внутри уже существующего класса в виде указателя (forward declaration) в .h файле (хедере). Реализация находится в .cpp файле.
 Плюсы:
 - меньше тратится время на перекомпилирование .h файла. Ускорение сборки происходит из-за уменьшения количества директив #include в хедерах
 - доступ в pImpl осуществляется косвенно через указатель, пользователь не видит никаких деталей реализации класса
 Минусы:
 - тратится память на дополнительный класс
 - загромождение кода в .cpp файле
*/


class HRManager::Impl
{
public:
    Impl() noexcept;
    ~Impl() = default;

    void PrintName() const noexcept;
    const std::string& GetName() const noexcept;

private:
    std::string _name;
};


HRManager::Impl::Impl() noexcept :
    _name("HRManager")
{

}

void HRManager::Impl::PrintName() const noexcept
{
    std::cout << "class name: " << _name << std::endl;
}

const std::string& HRManager::Impl::GetName() const noexcept
{
    return _name;
}

HRManager::HRManager() noexcept : _impl(new Impl())
{

}

void HRManager::PrintName() const noexcept
{
    _impl->PrintName();
}

const std::string& HRManager::GetName() const noexcept
{
    return _impl->GetName();
}

/*
  std::unique_ptr - является полным типом а Impl - нет, поэтому при удалении необходимо явно определить деструктор, для этого используется пользовательский deleter
*/
void HRManager::Deleter::operator()(Impl* impl)
{
    delete impl;
}
