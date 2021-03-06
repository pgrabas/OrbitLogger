#pragma once

#include "orbit_logger.h"

namespace OrbitLogger {

struct LogFileOutputPolicy : public LogSinkBasePolicy {
	~LogFileOutputPolicy() {
		m_File << "\n\n";
		m_File.close();
	}
    void Open(const std::string &file, bool append = true) {
        m_File.open(file, std::ios::out | (append ? std::ios::app : 0));
        GetSinkBase().PrintBanner();
    }
	void Open(const char *file, bool append = true) {
		m_File.open(file, std::ios::out | (append ? std::ios::app : 0));
		GetSinkBase().PrintBanner();
	}
	void Write(const LogLine *line, const char *c) {
		m_File << c << std::flush;
	}
protected:
	std::ofstream m_File;
};

template<typename FilteringPolicy, typename FormatPolicy>
struct BaseFileLoggerSink : public LogSink <LogFileOutputPolicy, FilteringPolicy, FormatPolicy > {
	BaseFileLoggerSink() { }

	template<typename ... ARGS>
	BaseFileLoggerSink(ARGS&&...args) {
		this->Open(std::forward<ARGS>(args)...);
	}
};

using StdFileLoggerSink = BaseFileLoggerSink <LogStandardFormatter, LogNoFilteringPolicy>;
using StdNoDebugFileLoggerSink = BaseFileLoggerSink <LogStandardFormatter, LogNoDebugFilteringPolicy>;

} //namespace OrbitLogger
