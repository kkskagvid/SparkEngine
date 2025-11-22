#pragma once

namespace Spark::Core
{
    class ConsoleLoggerStreamHandler : public LoggerStreamHandler
    {
    public:
        ~ConsoleLoggerStreamHandler() = default;
        void Write(const std::string& message) override;
        void Rotate() override;
    };
}
