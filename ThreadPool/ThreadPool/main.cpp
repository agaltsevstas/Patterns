#include "ThreadPool.h"
#include "Timer.h"

#include <iostream>

/*
 Видео: https://www.youtube.com/watch?v=-TuJP8pUBW0&list=WL&index=3&pp=gAQBiAQB
 Сайт: https://github.com/sajeruk/ThreadPool/tree/5a3124f23d33007447a45f6c8710b242ed8c6e89
 */

/*
 Сайты: https://github.com/YFatMR/Habr/blob/main/thread_pool/best_version.cpp
 */


int int_sum(int lhs, int rhs)
{
    return lhs + rhs;
}

void void_sum(int lhs, int rhs, int& sum)
{
    sum = lhs + rhs;
}

void void_without_arguments()
{
    // std::cout << "void_without_arguments" << std::endl;
}

template <class TPool>
class Example
{
public:
    void Function()
    {
        _pool.AddTask(&Example::_Function, this);
    }
    
private:
    void _Function()
    {

    }
    
    TPool _pool;
};


int main()
{
    Timer timer;
    constexpr size_t size = 3000;
    
    /// first implementation
    {
        using namespace first_implementation;
        std::cout << "first implementation" << std::endl;
        
        ThreadPool pool;
        timer.start();
        {
            pool.AddTask(int_sum, 1, 2);
            int sum = 0;
            pool.AddTask(void_sum, 1, 2, sum);
            pool.AddTask(void_without_arguments);
            
            for (size_t i = 0; i < size; ++i)
            {
                pool.AddTask([i]()
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                });
            }
        }
        timer.stop();
        std::cout << "ThreadPool time: " << timer.elapsedMilliseconds() << " мс" << std::endl;
        
        Example<ThreadPool> example;
        example.Function();
    }
    /// second implementation
    {
        using namespace second_implementation;
        std::cout << "second implementation" << std::endl;
        
        ThreadPool pool;
        
        timer.start();
        {
            pool.AddTask(int_sum, 1, 2);
            int sum = 0;
            pool.AddTask(void_sum, 1, 2, sum);
            pool.AddTask(void_without_arguments);
            
            for (size_t i = 0; i < size; ++i)
            {
                pool.AddTask([]()
                {
                     std::this_thread::sleep_for(std::chrono::milliseconds(1));
                });
            }
        }
        timer.stop();
        std::cout << "ThreadPool time: " << timer.elapsedMilliseconds() << " мс" << std::endl;
        
        /// Не работает: не переделывать
        /*
         Example<ThreadPool> example;
         example.Function();
         */
    }
    /// third implementation
    {
        using namespace third_implementation;
        std::cout << "third implementation" << std::endl;
        
        ThreadPool pool;
        timer.start();
        {
            pool.AddTask(int_sum, 1, 2);
            int sum = 0;
            pool.AddTask(void_sum, 1, 2, sum);
            pool.AddTask(void_without_arguments);
            
            /// Не работает: не переделывать
            /*
             for (size_t i = 0; i < size; ++i)
             {
                 pool.AddTask([]()
                 {
                     std::this_thread::sleep_for(std::chrono::milliseconds(1));
                 });
             }
             */
            
        }
        timer.stop();
        std::cout << "ThreadPool time: " << timer.elapsedMilliseconds() << " мс" << std::endl;
        
        {
            // variant 1
            int res;
            pool.WaitResult(0, res);
            std::cout << res << std::endl;

            // variant 2
            std::cout << std::any_cast<int>(pool.WaitResult(0)) << std::endl;
        }

        pool.Wait(1);
        pool.WaitAll(); // waiting for task with id 2
        
        /// Не работает: не переделывать
        /*
         Example<ThreadPool> example;
         example.Function();
         */
    }
            
    return 0;
}
