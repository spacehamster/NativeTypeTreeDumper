#pragma once
#include "Util.h"
#include "SymbolImporter.h"
#include "MemLabel.h"
#include "RuntimeTypeInfo.h"

struct TypeTreeNode
{
	int16_t m_Version;
	int8_t m_Level;
	int8_t m_IsArray;
	int32_t m_TypeStrOffset; // offset; &(1<<31) => uses common buffer; otherwise local buffer
	int32_t m_NameStrOffset; // same as Type
	int32_t m_ByteSize;
	int32_t m_Index;
	int32_t m_MetaFlag;
#ifdef UNITY_2019_1_OR_NEWER
	uint64_t m_RefTypeHash;
#endif
};

template <typename T>
struct dynamic_array
{
	T* data;
	MemLabelId label;
	size_t size;
	size_t cap; // < 0 => data is in union{T*, T[N]}
}; // 0x14

#ifdef UNITY_2019_1_OR_NEWER
struct TypeTreeShareableData {
	dynamic_array<TypeTreeNode> m_Nodes;
	dynamic_array<char> m_StringData;
	dynamic_array<void*> m_ByteOffsets;
	TransferInstructionFlags FlagsAtGeneration;
	int RefCount;
	MemLabelId* MemLabel;
};
#endif

struct TypeTree
{
#if defined(UNITY_2019_2) || defined(UNITY_2019_1)
	TypeTreeShareableData* Data;
	TypeTreeShareableData m_PrivateData;
#elif defined(UNITY_2019_1_OR_NEWER)
	TypeTreeShareableData* Data;
	void* ReferencedTypes;
	bool PoolOwned;
#elif VERSION_GE(5, 0)
	dynamic_array<TypeTreeNode> m_Nodes;
	dynamic_array<char> m_StringData;
	dynamic_array<void*> m_ByteOffsets;
#else
	TypeTreeList m_Children;
	TypeTree* m_Father;
	TypeTreeString m_Type; // 0x14
	TypeTreeString m_Name; // 0x30
	int m_ByteSize;
	int m_Index;
	int m_IsArray;
	int m_Version;
	int m_MetaFlag;
	int m_ByteOffset;
	int m_DirectPtr;
#endif
	std::string Dump(char* globalBuf);
	void Write(FILE* file);
	dynamic_array<TypeTreeNode>& GetNodes();
	dynamic_array<char>& GetStringData();
	dynamic_array<void*>& GetByteOffsets();
	static void DumpStructLayout();
	static void InitBindings(SymbolImporter& importer);
	static TypeTree* CreateTypeTree();
};

struct TypeTreeIterator 
{
	TypeTree* m_TypeTree;
	//Note: Ghidra says 4 byte list, but dumping crashes unless it is 16 bytes
	TypeTreeList m_ParentIterator;
};
