#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <optional>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

#if __has_include(<WinSock2.h>)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <WinSock2.h>
#endif

#include "Network/UDPClient.h"
#include "Network/UDPServer.h"
#include <Common/BlockingQueue.h>
#include "Common/Logger/Logger.h"

using namespace Xaba;
using namespace std::string_literals;
using namespace Xaba::Network;	

struct UDPServerTest : public ::testing::Test
{
	static void SetUpTestSuite()
	{
		#ifdef WIN32
			WSADATA wsaData;
            // NOLINTNEXTLINE(misc-include-cleaner)
			auto res = WSAStartup(MAKEWORD(2, 2), &wsaData);

            // NOLINTNEXTLINE(misc-include-cleaner)
			if (res != NOERROR) {
				// NOLINTNEXTLINE(misc-include-cleaner)
				throw std::runtime_error("WSAStartup failed: "s + std::to_string(res));
			}
		#endif // WIN32

	}
	static void TearDownTestSuite(){
		#ifdef WIN32
			WSACleanup();
		#endif // WIN32
	}
};
struct LoggerMock : public ILogger {
	void Debug([[maybe_unused]] std::string_view message) override {
	}
	void Info([[maybe_unused]] std::string_view message) override {
	}
	void Error([[maybe_unused]] std::string_view message) override {
	}
};

// NOLINTBEGIN(readability-function-cognitive-complexity)
TEST_F(UDPServerTest, Init)
{
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
	uint16_t fromPort = 16000;
	std::optional<UDPServer> server;
	const std::shared_ptr<ILogger> logger = std::make_shared<LoggerMock>();
	const int maxAttempts = 10;
	// NOLINTNEXTLINE(misc-include-cleaner)
	for (size_t attempt = 0; attempt < maxAttempts; attempt++)
	{
		try
		{
			server.emplace("127.0.0.1", fromPort, logger);
			server->Init();
			break;	
		}
		// NOLINTNEXTLINE(misc-include-cleaner)
		catch (const std::exception& ex)
		{
			logger->Error(ex.what());
			fromPort += 2;
		}
	}
	ASSERT_TRUE(server.has_value());
	UDPClient client("127.0.0.1", fromPort, logger);
	client.Init();
	client.SendData("Hello");
        
	if (!server) { return; }
	server->KeepRunning();
	auto data = server->queue.getNext();
	EXPECT_EQ(data.size(), 5);
	EXPECT_EQ(data[0], 'H');
	EXPECT_EQ(data[1], 'e');
	EXPECT_EQ(data[2], 'l');
	EXPECT_EQ(data[3], 'l');
	EXPECT_EQ(data[4], 'o');
}
// NOLINTEND(readability-function-cognitive-complexity)