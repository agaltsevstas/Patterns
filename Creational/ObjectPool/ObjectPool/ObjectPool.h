#ifndef ObjectPool_h
#define ObjectPool_h

#include <queue>
#include <unordered_map>

template <class T>
class ObjectPool
{
public:
    T* Allocate();
    T* TryAllocate();
    void Deallocate(T* object);
    
private:
    std::queue<std::unique_ptr<T>> _free;
    std::unordered_map<T*, std::unique_ptr<T>> _allocated;
};

template <class T>
T* ObjectPool<T>::Allocate()
{
    if (_free.empty())
        _free.push(std::make_unique<T>());
    
    auto ptr = _free.front().get();
    _allocated.insert({ptr, std::move(_free.front())});
    _free.pop();
    return ptr;
}

template <class T>
T* ObjectPool<T>::TryAllocate()
{
    if (_free.empty())
        return nullptr;
    
    return Allocate();
}

template <class T>
void ObjectPool<T>::Deallocate(T* object)
{
    auto it = _allocated.find(object);
    if (it == _allocated.end())
        throw std::invalid_argument(typeid(object).name());
    
    _free.push(std::move(it->second));
    _allocated.erase(it);
}

#endif /* ObjectPool_h */
