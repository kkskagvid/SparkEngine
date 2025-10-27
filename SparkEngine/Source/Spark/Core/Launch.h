#pragma once

#ifdef SPARK_PLATFORM_WINDOWS
    #ifdef SPARK_BUILD_DLL
        #define LAUNCH_API __declspec(dllexport)
    #else
        #define LAUNCH_API __declspec(dllimport)
    #endif
#else
    #define LAUNCH_API
#endif

/**
* 引擎主入口
* @param argv char** 类型参数数组
*/
LAUNCH_API int EngineMain(char** argv);
