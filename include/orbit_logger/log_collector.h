#pragma once

#include <cstdarg>
#include <memory>
#include <orbit_logger/log_line_sink.h>

namespace OrbitLogger {

class LogCollector {
public:
    struct LogCollectorImpl;

    static bool Start();
    static bool Stop();
    static bool IsRunning();

    static bool SetCaptureStdOut(LogChannel ch = LogChannels::Info);
    static bool SetCaptureStdErr(LogChannel ch = LogChannels::Error);

    static void PushLinePtr(const LogLineSourceInfo *SourceInfo, const char *line);
    static void PushLineCopy(const LogLineSourceInfo *SourceInfo, const char *line, size_t length);

    template <size_t LEN> static void PushLine(const LogLineSourceInfo *SourceInfo, const char line[LEN]) {
        PushLinePtr(SourceInfo, line);
    }

    static void PushLine(const LogLineSourceInfo *SourceInfo, const char *fmt, ...) {
        char buffer[Configuration::StringFormatBuffer];
        va_list args;
        va_start(args, fmt);
        int length = vsprintf_s(buffer, fmt, args);
        va_end(args);

        if (length < 0)
            return;

        PushLineCopy(SourceInfo, buffer, length);
    }

    static void PushLine(const LogLineSourceInfo *SourceInfo, const std::ostringstream &ss);
    static bool IsLineEnabled(const LogLineSourceInfo *SourceInfo);

    template <class T, class... ARGS> static T *AddLogSink(ARGS &&... args) {
        return dynamic_cast<T *>(InsertLogSink(std::make_unique<T>(std::forward<ARGS>(args)...)));
    }

    static void SetChannelName(LogChannel Channel, const char *Name, bool EnableChannel = true);
    static void SetChannelState(LogChannel Channel, bool Enabled);

    static bool IsChannelEnabled(LogChannel Channel);
    static bool IsChannelEnabled(const LogLineSourceInfo *SourceInfo) {
        return IsChannelEnabled(SourceInfo->m_Channel);
    }

    struct ChannelInfo {
        bool m_Enabled;
        LogChannel m_Channel;
        const char *m_Name;
        uint32_t m_LinesPushed;
    };
    using ChannelInfoTable = std::array<ChannelInfo, LogChannels::MaxLogChannels>;
    static bool GetChannelInfo(ChannelInfoTable &table);

    static iLogSinkBase *InsertLogSink(std::unique_ptr<iLogSinkBase> sink);

    static LogCollectorImpl *GetInstance() { return s_Instance.m_Impl.get(); }
    static void SetExternalInstance(LogCollectorImpl *instance);

protected:
    LogCollector();
    ~LogCollector();

    std::unique_ptr<LogCollectorImpl> m_Impl;
    bool m_LocalInstance;

private:
    static LogCollector s_Instance;
};

} // namespace OrbitLogger
