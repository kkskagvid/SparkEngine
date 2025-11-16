#include "SparkPCH.h"

#include <unicode/unistr.h>
#include <unicode/putil.h>
#include <unicode/ucnv.h>

#include "Spark/Core/String/StringUtils.h"

namespace Spark
{
    std::wstring ConvertCStringToWString(const char* mbStr, const char* codePage)
    {
        UErrorCode status = U_ZERO_ERROR;
        UConverter* conv = ucnv_open(codePage, &status);
        if (U_FAILURE(status))
        {
            throw std::logic_error("Failed to open converter for code page: " + std::string(codePage));
        }

        // 计算源长度
        int32_t srcLen = static_cast<int32_t>(strlen(mbStr));
        const char* src = mbStr;
        const char* srcLimit = mbStr + srcLen;

        // 初始缓冲区大小
        // 预分配足够空间
        int32_t destCapacity = srcLen * 4 + 1; 
        std::vector<UChar> buffer(destCapacity);
        UChar* dest = buffer.data();
        const UChar* destLimit = dest + destCapacity;

        // 执行转换
        ucnv_toUnicode(conv, &dest, destLimit, &src, srcLimit, nullptr, false, &status);

        // 检查是否需要扩容
        if (U_FAILURE(status) && status != U_BUFFER_OVERFLOW_ERROR)
        {
            ucnv_close(conv);
            throw std::logic_error("Conversion from " + std::string(codePage) + " to UTF-16 failed.");
        }

        // 如果缓冲区不足，重新分配
        while (status == U_BUFFER_OVERFLOW_ERROR)
        {
            destCapacity *= 2;
            buffer.resize(destCapacity);
            dest = buffer.data();
            destLimit = dest + destCapacity;
            src = mbStr; // 重置源指针
            ucnv_toUnicode(conv, &dest, destLimit, &src, srcLimit, nullptr, false, &status);
        }

        ucnv_close(conv);

        // 构造宽字符串
#if defined(SPARK_PLATFORM_WINDOWS)
        return std::wstring(reinterpret_cast<wchar_t*>(buffer.data()), u_strlen(buffer.data()));
#else
        // Linux: 需转换 UTF-16 -> UTF-32
        std::vector<UChar32> utf32Buffer(destCapacity);
        int32_t utf32Length = u_strToUTF32(utf32Buffer.data(), destCapacity, nullptr, buffer.data(), -1, U_UNICODE_STRING_NOT_NULL_TERMINATED, &status);
        if (U_FAILURE(status))
        {
            throw std::logic_error("Conversion from UTF-16 to UTF-32 failed.");
        }
        return std::wstring(reinterpret_cast<wchar_t*>(utf32Buffer.data()), utf32Length);
#endif
    }

    std::vector<std::string> ConvertWStringArgsToStringVector(LPWSTR* argv)
    {
        std::vector<std::string> result;
        if (!argv) return result;

        for (int i = 0; argv[i]; ++i)
        {
            icu::UnicodeString unicodeStr(reinterpret_cast<const UChar*>(argv[i]));
            std::string utf8Str;
            unicodeStr.toUTF8String(utf8Str);
            result.push_back(utf8Str);
        }

        return result;
    }

    char** ConvertWStringArgsToCharArray(LPWSTR* argv)
    {
        if (!argv) return nullptr;

        int count = 0;
        while (argv[count]) ++count;

        char** result = new char* [count + 1];
        for (int i = 0; i < count; ++i)
        {
            icu::UnicodeString unicodeStr(reinterpret_cast<const UChar*>(argv[i]));
            std::string utf8Str;
            unicodeStr.toUTF8String(utf8Str);
            result[i] = new char[utf8Str.size() + 1];
            std::strcpy(result[i], utf8Str.c_str());
        }
        result[count] = nullptr;

        return result;
    }

    void FreeCharArray(char** argv)
    {
        if (argv)
        {
            delete[] argv;
        }
    }
}
