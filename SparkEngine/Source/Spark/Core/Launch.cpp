#include "SparkPCH.h"

#include "Spark/Core/ThreadPool.h"

#include "Spark/Core/Event/EventCategory.h"
#include "Spark/Core/Event/Event.h"

#include "Spark/Core/Log/Log.h"
#include "Spark/Core/Log/ConsoleLoggerStreamHandler.h"


#include "Spark/Core/Launch.h"

void TryAttachDebuggerWindowInWindows()
{
    if (::AttachConsole(ATTACH_PARENT_PROCESS))
    {
        std::cout << "Attached to parent process console" << std::endl;
    }
    if (!::AllocConsole())
    {
        std::cout << "Failed to allocate console" << std::endl;
    }
}

void RedirectStdOutputToConsole()
{
    FILE* unused;
    if (freopen_s(&unused, "CONOUT$", "w", stdout))
    {
        std::cout << "Failed to redirect stdout" << std::endl;
    }
    if (freopen_s(&unused, "CONOUT$", "w", stderr))
    {
        std::cout << "Failed to redirect stderr" << std::endl;
    }
}

void TestLogger()
{
    Spark::Logger& logger = Spark::Logger::GetLogger("Test");

    Spark::ConsoleLoggerStreamHandler* clsh = {};
    logger.AddStreamHandler(clsh);

    logger.SyncLogger(Spark::LogLevel::Info, __FILE__, "This is a test log: {}", 123);
    logger.SyncLogger(Spark::LogLevel::Debug, __FILE__, "Debug log: {}", 456);
    logger.SyncLogger(Spark::LogLevel::Error, __FILE__, "Error log: {}", "An error occurred");
}

LAUNCH_API int EngineMain(char** argv)
{
    TryAttachDebuggerWindowInWindows();
    RedirectStdOutputToConsole();

    TestLogger();

    while (true);

    return 0;
}
