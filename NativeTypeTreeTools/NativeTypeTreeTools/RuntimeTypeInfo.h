#pragma once
#include "Util.h"
#include "Enums.h"
#include "SymbolImporter.h"
#include <vector>
struct DerivedFromInfo
{
	uint32_t typeIndex;
	uint32_t descendantCount;
};

struct TypeTreeString {
	char Data[28];
	char* c_str();
};

//std::list<TypeTree,stl_allocator<TypeTree,65,4>>
struct TypeTreeList {
	char Data[16];
};


struct RuntimeTypeInfo {
#ifdef UNITY_2017_3_OR_NEWER
	RuntimeTypeInfo* base;
	void* factory; // probably constructor
	const char* className;
	const char* classNamespace;
	const char* module;
	PersistentTypeID persistentTypeID;
	int32_t size;
	DerivedFromInfo derivedFromInfo;
	bool isAbstract;
	bool isSealed;
	bool isEditorOnly;
	void* attributes;
	uint64_t attributeCount;
#elif VERSION_GE(5, 0)
	RuntimeTypeInfo* base;
	void* factory; // probably constructor
	const char* className;
	const char* classNamespace;
	PersistentTypeID persistentTypeID;
	int32_t size;
	DerivedFromInfo derivedFromInfo;
	bool isAbstract;
	bool isSealed;
	bool isEditorOnly;
#else
	RuntimeTypeInfo* base;
	void* factory;
	PersistentTypeID persistentTypeID;
	TypeTreeString className;
	int size;
	bool isAbstract;
#endif
	static void InitBindings(SymbolImporter& importer);
	static std::vector<RuntimeTypeInfo*> GetRuntimeTypes();
	static void DumpStructLayout();
	const char* GetName();
};

struct RuntimeTypeArray
{
	size_t count;
	RuntimeTypeInfo* Types[1];
};