#include "Logger.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <spdlog/sinks/msvc_sink.h>
#else
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

std::shared_ptr<spdlog::logger> Logger::sLogger;

void Logger::Init()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	// ref: https://github.com/microsoft/PowerToys/blob/main/src/common/logger/logger.cpp
	auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
	sLogger = std::make_shared<spdlog::logger>("d3dlog", msvc_sink);
#else
	sLogger = spdlog::stdout_color_mt("d3dlog");
#endif

	sLogger->set_level(spdlog::level::trace);
	// set log pattern: [time] logger_name: text (all colored)
	// ref: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
	sLogger->set_pattern("%^[%L] [%Y-%m-%d %H:%M:%S.%f] [p-%P:t-%t] %v%$");
}
