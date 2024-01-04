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
		std::thread _thread{};
		std::shared_ptr<T> _instance{ nullptr };
		std::atomic<bool> _stopped{ false };

	public:
		ThreadManager<T>(const ThreadManager<T>& from) = delete;
		ThreadManager<T>& operator=(const ThreadManager<T>& from) = delete;

		ThreadManager<T>(ThreadManager<T>&& from) = default;
		ThreadManager<T>& operator=(ThreadManager<T>&& from) = default;

		ThreadManager(std::shared_ptr<T> instance): _instance(std::move(instance)) {
		}

		~ThreadManager() {
			if (_thread.joinable()) {
				Stop();
				_thread.join();
			}
		}
		void Start() {
			std::stringstream ss{};
			ss << std::this_thread::get_id();

			Logger::Info("Start Thread from "s + ss.str());

			_thread = std::thread([this]() {
				try {
					std::stringstream ss2{};
					ss2 << std::this_thread::get_id();
					
					Logger::Info("New Thread started: " + ss2.str());
					
					while (true){ 
						if (_stopped) break;
						if (_instance->Fetch()) {
							_stopped = true;
							break;
						};
					}
					Logger::Info(" Thread stopped" + ss2.str());
				}
				catch (const std::exception& ex) {
					Logger::Error("Error during thread running :"s + ex.what());
				}
			});
		};

		void Stop() {
			_stopped = true;
		};

		std::shared_ptr<T> GetInstance() {
			return _instance;
		}
	};
}
