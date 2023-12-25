#pragma once

class IExcludeFactoryClassTwo
{
public:
	static IExcludeFactoryClassTwo* Instance() noexcept;

	virtual void Start() = 0;
	virtual void End() = 0;

protected:
	IExcludeFactoryClassTwo() noexcept;

	virtual ~IExcludeFactoryClassTwo();

private:
	static IExcludeFactoryClassTwo* _instance;
};
