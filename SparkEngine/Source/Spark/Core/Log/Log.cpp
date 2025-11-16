#include "SparkPCH.h"

#include "Spark/Core/Log/Log.h"

namespace Spark
{
    template<typename ...Args>
    inline void Logger::SyncLogger(LogLevel level, const char* filename, const char* format, Args && ...args)
    {
    }

    template<typename ...Args>
    void Logger::AsyncLogger(LogLevel level, const char* filename, const char* format, Args && ...args)
    {
    }

    void Logger::SetLevel(LogLevel level)
    {
    }

    void Logger::AddStreamHandler(LoggerStreamHandler* handler)
    {
    }

    void Logger::RemoveStreamHandler(LoggerStreamHandler* handler)
    {
    }

    void Logger::RemoveAllStreamHandler()
    {
    }

    void Logger::AddFilter(LogFilter* filter)
    {
    }

    void Logger::RemoveFilter(LogFilter* filter)
    {
    }

    void Logger::RemoveAllFilter()
    {
    }
}
