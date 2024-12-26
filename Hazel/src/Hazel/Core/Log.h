#pragma once
#include "Hazel/Core/Core.h"
#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/fmt/bundled/ostream.h"

namespace hz {
	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}
// Core Logging Macros
#define HZ_CORE_TRACE(...)	hz::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)	hz::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)	hz::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)	hz::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_FATAL(...)	hz::Log::GetCoreLogger()->critical(__VA_ARGS__)
// Client Logging Macros
#define HZ_TRACE(...)	hz::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)	hz::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)	hz::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)	hz::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZ_FATAL(...)	hz::Log::GetClientLogger()->critical(__VA_ARGS__)