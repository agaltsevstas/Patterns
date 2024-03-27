#include <iostream>
#include "AbstractFactory.h"
#include "ExcludeFactoryClassOne.h"
#include "ExcludeFactoryClassTwo.h"


int main()
{
	auto* excludeFactoryClassOne = IExcludeFactoryClassOne::Instance();
	if (excludeFactoryClassOne)
	{
		excludeFactoryClassOne->Start();
		excludeFactoryClassOne->End();
	}

	auto* excludeFactoryClassTwo = IExcludeFactoryClassTwo::Instance();
	if (excludeFactoryClassTwo)
	{
		excludeFactoryClassTwo->Start();
		excludeFactoryClassTwo->End();
	}

	IAbstractFactory* factoryClassOne = IAbstractFactory::GetInstance("FactoryClassOne");
	if (factoryClassOne)
	{
		factoryClassOne->Start();
		factoryClassOne->End();
	}

	IAbstractFactory* factoryClassTwo = IAbstractFactory::GetInstance("FactoryClassTwo");
	if (factoryClassTwo)
	{
		factoryClassTwo->Start();
		factoryClassTwo->End();
	}
    
	return 0;
}
