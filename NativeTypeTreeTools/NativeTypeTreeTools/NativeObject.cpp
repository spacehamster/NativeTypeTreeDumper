#include "NativeObject.h"
#include "RuntimeTypeInfo.h"
#include "MemLabel.h"
#include "TypeTree.h"
#ifdef VERSION_GE(5, 0)
MemLabelId* kMemBaseObject;
#endif
#ifdef UNITY_2017_3_OR_NEWER
typedef Object* (__cdecl* Object__Produce_t)(struct RuntimeTypeInfo* classInfo, struct RuntimeTypeInfo* classInfo2, int instanceID, MemLabelId memLabel, ObjectCreationMode mode);
#elif VERSION_GE(5, 5)
typedef Object* (__cdecl* Object__Produce_t)(struct RuntimeTypeInfo* classInfo, int instanceID, MemLabelId memLabel, ObjectCreationMode mode);
#else
typedef Object* (__cdecl* Object__Produce_t)(int classID, int instanceID, MemLabelId memLabel, ObjectCreationMode mode);
#endif
Object__Produce_t Object__Produce;

typedef bool(__cdecl* GetTypeTree_t)(Object* obj, TransferInstructionFlags flags, TypeTree* tree);
GetTypeTree_t GetTypeTree;

typedef void(__cdecl* GenerateTypeTree_t)(Object* object, TypeTree* typeTree, TransferInstructionFlags options);
GenerateTypeTree_t GenerateTypeTree;

typedef Object* (__cdecl* GetSpriteAtlasDatabase_t)(void);
GetSpriteAtlasDatabase_t GetSpriteAtlasDatabase;

typedef Object* (__cdecl* GetSceneVisibilityState_t)(void);
GetSceneVisibilityState_t GetSceneVisibilityState;

typedef Object* (__cdecl* GetInspectorExpandedState_t)(void);
GetInspectorExpandedState_t GetInspectorExpandedState;

typedef Object* (__cdecl* GetAnnotationManager_t)(void);
GetAnnotationManager_t GetAnnotationManager;

typedef Object* (__cdecl* GetMonoManager_t)(void);
GetMonoManager_t GetMonoManager;

typedef MonoObject* (__cdecl* EditorUtility_CUSTOM_InstanceIDToObject_t)(int instanceID);
EditorUtility_CUSTOM_InstanceIDToObject_t EditorUtility_CUSTOM_InstanceIDToObject;

typedef void(__cdecl* Object_CUSTOM_DestroyImmediate_t)(void* pcriptingBackendNativeObjectPtrOpaque, bool allowDestroyingAssets);
Object_CUSTOM_DestroyImmediate_t Object_CUSTOM_DestroyImmediate;

typedef void(__cdecl* DestroyObjectFromScriptingImmediate_t)(Object* object, bool allowDestroyingAssets);
DestroyObjectFromScriptingImmediate_t DestroyObjectFromScriptingImmediate;



void Object::InitBindings(SymbolImporter& importer) {
#if VERSION_GE(5, 2)
	importer.ResolveSymbol("?kMemBaseObject@@3UMemLabelId@@A",
		(void*&)kMemBaseObject);
#else
	importer.ResolveSymbol("?kMemBaseObject@@3UkMemBaseObjectStruct@@A",
		(void*&)kMemBaseObject);
#endif

#ifdef UNITY_2017_3_OR_NEWER
	importer.ResolveSymbol("?Produce@Object@@CAPEAV1@PEBVType@Unity@@0HUMemLabelId@@W4ObjectCreationMode@@@Z",
		(void*&)Object__Produce);
#elif VERSION_GE(5, 5)
	importer.ResolveSymbol("?Produce@Object@@SAPEAV1@PEBVType@Unity@@HUMemLabelId@@W4ObjectCreationMode@@@Z",
		(void*&)Object__Produce);
#elif VERSION_GE(5, 0)
	//static class Object * __cdecl Object::Produce(int,int,struct MemLabelId,enum ObjectCreationMode)
	importer.ResolveSymbol("?Produce@Object@@SAPEAV1@HHUMemLabelId@@W4ObjectCreationMode@@@Z",
		(void*&)Object__Produce);
#elif VERSION_GE(3, 5)
	//static class Object * __cdecl Object::Produce(int,int,struct MemLabelId,enum ObjectCreationMode)
	importer.ResolveSymbol("?Produce@Object@@SAPAV1@HHUMemLabelId@@W4ObjectCreationMode@@@Z",
		(void*&)Object__Produce);
#else
	importer.ResolveSymbol("?Produce@Object@@SAPAV1@HHPAVBaseAllocator@@W4ObjectCreationMode@@@Z",
		(void*&)Object__Produce);
#endif
#ifdef UNITY_2017_1_OR_NEWER
	importer.ResolveSymbol("?GetSpriteAtlasDatabase@@YAAEAVSpriteAtlasDatabase@@XZ",
		(void*&)GetSpriteAtlasDatabase);
#endif
#ifdef UNITY_2019_1_OR_NEWER
	importer.ResolveSymbol("?GetSceneVisibilityState@@YAAEAVSceneVisibilityState@@XZ",
		(void*&)GetSceneVisibilityState);
#endif
#if VERSION_GE(5, 0)
	importer.ResolveSymbol("?GetInspectorExpandedState@@YAAEAVInspectorExpandedState@@XZ",
		(void*&)GetInspectorExpandedState);
	importer.ResolveSymbol("?GetAnnotationManager@@YAAEAVAnnotationManager@@XZ",
		(void*&)GetAnnotationManager);
	//class MonoManager & __cdecl GetMonoManager(void)
	importer.ResolveSymbol("?GetMonoManager@@YAAEAVMonoManager@@XZ",
		(void*&)GetMonoManager);
#else
	//class InspectorExpandedState & __cdecl GetInspectorExpandedState(void)
	importer.ResolveSymbol("?GetInspectorExpandedState@@YAAAVInspectorExpandedState@@XZ",
		(void*&)GetInspectorExpandedState);
	//class AnnotationManager & __cdecl GetAnnotationManager(void)
	importer.ResolveSymbol("?GetAnnotationManager@@YAAAVAnnotationManager@@XZ",
		(void*&)GetAnnotationManager);
	//class MonoManager & __cdecl GetMonoManager(void)
	importer.ResolveSymbol("?GetMonoManager@@YAAAVMonoManager@@XZ",
		(void*&)GetMonoManager);
#endif

#ifdef UNITY_2019_1_OR_NEWER
	importer.ResolveSymbol("?GetTypeTree@TypeTreeCache@@YA_NPEBVObject@@W4TransferInstructionFlags@@AEAVTypeTree@@@Z",
		(void*&)GetTypeTree);
#elif VERSION_GE(5, 4)
	//void __cdecl GenerateTypeTree(class Object const &,class TypeTree &,enum TransferInstructionFlags)
	importer.ResolveSymbol("?GenerateTypeTree@@YAXAEBVObject@@AEAVTypeTree@@W4TransferInstructionFlags@@@Z",
		(void*&)GenerateTypeTree);
#elif VERSION_GE(5, 0)
	//void __cdecl GenerateTypeTree(class Object &,class TypeTree *,enum TransferInstructionFlags)
	importer.ResolveSymbol("?GenerateTypeTree@@YAXAEAVObject@@PEAVTypeTree@@W4TransferInstructionFlags@@@Z",
		(void*&)GenerateTypeTree);
#else
	//void __cdecl GenerateTypeTree(class Object &,class TypeTree *,int)
	importer.ResolveSymbol("?GenerateTypeTree@@YAXAAVObject@@PAVTypeTree@@H@Z",
		(void*&)GenerateTypeTree);
#endif

#ifdef UNITY_2019_1_OR_NEWER
	importer.ResolveSymbol("?EditorUtility_CUSTOM_InstanceIDToObject@@YAPEAVScriptingBackendNativeObjectPtrOpaque@@H@Z",
		(void*&)EditorUtility_CUSTOM_InstanceIDToObject);
#elif VERSION_GE(5, 5)
	importer.ResolveSymbol("?EditorUtility_CUSTOM_InstanceIDToObject@@YAPEAUMonoObject@@H@Z",
		(void*&)EditorUtility_CUSTOM_InstanceIDToObject);
#else

#endif

#ifdef UNITY_2019_1_OR_NEWER
	importer.ResolveSymbol("?Object_CUSTOM_DestroyImmediate@@YAXPEAVScriptingBackendNativeObjectPtrOpaque@@E@Z",
		(void*&)Object_CUSTOM_DestroyImmediate);
#elif VERSION_GE(5, 5)
	importer.ResolveSymbol("?Object_CUSTOM_DestroyImmediate@@YAXPEAUMonoObject@@E@Z",
		(void*&)Object_CUSTOM_DestroyImmediate);
#elif VERSION_GE(5, 0)
	//void __cdecl Scripting::DestroyObjectFromScriptingImmediate(class Object *,bool)
	importer.ResolveSymbol("?DestroyObjectFromScriptingImmediate@Scripting@@YAXPEAVObject@@_N@Z",
		(void*&)DestroyObjectFromScriptingImmediate);
#elif VERSION_GE(4, 3)
	//void __cdecl Scripting::DestroyObjectFromScriptingImmediate(class Object *,bool)
	importer.ResolveSymbol("?DestroyObjectFromScriptingImmediate@Scripting@@YAXPAVObject@@_N@Z",
		(void*&)DestroyObjectFromScriptingImmediate);
#else
	//void __cdecl DestroyObjectFromScriptingImmediate(class Object *,bool)
	importer.ResolveSymbol("?DestroyObjectFromScriptingImmediate@@YAXPAVObject@@_N@Z",
		(void*&)DestroyObjectFromScriptingImmediate);
#endif
}

Object* Object::GetOrProduce(RuntimeTypeInfo* type, int instanceID, ObjectCreationMode creationMode) {
	switch (type->persistentTypeID)
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
#elif VERSION_GE(5, 5)
	return Object__Produce(type, 0, *kMemBaseObject, ObjectCreationMode::Default);
#else
	return Object__Produce((int)type->persistentTypeID, 0, *kMemBaseObject, ObjectCreationMode::DefaultNoLock);
#endif
}
void Object::DestroyObject(Object* obj, RuntimeTypeInfo* type) {
	if (!obj->IsPersistent() &&
		type->persistentTypeID != PersistentTypeID::SpriteAtlasDatabase &&
		type->persistentTypeID != PersistentTypeID::SceneVisibilityState &&
		type->persistentTypeID != PersistentTypeID::InspectorExpandedState &&
		type->persistentTypeID != PersistentTypeID::AnnotationManager &&
		type->persistentTypeID != PersistentTypeID::MonoManager &&
		type->persistentTypeID != PersistentTypeID::AssetBundle)
	{
#if VERSION_GE(5, 5)
		Log("Getting MonoObject for %s - instanceID %d.\n", type->GetName(), obj->instanceID);
		MonoObject* managed = EditorUtility_CUSTOM_InstanceIDToObject(obj->instanceID);
		Log("Destroying MonoObject. Exists %d\n", managed != NULL);
		Object_CUSTOM_DestroyImmediate(managed, false);
#else
		Log("Destroying Object %s. instanceID %d IsPersistent %d\n", type->GetName(), obj->instanceID, obj->IsPersistent());
		DestroyObjectFromScriptingImmediate(obj, false);
#endif
	}
}

void Object::ProduceTypeTree(Object* obj, TypeTree* typeTree) {
#ifdef UNITY_2019_1_OR_NEWER
	if (!GetTypeTree(obj, TransferInstructionFlags::SerializeGameRelease, typeTree)) {
		Log("Error calling GetTypeTree\n");
	}
#else
	GenerateTypeTree(obj, typeTree, TransferInstructionFlags::SerializeGameRelease);
#endif
}

void Object::DumpStructLayout() {
	LOG_TYPE(Object);
	LOG_MEMBER(Object, virtualFunctionTable);
	LOG_MEMBER(Object, instanceID);
	LOG_MEMBER(Object, bits);
	Log("\n");
}