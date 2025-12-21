#pragma once
#include <string_view>

namespace Xaba {
	
	class ILogger
	{
		
	public:
		virtual void Debug(std::string_view message) = 0;
		virtual void Info(std::string_view message) = 0;
		virtual void Error(std::string_view message) = 0;

		ILogger() = default;
        ILogger(const ILogger &) = delete;
        ILogger(ILogger &&) = delete;
        ILogger &operator=(const ILogger &) = delete;
        ILogger &operator=(ILogger &&) = delete;
		virtual ~ILogger() = default;
	};
}