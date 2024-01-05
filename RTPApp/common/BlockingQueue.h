#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
namespace Xaba {
	template<typename VALUE_TYPE>
	class BlockingQueue {
		std::queue<VALUE_TYPE> items;
		std::condition_variable cv;
		std::mutex mtx;
	public:
		void push(VALUE_TYPE&& value);
		VALUE_TYPE getNext();
	};

	template<typename VALUE_TYPE>
	void BlockingQueue<VALUE_TYPE>::push(VALUE_TYPE&& value) {
		std::unique_lock<std::mutex> lock(mtx);
		items.push(std::move(value));
		cv.notify_one();
	}

	template<typename VALUE_TYPE>
	VALUE_TYPE BlockingQueue<VALUE_TYPE>::getNext() {
		VALUE_TYPE result;
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [this] {return (!items.empty()); });

			result = std::move(items.front());
			items.pop(); 
		}
		return result;
	}
};