#pragma once

class IExcludeFactoryClassTwo
{
public:
	static IExcludeFactoryClassTwo* Instance();

	virtual void Start() = 0;
	virtual void End() = 0;

protected:
	IExcludeFactoryClassTwo();

	virtual ~IExcludeFactoryClassTwo();

private:
	static IExcludeFactoryClassTwo* _instance;
};
