// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include "Defines.h"
#include "dllmain.h"
#include "dia2.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include "PdbSymbolImporter.h"
#include "Util.h"
#include "Structs.h"
#include "PersistentTypeID.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
typedef void(__cdecl* GenerateTypeTree_t)(Object* object, TypeTree* typeTree, TransferInstructionFlags options);
GenerateTypeTree_t GenerateTypeTree;
#ifdef UNITY_2017_3_OR_NEWER
typedef Object* (__cdecl* Object__Produce_t)(struct RTTIClass* classInfo, struct RTTIClass* classInfo2, int instanceID, MemLabelId memLabel, ObjectCreationMode mode);
#else
typedef Object* (__cdecl* Object__Produce_t)(struct RTTIClass* classInfo, int instanceID, MemLabelId memLabel, ObjectCreationMode mode);
#endif
Object__Produce_t Object__Produce;


typedef MonoObject*(__cdecl* EditorUtility_CUSTOM_InstanceIDToObject_t)(int instanceID);
EditorUtility_CUSTOM_InstanceIDToObject_t EditorUtility_CUSTOM_InstanceIDToObject;

typedef void(__cdecl* Object_CUSTOM_DestroyImmediate_t)(void* pcriptingBackendNativeObjectPtrOpaque, bool allowDestroyingAssets);
Object_CUSTOM_DestroyImmediate_t Object_CUSTOM_DestroyImmediate;

#if defined(UNITY_2019_1) || defined(UNITY_2019_2)
typedef void(__thiscall* TypeTree__TypeTree_t)(TypeTree* self, MemLabelId* memLabel, bool unk);
TypeTree__TypeTree_t TypeTree__TypeTree;
#else
typedef void(__thiscall* TypeTree__TypeTree_t)(TypeTree* self, MemLabelId* memLabel);
TypeTree__TypeTree_t TypeTree__TypeTree;
#endif
#ifdef UNITY_2019_1_OR_NEWER
typedef bool(__cdecl* GetTypeTree_t)(Object* obj, TransferInstructionFlags flags, TypeTree* tree);
GetTypeTree_t GetTypeTree;
#endif
typedef Object*(__cdecl* GetSpriteAtlasDatabase_t)(void);
GetSpriteAtlasDatabase_t GetSpriteAtlasDatabase;

typedef Object* (__cdecl* GetSceneVisibilityState_t)(void);
GetSceneVisibilityState_t GetSceneVisibilityState;

typedef Object* (__cdecl* GetInspectorExpandedState_t)(void);
GetInspectorExpandedState_t GetInspectorExpandedState;

typedef Object* (__cdecl* GetAnnotationManager_t)(void);
GetAnnotationManager_t GetAnnotationManager;

typedef Object* (__cdecl* GetMonoManager_t)(void);
GetMonoManager_t GetMonoManager;

char** CommonString_BufferBegin;
char** CommonString_BufferEnd;
RuntimeTypeArray* gRuntimeTypeArray;
MemLabelId* kMemBaseObject;
MemLabelId* kMemTypeTree;
void InitBindings(const char* moduleName) {
    PdbSymbolImporter importer;
    if (!importer.LoadFromExe(moduleName)) {
        return;
    };
    unsigned long long address;
	importer.AssignAddress("?kMemBaseObject@@3UMemLabelId@@A",
		(void*&)kMemBaseObject);
	importer.AssignAddress("?kMemTypeTree@@3UMemLabelId@@A",
		(void*&)kMemTypeTree);
    importer.AssignAddress("?BufferBegin@CommonString@Unity@@3QEBDEB", (void*&)CommonString_BufferBegin);
    importer.AssignAddress("?BufferEnd@CommonString@Unity@@3QEBDEB", (void*&)CommonString_BufferEnd);
#ifdef UNITY_2017_3_OR_NEWER
    importer.AssignAddress("?ms_runtimeTypes@RTTI@@0URuntimeTypeArray@1@A", (void*&)gRuntimeTypeArray);
	importer.AssignAddress("?Produce@Object@@CAPEAV1@PEBVType@Unity@@0HUMemLabelId@@W4ObjectCreationMode@@@Z",
		(void*&)Object__Produce);
#else
	importer.AssignAddress("?ms_runtimeTypes@RTTI@@2URuntimeTypeArray@1@A", (void*&)gRuntimeTypeArray);
	importer.AssignAddress("?Produce@Object@@SAPEAV1@PEBVType@Unity@@HUMemLabelId@@W4ObjectCreationMode@@@Z",
		(void*&)Object__Produce);
#endif
#if defined(UNITY_2019_1) || defined(UNITY_2019_2)
	importer.AssignAddress("??0TypeTree@@QEAA@AEBUMemLabelId@@_N@Z",
		(void*&)TypeTree__TypeTree);
#else
	importer.AssignAddress("??0TypeTree@@QEAA@AEBUMemLabelId@@@Z",
		(void*&)TypeTree__TypeTree);
#endif
#ifdef UNITY_2017_1_OR_NEWER
	importer.AssignAddress("?GetSpriteAtlasDatabase@@YAAEAVSpriteAtlasDatabase@@XZ",
		(void*&)GetSpriteAtlasDatabase);
#endif
#ifdef UNITY_2019_1_OR_NEWER
	importer.AssignAddress("?GetSceneVisibilityState@@YAAEAVSceneVisibilityState@@XZ",
		(void*&)GetSceneVisibilityState);
#endif
	importer.AssignAddress("?GetInspectorExpandedState@@YAAEAVInspectorExpandedState@@XZ",
		(void*&)GetInspectorExpandedState);
	importer.AssignAddress("?GetAnnotationManager@@YAAEAVAnnotationManager@@XZ",
		(void*&)GetAnnotationManager);
	importer.AssignAddress("?GetMonoManager@@YAAEAVMonoManager@@XZ",
		(void*&)GetMonoManager);

#ifdef UNITY_2019_1_OR_NEWER
	importer.AssignAddress("?GetTypeTree@TypeTreeCache@@YA_NPEBVObject@@W4TransferInstructionFlags@@AEAVTypeTree@@@Z",
		(void*&)GetTypeTree);
	importer.AssignAddress("?EditorUtility_CUSTOM_InstanceIDToObject@@YAPEAVScriptingBackendNativeObjectPtrOpaque@@H@Z",
		(void*&)EditorUtility_CUSTOM_InstanceIDToObject);
	importer.AssignAddress("?Object_CUSTOM_DestroyImmediate@@YAXPEAVScriptingBackendNativeObjectPtrOpaque@@E@Z",
		(void*&)Object_CUSTOM_DestroyImmediate);
#else
	importer.AssignAddress("?GenerateTypeTree@@YAXAEBVObject@@AEAVTypeTree@@W4TransferInstructionFlags@@@Z",
		(void*&)GenerateTypeTree);
	importer.AssignAddress("?EditorUtility_CUSTOM_InstanceIDToObject@@YAPEAUMonoObject@@H@Z",
		(void*&)EditorUtility_CUSTOM_InstanceIDToObject);
	importer.AssignAddress("?Object_CUSTOM_DestroyImmediate@@YAXPEAUMonoObject@@E@Z",
		(void*&)Object_CUSTOM_DestroyImmediate);
#endif
}
Object* GetOrProduce(RTTIClass * type, int instanceID, ObjectCreationMode creationMode) {
	switch(type->persistentTypeID)
	{
		case PersistentTypeID::SpriteAtlasDatabase:
			if (GetSpriteAtlasDatabase == NULL) throw std::runtime_error("GetSpriteAtlasDatabase is null");
			return GetSpriteAtlasDatabase();
		case PersistentTypeID::SceneVisibilityState:
			if (GetSceneVisibilityState == NULL) throw std::runtime_error("GetSceneVisibilityState is null");
			return GetSceneVisibilityState();
		case PersistentTypeID::InspectorExpandedState:
			if (GetInspectorExpandedState == NULL) throw std::runtime_error("GetInspectorExpandedState is null");
			return GetInspectorExpandedState();
		case PersistentTypeID::AnnotationManager:
			if (GetAnnotationManager == NULL) throw std::runtime_error("GetAnnotationManager is null");
			return GetAnnotationManager();
		case PersistentTypeID::MonoManager:
			if (GetMonoManager == NULL) throw std::runtime_error("GetMonoManager is null");
			return GetMonoManager();
	}
	if (type->isAbstract) return NULL;
#ifdef UNITY_2017_3_OR_NEWER
	return Object__Produce(type, type, 0, *kMemBaseObject, ObjectCreationMode::Default);
#else
	return Object__Produce(type, 0, *kMemBaseObject, ObjectCreationMode::Default);
#endif
}
void DestroyObject(Object* obj, RTTIClass* type) {
	if (!obj->IsPersistent() &&
		type->persistentTypeID != PersistentTypeID::SpriteAtlasDatabase &&
		type->persistentTypeID != PersistentTypeID::SceneVisibilityState &&
		type->persistentTypeID != PersistentTypeID::InspectorExpandedState &&
		type->persistentTypeID != PersistentTypeID::AnnotationManager &&
		type->persistentTypeID != PersistentTypeID::MonoManager &&
		type->persistentTypeID != PersistentTypeID::AssetBundle)
	{
		Log("Getting MonoObject for %s - instanceID %d.\n", type->className, obj->instanceID);
		MonoObject* managed = EditorUtility_CUSTOM_InstanceIDToObject(obj->instanceID);
		Log("Destroying MonoObject. Exists %d\n", managed != NULL);
		Object_CUSTOM_DestroyImmediate(managed, false);
	}
}
extern "C" {
    EXPORT void DumpStructDebug() { 

#if defined(UNITY_2019_2) || defined(UNITY_2019_1)
		LOG_TYPE(TypeTree);
		LOG_MEMBER(TypeTree, Data);
		LOG_MEMBER(TypeTree, m_PrivateData);
		Log("\n");

		LOG_TYPE(TypeTreeShareableData);
		LOG_MEMBER(TypeTreeShareableData, m_Nodes);
		LOG_MEMBER(TypeTreeShareableData, m_StringData);
		LOG_MEMBER(TypeTreeShareableData, m_ByteOffsets);
		LOG_MEMBER(TypeTreeShareableData, FlagsAtGeneration);
		LOG_MEMBER(TypeTreeShareableData, RefCount);
		LOG_MEMBER(TypeTreeShareableData, MemLabel);
		Log("\n");
#elif defined(UNITY_2019_1_OR_NEWER)
		LOG_TYPE(TypeTree);
		LOG_MEMBER(TypeTree, Data);
		LOG_MEMBER(TypeTree, ReferencedTypes);
		LOG_MEMBER(TypeTree, PoolOwned);
		Log("\n");

		LOG_TYPE(TypeTreeShareableData);
		LOG_MEMBER(TypeTreeShareableData, m_Nodes);
		LOG_MEMBER(TypeTreeShareableData, m_StringData);
		LOG_MEMBER(TypeTreeShareableData, m_ByteOffsets);
		LOG_MEMBER(TypeTreeShareableData, FlagsAtGeneration);
		LOG_MEMBER(TypeTreeShareableData, RefCount);
		LOG_MEMBER(TypeTreeShareableData, MemLabel);
		Log("\n");
#else
		LOG_TYPE(TypeTree);
		LOG_MEMBER(TypeTree, m_Nodes);
		LOG_MEMBER(TypeTree, m_StringData);
		LOG_MEMBER(TypeTree, m_ByteOffsets);
		Log("\n");
#endif

        LOG_TYPE(TypeTreeNode);
        LOG_MEMBER(TypeTreeNode, m_Version);
        LOG_MEMBER(TypeTreeNode, m_Level);
        LOG_MEMBER(TypeTreeNode, m_IsArray);
        LOG_MEMBER(TypeTreeNode, m_TypeStrOffset);
        LOG_MEMBER(TypeTreeNode, m_NameStrOffset);
        LOG_MEMBER(TypeTreeNode, m_ByteSize);
        LOG_MEMBER(TypeTreeNode, m_Index);
        LOG_MEMBER(TypeTreeNode, m_MetaFlag);
#ifdef UNITY_2019_1_OR_NEWER
		LOG_MEMBER(TypeTreeNode, m_RefTypeHash);
#endif
        Log("\n");

        LOG_TYPE(MemLabelId);
        LOG_MEMBER(MemLabelId, m_RootReferenceWithSalt);
        LOG_MEMBER(MemLabelId, identifier);
        Log("\n");

        LOG_TYPE(AllocationRootWithSalt);
        LOG_MEMBER(AllocationRootWithSalt, m_Salt);
        LOG_MEMBER(AllocationRootWithSalt, m_RootReferenceIndex);
        Log("\n");

        LOG_TYPE(dynamic_array<int>);
        LOG_MEMBER(dynamic_array<int>, data);
        LOG_MEMBER(dynamic_array<int>, label);
        LOG_MEMBER(dynamic_array<int>, size);
        LOG_MEMBER(dynamic_array<int>, cap);
        Log("\n");

        LOG_TYPE(RuntimeTypeArray);
        LOG_MEMBER(RuntimeTypeArray, count);
        LOG_MEMBER(RuntimeTypeArray, Types);
        Log("\n");

        LOG_TYPE(RTTIClass);
        LOG_MEMBER(RTTIClass, base);
        LOG_MEMBER(RTTIClass, factory);
        LOG_MEMBER(RTTIClass, className);
        LOG_MEMBER(RTTIClass, classNamespace);
#ifdef UNITY_2017_3_OR_NEWER
        LOG_MEMBER(RTTIClass, module);
#endif
        LOG_MEMBER(RTTIClass, persistentTypeID);
        LOG_MEMBER(RTTIClass, size);
        LOG_MEMBER(RTTIClass, derivedFromInfo);
        LOG_MEMBER(RTTIClass, isAbstract);
        LOG_MEMBER(RTTIClass, isSealed);
        LOG_MEMBER(RTTIClass, isEditorOnly);
#ifdef UNITY_2017_3_OR_NEWER
        LOG_MEMBER(RTTIClass, attributes);
		LOG_MEMBER(RTTIClass, attributeCount);
#endif
        Log("\n");


        LOG_TYPE(Object);
        LOG_MEMBER(Object, virtualFunctionTable);
        LOG_MEMBER(Object, instanceID);
        LOG_MEMBER(Object, bits);
        Log("\n");

        CloseLog();
	}
	EXPORT void ExportStringData(const char* moduleName) {
		try {
			InitBindings(moduleName);
			Log("Writing Strings\n");
			unsigned long length = *CommonString_BufferEnd - *CommonString_BufferBegin - 1;
			if (length <= 0) {
				Log("Error: string.dat length is %lld", length);
			}
			else {
				CreateDirectory(L"Output", NULL);
				FILE* file = fopen("Output/strings.dat", "wb");
				fwrite(*CommonString_BufferBegin, sizeof(char), length, file);
				fclose(file);
			}
		}
		catch (std::exception err) {
			Log("Error: %s\n", err.what());
		}
		CloseLog();
	}
	EXPORT void ExportClassesJson(const char* moduleName) {
		Log("ExportClassesJson\n");
		try {
			InitBindings(moduleName);
			Log("%d types", gRuntimeTypeArray->count);
			CreateDirectory(L"Output", NULL);
			FILE* json = fopen("Output/classes.json", "w");
			fprintf(json, "{\n");
			for (int i = 0; i < gRuntimeTypeArray->count; i++) {
				auto type = gRuntimeTypeArray->Types[i];
				if (type == NULL) {
					Log("Found NULL pointer for RuntimeType %d\n", i);
				}
				fprintf(json, "    \"%d\": \"%s\"", type->persistentTypeID, type->className);
				if (i < gRuntimeTypeArray->count - 1) {
					fprintf(json, ",");
				}
				fprintf(json, "\n");
			}
			fprintf(json, "}");
			fclose(json);
		}
		catch (std::exception err) {
			Log("Error: %s\n", err.what());
		}
		CloseLog();
	}
	EXPORT void ExportStructData(const char* moduleName, const char* unityVersion, uint32_t runtimePlatform) {
		try {
			InitBindings(moduleName);
			Log("Dumping %d types. ModuleName %s, UnityVersion %s\n", 
				gRuntimeTypeArray->count, 
				moduleName, 
				unityVersion);
			CreateDirectory(L"Output", NULL);
			FILE* file = fopen("Output/structs.dat", "wb");

			fprintf(file, unityVersion);
			fwrite(&runtimePlatform, 4, 1, file);
			bool hasTypeTrees = true;
			fwrite(&hasTypeTrees, 1, 1, file);
			int typeCount = 0;
			fpos_t countPos = 0;
			fgetpos(file, &countPos);
			fwrite(&typeCount, 4, 1, file);
			Log("Writing RunTimeTypes\n");
			for (int i = 0; i < gRuntimeTypeArray->count; i++) {
				RTTIClass* type = gRuntimeTypeArray->Types[i];
				RTTIClass* iter = type;
				Log("Type %d Child Class %s, %d, %d\n",
					i,
					type->className,
					type->persistentTypeID,
					type->size);
				Log("Type %d getting base type", i);
				while (iter != NULL && iter->isAbstract) {
					Log("%s is abstract\n", iter->className);
					iter = iter->base;
				}
				if (iter == NULL) {
					Log("Could not find concrete type for %d %s\n", i, type->className);
					continue;
				}
				Log("Type %d BaseType is %s %d %d\n",
					i,
					iter->className,
					iter->persistentTypeID,
					iter->size);
				Log("Type %d Getting native object\n", i);
				Object* obj = GetOrProduce(iter, 0, ObjectCreationMode::Default);
				if (obj == NULL) {
					Log("Type %d %s: Produced null object\n", i, iter->className);
					continue;
				}
				Log("Type %d Produced object %d\n", i, obj->instanceID);
				Log("Type %d Getting Type Tree\n", i);

				TypeTree* typeTree = (TypeTree*)malloc(sizeof(TypeTree));
#if defined(UNITY_2019_1) || defined(UNITY_2019_2)
				TypeTree__TypeTree(typeTree, kMemTypeTree, false);
#else
				TypeTree__TypeTree(typeTree, kMemTypeTree);
#endif
				Log("Type %d %s: Calling GetTypeTree.\n", i, type->className);
#ifdef UNITY_2019_1_OR_NEWER
				if (!GetTypeTree(obj, TransferInstructionFlags::SerializeGameRelease, typeTree)) {
					Log("Error calling GetTypeTree\n");
				}
#else
				GenerateTypeTree(obj, typeTree, TransferInstructionFlags::SerializeGameRelease);
#endif
				Log("Type %d Getting PersistentTypeID", i);
				fwrite(&iter->persistentTypeID, sizeof(iter->persistentTypeID), 1, file);
				Log("Type %d Getting GUID\n", i);
				for (int j = 0, n = (int)iter->persistentTypeID < 0 ? 0x20 : 0x10; j < n; ++j) {
					char fakeGuid = 0;
					fwrite(&fakeGuid, sizeof(fakeGuid), 1, file);
				}
				Log("Type %d Creating Binary Dump\n");
				typeTree->Write(file);
				typeCount++;
				DestroyObject(obj, type);
			}
			int zero = 0;
			fwrite(&zero, 4, 1, file);
			fsetpos(file, &countPos);
			fwrite(&typeCount, 4, 1, file);
			fclose(file);
		}
		catch (std::exception err) {
			Log("Error: %s\n", err.what());
		}
		CloseLog();
	}
	EXPORT void ExportStructDump(const char* moduleName) {
		try {
			InitBindings(moduleName);
			Log("kMemTypeTree\n");
			Log("  %d %x\n", kMemTypeTree->m_RootReferenceWithSalt.m_Salt, kMemTypeTree->m_RootReferenceWithSalt.m_Salt);
			Log("  %d %x\n", kMemTypeTree->m_RootReferenceWithSalt.m_RootReferenceIndex, kMemTypeTree->m_RootReferenceWithSalt.m_RootReferenceIndex);
			Log("  %d %x\n", kMemTypeTree->identifier, kMemTypeTree->identifier);
			Log("Dumping %d types\n", gRuntimeTypeArray->count);
			CreateDirectory(L"Output", NULL);
			FILE* file = fopen("Output/structs.dump", "w");
			for (int i = 0; i < gRuntimeTypeArray->count; i++) {
				RTTIClass* type = gRuntimeTypeArray->Types[i];
				RTTIClass* iter = type;
				std::string inheritance{};
				while (true) {
					inheritance += iter->className;
					if (iter->base == NULL) break;
					inheritance += " <- ";
					iter = iter->base;
				}
				fprintf(file, "\n// classID{%d}: %s\n", type->persistentTypeID, inheritance.c_str());
				iter = type;
				while (iter != NULL && iter->isAbstract) {
					fprintf(file, "// %s is abstract\n", iter->className);
					iter = iter->base;
				}
				if (iter == NULL) {
					Log("Could not find concrete type for %d %s\n", i, type->className);
					continue;
				}

				Object* obj = GetOrProduce(iter, 0, ObjectCreationMode::Default);
				if (obj == NULL) {
					Log("Type %d %s: Produced null object\n", i, iter->className);
					continue;
				}
				else {
					Log("Type %d %s: Generating type. PersistentId %d, Persistent %d\n", i, type->className, type->persistentTypeID, obj->IsPersistent());
				}
				TypeTree* typeTree = (TypeTree*)malloc(sizeof(TypeTree));
#if defined(UNITY_2019_1) || defined(UNITY_2019_2)
				TypeTree__TypeTree(typeTree, kMemTypeTree, false);
#else
				TypeTree__TypeTree(typeTree, kMemTypeTree);
#endif
				Log("Type %d %s: Calling GetTypeTree.\n", i, type->className);
#ifdef UNITY_2019_1_OR_NEWER
				if (!GetTypeTree(obj, TransferInstructionFlags::SerializeGameRelease, typeTree)) {
					Log("Error calling GetTypeTree\n");
				}
#else
				GenerateTypeTree(obj, typeTree, TransferInstructionFlags::SerializeGameRelease);
#endif
				fputs(typeTree->Dump(*CommonString_BufferBegin).c_str(), file);
				DestroyObject(obj, type);
			}
			fclose(file);
		}
		catch (std::exception err) {
			Log("Error: %s\n", err.what());
		}
		CloseLog();
	}
}