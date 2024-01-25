#include "Employee.hpp"

Director::Director() noexcept
{

}

void Director::PrintName() const noexcept
{
	std::cout << "class name: " << _name << std::endl;
}

const std::string& Director::GetName() const noexcept
{
	return _name;
}