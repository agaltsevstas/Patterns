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
    std::cout << "void_without_arguments" << std::endl;
}


int main()
{
    Timer timer;
    constexpr size_t size = 3000;
    
    /// first implementation
    {
        using namespace first_implementation;
        std::cout << "first_implementation" << std::endl;
        /// Threadpool
        {
            timer.start();
            {
                ThreadPool threadPool(size);
                for (size_t i = 0; i < size; ++i)
                {
                    threadPool.AddTask([i]()
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    });
                }
            }
            timer.stop();
            std::cout << "ThreadPool time: " << timer.elapsedMilliseconds() << " мс" << std::endl;
        }
        /// threads
        {
            timer.start();
            std::vector<std::thread> threads;
            threads.reserve(size);
            for (size_t i = 0; i < size; ++i)
            {
                threads.emplace_back([i]()
                {
                     std::this_thread::sleep_for(std::chrono::milliseconds(10));
                });
            }
            
            for (auto& thread : threads)
            {
                thread.join();
            }
            timer.stop();
            std::cout << "threads time: " << timer.elapsedMilliseconds() << " мс" << std::endl;
        }
    }
    /// second implementation
    {
        using namespace second_implementation;
        std::cout << "second_implementation" << std::endl;
        
        ThreadPool pool(3);
        pool.AddTask(int_sum, 1, 2);
        int sum = 0;
        pool.AddTask(void_sum, 1, 2, sum);
        pool.AddTask(void_without_arguments);
        {
            // variant 1
            int res;
            pool.WaitResult(0, res);
            std::cout << res << std::endl;

            // variant 2
            std::cout << std::any_cast<int>(pool.WaitResult(0)) << std::endl;
        }

        pool.Wait(1);
        std::cout << sum << std::endl;
        pool.WaitAll(); // waiting for task with id 2
    }
            
    return 0;
}
