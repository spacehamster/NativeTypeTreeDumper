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