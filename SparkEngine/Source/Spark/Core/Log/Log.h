#pragma once

namespace Spark
{
    // 日志级别枚举
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

    class LogOutput
    {
    public:
        virtual ~LogOutput() = default;
        virtual void Write(const std::string& message) = 0;
        virtual void Rotate() = 0;
    };

    class ConsoleOutput : public LogOutput {
    public:
        void Write(const std::string& message) override
        {
            std::cout << message;
        }

        void Rotate() override {}
    };

    // 日志轮转和大小限制的文件输出
    class FileOutput : public LogOutput {
    public:
        FileOutput(const std::string& filename, size_t maxSize = 10 * 1024 * 1024)
            : filename(filename), maxSize(maxSize), file(filename, std::ios::app)
        {
            if (!file.is_open())
            {
                throw std::runtime_error("Failed to open log file: " + filename);
            }
            // 检查文件大小，如果超过maxSize，则轮转
            if (file.tellp() >= maxSize)
            {
                Rotate();
            }
        }

        void Write(const std::string& message) override
        {
            // 检查文件大小，进行轮转
            if (file.tellp() >= maxSize)
            {
               Rotate();
            }

            file << message;
            file.flush();
        }

        void Rotate() override
        {
            // 重命名当前文件
            std::string rotatedName = filename + "." + std::to_string(std::time(nullptr));
            std::rename(filename.c_str(), rotatedName.c_str());

            // 重新打开新文件
            file.close();
            file.open(filename, std::ios::app);
            if (!file.is_open())
            {
                throw std::runtime_error("Failed to open rotated log file: " + filename);
            }
        }

    private:
        std::string filename;
        size_t maxSize;
        std::ofstream file;
    };

    class Logger
    {
    public:
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        static Logger& GetInstance()
        {
            static Logger instance;
            return instance;
        }

    private:
        Logger() = default;

    };
}
