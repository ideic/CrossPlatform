#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
namespace Xaba {
	template<typename VALUE_TYPE>
	class BlockingQueue {
		std::queue<VALUE_TYPE> items_;
		std::condition_variable cv_;
		std::mutex mtx_;
	public:
		void push(VALUE_TYPE&& value);
		VALUE_TYPE getNext();
	};

	template<typename VALUE_TYPE>
	void BlockingQueue<VALUE_TYPE>::push(VALUE_TYPE&& value) {
		std::unique_lock<std::mutex> lock(mtx_);
		items_.push(std::move(value));
		cv_.notify_one();
	}

	template<typename VALUE_TYPE>
	VALUE_TYPE BlockingQueue<VALUE_TYPE>::getNext() {
		VALUE_TYPE result;
		{
			std::unique_lock<std::mutex> lock(mtx_);
			cv_.wait(lock, [this] {return (!items_.empty()); });

			result = std::move(items_.front());
			items_.pop(); 
		}
		return result;
	}
};
#include <deque>
#include <memory_resource>
#include <array>
#include <optional>
#include <atomic>
#include <thread>
using namespace std::chrono_literals;
namespace Xaba {
	template<typename VALUE_TYPE>
	class BlockingQueue2 {
		//std::dequeue<VALUE_TYPE> items_;
	//	std::condition_variable cv_;
	//	std::mutex mtx_;

		//std::vector<uint8_t> _buf;// (6'000'000, 0);// , 60'000> _buf{};
		constexpr static size_t bufSize_ = 30'000;
		std::pmr::monotonic_buffer_resource _monotonicPool{bufSize_};// { _buf.data(), _buf.size() };
		std::pmr::unsynchronized_pool_resource _pool{ &_monotonicPool };
		std::pmr::vector<VALUE_TYPE> _items_{&_pool };

		std::atomic<size_t> size_{ 0 };
		size_t upperIdx_{ 0 };
		size_t lowIdx_{ 0 };
	public:
		void push(VALUE_TYPE &&data);
		VALUE_TYPE getNext();

		BlockingQueue2() {
			_items_.resize(bufSize_);
		}
		//BlockingQueue2() {
		//	_buf.resize(2'000'000'000);
		//	_pool.emplace(_buf.data(), _buf.size());
		//	_items_ = std::pmr::deque<VALUE_TYPE>(&_pool.value());
		//}
	};
	template<typename VALUE_TYPE>
	void BlockingQueue2<VALUE_TYPE>::push(VALUE_TYPE &&data) {
		size_t size = size_;
		_items_[++upperIdx_] = std::move(data);
		while (!std::atomic_compare_exchange_strong(&size_, &size, size +1)) {
			size = size_;
		};
		if (upperIdx_ == bufSize_-1) {
			upperIdx_ = 0;
		}
		//std::unique_lock<std::mutex> lock(mtx_);
	}

	template<typename VALUE_TYPE>
	VALUE_TYPE BlockingQueue2<VALUE_TYPE>::getNext() {
		VALUE_TYPE result;
		size_t size = size_;
		while (size == 0) {
			std::this_thread::sleep_for(10ms);
			size = size_;
		}
		while (!std::atomic_compare_exchange_strong( &size_, &size, size - 1)) {
			size = size_;
		};
		result = std::move(_items_[lowIdx_++]);
		if (lowIdx_ == bufSize_-1) {
			lowIdx_ = 0;
		}
		return result;
	}
};