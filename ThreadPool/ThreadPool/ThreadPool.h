#ifndef ThreadPool_h
#define ThreadPool_h

#include <any>
#include <atomic>
#include <iostream>
#include <cassert>
#include <chrono>
#include <functional>
#include <future>
#include <queue>
#include <mutex>
#include <map>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>


/*
 Сайты: https://github.com/progschj/ThreadPool
 */
namespace first_implementation
{
    class ThreadPool
    {
    public:
        ThreadPool() : ThreadPool(10)
        {
            
        }
        
        explicit ThreadPool(size_t size):
        _stop(false)
        {
            for (decltype(size) i = 0; i < size; ++i)
            {
                _workers.emplace_back(
                    [this]
                    {
                        while (!_stop)
                        {
                            std::function<void()> task;
                            {
                                std::unique_lock lock(_mutex);
                                _cv.wait(lock, [this]{ return _stop || !_tasks.empty(); });
                                if (_stop && _tasks.empty())
                                    return;
                                
                                    task = std::move(_tasks.front());
                                    _tasks.pop();
                            }
                            
                            if (task)
                                task();
                        }
                    }
                );
            }
        }
        
        template<class TFunction, class... TArgs>
        auto AddTask(TFunction&& function, TArgs&&... args) -> std::future<typename std::invoke_result_t<TFunction, TArgs...>>
        {
            using Result = typename std::invoke_result<TFunction, TArgs...>::type;

            auto task = std::make_shared<std::packaged_task<Result()>>
            (
                std::bind(std::forward<TFunction>(function), std::forward<TArgs>(args)...)
            );
                
            std::future<Result> result = task->get_future();
            {
                std::unique_lock lock(_mutex);
                assert(!_stop);
                _tasks.emplace([task](){ return (*task)(); });
            }
            
            _cv.notify_one();
            return result;
        }
        
        ~ThreadPool()
        {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _stop = true;
            }
            
            _cv.notify_all();
            
            for (auto& worker: _workers)
                worker.join();
        }
    private:
        std::atomic<bool> _stop;
        std::mutex _mutex;
        std::vector<std::thread> _workers;
        std::queue<std::function<void()>> _tasks;
        std::condition_variable _cv;
    };
}

/*
 Видео: https://www.youtube.com/watch?v=-TuJP8pUBW0&list=WL&index=3&pp=gAQBiAQB
 Сайт: https://github.com/sajeruk/ThreadPool/tree/5a3124f23d33007447a45f6c8710b242ed8c6e89
 */
namespace second_implementation
{
    class ThreadPool 
    {
        class Worker
        {
        public:
            Worker(ThreadPool& pool):
            _pool(pool)
            {
                _pool._waitTask.notify_one();
            }
            
            void operator()()
            {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock lock(_pool._mutex);
                        _pool._hasTask.wait(lock, [this]()
                        {
                            return !_pool._isWorking || !_pool._tasks.empty();
                        });
                        if (!_pool._isWorking && _pool._tasks.empty())
                        {
                            return;
                        }
                        task = std::move(_pool._tasks.front());
                        _pool._tasks.pop();
                    }
                    task();
                    _pool._waitTask.notify_one();
                }
            }

        private:
            ThreadPool& _pool;
        };
    
        public:
            ThreadPool() :
            ThreadPool(std::thread::hardware_concurrency())
            {
                
            }
        
            ThreadPool(size_t size) :
            _isWorking(true)
            {
                _workers.reserve(size);
                for (decltype(size) i = 0; i < size; ++i)
                    _workers.emplace_back(Worker(*this));
                _manageThread = std::thread(&ThreadPool::ManageTasks, this);
            }

            template<class TFunction, class... TArgs>
            auto AddTask(TFunction&& function, TArgs&&... args) -> std::future<decltype(function(args...))>
            {
                using Result = decltype(function(args...));
                auto task = std::make_shared<std::packaged_task<Result()>>
                (
                    std::bind(std::forward<TFunction>(function), std::forward<TArgs>(args)...)
                );
                
                auto result = task->get_future();
                {
                    std::unique_lock lock(_mutex);
                    if (!_isWorking)
                    {
                        throw std::runtime_error("Cannot AddTask after stopping ThreadPool");
                    }
                    _tasks.emplace([task]() { (*task)(); });
                }
                
                return result;
            }

            ~ThreadPool()
            {
                {
                    std::unique_lock lock(_mutex);
                    _isWorking = false;
                }
                
                _hasTask.notify_all();
                _waitTask.notify_all();
                for (auto& worker : _workers)
                    worker.join();
                _manageThread.join();
            }

        private:
            void ManageTasks()
            {
                while (true)
                {
                    std::unique_lock lock(_mutex);
                    _waitTask.wait(lock, [this]()
                    {
                        return !_isWorking || !_tasks.empty();
                    });
                    
                    if (!_isWorking)
                    {
                        return;
                    } 
                    else if (!_tasks.empty())
                    {
                        _hasTask.notify_one();
                    }
                }
            }

    private:
        bool _isWorking;
        std::mutex _mutex;
        std::vector<std::thread> _workers;
        std::queue<std::function<void()>> _tasks;
        std::condition_variable _waitTask;
        std::condition_variable _hasTask;
        std::thread _manageThread;
    };
}


/*
 Сайты: https://github.com/YFatMR/Habr/blob/main/thread_pool/best_version.cpp
 */
namespace third_implementation
{
    class Task 
    {
    public:
        template <typename TFunction, typename ...TArgs, typename ...TTypes>
        Task(TFunction(*function)(TTypes...), TArgs&&... args) :
            _is_void(std::is_void_v<TFunction>)
        {
            if constexpr (std::is_void_v<TFunction>)
            {
                _void_func = std::bind(function, std::forward<TArgs>(args)...);
                _any_func = []()->int { return 0; };
            }
            else 
            {
                _void_func = []()->void {};
                _any_func = std::bind(function, std::forward<TArgs>(args)...);
            }
        }

        void operator()()
        {
            _void_func();
            _any_func_result = _any_func();
        }

        bool has_result() const noexcept
        {
            return !_is_void;
        }

        std::any get_result() const 
        {
            assert(!_is_void);
            assert(_any_func_result.has_value());
            return _any_func_result;
        }

    private:
        std::function<void()> _void_func;
        std::function<std::any()> _any_func;
        std::any _any_func_result;
        bool _is_void;
    };

    struct TaskInfo 
    {
        enum class TaskStatus
        {
            in_qeueue,
            completed
        };
        
        TaskStatus status = TaskStatus::in_qeueue;
        std::any result;
    };


    class ThreadPool
    {
        using TaskStatus = TaskInfo::TaskStatus;
    public:
        ThreadPool() : ThreadPool(std::thread::hardware_concurrency())
        {
            
        }
        
        ThreadPool(size_t size)
        {
            _threads.reserve(size);
            for (decltype(size) i = 0; i < size; ++i)
                _threads.emplace_back(&ThreadPool::Run, this);
        }

        template <typename TFunction, typename ...TArgs, typename ...TTypes>
        size_t AddTask(TFunction(*function)(TTypes...), TArgs&&... args)
        {
            const auto task_id = last_idx++;

            std::unique_lock lock(_tasks_info_mutex);
            _tasks_info[task_id] = TaskInfo();
            lock.unlock();

            std::lock_guard q_lock(_mutex);
            _queue.emplace(Task(function, std::forward<TArgs>(args)...), task_id);
            _cv.notify_one();
            return task_id;
        }

        void Wait(size_t task_id)
        {
            std::unique_lock lock(_tasks_info_mutex);
            _tasks_info_cv.wait(lock, [this, task_id]()->bool
            {
                return task_id < last_idx && _tasks_info[task_id].status == TaskStatus::completed;
            });
        }

        std::any WaitResult(size_t task_id)
        {
            std::unique_lock lock(_tasks_info_mutex);
            _tasks_info_cv.wait(lock, [this, task_id]()->bool
            {
                return task_id < last_idx && _tasks_info[task_id].status == TaskStatus::completed;
            });
            return _tasks_info[task_id].result;
        }

        template<class T>
        void WaitResult(size_t task_id, T& value)
        {
            std::unique_lock lock(_tasks_info_mutex);
            _tasks_info_cv.wait(lock, [this, task_id]()->bool
            {
                return task_id < last_idx && _tasks_info[task_id].status == TaskStatus::completed;
            });
            value = std::any_cast<T>(_tasks_info[task_id].result);
        }

        void WaitAll()
        {
            std::unique_lock lock(_tasks_info_mutex);
            _wait_all_cv.wait(lock, [this]()->bool { return cnt_completed_tasks == last_idx; });
        }

        bool IsCalculated(size_t task_id)
        {
            std::lock_guard lock(_tasks_info_mutex);
            return task_id < last_idx && _tasks_info[task_id].status == TaskStatus::completed;
        }

        ~ThreadPool()
        {
            quite = true;
            _cv.notify_all();
            for (size_t i = 0, I = _threads.size(); i < I; ++i)
                _threads[i].join();
        }

    private:
        void Run()
        {
            while (!quite) 
            {
                std::unique_lock lock(_mutex);
                _cv.wait(lock, [this]()->bool { return !_queue.empty() || quite; });

                if (!_queue.empty() && !quite)
                {
                    std::pair<Task, size_t> task = std::move(_queue.front());
                    _queue.pop();
                    lock.unlock();

                    task.first();

                    std::lock_guard lock(_tasks_info_mutex);
                    if (task.first.has_result())
                    {
                        _tasks_info[task.second].result = task.first.get_result();
                    }
                    _tasks_info[task.second].status = TaskStatus::completed;
                    ++cnt_completed_tasks;
                }
                
                _wait_all_cv.notify_all();
                _tasks_info_cv.notify_all(); // notify for wait function
            }
        }
        
    private:
        std::atomic<bool> quite = false;
        std::mutex _mutex;
        std::vector<std::thread> _threads;
        std::queue<std::pair<Task, size_t>> _queue;
        std::condition_variable _cv;
        std::unordered_map<size_t, TaskInfo> _tasks_info;
        std::condition_variable _tasks_info_cv;
        std::mutex _tasks_info_mutex;
        std::condition_variable _wait_all_cv;
        std::atomic<size_t> last_idx = 0;
        std::atomic<size_t> cnt_completed_tasks = 0;
    };
}

#endif /* ThreadPool_h */
