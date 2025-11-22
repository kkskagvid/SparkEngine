#pragma once
#pragma warning(disable : 4996)

#include "SparkPCH.h"

#include "Spark/Core/Launch.h"

namespace Spark::Core
{
    /**
     * @brief
     * 将 C 风格字符串转换为 wstring 字符串
     * 转换后的 wstring 字符串，转换失败返回空
     * 
     * @param mbStr 需要转换的多字节字符串（NULL 终止）
     * @param codePage 编码页
     */
    LAUNCH_API std::wstring ConvertCStringToWString(const char* mbStr, const char* codePage = "UTF-8");

    /**
     * @brief
     * 将 LPWSTR* 转换为 char**
     * 自动管理内存的 std::vector<std::string>，避免手动释放
     * 
     * @param argv LPWSTR* 参数数组（以 NULL 结尾）
     */
    LAUNCH_API std::vector<std::string> ConvertWStringArgsToStringVector(LPWSTR* argv);
    
    /**
     * @brief 
     * 将 LPWSTR* 参数数组转换为 char** 类型参数数组，并返回 C 风格指针（需手动释放）
     * 转换后的 char** 类型数组，以 NULL 结尾，需调用 FreeCharArray 释放内存
     * 
     * @param argv 输入的 LPWSTR* 参数数组（以 NULL 结尾）
     */
    LAUNCH_API char** ConvertWStringArgsToCharArray(LPWSTR* argv);

    /**
     * @brief 释放 ConvertWStringArgsToCharArray 分配的内存
     * 
     * @param argv 需要释放的参数数组
     */
    LAUNCH_API void FreeCharArray(char** argv);
};
