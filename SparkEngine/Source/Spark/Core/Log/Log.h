#pragma once

namespace Spark
{
    enum class LogLevel
    {
        None,
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };

    class LoggerStreamHandler
    {
    public:
        virtual ~LoggerStreamHandler() = default;
        virtual void Write(const std::string& message) = 0;
        virtual void Rotate() = 0;
    };

    class LogFilter
    {
    public:
        virtual ~LogFilter() = default;
        virtual bool Filter(const std::string& message) = 0;
    };

    class Logger
    {
    public:

        // [TAG] [xxxx-xx-xx xx:xx:xx] [level] [filename:line] message
        template<typename... Args>
        void SyncLogger(LogLevel level, const char* filename, const char* format, Args&&... args);

        template<typename... Args>
        void AsyncLogger(LogLevel level, const char* filename, const char* format, Args&&... args);

        void SetLevel(LogLevel level);
        void AddStreamHandler(LoggerStreamHandler* handler);
        void RemoveStreamHandler(LoggerStreamHandler* handler);
        void RemoveAllStreamHandler();

        void AddFilter(LogFilter* filter);
        void RemoveFilter(LogFilter* filter);
        void RemoveAllFilter();

    private:
        LogLevel m_CurrentLevel = LogLevel::Info;
        std::vector<LogFilter*> m_Filters;
        std::vector<LoggerStreamHandler*> m_Handlers;
    };
}
