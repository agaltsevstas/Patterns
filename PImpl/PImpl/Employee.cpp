#include "Employee.hpp"

Director::Director()
{

}

void Director::PrintName() const
{
	std::cout << "class name: " << _name << std::endl;
}

const std::string& Director::GetName() const
{
	return _name;
}
