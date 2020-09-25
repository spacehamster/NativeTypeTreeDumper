#include "Structs.h"
#include "TypeTree.h"
#include "NativeObject.h"

MemLabelId* kMemTypeTree;

#if defined(UNITY_2019_1) || defined(UNITY_2019_2)
typedef void(__thiscall* TypeTree__TypeTree_t)(TypeTree* self, MemLabelId* memLabel, bool unk);
TypeTree__TypeTree_t TypeTree__TypeTree;
#elif VERSION_GE(5, 3)
typedef void(__thiscall* TypeTree__TypeTree_t)(TypeTree* self, MemLabelId* memLabel);
TypeTree__TypeTree_t TypeTree__TypeTree;
#else
typedef void(__thiscall* TypeTree__TypeTree_t)(TypeTree* self);
TypeTree__TypeTree_t TypeTree__TypeTree;
#endif
#if VERSION_LT(5, 0)
typedef void(__thiscall* TypeTreeIterator__TypeTreeIterator_t)(TypeTreeIterator* self, TypeTree* param_1);
TypeTreeIterator__TypeTreeIterator_t TypeTreeIterator__TypeTreeIterator;

typedef TypeTreeIterator (__thiscall* Next_t)(TypeTreeIterator& self);
Next_t Next;

typedef TypeTreeIterator (__thiscall* Children_t)(TypeTreeIterator& self);
Children_t Children;

typedef TypeTreeIterator (__thiscall* Father_t)(TypeTreeIterator& self);
Father_t Father;

typedef bool(__thiscall* IsNull_t)(TypeTreeIterator& self);
IsNull_t IsNull;

#endif
void TypeTree::InitBindings(SymbolImporter& importer) {
#if VERSION_GE(5, 3)
	//struct MemLabelId kMemTypeTree
	importer.ResolveSymbol("?kMemTypeTree@@3UMemLabelId@@A",
		(void*&)kMemTypeTree);
#endif
#if defined(UNITY_2019_1) || defined(UNITY_2019_2)
	//public: __cdecl TypeTree::TypeTree(struct MemLabelId const &,bool)
	importer.ResolveSymbol("??0TypeTree@@QEAA@AEBUMemLabelId@@_N@Z",
		(void*&)TypeTree__TypeTree);
#elif VERSION_GE(5, 3)
	//public: __cdecl TypeTree::TypeTree(struct MemLabelId const &)
	importer.ResolveSymbol("??0TypeTree@@QEAA@AEBUMemLabelId@@@Z",
		(void*&)TypeTree__TypeTree);
#elif VERSION_GE(5, 0)
	//public: __cdecl TypeTree::TypeTree(void)
	importer.ResolveSymbol("??0TypeTree@@QEAA@XZ",
		(void*&)TypeTree__TypeTree);
#else
	//public: __thiscall TypeTree::TypeTree(void)
	importer.ResolveSymbol("??0TypeTree@@QAE@XZ",
	(void*&)TypeTree__TypeTree);
#endif
#if VERSION_LT(5, 0)
	//public: __thiscall TypeTreeIterator::TypeTreeIterator(class TypeTree const *)
	importer.ResolveSymbol("??0TypeTreeIterator@@QAE@PBVTypeTree@@@Z",
		(void*&)TypeTreeIterator__TypeTreeIterator);
	importer.ResolveSymbol("?Next@TypeTreeIterator@@QBE?AV1@XZ",
		(void*&)Next);
	importer.ResolveSymbol("?Children@TypeTreeIterator@@QBE?AV1@XZ",
		(void*&)Children);
	importer.ResolveSymbol("?Father@TypeTreeIterator@@QBE?AV1@XZ",
		(void*&)Father);
	//public: bool __thiscall TypeTreeIterator::IsNull(void)const
	importer.ResolveSymbol("?IsNull@TypeTreeIterator@@QBE_NXZ",
		(void*&)IsNull);
#endif
}
dynamic_array<TypeTreeNode>& TypeTree::GetNodes() {
#ifdef UNITY_2019_1_OR_NEWER
	return Data->m_Nodes;
#elif VERSION_GE(5, 0)
	return m_Nodes;
#else
	throw std::runtime_error("GetNodes not implemented for this version");
#endif
}
dynamic_array<char>& TypeTree::GetStringData() {
#ifdef UNITY_2019_1_OR_NEWER
	return Data->m_StringData;
#elif VERSION_GE(5, 0)
	return m_StringData;
#else
	throw std::runtime_error("GetStringData not implemented for this version");
#endif
}
dynamic_array<void*>& TypeTree::GetByteOffsets() {
#ifdef UNITY_2019_1_OR_NEWER
	return Data->m_ByteOffsets;
#elif VERSION_GE(5, 0)
	return m_ByteOffsets;
#else
	throw std::runtime_error("GetByteOffsets not implemented for this version");
#endif
}
#if VERSION_GE(5, 0)
std::string TypeTree::Dump(char* globalBuf) {
	std::string result{};
	char debug[512];
	memset(debug, 0, 512);
	for (int i = 0; i < GetNodes().size; i++) {
		auto& node = GetNodes().data[i];
		char* type;
		char* name;
		if (node.m_TypeStrOffset < 0) {
			type = globalBuf + (0x7fffffff & node.m_TypeStrOffset);
		}
		else {
			type = GetStringData().data + node.m_TypeStrOffset;
		}
		if (node.m_NameStrOffset < 0) {
			name = globalBuf + (0x7fffffff & node.m_NameStrOffset);
		}
		else {
			name = GetStringData().data + node.m_NameStrOffset;
		}
		sprintf_s(debug, "%s %s // ByteSize{%x}, Index{%x}, IsArray{%d}, MetaFlag{%x}",
			type, name, node.m_ByteSize, node.m_Index, node.m_IsArray, node.m_MetaFlag);
		for (int j = 0; j < node.m_Level; j++) {
			result += "  ";
		}
		result += std::string(debug);
		result += "\n";
	}
	return result;
}
#else

// Dump the current field
std::string ToString(TypeTree& tree)
{
	std::string result{ };
	char debug[512];
	memset(debug, 0, 512);
	sprintf(debug, "%s %s // ByteOffset{%x}, ByteSize{%x}, Index{%x}, IsArray{%d}, MetaFlag{%x}",
		tree.m_Type.c_str(), tree.m_Name.c_str(), tree.m_ByteOffset, tree.m_ByteSize, tree.m_Index, tree.m_IsArray, tree.m_MetaFlag);
	result += std::string(debug);
	return result;
}

std::string DumpIterator(TypeTreeIterator& it, int level = 0)
{
	std::string curr{ };
	while (it.m_TypeTree != NULL) 
	{
		if (IsNull(it)) {
			throw std::runtime_error("TypeTreeIterator is null");
		}
		for (int i = 0; i < level; i++)
		{
			curr += std::string("  ");
		}
		std::string currString = ToString(*it.m_TypeTree);
		curr += currString + "\n";
		TypeTreeIterator children = Children(it);
		curr += DumpIterator(children, level + 1);
		it = Next(it);
	}
	return curr;
}

std::string TypeTree::Dump(char* globalBuf) {
	TypeTreeIterator* it = (TypeTreeIterator*)malloc(sizeof(TypeTreeIterator));
	TypeTreeIterator__TypeTreeIterator(it, this);
	return DumpIterator(*it);
}
#endif
#if VERSION_LT(5, 0)
std::vector<TypeTree*> GetChildren(TypeTree* tree) {
	TypeTreeIterator it;
	TypeTreeIterator__TypeTreeIterator(&it, tree);
	std::vector<TypeTree*> result;
	it = Children(it);
	while (it.m_TypeTree != NULL) {
		result.push_back(it.m_TypeTree);
		it = Next(it);
	}
	return result;
}
#endif
void TypeTree::Write(FILE* file)
{
#if VERSION_GE(5, 0)
	fwrite(&GetNodes().size, 4, 1, file);
	fwrite(&GetStringData().size, 4, 1, file);
	fwrite(GetNodes().data, GetNodes().size * sizeof(TypeTreeNode), 1, file);
	fwrite(GetStringData().data, GetStringData().size, 1, file);
#else
	auto type = m_Type.c_str();
	auto name = m_Name.c_str();
	int lenType = 1 + strlen(type);
	int lenName = 1 + strlen(name);
	fwrite(type, 1, lenType, file);
	fwrite(name, 1, lenName, file);
	fwrite(&m_ByteSize, 4, 5, file);
	auto children = GetChildren(this);
	int lenChildren = children.size();
	fwrite(&lenChildren, 4, 1, file);
	for (const auto& child : children)
	{
		child->Write(file);
	}
#endif
}
TypeTree* TypeTree::CreateTypeTree() {
	TypeTree* typeTree = (TypeTree*)malloc(sizeof(TypeTree));
#if defined(UNITY_2019_1) || defined(UNITY_2019_2)
	TypeTree__TypeTree(typeTree, kMemTypeTree, false);
#elif VERSION_GE(5, 3)
	TypeTree__TypeTree(typeTree, kMemTypeTree);
#else
	TypeTree__TypeTree(typeTree);
#endif
	return typeTree;
}
void TypeTree::DumpStructLayout() {
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
#elif VERSION_GE(5, 0)
	LOG_TYPE(TypeTree);
	LOG_MEMBER(TypeTree, m_Nodes);
	LOG_MEMBER(TypeTree, m_StringData);
	LOG_MEMBER(TypeTree, m_ByteOffsets);
	Log("\n");
#else
	LOG_TYPE(TypeTree);
	LOG_MEMBER(TypeTree, m_Children);
	LOG_MEMBER(TypeTree, m_Father);
	LOG_MEMBER(TypeTree, m_Type);
	LOG_MEMBER(TypeTree, m_Name);
	LOG_MEMBER(TypeTree, m_ByteSize);
	LOG_MEMBER(TypeTree, m_Index);
	LOG_MEMBER(TypeTree, m_IsArray);
	LOG_MEMBER(TypeTree, m_Version);
	LOG_MEMBER(TypeTree, m_MetaFlag);
	LOG_MEMBER(TypeTree, m_ByteOffset);
	LOG_MEMBER(TypeTree, m_DirectPtr);
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

	LOG_TYPE(dynamic_array<int>);
	LOG_MEMBER(dynamic_array<int>, data);
	LOG_MEMBER(dynamic_array<int>, label);
	LOG_MEMBER(dynamic_array<int>, size);
	LOG_MEMBER(dynamic_array<int>, cap);
	Log("\n");
}