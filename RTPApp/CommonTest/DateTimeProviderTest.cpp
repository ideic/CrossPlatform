#include <gtest/gtest.h>
#include <chrono>
#include <memory>

#include "Common/DateTimeProvider.h"

using namespace std::chrono_literals;

struct ChronoWrapperMock final: public Xaba::IDateTimeProvider
{
	[[nodiscard]]std::chrono::system_clock::time_point Now() const override
	{
		return mockTime;
	}
	void Sleep(std::chrono::milliseconds msec) const override
	{
		// Mock sleep does nothing
	}
    std::chrono::system_clock::time_point mockTime;
};

TEST(DateTimeProviderTest, NowReturnsCurrentTime) {
  const std::shared_ptr<ChronoWrapperMock> mockChrono = std::make_shared<ChronoWrapperMock>();
  
  // NOLINTNEXTLINE(misc-include-cleaner)
  constexpr auto longTime = 10s;
  Xaba::DateTimeUtility::SetProvider(mockChrono);

  auto now = std::chrono::system_clock::now();
  mockChrono->mockTime = now;
  Xaba::DateTimeUtility::Sleep(longTime);
  auto providerTime = Xaba::DateTimeUtility::Now();
  EXPECT_EQ(providerTime, now);
}