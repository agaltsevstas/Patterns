#ifndef Singleton_h
#define Singleton_h

/*
 Миксины (Mixin): NonCopyable, NonMoveable
 */
struct NonCopyable
{
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

struct NonMoveable
{
    NonMoveable() = default;
    NonMoveable(NonMoveable&&) noexcept = delete;
    NonMoveable& operator=(NonCopyable&&) noexcept = delete;
};

/*
 Синглтон Майерса + CRTP
 */
template<class Derived>
struct Singleton : private NonCopyable, private NonMoveable // Делаем приватные конструкторы базовых классов
{
    static Derived &Instance()
    {
        static Derived instance;
        return instance;
    }
    
protected:
    Singleton() = default;
};

struct Singleton1 : public Singleton<Singleton1>{};
struct Singleton2 : public Singleton<Singleton2>{};

#endif /* Singleton_h */
