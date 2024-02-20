#include "ThreadPool.h"

#include <iostream>
namespace ent {
    namespace util {
        ThreadPool::ThreadPool() {
        }
        ThreadPool::ThreadPool(ui32 numThreads) {
            initThreads(numThreads);
        }

        ThreadPool::~ThreadPool() {
            stop = true;
            condition.notify_all();
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                tasks.clear();
            }

            for (auto& thread : threads)
                thread.join();
        }

        void ThreadPool::pause() {
            mPause = true;
        }

        void ThreadPool::resume() {
            mPause = false;
        }

        void ThreadPool::wait() {
            while (tasks.size()) {
                // Do nothing
            }
        }

        void ThreadPool::initThreads(ui32 numThreads) {
            mPause = false;
            stop = false;

            for (ui32 i = 0; i < numThreads; i++) {
                threads.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;

                        while (mPause); // Do nothing

                        {
                            std::unique_lock<std::mutex> lock(queueMutex);
                            condition.wait(lock, [this] { return stop || !tasks.empty(); });
                            if (stop && tasks.empty())
                                return;

                            task = std::move(tasks.front());
                            tasks.pop_front();
                        }

                        task();
                    }
                });
            }
        }
    }
}

