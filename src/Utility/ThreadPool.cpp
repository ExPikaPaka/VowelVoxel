#include "ThreadPool.h"

namespace ent {
	namespace util {
        ThreadPool::ThreadPool(ui32 numThreads) {
            initThreads(numThreads);
        }

        ThreadPool::~ThreadPool() {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
            
            condition.notify_all();
            for (auto& thread : threads)
                thread.join();
        }

        void ThreadPool::wait() {
            while (tasks.size()) {
                // Do nothing
            }
        }

        void ThreadPool::initThreads(ui32 numThreads) {
            stop = false;

			for (ui32 i = 0; i < numThreads; i++) {
                threads.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;

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

