#include <cstdarg>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>
#include <array>
#include <RE/Skyrim.h>
#include "skse/jc_skse.h"
#include "common/IDebugLog.h"

using namespace jc_skse;

constexpr std::array<IDebugLog::LogLevel, 1> ignore = { IDebugLog::LogLevel::kLevel_VerboseMessage };

namespace
{
    std::string get_timestamp()
    {
        const auto current_time = std::chrono::system_clock::now();
        const auto time = std::chrono::system_clock::to_time_t(current_time);

        std::tm tm{};
        localtime_s(&tm, &time);

        const auto milliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                current_time.time_since_epoch()) %
            1000;

        std::ostringstream string_stream;
        string_stream << std::put_time(&tm, "%H:%M:%S")
                      << '.' << std::setfill('0') << std::setw(3)
                      << milliseconds.count();

        return string_stream.str();
    }

    char get_log_level_char(IDebugLog::LogLevel level)
    {
        switch (level) {
        case IDebugLog::kLevel_FatalError:
            return 'F';
        case IDebugLog::kLevel_Error:
            return 'E';
        case IDebugLog::kLevel_Warning:
            return 'W';
        case IDebugLog::kLevel_Message:
            return 'I';
        case IDebugLog::kLevel_DebugMessage:
            return 'D';
        case IDebugLog::kLevel_VerboseMessage:
            return 'V';
        default:
            return 'I';
        }
    }
}

bool isValidLevel(IDebugLog::LogLevel new_level = IDebugLog::kLevel_Message) {
    return std::any_of(ignore.begin(), ignore.end(),
                [new_level](IDebugLog::LogLevel l) { return l != new_level; });
}

void JC_log_full(IDebugLog::LogLevel level, const char* fmt, va_list& args)
{
    const std::string full_fmt =
        '[' + get_timestamp() + "] [" +
        get_log_level_char(level) + "] " +
        fmt;

    va_list args_copy;
    va_copy(args_copy, args);


    if (isValidLevel(level)) {
        console_print(full_fmt.c_str(), args);
        gLog.Log(level, full_fmt.c_str(), args_copy);
    }

    va_end(args_copy);
}

void JC_log_full(IDebugLog::LogLevel level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    JC_log_full(level, fmt, args);
    va_end(args);
}

void JC_log(const char* fmt, va_list& args) {
     JC_log_full(IDebugLog::kLevel_Message, fmt, args);
}

void JC_log(const char* fmt, ...) {
    va_list	args;
    va_start(args, fmt);
    JC_log(fmt, args);
    va_end(args);
}


