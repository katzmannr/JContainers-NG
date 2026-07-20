#include <cstdarg>
#include <RE/Skyrim.h>
#include "skse/jc_skse.h"
#include "common/IDebugLog.h"

using namespace jc_skse;

void JC_log(const char* fmt, va_list& args) {
    va_list	args_copy;

    va_copy(args_copy, args);

    console_print(fmt, args);
    gLog.Log(IDebugLog::kLevel_Message, fmt, args_copy);

    va_end(args_copy);
}

void JC_log(const char* fmt, ...) {
    va_list	args;
    va_start(args, fmt);
    JC_log(fmt, args);
    va_end(args);
}

