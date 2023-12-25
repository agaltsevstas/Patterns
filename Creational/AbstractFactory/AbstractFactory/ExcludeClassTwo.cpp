#include "ExcludeFactoryClassTwo.h"

#include <iostream>
#include <string>
#include <cassert>

class ExcludeClassTwo : public IExcludeFactoryClassTwo
{
public:
	ExcludeClassTwo();

	void Start() override;
	void End() override;

private:
	std::string _str;
};

static ExcludeClassTwo excludeClassTwo;

ExcludeClassTwo::ExcludeClassTwo() :
	_str("ExcludeClassTwo")
{

}

void ExcludeClassTwo::Start()
{
	std::cout << _str << " started" << std::endl;
}

void ExcludeClassTwo::End()
{
	std::cout << _str << " ended" << std::endl;
}
