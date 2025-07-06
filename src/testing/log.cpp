//Copyright 2025-Present riplin

#ifdef DEBUG

#include <unordered_set>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <hag/testing/log.h>

namespace Hag::Testing::Log
{

static bool s_Enabled = false;
static char s_FileName[256];

void Configure(const char* fileName)
{
    assert(fileName != nullptr);
    strcpy(s_FileName, fileName);
    remove(fileName);
    s_Enabled = true;
}

static std::unordered_set<const char*> s_Focus;

void FocusCategory(const char* category)
{
    s_Focus.insert(category);
}

static bool Include(const char* category)
{
    if (!s_Focus.empty())
    {
        return s_Focus.contains(category);
    }
    return true;
}

void Log(const char* category, const char* format, ...)
{
    if (s_Enabled && Include(category))
    {
        va_list arg;
        va_start (arg, format);
        FILE* fp = fopen(s_FileName, "a");
        if (fp != nullptr)
        {
            fseek(fp, 0, SEEK_END);
            fprintf(fp, "[%s] ", category);
            vfprintf(fp, format, arg);
            fprintf(fp, "\n");
            fclose(fp);
        }
        va_end (arg);
    }
}

}

#endif