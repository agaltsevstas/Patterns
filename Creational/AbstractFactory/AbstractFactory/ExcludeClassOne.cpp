#include "ExcludeFactoryClassOne.h"


class ExcludeClassOne : public IExcludeFactoryClassOne
{
public:
	ExcludeClassOne();

	void Start() override;
	void End() override;

private:
	std::string _str;
};

CREATOR_REGISTRATION(ExcludeClassOne);
//static ExcludeClass excludeClassOne;

ExcludeClassOne::ExcludeClassOne() :
	_str("ExcludeClassOne")
{

}

void ExcludeClassOne::Start()
{
	std::cout << _str << " started" << std::endl;
}

void ExcludeClassOne::End()
{
	std::cout << _str << " ended" << std::endl;
}
