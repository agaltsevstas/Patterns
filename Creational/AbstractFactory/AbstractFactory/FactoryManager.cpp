#include "AbstractFactory.h"

#include "ExcludeFactoryClassOne.h"
#include "ExcludeFactoryClassTwo.h"

ICREATOR_REGISTRATION(IExcludeFactoryClassOne)

IAbstractFactory::FactoryMap IAbstractFactory::_factoryMap;
void IAbstractFactory::Register(std::string clientName, IAbstractFactory* factory)
{
    _factoryMap.insert(FactoryMap::value_type(clientName, factory));
}

IAbstractFactory* IAbstractFactory::GetInstance(std::string client)
{
    return _factoryMap.find(client)->second;
}
