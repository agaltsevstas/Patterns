#include "ExcludeFactoryClassTwo.h"

#include "cassert"

IExcludeFactoryClassTwo* IExcludeFactoryClassTwo::_instance = nullptr;
IExcludeFactoryClassTwo* IExcludeFactoryClassTwo::Instance()
{
	return _instance;
}

IExcludeFactoryClassTwo::IExcludeFactoryClassTwo()
{
	assert(_instance == nullptr); _instance = this;
}

IExcludeFactoryClassTwo::~IExcludeFactoryClassTwo()
{

}
