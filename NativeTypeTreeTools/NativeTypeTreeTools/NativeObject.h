#pragma once
#include "Util.h"
#include "SymbolImporter.h"
#include "RuntimeTypeInfo.h"
#include "TypeTree.h"
struct Object {
	void* virtualFunctionTable;
	int32_t instanceID;
	uint32_t bits;
	bool IsPersistent() {
#if VERSION_GE(5, 0)
	return (bits & (1 << 20)) != 0;
#else
	return (bits & (1 << 19)) != 0;
#endif
	}
	static void InitBindings(SymbolImporter& importer);
	static Object* GetOrProduce(RuntimeTypeInfo* type, int instanceID, ObjectCreationMode creationMode);
	static void DestroyObject(Object* obj, RuntimeTypeInfo* type);
	static void ProduceTypeTree(Object* obj, TypeTree* tree);
	static void DumpStructLayout();
};
class MonoObject;
class NativeObject;