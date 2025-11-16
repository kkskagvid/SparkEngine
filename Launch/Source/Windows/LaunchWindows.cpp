#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shellapi.h>

#include <Spark/Core/Launch.h>
#include <spark/Core/String/StringUtils.h>

// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
// The following line is to favor the high performance NVIDIA GPU if there are multiple GPUs
// Has to be .exe module to be correctly detected.
extern "C" { _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001; }

// And the AMD equivalent
// Also has to be .exe module to be correctly detected.
extern "C" { _declspec(dllexport) unsigned int AmdPowerXpressRequestHighPerformance = 0x00000001; }

int LaunchWindowsStartup()
{
    int args;
    LPWSTR cmdStr = ::GetCommandLineW();
    LPWSTR* argvStr = CommandLineToArgvW(cmdStr, &args);
    char** argv = Spark::ConvertWStringArgsToCharArray(argvStr);

    int errorCode = EngineMain(argv);

    HeapFree(GetProcessHeap(), 0, argvStr);
    Spark::FreeCharArray(argv);

    return errorCode;
}

void LaunchWindowsShutdown()
{

}

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    int result = LaunchWindowsStartup();
    LaunchWindowsShutdown();

    return result;
}
