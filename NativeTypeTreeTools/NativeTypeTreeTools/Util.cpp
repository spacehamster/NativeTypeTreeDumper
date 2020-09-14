#include "util.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
FILE* g_Log = NULL;
void Log(const char* fmt, ...) {
    if (g_Log == NULL) {
		CreateDirectory(L"Logs", NULL);
        g_Log = fopen("Logs/NativeTypeTreeToolsLog.txt", "w");
    }
    va_list argp;
    va_start(argp, fmt);
    vfprintf(g_Log, fmt, argp);
    va_end(argp);
    fflush(g_Log);
}
void CloseLog() {
    if (g_Log != NULL) {
        fclose(g_Log);
        g_Log = NULL;
    }
}
std::wstring Widen(const char* str) {
    size_t cSize = strnlen_s(str, MAX_PATH) + 1;
    std::wstring wModuleName(cSize, L'#');
    size_t charConvertedCount;
    mbstowcs_s(&charConvertedCount, &wModuleName[0], cSize, str, cSize - 1);
    return wModuleName;
}