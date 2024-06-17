#pragma once

#include <spdlog/spdlog.h>

class Logger
{
public:
	static void Init();

	static std::shared_ptr<spdlog::logger> GetLogger()
	{
		return sLogger;
	}

private:
	static std::shared_ptr<spdlog::logger> sLogger;
};

#define LOG_TRACE(...) ::Logger::GetLogger()->trace(__VA_ARGS__) // explain: variadic macro
#define LOG_INFO(...)  Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::GetLogger()->error(__VA_ARGS__) 
#define LOG_CRITICAL(...) ::Logger::GetLogger()->critical(__VA_ARGS__)
