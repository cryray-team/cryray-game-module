///////////////////////////////////////////////////////////////////////////
//	File 		: ThreadsID.h
//	Author		: OldSerpskiStalker
//	Description : Queue to run timers separately from the main thread
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace ThreadManagerID
{
    class ThreadPool
    {
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::mutex stopMutex;
        std::condition_variable condition;
        std::atomic<bool> stop{false};

    public:
        ThreadPool(size_t numThreads) : stop(false)
        {
            for (size_t i = 0; i < numThreads; ++i)
            {
                workers.emplace_back([this] {
                    while (true)
                    {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(queueMutex);
                            condition.wait(lock, [this] { return stop.load() || !tasks.empty(); });
                            if (stop.load())
                            {
                                if (tasks.empty())
                                {
                                    lock.unlock();
                                    return;
                                }
                            }
                            task = std::move(tasks.front());
                            tasks.pop();
                            lock.unlock(); // Снятие блокировки перед выполнением задачи
                        }
                        try
                        {
                            task();
                        }
                        catch (const std::exception& e)
                        {
                            // Обработка исключения при выполнении задачи
                            // Можно здесь выбросить исключение или просто сообщить о нём
                            //std::cerr << "Exception in ThreadPool task: " << e.what() << std::endl;
                        }
                    }
                });
            }
        }

        template <typename F, typename... Args>
        static inline auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
        {
            using return_type = decltype(f(args...));

            auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...));

            std::future<return_type> result = task->get_future();
            {
                std::lock_guard<std::mutex> lock(getInstance().stopMutex);
                if (getInstance().stop.load())
                    throw std::runtime_error("enqueue on stopped ThreadPool");

                std::lock_guard<std::mutex> queueLock(getInstance().queueMutex);
                getInstance().tasks.emplace([task]() { (*task)(); });
            }

            getInstance().condition.notify_one();
            return result;
        }

        ~ThreadPool()
        {
            {
                std::lock_guard<std::mutex> lock(stopMutex);
                stop.store(true);
            }

            condition.notify_all();

            for (std::thread& worker : workers)
                worker.join();
        }

        static ThreadPool& getInstance()
        {
            static ThreadPool instance(std::thread::hardware_concurrency());
            return instance;
        }
    };
} // namespace ThreadManagerID