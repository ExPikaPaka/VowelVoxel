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
			ThreadPool();
			ThreadPool(ui32 numThreads);

			~ThreadPool();

			void initThreads(ui32 numThreads);

			template<class T>
			void enqueue(T&& f);

			void pause();
			void resume();

			void wait();
			std::deque<std::function<void()>> tasks;
		private:
			std::vector<std::thread> threads;

			std::mutex queueMutex;
			std::condition_variable condition;

			volatile bool mPause;
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

