#include <gtest/gtest.h>

#include "Network/UDPServer.h"
#include "Network/UDPClient.h"
using namespace Xaba::Network;	

#if __has_include(<WinSock2.h>)
	#include <WinSock2.h>
#endif
#include "Common/Logger/Logger.h"

using namespace Xaba;
using namespace std::string_literals;

struct UDPServerTest : public ::testing::Test
{
	static void SetUpTestSuite()
	{
		#ifdef WIN32
			WSADATA wsaData;
			auto res = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (res != NOERROR) {
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

TEST_F(UDPServerTest, Init)
{
	uint16_t i = 16000;
	std::optional<UDPServer> server;
	std::shared_ptr<ILogger> logger = std::make_shared<LoggerMock>();
	for (size_t attempt = 0; attempt < 100; attempt++)
	{
		try
		{
			server.emplace("127.0.0.1", i, logger);
			server->Init();
			break;	
		}
		catch (const std::exception& ex)
		{
			logger->Error(ex.what());
			i += 2;
		}
	}
	ASSERT_TRUE(server.has_value());
	UDPClient client("127.0.0.1", i, logger);
	client.Init();
	client.SendData("Hello");
	server->KeepRunning();
	auto data = server->queue.getNext();
	EXPECT_EQ(data.size(), 5);
	EXPECT_EQ(data[0], 'H');
	EXPECT_EQ(data[1], 'e');
	EXPECT_EQ(data[2], 'l');
	EXPECT_EQ(data[3], 'l');
	EXPECT_EQ(data[4], 'o');
}