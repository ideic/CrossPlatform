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
	bool Fetch() {
		called = true;
		threadId = std::this_thread::get_id();
		return true;
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