#include "SparkPCH.h"

#include "Spark/Core/Log/Log.h"

#define LOG_LEVEL_CASE(level) \
    case LogLevel::level: return #level;

namespace Spark::Core
{
    Logger& Logger::GetLogger()
    {
        static Logger logger;
        return logger;
    }

    Logger& Logger::GetLogger(const char* name)
    {
        static Logger logger(name);
        return logger;
    }

    void Logger::SetLevel(LogLevel level)
    {
        this->m_CurrentLevel = level;
    }

    void Logger::AddStreamHandler(LoggerStreamHandler* handler)
    {
        this->m_Handlers.push_back(handler);
    }

    void Logger::RemoveStreamHandler(LoggerStreamHandler* handler)
    {
        this->m_Handlers.erase(std::remove(m_Handlers.begin(), m_Handlers.end(), handler), m_Handlers.end());
    }

    void Logger::RemoveAllStreamHandler()
    {
        this->m_Handlers.clear();
    }

    void Logger::AddFilter(LogFilter* filter)
    {
        this->m_Filters.push_back(filter);
    }

    void Logger::RemoveFilter(LogFilter* filter)
    {
        this->m_Filters.erase(std::remove(m_Filters.begin(), m_Filters.end(), filter), m_Filters.end());
    }

    void Logger::RemoveAllFilter()
    {
        this->m_Filters.clear();
    }

    Logger::Logger()
    {

    }

    Logger::Logger(const char* name)
    {
        this->m_Name = name;
    }

    std::string_view Logger::LogLevelToString(LogLevel level)
    {
        switch (level)
        {
            LOG_LEVEL_CASE(None)
            LOG_LEVEL_CASE(Trace)
            LOG_LEVEL_CASE(Debug)
            LOG_LEVEL_CASE(Info)
            LOG_LEVEL_CASE(Warn)
            LOG_LEVEL_CASE(Error)
            LOG_LEVEL_CASE(Fatal)
        default:
            return "UNKOWN";
        }
    }
}
