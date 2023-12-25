#include <iostream>
#include <string>

#include "AbstractFactory.h"

class FactoryClassOne : public IAbstractFactory
{
public:
	FactoryClassOne();
	void Start() override;
	void End() override;

private:
	std::string _str;
};

CREATOR_REGISTRATION(FactoryClassOne);
//static FactoryClassOne factoryClassOne;

FactoryClassOne::FactoryClassOne() :
	_str("FactoryClassOne")
{
	Register("FactoryClassOne", this);
}

void FactoryClassOne::Start()
{
	std::cout << _str << " started" << std::endl;
}

void FactoryClassOne::End()
{
	std::cout << _str << " ended" << std::endl;
}
