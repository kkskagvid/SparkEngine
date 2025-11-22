#pragma once
#undef max

namespace Spark::Core
{
    class ThreadPool {
    public:
        static ThreadPool& GetInstance() {
            static ThreadPool instance;
            return instance;
        }

        template <class F>
        void Enqueue(F&& f) {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                tasks.push(std::forward<F>(f));
            }
            condition.notify_one();
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for (auto& thread : workers) {
                thread.join();
            }
        }

    private:
        ThreadPool(size_t thread_count = std::max(2u, std::thread::hardware_concurrency())) {
            for (size_t i = 0; i < thread_count; ++i) {
                workers.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(queue_mutex);
                            condition.wait(lock, [this] {
                                return stop || !tasks.empty();
                                });
                            if (stop && tasks.empty()) return;
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        try {
                            task();
                        }
                        catch (...) {
                            //
                            // TODO：记录异常日志
                        }
                    }
                    });
            }
        }

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;

        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        std::atomic<bool> stop = false;
    };
}
