#pragma once
#include <iostream>
#include <string>
#include <cassert>

#include "AbstractFactory.h"

/// Интерфейс для вызова отдельных классов вне абстрактоной фабрики
class IExcludeFactoryClassOne : public IBase
{
public:
	virtual void Start() = 0;
	virtual void End() = 0;

	//static ExcludeFactoryClass* Instance();

protected:
	//ExcludeFactoryClass() { assert(_instance == nullptr); _instance = this; }

private:
	//static ExcludeFactoryClass* _instance;

	ICREATOR_DECLARATION(IExcludeFactoryClassOne)
};
