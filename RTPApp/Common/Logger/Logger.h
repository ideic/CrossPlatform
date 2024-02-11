#pragma once
#include <string_view>

namespace Xaba {
	
	class ILogger
	{
		
	public:
		virtual void Debug(std::string_view message) = 0;
		virtual void Info(std::string_view message) = 0;
		virtual void Error(std::string_view message) = 0;
		virtual ~ILogger() = default;
	};
}