#include <gtest/gtest.h>
#include "Common/ThreadManager.h"
#include "Common/Logger/Logger.h"
#include <chrono>
#include <thread>
#include <memory>
#include <atomic>
#include <string_view>
#include <utility>

using namespace Xaba;
using namespace std::chrono_literals;
struct TestClass {
	std::atomic<bool> called{ false };
	std::thread::id threadId;
	std::atomic<bool> keepRunningReturn{ false };
	bool KeepRunning() {
		called = true;
		threadId = std::this_thread::get_id();
		return keepRunningReturn;
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
		// NOLINTNEXTLINE(misc-include-cleaner)
		std::this_thread::sleep_for(1ms);
	}
	EXPECT_NE(instance->threadId, std::this_thread::get_id());
	manager.Stop();
    // NOLINTNEXTLINE(misc-include-cleaner)
	std::this_thread::sleep_for(1ms);
	instance->called = false;
    // NOLINTNEXTLINE(misc-include-cleaner)
	std::this_thread::sleep_for(1ms);

	EXPECT_FALSE(instance->called);
}

TEST(ThreadManagerTest, moveFails) {
	auto instance = std::make_shared<TestClass>();
	auto logger = std::make_shared<LoggerMock>();
	ThreadManager<TestClass> manager(instance, logger);
	instance->keepRunningReturn = true;
	manager.Start();
	while (!instance->called) {
        // NOLINTNEXTLINE(misc-include-cleaner)
		std::this_thread::sleep_for(1ms);
	}
	EXPECT_NE(instance->threadId, std::this_thread::get_id());
	EXPECT_ANY_THROW({ auto thread2 = std::move(manager); });
}

TEST(ThreadManagerTest, move) {
	auto instance = std::make_shared<TestClass>();
	auto logger = std::make_shared<LoggerMock>();
	ThreadManager<TestClass> manager(instance, logger);
	instance->keepRunningReturn = false;
	manager.Start();
	while (!instance->called) {
		// NOLINTNEXTLINE(misc-include-cleaner)
		std::this_thread::sleep_for(1ms);
	}
	EXPECT_NE(instance->threadId, std::this_thread::get_id());
	manager.Stop();
	auto thread2 = std::move(manager);
}