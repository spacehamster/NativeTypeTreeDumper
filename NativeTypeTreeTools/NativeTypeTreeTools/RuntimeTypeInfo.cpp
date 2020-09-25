#include "RuntimeTypeInfo.h"
#if VERSION_GE(5, 0)
RuntimeTypeArray* gRuntimeTypeArray;
#endif
#if VERSION_LT(5, 5)
typedef RuntimeTypeInfo* (__cdecl* Object__ClassIDToRTTI_t)(int classID);
Object__ClassIDToRTTI_t ClassIDToRTTI;
#endif

typedef char* (__thiscall* TypeTreeString_c_str_t)(void *);
TypeTreeString_c_str_t TypeTreeString_c_str;

void RuntimeTypeInfo::InitBindings(SymbolImporter & importer) {
#if VERSION_GE(2017, 3)
	importer.ResolveSymbol("?ms_runtimeTypes@RTTI@@0URuntimeTypeArray@1@A", (void*&)gRuntimeTypeArray);
#elif VERSION_GE(5, 5)
	importer.ResolveSymbol("?ms_runtimeTypes@RTTI@@2URuntimeTypeArray@1@A", (void*&)gRuntimeTypeArray);
#elif VERSION_GE(5, 4)
	//public: static struct RTTI * __cdecl Object::ClassIDToRTTI(int)
	importer.ResolveSymbol("?ClassIDToRTTI@Object@@SAPEAURTTI@@H@Z", (void*&)ClassIDToRTTI);
#elif VERSION_GE(5, 1)
	//public: static struct Object::RTTI * __cdecl Object::ClassIDToRTTI(int)
	importer.ResolveSymbol("?ClassIDToRTTI@Object@@SAPEAURTTI@1@H@Z", (void*&)ClassIDToRTTI);
#else
	//public: static struct Object::RTTI * __cdecl Object::ClassIDToRTTI(int)
	importer.ResolveSymbol("?ClassIDToRTTI@Object@@SAPAURTTI@1@H@Z", (void*&)ClassIDToRTTI);
	importer.ResolveSymbolStartsWith("?c_str@?$basic_string@", (void*&)TypeTreeString_c_str);
#endif
}
std::vector<RuntimeTypeInfo*> RuntimeTypeInfo::GetRuntimeTypes() {
	std::vector<RuntimeTypeInfo*> result;
#if VERSION_GE(5, 0)
	for (int i = 0; i < gRuntimeTypeArray->count; i++) {
		result.push_back(gRuntimeTypeArray->Types[i]);
	}
#else 
	for (int i = 0; i < 2000; i++) {
		RuntimeTypeInfo* rtti = ClassIDToRTTI(i);
		if (rtti == NULL) continue;
		if ((int)rtti->persistentTypeID != i)
		{
			throw new std::runtime_error("PersistentTypeID does not match ClassID");
		}
		result.push_back(rtti);
	}
#endif
	return result;
}

const char* RuntimeTypeInfo::GetName() {
#if VERSION_GE(5, 0)
	return className;
#else
	return TypeTreeString_c_str(&this->className);
#endif
}
void RuntimeTypeInfo::DumpStructLayout() {
	LOG_TYPE(RuntimeTypeArray);
	LOG_MEMBER(RuntimeTypeArray, count);
	LOG_MEMBER(RuntimeTypeArray, Types);
	Log("\n");

	LOG_TYPE(RuntimeTypeInfo);
	LOG_MEMBER(RuntimeTypeInfo, base);
	LOG_MEMBER(RuntimeTypeInfo, factory);
	LOG_MEMBER(RuntimeTypeInfo, className);
#if VERSION_GE(5, 0)
	LOG_MEMBER(RuntimeTypeInfo, classNamespace);
#endif
#ifdef UNITY_2017_3_OR_NEWER
	LOG_MEMBER(RuntimeTypeInfo, module);
#endif
	LOG_MEMBER(RuntimeTypeInfo, persistentTypeID);
	LOG_MEMBER(RuntimeTypeInfo, size);
#if VERSION_GE(5, 0)
	LOG_MEMBER(RuntimeTypeInfo, derivedFromInfo);
#endif
	LOG_MEMBER(RuntimeTypeInfo, isAbstract);
#if VERSION_GE(5, 0)
	LOG_MEMBER(RuntimeTypeInfo, isSealed);
	LOG_MEMBER(RuntimeTypeInfo, isEditorOnly);
#endif
#ifdef UNITY_2017_3_OR_NEWER
	LOG_MEMBER(RuntimeTypeInfo, attributes);
	LOG_MEMBER(RuntimeTypeInfo, attributeCount);
#endif
	Log("\n");

#if VERSION_LT(5, 0)
	LOG_TYPE(TypeTreeString);
	Log("\n");
#endif
}


char * TypeTreeString::c_str() {
	return TypeTreeString_c_str(this);
}