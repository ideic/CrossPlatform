#pragma once
#include <thread>
#include <string>
#include <sstream>
#include "Logger/Logger.h"
#include <utility>
#include <atomic>
#include <memory>
#include <mutex>
namespace Xaba {
	using namespace std::chrono_literals;
	using namespace std::string_literals;
	template<class T> class ThreadManager {
	private:
		std::thread thread_{};
		std::shared_ptr<T> instance_{ nullptr };
		std::atomic<bool> stopped_{ false };
		std::shared_ptr<ILogger> logger_{ nullptr };
	public:
		ThreadManager(const ThreadManager& from) = delete;
		ThreadManager& operator=(const ThreadManager& from) = delete;

		ThreadManager(ThreadManager &&from) {
			if (!from.stopped_) {
				throw std::runtime_error("ThreadManager can't be moved while running");
			}
			thread_ = std::move(from.thread_);
			instance_ = std::move(from.instance_);
			stopped_ = false;
			logger_ = std::move(from.logger_);
		} ;
		ThreadManager& operator=(ThreadManager&& from) {
			if (!from.stopped_) {
				throw std::runtime_error("ThreadManager can't be moved while running");
			}
			thread_ = std::move(from.thread_);
			instance_ = std::move(from.instance_);
			stopped_ = false;
			logger_ = std::move(from.logger_);
			return *this;
		};

		ThreadManager(std::shared_ptr<T> instance,  std::shared_ptr<ILogger> logger):
			instance_(std::move(instance)), logger_(std::move(logger)) {
		}

		~ThreadManager() {
			if (thread_.joinable()) {
				Stop();
				thread_.join();
			}
		}
		void Start() {
			std::stringstream ss{};
			ss << std::this_thread::get_id();

			logger_->Info("Start thread_ from "s + ss.str());

			thread_ = std::thread([this]() {
				try {
					std::stringstream ss2{};
					ss2 << std::this_thread::get_id();
					
					logger_->Info("New thread_ started: " + ss2.str());
					
					while (!stopped_){ 
						if (!instance_->KeepRunning()) {
							stopped_ = true;
						};
					}
					logger_->Info(" thread_ stopped" + ss2.str());
				}
				catch (const std::exception& ex) {
					logger_->Error("Error during thread_ running :"s + ex.what());
				}
			});
		};

		void Stop() {
			stopped_ = true;
		};

		std::shared_ptr<T> GetInstance() {
			return instance_;
		}
	};
}
