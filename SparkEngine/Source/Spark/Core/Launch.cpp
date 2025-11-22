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
    Spark::Core::Logger& logger = Spark::Core::Logger::GetLogger("Test");

    std::shared_ptr<Spark::Core::ConsoleLoggerStreamHandler> clsh = std::make_shared<Spark::Core::ConsoleLoggerStreamHandler>();
    logger.AddStreamHandler(clsh.get());

    logger.SyncLogger(Spark::Core::LogLevel::Info, __FILE__, __LINE__, "This is a test log: {}", 123);
    logger.SyncLogger(Spark::Core::LogLevel::Debug, __FILE__, __LINE__, "Debug log: {}", 456);
    logger.SyncLogger(Spark::Core::LogLevel::Error, __FILE__, __LINE__, "Error log: {}", "An error occurred");
}

LAUNCH_API int EngineMain(char** argv)
{
    TryAttachDebuggerWindowInWindows();
    RedirectStdOutputToConsole();

    TestLogger();

    while (true);

    return 0;
}
