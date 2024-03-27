#include <iostream>
#include <string>

#include "AbstractFactory.h"

class FactoryClassTwo : public IAbstractFactory
{
public:
	FactoryClassTwo();
	void Start() override;
	void End() override;

private:
	std::string _str;
};

CREATOR_REGISTRATION(FactoryClassTwo);
//static FactoryClassTwo factoryClassTwo;

FactoryClassTwo::FactoryClassTwo() :
	_str("FactoryClassTwo")
{
	Register("FactoryClassTwo", this);
}

void FactoryClassTwo::Start()
{
	std::cout << _str << " started" << std::endl;
}

void FactoryClassTwo::End()
{
	std::cout << _str << " ended" << std::endl;
}
