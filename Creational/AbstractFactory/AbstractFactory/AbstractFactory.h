#pragma once

#include <cassert>
#include <map>
#include <string>

class IBase
{
protected:
    IBase() = default;
    IBase(const IBase&) = delete;
    IBase& operator=(const IBase&) = delete;
    ~IBase() = default;
};

/// Декларация интерфейсного конструктора
#define ICREATOR_DECLARATION(className)\
        private: static className* _instance;\
        public: static className* Instance(){return _instance;}\
        protected: className(){assert(_instance == nullptr); _instance = this;}

        /// Реализация конструктора
#define ICREATOR_REGISTRATION(className)\
        className* className::_instance = nullptr;

        /// Реализация конструктора
#define CREATOR_REGISTRATION(className)\
        static className _instance##className;


class IAbstractFactory : public IBase
{
    using FactoryMap = std::map<std::string, IAbstractFactory*>;

public:
	//static IAbstractFactory* Instance() noexcept;
    static IAbstractFactory* GetInstance(std::string className);

	virtual void Start() = 0;
	virtual void End() = 0;
protected:
    void Register(std::string className, IAbstractFactory* factoryClass);
	//IAbstractFactory() noexcept;

	//virtual ~IAbstractFactory();
private:
    static FactoryMap _factoryMap;
	//static IAbstractFactory* _instance;
};

