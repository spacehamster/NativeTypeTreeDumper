#ifndef UTIL_H
#define UTIL_H
void Log(const char* fmt, ...);
void CloseLog();
#define MEMBER_SIZE(type, field) sizeof(((type *)0)->field)
#define LOG_TYPE(type) Log("%s: %d (0x%x)\n", typeid(type).name(), sizeof(type), sizeof(type))
#define LOG_MEMBER(type, field) Log("    %s: offset %d (0x%x) size %d (0x%x)\n", #field, offsetof(type, field), offsetof(type, field), MEMBER_SIZE(type, field), MEMBER_SIZE(type, field));
#endif