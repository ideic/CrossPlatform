#include <gtest/gtest.h>
#include "Common/BlockingQueue.h"
#include <chrono>
#include <thread>
#include <vector>
using namespace Xaba;
using namespace std::chrono_literals;
// Demonstrate some basic assertions.
TEST(BlockingQueueTest, MeasureThroughput) {
	BlockingQueue<std::vector<uint8_t>> queue;
	auto start = std::chrono::system_clock::now();

	std::thread producer([&queue]() {
		
		for (int i = 0; i < 800'000; i++)
		{
			std::vector<uint8_t> data(1500);
			queue.push(std::move(data));
		}
	});

	auto consumer = [&queue]() {
		std::vector<uint8_t> data;
		while (true)
		{
			data = queue.getNext();
			if (data.empty())
			{
				break;
			}
		}
	};
	std::thread consumer1(consumer);
	std::thread consumer2(consumer);
	std::thread consumer3(consumer);

	producer.join();

	if (consumer1.joinable()) {
		for (size_t i = 0; i < 10; i++)
		{
			queue.push({});
		}
		consumer1.join();
	}
	if (consumer2.joinable()) consumer2.join();
	if (consumer3.joinable()) consumer3.join();
	auto end = std::chrono::system_clock::now();
	EXPECT_LT(std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count(), 1000);//9.6Gbps
}