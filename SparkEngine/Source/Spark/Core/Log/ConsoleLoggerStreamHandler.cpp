#include "SparkPCH.h"

#include "Spark/Core/Log/Log.h"

#include "Spark/Core/Log/ConsoleLoggerStreamHandler.h"

namespace Spark::Core
{
    void ConsoleLoggerStreamHandler::Write(const std::string& message)
    {
        std::cout << message << std::endl;
    }

    void ConsoleLoggerStreamHandler::Rotate()
    {
    }
}
