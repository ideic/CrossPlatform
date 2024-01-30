#include <gtest/gtest.h>
#include "Common/ThreadManager.h"
#include "Common/Logger/Logger.h"
#include <chrono>
#include <thread>
#include <memory>
#include <atomic>
using namespace Xaba;
using namespace std::chrono_literals;
struct TestClass {
	std::atomic<bool> called{ false };
	std::thread::id threadId;
	std::atomic<bool> fetchReturn{ true };
	bool Fetch() {
		called = true;
		threadId = std::this_thread::get_id();
		return fetchReturn;
	}
};
struct LoggerMock : public ILogger {
	void Debug([[maybe_unused]]  std::string_view message) override {
	}
	void Info([[maybe_unused]] std::string_view message) override {
	}
	void Error([[maybe_unused]] std::string_view message) override {
	}
};
TEST(ThreadManagerTest, start_stop) {
	auto instance = std::make_shared<TestClass>();
	auto logger = std::make_shared<LoggerMock>();
	ThreadManager<TestClass> manager(instance, logger);
	manager.Start();
	while (!instance->called) {
		std::this_thread::sleep_for(1ms);
	}
	EXPECT_NE(instance->threadId, std::this_thread::get_id());
	manager.Stop();
	std::this_thread::sleep_for(1ms);
	instance->called = false;
	std::this_thread::sleep_for(1ms);

	EXPECT_FALSE(instance->called);
}

TEST(ThreadManagerTest, moveFails) {
	auto instance = std::make_shared<TestClass>();
	auto logger = std::make_shared<LoggerMock>();
	ThreadManager<TestClass> manager(instance, logger);
	instance->fetchReturn = false;
	manager.Start();
	while (!instance->called) {
		std::this_thread::sleep_for(1ms);
	}
	EXPECT_NE(instance->threadId, std::this_thread::get_id());
	EXPECT_ANY_THROW({ auto t2 = std::move(manager); });
	manager.Stop();
}

TEST(ThreadManagerTest, move) {
	auto instance = std::make_shared<TestClass>();
	auto logger = std::make_shared<LoggerMock>();
	ThreadManager<TestClass> manager(instance, logger);
	instance->fetchReturn = true;
	manager.Start();
	while (!instance->called) {
		std::this_thread::sleep_for(1ms);
	}
	EXPECT_NE(instance->threadId, std::this_thread::get_id());
	manager.Stop();
	auto t2 = std::move(manager);
	
}