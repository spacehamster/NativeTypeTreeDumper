#include "Exporter.h"
#include "NativeObject.h"
#include "RuntimeTypeInfo.h"
#include "TypeTree.h"
#include "Defines.h"
#include "dia2.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Util.h"
#include "SymbolImporter.h"
#include "Structs.h"
#include "Enums.h"
#define EXPORT __declspec(dllexport) 
extern "C" {
	EXPORT void ExportStringData(const char* moduleName) {
#if VERSION_GE(5, 0)
		try {
			Exporter exporter(moduleName);
			exporter.ExportStringData();
		}
		catch (std::exception err) {
			Log("Error: %s\n", err.what());
		}
		CloseLog();
#endif
	}
	EXPORT void ExportRTTIDump(const char* moduleName) {
		try {
			Exporter exporter(moduleName);
			exporter.ExportRTTIDump();
		}
		catch (std::exception err) {
			Log("Error: %s\n", err.what());
		}
		CloseLog();
	}
	EXPORT void ExportClassesJson(const char* moduleName) {
		Log("ExportClassesJson\n");
		try {
			Exporter exporter(moduleName);
			exporter.ExportClassesJson();
		}
		catch (std::exception err) {
			Log("Error: %s\n", err.what());
		}
		CloseLog();
	}
	EXPORT void ExportStructData(const char* moduleName, const char* unityVersion, uint32_t runtimePlatform) {
		try {
			Exporter exporter(moduleName);
			exporter.ExportStructData(unityVersion, runtimePlatform);
		}
		catch (std::exception err) {
			Log("Error: %s\n", err.what());
		}
		CloseLog();
	}
	EXPORT void ExportStructDump(const char* moduleName) {
		try {
			Exporter exporter(moduleName);
			exporter.ExportStructDump();
		}
		catch (std::exception err) {
			Log("Error: %s\n", err.what());
		}
		CloseLog();
	}
	EXPORT void DumpStructDebug() {
		TypeTree::DumpStructLayout();
		Object::DumpStructLayout();
		RuntimeTypeInfo::DumpStructLayout();
		LOG_TYPE(MemLabelId);
#if VERSION_GE(5, 3)
		LOG_MEMBER(MemLabelId, m_RootReferenceWithSalt);
		LOG_MEMBER(MemLabelId, label);
		Log("\n");

		LOG_TYPE(AllocationRootWithSalt);
		LOG_MEMBER(AllocationRootWithSalt, m_Salt);
		LOG_MEMBER(AllocationRootWithSalt, m_RootReferenceIndex);
		Log("\n");
#endif
		CloseLog();
	}
}


