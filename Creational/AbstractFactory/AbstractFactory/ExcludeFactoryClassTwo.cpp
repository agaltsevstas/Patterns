#include "ExcludeFactoryClassTwo.h"

#include "cassert"

IExcludeFactoryClassTwo* IExcludeFactoryClassTwo::_instance = nullptr;
IExcludeFactoryClassTwo* IExcludeFactoryClassTwo::Instance() noexcept
{
	return _instance;
}

IExcludeFactoryClassTwo::IExcludeFactoryClassTwo() noexcept
{
	assert(_instance == nullptr); _instance = this;
}

IExcludeFactoryClassTwo::~IExcludeFactoryClassTwo()
{

}
