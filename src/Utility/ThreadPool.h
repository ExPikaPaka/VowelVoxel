#pragma once
#include "..\dataTypes.h"
#include <thread>
#include <mutex>
#include <vector>
#include <deque>
#include <functional>

namespace ent {
	namespace util {
		class ThreadPool {
		public:
			ThreadPool(ui32 numThreads);

			~ThreadPool();

			template<class T>
			void enqueue(T&& f);

			void wait();
			std::deque<std::function<void()>> tasks;
		private:
			void initThreads(ui32 numThreads);
			std::vector<std::thread> threads;

			std::mutex queueMutex;
			std::condition_variable condition;

			bool stop;
		};


		///////////////////////////////////////////////////////////////////////////////////
		// ThreadPool Template Implementation													 //
		///////////////////////////////////////////////////////////////////////////////////
		template<class T>
		inline void ThreadPool::enqueue(T&& t) {
				std::unique_lock<std::mutex> lock(queueMutex);
				tasks.emplace_back(std::forward<T>(t));
				
				condition.notify_one();
		}
	}
}

