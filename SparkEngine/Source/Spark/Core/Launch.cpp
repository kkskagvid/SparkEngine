#include "SparkPCH.h"

#include "Spark/Core/ThreadPool.h"
#include "Spark/Core/Event/EventCategory.h"
#include "Spark/Core/Event/Event.h"

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

LAUNCH_API int EngineMain(char** argv)
{
    TryAttachDebuggerWindowInWindows();

    while (true);

    return 0;
}
