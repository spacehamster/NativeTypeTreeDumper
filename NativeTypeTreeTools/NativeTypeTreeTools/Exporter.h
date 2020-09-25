#pragma once
#include "Util.h"
class Exporter {
public:
	Exporter(const char* moduleName);
	void ExportStringData();
	void ExportRTTIDump();
	void ExportClassesJson();
	void ExportStructData(const char* unityVersion, uint32_t runtimePlatform);
	void ExportStructDump();
private:
	const char* moduleName;
	void InitBindings();
};
