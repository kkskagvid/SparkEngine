#pragma once
#pragma warning(disable : 4996)

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/os.h>

namespace Spark::Core
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
        static Logger& GetLogger();
        static Logger& GetLogger(const char* name);

        // [xxxx-xx-xx xx:xx:xx] [level] [TAG] [filename:line] message
        template<typename... Args>
        void SyncLogger(LogLevel level, const char* filename, int line, const char* format, Args&&... args)
        {
            std::lock_guard<std::mutex> lock(mtx);

            std::string_view levelStr = LogLevelToString(level);
            std::string time = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(std::time(nullptr)));
            std::string formatMessage = fmt::format(format, std::forward<Args>(args)...);

            std::string message = fmt::format("[{}] [{}] [{}] [{}:{}] {}", time, levelStr, this->m_Name, filename, line, formatMessage);

            if (!m_Filters.empty())
            {
                for (const auto& filter : m_Filters)
                {
                    if (!filter->Filter(message))
                    {
                        return;
                    }
                }
            }

            for (auto handler : m_Handlers)
            {
                handler->Write(message);
            }
        }


        template<typename... Args>
        void AsyncLogger(LogLevel level, const char* filename, const char* format, Args&&... args)
        {

        }

        void SetLevel(LogLevel level);
        void AddStreamHandler(LoggerStreamHandler* handler);
        void RemoveStreamHandler(LoggerStreamHandler* handler);
        void RemoveAllStreamHandler();

        void AddFilter(LogFilter* filter);
        void RemoveFilter(LogFilter* filter);
        void RemoveAllFilter();

    private:
        Logger();
        Logger(const char* name);
        ~Logger() = default;

        std::string_view LogLevelToString(LogLevel level);

        std::string m_Name;
        LogLevel m_CurrentLevel = LogLevel::Info;
        std::vector<LogFilter*> m_Filters = {};
        std::vector<LoggerStreamHandler*> m_Handlers = {};
        std::mutex mtx;
    };
}
