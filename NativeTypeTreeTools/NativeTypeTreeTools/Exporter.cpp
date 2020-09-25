#pragma once
#include "Exporter.h"
#include "RuntimeTypeInfo.h"
#include "NativeObject.h"
#include "CommonStrings.h"
#include "TypeTree.h"
Exporter::Exporter(const char* moduleName) {
	this->moduleName = moduleName;
}

void Exporter::InitBindings()
{
	SymbolImporter importer;
	importer.LoadFromExe(moduleName);
	RuntimeTypeInfo::InitBindings(importer);
	CommonString::InitBindings(importer);
	TypeTree::InitBindings(importer);
	Object::InitBindings(importer);
}

void Exporter::ExportStringData()
{
	InitBindings();
	Log("Writing Strings\n");
	unsigned long length = CommonString::GetBufferEnd() - CommonString::GetBufferBegin() - 1;
	if (length <= 0) {
		Log("Error: string.dat length is %lld", length);
	}
	else {
		CreateDirectory(TEXT("Output"), NULL);
		FILE* file = fopen("Output/strings.dat", "wb");
		fwrite(CommonString::GetBufferBegin(), sizeof(char), length, file);
		fclose(file);
	}
}
void Exporter::ExportRTTIDump()
{
	this->InitBindings();
	Log("Export RTTI Dump\n");
	auto types = RuntimeTypeInfo::GetRuntimeTypes();
	FILE* file = fopen("Output/RTTI.txt", "w");
	for (int i = 0; i < types.size(); i++) {
		auto type = types[i];
		fprintf(file, "Type %d @ %p\n", i, type);
		fprintf(file, "    Base: %p\n", type->base);
		fprintf(file, "    Factory: %p\n", type->factory);
		fprintf(file, "    PersistentTypeID: %d\n", type->persistentTypeID);
		fprintf(file, "    ClassName.Ptr: %p\n", type->GetName());
		fprintf(file, "    ClassName: %s\n", type->GetName());
#if VERSION_GE(5, 0)
		fprintf(file, "    ClassNamespace: %s\n", type->classNamespace);
#endif
		fprintf(file, "    Size: %d\n", type->size);
#if VERSION_GE(5, 0)
		fprintf(file, "    DescendantCount: %d\n", type->derivedFromInfo.descendantCount);
		fprintf(file, "    TypeIndex: %d\n", type->derivedFromInfo.typeIndex);
#endif
		fprintf(file, "    IsAbstract: %d\n", type->isAbstract);
#if VERSION_GE(5, 0)
		fprintf(file, "    isSealed: %d\n", type->isSealed);
		fprintf(file, "    isEditorOnly: %d\n", type->isEditorOnly);
#endif
	}
	fclose(file);
}

void Exporter::ExportClassesJson()
{
	this->InitBindings();
	Log("ExportClassesJson\n");
	auto types = RuntimeTypeInfo::GetRuntimeTypes();
	Log("%d types\n", types.size());;
	CreateDirectory(TEXT("Output"), NULL);
	FILE* json = fopen("Output/classes.json", "w");
	fprintf(json, "{\n");
	for (int i = 0; i < types.size(); i++) {
		auto type = types[i];
		if (type == NULL) {
			Log("Found NULL pointer for RuntimeType %d\n", i);
			throw std::runtime_error("RuntimeType is null");
		}
		const char* name = type->GetName();
		if (name == NULL) name = "NULL";
		fprintf(json, "    \"%d\": \"%s\"", type->persistentTypeID, name);
		if (i < types.size() - 1) {
			fprintf(json, ",");
		}
		fprintf(json, "\n");
	}
	fprintf(json, "}");
	fclose(json);
}

void Exporter::ExportStructData(const char* unityVersion, uint32_t runtimePlatform)
{
	InitBindings();
	auto types = RuntimeTypeInfo::GetRuntimeTypes();
	Log("Dumping %d types. ModuleName %s, UnityVersion %s\n",
		types.size(),
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
	for (int i = 0; i < types.size(); i++) {
		RuntimeTypeInfo* type = types[i];
		RuntimeTypeInfo* iter = type;
		Log("Type %d Child Class %s, %d, %d\n",
			i,
			type->GetName(),
			type->persistentTypeID,
			type->size);
		Log("Type %d getting base type", i);
		while (iter != NULL && iter->isAbstract) {
			Log("%s is abstract\n", iter->GetName());
			iter = iter->base;
		}
		if (iter == NULL) {
			Log("Could not find concrete type for %d %s\n", i, type->GetName());
			continue;
		}
		Log("Type %d BaseType is %s %d %d\n",
			i,
			iter->GetName(),
			iter->persistentTypeID,
			iter->size);
		Log("Type %d Getting native object\n", i);
		Object* obj = Object::GetOrProduce(iter, 0, ObjectCreationMode::Default);
		if (obj == NULL) {
			Log("Type %d %s: Produced null object\n", i, iter->GetName());
			continue;
		}
		Log("Type %d Produced object %d\n", i, obj->instanceID);
		Log("Type %d Getting Type Tree\n", i);

		TypeTree* typeTree = TypeTree::CreateTypeTree();
		Log("Type %d %s: Calling GetTypeTree.\n", i, type->GetName());
		Object::ProduceTypeTree(obj, typeTree);
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
		Object::DestroyObject(obj, type);
	}
	int zero = 0;
	fwrite(&zero, 4, 1, file);
	fsetpos(file, &countPos);
	fwrite(&typeCount, 4, 1, file);
	fclose(file);
}

void Exporter::ExportStructDump()
{
	InitBindings();
	auto types = RuntimeTypeInfo::GetRuntimeTypes();
	Log("Dumping %d types\n", types.size());
	CreateDirectory(L"Output", NULL);
	FILE* file = fopen("Output/structs.dump", "w");
	for (int i = 0; i < types.size(); i++) {
		RuntimeTypeInfo* type = types[i];
		RuntimeTypeInfo* iter = type;
		std::string inheritance{};
		while (true) {
			inheritance += iter->GetName();
			if (iter->base == NULL) break;
			inheritance += " <- ";
			iter = iter->base;
		}
		fprintf(file, "\n// classID{%d}: %s\n", type->persistentTypeID, inheritance.c_str());
		iter = type;
		while (iter != NULL && iter->isAbstract) {
			fprintf(file, "// %s is abstract\n", iter->GetName());
			iter = iter->base;
		}
		if (iter == NULL) {
			Log("Could not find concrete type for %d %s\n", i, type->GetName());
			continue;
		}
		Log("Type %d %s: Calling GetOrProduce\n", i, iter->GetName());
		Object* obj = Object::GetOrProduce(iter, 0, ObjectCreationMode::Default);
		if (obj == NULL) {
			Log("Type %d %s: Produced null object\n", i, iter->GetName());
			continue;
		}
		else {
			Log("Type %d %s: Generating type. PersistentId %d, Persistent %d\n", i, type->GetName(), type->persistentTypeID, obj->IsPersistent());
		}
		TypeTree* typeTree = TypeTree::CreateTypeTree();
		Log("Type %d %s: Calling GetTypeTree.\n", i, type->GetName());
		Object::ProduceTypeTree(obj, typeTree);
		fputs(typeTree->Dump(CommonString::GetPtr()).c_str(), file);
		Log("Type %d %s: Destroying Object.\n", i, type->GetName());
		Object::DestroyObject(obj, type);
	}
	fclose(file);
}