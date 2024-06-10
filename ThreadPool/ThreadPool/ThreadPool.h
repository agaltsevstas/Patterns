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
 Видео: https://www.youtube.com/watch?v=-TuJP8pUBW0&list=WL&index=3&pp=gAQBiAQB
 Сайт: https://github.com/sajeruk/ThreadPool/tree/5a3124f23d33007447a45f6c8710b242ed8c6e89
 */

/*
 Сайты: https://github.com/YFatMR/Habr/blob/main/thread_pool/best_version.cpp
 */

namespace first_implementation
{
    class ThreadPool 
    {
        private:
            class Worker 
            {
            public:
                Worker(ThreadPool& pool): _pool(pool)
                {
                    _pool._waitTask.notify_one();
                }
                
                void operator()()
                {
                    while (true) 
                    {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(_pool._mutex);
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
            ThreadPool(size_t poolSize) : _isWorking(true)
            {
                _workers.reserve(poolSize);
                for (size_t i = 0; i < poolSize; ++i)
                    _workers.emplace_back(Worker(*this));
                _manageThread = std::thread(&ThreadPool::ManageTasks, this);
            }

            template<class Func, class... Args>
            auto AddTask(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>
            {
                using result_type = decltype(func(args...));
                auto task = std::make_shared<std::packaged_task<result_type()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
                
                auto result = task->get_future();
                {
                    std::unique_lock<std::mutex> lock(_mutex);
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
                    std::unique_lock<std::mutex> lock(_mutex);
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
                    std::unique_lock<std::mutex> lock(_mutex);
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
            std::vector<std::thread> _workers;
            std::queue<std::function<void()>> _tasks;
            std::condition_variable _waitTask;
            std::condition_variable _hasTask;
            std::mutex _mutex;
            std::thread _manageThread;
            bool _isWorking;
        };
}

namespace second_implementation
{
    class Task 
    {
    public:
        template <typename FuncRetType, typename ...Args, typename ...FuncTypes>
        Task(FuncRetType(*func)(FuncTypes...), Args&&... args) :
            _is_void(std::is_void_v<FuncRetType>)
        {

            if constexpr (std::is_void_v<FuncRetType>) 
            {
                _void_func = std::bind(func, args...);
                _any_func = []()->int { return 0; };
            }
            else 
            {
                _void_func = []()->void {};
                _any_func = std::bind(func, args...);
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
        ThreadPool(const uint32_t num_threads)
        {
            _threads.reserve(num_threads);
            for (uint32_t i = 0; i < num_threads; ++i)
                _threads.emplace_back(&ThreadPool::Run, this);
        }

        template <typename Func, typename ...Args, typename ...FuncTypes>
        uint64_t AddTask(Func(*func)(FuncTypes...), Args&&... args)
        {
            const uint64_t task_id = last_idx++;

            std::unique_lock<std::mutex> lock(_tasks_info_mutex);
            _tasks_info[task_id] = TaskInfo();
            lock.unlock();

            std::lock_guard<std::mutex> q_lock(_mutex);
            _queue.emplace(Task(func, std::forward<Args>(args)...), task_id);
            _cv.notify_one();
            return task_id;
        }

        void Wait(const uint64_t task_id)
        {
            std::unique_lock<std::mutex> lock(_tasks_info_mutex);
            _tasks_info_cv.wait(lock, [this, task_id]()->bool
            {
                return task_id < last_idx && _tasks_info[task_id].status == TaskStatus::completed;
            });
        }

        std::any WaitResult(const uint64_t task_id)
        {
            std::unique_lock<std::mutex> lock(_tasks_info_mutex);
            _tasks_info_cv.wait(lock, [this, task_id]()->bool
            {
                return task_id < last_idx && _tasks_info[task_id].status == TaskStatus::completed;
            });
            return _tasks_info[task_id].result;
        }

        template<class T>
        void WaitResult(const uint64_t task_id, T& value)
        {
            std::unique_lock<std::mutex> lock(_tasks_info_mutex);
            _tasks_info_cv.wait(lock, [this, task_id]()->bool
            {
                return task_id < last_idx && _tasks_info[task_id].status == TaskStatus::completed;
            });
            value = std::any_cast<T>(_tasks_info[task_id].result);
        }

        void WaitAll()
        {
            std::unique_lock<std::mutex> lock(_tasks_info_mutex);
            _wait_all_cv.wait(lock, [this]()->bool { return cnt_completed_tasks == last_idx; });
        }

        bool IsCalculated(const uint64_t task_id)
        {
            std::lock_guard<std::mutex> lock(_tasks_info_mutex);
            return task_id < last_idx && _tasks_info[task_id].status == TaskStatus::completed;
        }

        ~ThreadPool()
        {
            quite = true;
            _cv.notify_all();
            for (int i = 0, I = (int)_threads.size(); i < I; ++i)
                _threads[i].join();
        }

    private:

        void Run()
        {
            while (!quite) 
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this]()->bool { return !_queue.empty() || quite; });

                if (!_queue.empty() && !quite)
                {
                    std::pair<Task, uint64_t> task = std::move(_queue.front());
                    _queue.pop();
                    lock.unlock();

                    task.first();

                    std::lock_guard<std::mutex> lock(_tasks_info_mutex);
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
        std::vector<std::thread> _threads;
        std::queue<std::pair<Task, uint64_t>> _queue;
        std::mutex _mutex;
        std::condition_variable _cv;
        std::unordered_map<uint64_t, TaskInfo> _tasks_info;
        std::condition_variable _tasks_info_cv;
        std::mutex _tasks_info_mutex;
        std::condition_variable _wait_all_cv;
        std::atomic<bool> quite = false;
        std::atomic<uint64_t> last_idx = 0;
        std::atomic<uint64_t> cnt_completed_tasks = 0;
    };
}

#endif /* ThreadPool_h */
