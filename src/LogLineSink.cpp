#include "Platform.h"
#include "orbit_logger.h"
#include <ctime>

namespace OrbitLogger {

iLogSinkBase::iLogSinkBase() {}

iLogSinkBase::~iLogSinkBase() {}

void iLogSinkBase::PrintBanner() {
    RawLine("=========================== SESSION ==========================\n");
    RawLine("OrbitLogger version: " ORBIT_LOGGER_VERSION_STRING "\n");
    RawLine("OrbitLogger build date: " __DATE__ " at " __TIME__ "\n");
#ifdef CONFIGURATION_NAME
    RawLine("OrbitLogger Build configuration: " CONFIGURATION_NAME "\n");
#endif
#ifdef OS_NAME
    RawLine("OS: " OS_NAME "\n");
#endif
    RawLine("Start date: ");

    std::time_t t = std::time(NULL);
    char mbstr[100];
    // TODO: std::localtime is not safe
    if (std::strftime(mbstr, sizeof(mbstr), "%A %c", std::localtime(&t))) {
        RawLine(mbstr);
        RawLine("\n");
    } else
        RawLine("{unable to covert date}\n");

    RawLine("\n");
}

//----------------------------------------------------------------------------------

LogStandardFormatter::LogStandardFormatter() {
#ifdef ORBITLOGGER_EXTENDED_LOG_COUNTERS
    m_Line = 0;
    memset(m_Type, 0, sizeof(m_Type));
#endif
}

void LogStandardFormatter::Format(const LogLine *line, char *buffer, size_t buffer_size) {
    char LogHeader[128];

    ThreadInfo::SignatureBuffer thname(line->m_ThreadSign);
#ifdef ORBITLOGGER_EXTENDED_LOG_COUNTERS
    snprintf(LogHeader, sizeof(LogHeader), "[%4s:%4d][%7.3f:%4d][%4s:%04x]", line->m_ModeStr,
             NextType(line->m_SourceInfo->m_Channel), line->m_ExecutionSecs, NextLine(), thname.m_String,
             line->m_ThreadID);
#else
    snprintf(LogHeader, sizeof(LogHeader), "[%4s][%7.3f][%4s:%04x]", line->m_ModeStr, line->m_ExecutionSecs,
             thname.m_String, line->m_ThreadID);
#endif

    char LogLocation[1024];
    if (line->m_SourceInfo && line->m_SourceInfo->m_File && line->m_SourceInfo->m_Function) {
        auto *srcinfo = line->m_SourceInfo;
#ifndef WIN32_LEAN_AND_MEAN
        const char *f = strrchr(srcinfo->m_File, '\\');
#elif defined(LINUX)
#error Missing Linux implementation
#else
#error Unknown OS
#endif

        if (!f)
            f = srcinfo->m_File;
        else
            ++f;

        if (srcinfo->m_Line)
            snprintf(LogLocation, sizeof(LogLocation), "[%s:%d@%s] ", f, srcinfo->m_Line, srcinfo->m_Function);
        else
            snprintf(LogLocation, sizeof(LogLocation), "[%s] ", f);
    } else
        LogLocation[0] = 0;

    snprintf(buffer, buffer_size, "%s%s%s\n", LogHeader, LogLocation, line->m_Message);
}

//----------------------------------------------------------------------------------

LogShortFormatter::LogShortFormatter() {
#ifdef ORBITLOGGER_EXTENDED_LOG_COUNTERS
    m_Line = 0;
    memset(m_Type, 0, sizeof(m_Type));
#endif
}

void LogShortFormatter::Format(const LogLine *line, char *buffer, size_t buffer_size) {
    char LogHeader[128];

    ThreadInfo::SignatureBuffer thname(line->m_ThreadSign);
#ifdef ORBITLOGGER_EXTENDED_LOG_COUNTERS
    snprintf(LogHeader, sizeof(LogHeader), "[%4s:%4d][%7.3f:%4d][%4s:%04x]", line->m_ModeStr,
             NextType(line->m_SourceInfo->m_Channel), line->m_ExecutionSecs, NextLine(), thname.m_String,
             line->m_ThreadID);
#else
    snprintf(LogHeader, sizeof(LogHeader), "[%4s][%7.3f][%4s:%04x]", line->m_ModeStr, line->m_ExecutionSecs,
             thname.m_String, line->m_ThreadID);
#endif

    char LogLocation[1024];
    if (line->m_SourceInfo && line->m_SourceInfo->m_File) {
        auto *srcinfo = line->m_SourceInfo;
#ifndef WIN32_LEAN_AND_MEAN
        const char *f = strrchr(srcinfo->m_File, '\\');
#elif defined(LINUX)
#error Missing Linux implementation
#else
#error Unknown OS
#endif

        if (!f)
            f = srcinfo->m_File;
        else
            ++f;

        if (srcinfo->m_Line)
            snprintf(LogLocation, sizeof(LogLocation), "[%s:%d] ", f, srcinfo->m_Line);
        else
            snprintf(LogLocation, sizeof(LogLocation), "[%s] ", f);
    } else
        LogLocation[0] = 0;

    snprintf(buffer, buffer_size, "%s%s%s\n", LogHeader, LogLocation, line->m_Message);
}

} // namespace OrbitLogger
