#include <gtest/gtest.h>
#include "Common/BlockingQueue.h"
#include <chrono>
#include <thread>
#include <vector>
#include "TrackNew.h"
using namespace Xaba;
using namespace std::chrono_literals;
// Demonstrate some basic assertions.
TEST(BlockingQueueTest, MeasureThroughput) {
	BlockingQueue<std::vector<uint8_t>> queue;
	TrackNew::reset();
	auto start = std::chrono::system_clock::now();
	std::thread producer([&queue]() {
		
		for (int i = 0; i < 800'000; i++) // 800'000 * 1500 * 8 = 9.6Gbps
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

	producer.join();

	if (consumer1.joinable()) {
		queue.push({});
		consumer1.join();
	}
	auto end = std::chrono::system_clock::now();
	TrackNew::status();
	EXPECT_LT(std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count(), 1000);//9.6Gbps
}

TEST(BlockingQueueTest, MeasureThroughput2) {
	BlockingQueue2<std::vector<uint8_t>> queue;
	TrackNew::reset();
	auto start = std::chrono::system_clock::now();
	std::thread producer([&queue]() {

		for (int i = 0; i < 800'000; i++) // 800'000 * 1500 * 8 = 9.6Gbps
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

	producer.join();

	if (consumer1.joinable()) {
		
		queue.push({});
		
		consumer1.join();
	}
	auto end = std::chrono::system_clock::now();
	TrackNew::status();
	EXPECT_LT(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), 1000);//9.6Gbps
}
TEST(BlockingQueueTest, push_get_next) {
	
	BlockingQueue<std::vector<uint8_t>> queue;
	std::vector<uint8_t> data{0,1,2,3,4};
	queue.push(std::move(data));
	auto data2 = queue.getNext();
	EXPECT_EQ(data2.size(), 5);
	EXPECT_EQ(data2[0], 0);
	EXPECT_EQ(data2[1], 1);		
	EXPECT_EQ(data2[2], 2);
	EXPECT_EQ(data2[3], 3);
	EXPECT_EQ(data2[4], 4);
}