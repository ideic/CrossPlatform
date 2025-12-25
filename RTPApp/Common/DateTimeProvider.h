#pragma once
#include <chrono>
#include <thread>
#include <memory>
namespace Xaba {
	class IDateTimeProvider
	{
		public:
          virtual std::chrono::system_clock::time_point Now() const = 0;
          virtual void Sleep(std::chrono::milliseconds msec) const = 0;
          virtual ~IDateTimeProvider() = default;
          IDateTimeProvider() = default;
          IDateTimeProvider(const IDateTimeProvider &) = delete;
          IDateTimeProvider &operator=(const IDateTimeProvider &) = delete;
          IDateTimeProvider(IDateTimeProvider &&) = delete;
          IDateTimeProvider &operator=(IDateTimeProvider &&) = delete;
	};

	class ChronoDateTimeProvider : public Xaba::IDateTimeProvider
	{
		public:
          [[nodiscard]] std::chrono::system_clock::time_point Now() const override { 
			  return std::chrono::system_clock::now(); 
		  }
		  void Sleep(std::chrono::milliseconds msec) const override { 
			  std::this_thread::sleep_for(msec); 
		  }
    };
	class DateTimeUtility
	{
		public:
          [[nodiscard]] static std::chrono::system_clock::time_point Now(){
			  return m_provider->Now(); 
		  }
          static void Sleep(std::chrono::milliseconds msec) { 
			  m_provider->Sleep(msec); 
		  }
          static void SetProvider(std::shared_ptr<Xaba::IDateTimeProvider> provider) {
			  m_provider = std::move(provider);
		  }
        private:
          inline static std::shared_ptr<Xaba::IDateTimeProvider> m_provider = std::make_shared<ChronoDateTimeProvider>();
	};
}// namespace Xaba