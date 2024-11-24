#include <iostream>

/*
 Сайты: https://pro-prof.com/forums/topic/cplusplus-iterator-pattern?ysclid=ltrh38jzc9189168674
 */

/*
 Итератор (iterator) - позволяет получить последовательный доступ к элементам объекта, не раскрывая его внутреннего представления (list, forward_list). Итератор может хранить «сырой указатель», таким образом преобразуя опасный интерфейс указателя на элемент контейнера в безопасный. Итератор может также хранить ссылку на объект, к которому он принадлежит – это позволяет выполнять сравнение между итераторами разных объектов. С помощью функций поиска find, find_if можно перебирать итераторы.
 Имеет методы:
 - получение следующего/предыдущего элемента c перегрузкой operator++/--(int).
 - доступ к значению элемента с перегрузкой operator*()/operator->().
 - сравнение итераторов c перегрузкой operator==/operator!=.
 */
template <class T>
class List
{
    class Iterator;
    friend class Iterator;
    struct Node
    {
        Node() = default;
        template <typename ...Args>
        Node(Node* iPrev, Node* iNext, Args&& ...args) :
        prev(iPrev),
        next(iNext),
        value(std::forward<Args>(args)...)
        {
            
        }
        
        T value;
        Node* prev = nullptr;
        Node* next = nullptr;
    };
    
public:
    List() = default;
    List(const std::initializer_list<T>& list);
    List(const Iterator& begin, const Iterator& end);
    List(const List& other);
    List(List&& other) noexcept;
    ~List();
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;
    bool operator==(const List& other);
    void Swap(List& other) noexcept;
    bool Empty() const noexcept;
    size_t Size() const noexcept;
    void Reverse();
    void Clear();
    
    Iterator Begin() const;
    Iterator End() const;
    Iterator Insert(const Iterator& it, const T& value);
    Iterator Erase(const Iterator& it);
    Iterator Erase(const Iterator& begin, const Iterator& end);
    
private:
    void Copy(const List& other)
    {
        Node* begin_other = other._begin;
        if (!begin_other)
            return;
        _begin = new Node(nullptr, nullptr, begin_other->value);
        _end = _begin;
        begin_other = begin_other->next;

        while (begin_other)
        {
            Node* tmpEnd = _end;
            _end->next = new Node();
            _end = _end->next;
            _end->prev = tmpEnd;
            _end->next = nullptr;
            _end->value = begin_other->value;
            begin_other = begin_other->next;
        }
    }
    
private:
    Node* _begin = nullptr;
    Node* _end = nullptr;
    size_t _size = 0;
};


template<typename T>
class List<T>::Iterator
{
    friend class List;
public:
    Iterator(const List& list, Node* node):
        _list(list)
    {

        _node = node;
    }

    inline Iterator& operator++()
    {
        _node = _node ? _node->next : nullptr;
        return *this;
    }

    inline Iterator operator++(int)
    {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    inline Iterator& operator--()
    {
        _node = _node ? _node->prev : nullptr;
        return *this;
    }

    inline Iterator operator--(int)
    {
        Iterator temp = *this;
        --(*this);
        return temp;
    }

    inline T& operator*()
    {
        if (!_node)
            throw std::runtime_error("iterator is null");
        return _node->value;
    }

    inline T& operator->() const
    {
        if (!_node)
            throw std::runtime_error("iterator is null");
        return _node->value;
    }

    inline Iterator& operator=(const Iterator& other)
    {
        if (&_list == &other._list)
            _node = other._node;
        return *this;
    }

    inline bool operator==(const Iterator& other) const
    {
        return &_list == &other._list && _node == other._node;
    }

    inline bool operator!=(const Iterator& other) const
    {
        return &_list != &other._list || _node != other._node;
    }

private:
    const List& _list;
    Node* _node = nullptr;
};


template <class T>
List<T>::List(const List<T>::Iterator& begin, const List<T>::Iterator& end)
{
    for (auto it = begin; it != end; ++it)
    {
        if (_end)
        {
            Node* node = new Node(_end, nullptr, *it);
            _end->next = node;
            _end = node;
        }
        else
        {
            _begin = _end = new Node(nullptr, nullptr, *it);
        }

        ++_size;
    }
}

template <class T>
List<T>::List(const List& other)
{
    Copy(other);
}

template <class T>
List<T>::List(List&& other) noexcept
{
    _begin = std::exchange(other._begin, nullptr);
    _end = std::exchange(other._end, nullptr);
    _size = std::exchange(other._size, 0);
}

template <class T>
List<T>::~List()
{
    Clear();
}

template <class T>
List<T>& List<T>::operator=(const List& other)
{
    if (this == &other) // object = object
        return *this;
    
    Clear();
    Copy(other);
    
    return *this;
}

template <class T>
List<T>& List<T>::operator=(List&& other) noexcept
{
    if (this == &other) // object = object
        return *this;
    
    _begin = std::exchange(other._begin, nullptr);
    _end = std::exchange(other._end, nullptr);
    _size = std::exchange(other._size, 0);
    
    return *this;
}

template <class T>
bool List<T>::operator==(const List& other)
{
    if (Size() != other.Size())
        return false;
    
    for (auto it = Begin(), it_other = other.Begin(); it != End(); ++it, ++it_other)
    {
        if (*it != *it_other)
            return false;
    }
    
    return true;
}

template <class T>
void List<T>::Swap(List& other) noexcept
{
    if (this == &other) // object.Swap(object)
        return;
    
    std::swap(_begin, other._begin);
    std::swap(_end, other._end);
    std::swap(_size, other._size);
}

template <class T>
bool List<T>::Empty() const noexcept
{
    return Size() == 0;
}

template <class T>
size_t List<T>::Size() const noexcept
{
    return _size;
}

template <class T>
void List<T>::Reverse()
{
    auto tmpEnd = _end;
    while (_begin != _end)
    {
      _end->prev = std::exchange(_end->next, _end->prev); // _end->prev = _end->next, _end->next = _end->prev
      _end = _end->next;
    }
    _begin->prev = std::exchange(_begin->next, _begin->prev); // _begin->prev = _begin->next, _begin->next = _begin->prev
    _begin = tmpEnd;
}

template <class T>
void List<T>::Clear()
{
    while (_begin)
    {
        Node* tmp = _begin->next;
        delete _begin;
        _begin = tmp;
    }
    
    _begin = nullptr;
    _end = nullptr;
    _size = 0;
}

template <class T>
List<T>::Iterator List<T>::Begin() const
{
    return Iterator(*this, _begin);
};

template <class T>
List<T>::Iterator List<T>::End() const
{
    return _end ? Iterator(*this, _end->next) : Iterator(*this, nullptr); // return nullptr!!!
};

template <class T>
List<T>::Iterator List<T>::Insert(const Iterator& it, const T& value)
{
    if (!it._node)
    {
        _begin = _end = new Node(nullptr, nullptr, value);
        return Iterator(*this, _end);
    }
    
    Node* tmp = it._node;
    Node* tmpPrev = it._node->prev;
    Node* tmpNext = it._node->next;
    Node* node = new Node(nullptr, nullptr, value);

    if (tmpPrev && tmpNext) // Вставка в середину
    {
        node->prev = tmpPrev;
        node->next = tmp;
        tmpPrev->next = node;
        tmp->prev = node;
    }
    else if (tmpNext) // Вставка в начало
    {
        node->next = tmp;
        tmp->prev = node;
    }
    else if (tmpPrev) // Вставка в конец
    {
        node->prev = tmpPrev;
        node->next = tmp;
        tmp->prev = node;
        tmpPrev->next = node;
    }
    else // Вставка после 1 элемента, если элемент 1
    {
        node->prev = tmp;
        tmp->next = node;
    }
    
    ++_size;
    return Iterator(*this, node);
}

template <class T>
List<T>::Iterator List<T>::Erase(const Iterator& it)
{
    if (!it._node)
        throw std::runtime_error("iterator is empty");
    else if (it._node == _begin)
    {
        Node* tmp = _begin;
        _begin = _begin->next;
        delete tmp;
        --_size;
        return Begin();
    }
    else if (it._node == _end)
    {
        Node* tmp = _end;
        _end = _begin->prev;
        delete tmp;
        --_size;
        return End();
    }
    
    Node* tmp = it._node;
    Node* tmpPrev = it._node->prev;
    Node* tmpNext = it._node->next;
    
    tmpPrev->next = tmpNext;
    tmpNext->prev = tmpPrev;
    delete tmp;
    --_size;
    return Iterator(*this, tmpNext);
}

template <class T>
List<T>::Iterator List<T>::Erase(const Iterator& begin, const Iterator& end)
{
    for (auto it = begin; it != end;)
        it = Erase(it);
    return end;
}



int main()
{
    List<int> list;
    try
    {
        list.Erase(list.Begin());
    }
    catch (const std::runtime_error& exception)
    {
        std::cout << exception.what() << std::endl;
    }
    
    list.Insert(list.Begin(), 0);
    list.Clear();
    list.Insert(++list.Begin(), 2);
    [[maybe_unused]] auto compare1 = list.Begin() == list.End();
    [[maybe_unused]] auto compare2 = list.Begin() != list.End();
    list.Swap(list);
    list = list;
    list = std::move(list);
    List<int> list2(list);
    List<int> list3;
    list3 = list;
    List<int> list4(std::move(list2));
    List<int> list5;
    list5 = std::move(list3);
    list4.Swap(list5);
    List<int> list6(list4.Begin(), list4.End());
    
    std::cout << "list: ";
    for (auto it = list.Begin(); it != list.End(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
    
    std::cout << "list reverse: ";
    list.Reverse();
    for (auto it = list.Begin(); it != list.End(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
    
    list.Erase(list.Begin());
    list.Erase(list.Begin(), list.End());
    
    return 0;
}
