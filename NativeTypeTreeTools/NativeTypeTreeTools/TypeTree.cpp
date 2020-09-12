#include "Structs.h"

dynamic_array<TypeTreeNode>& TypeTree::GetNodes() {
#ifdef UNITY_2019_1_OR_NEWER
	return Data->m_Nodes;
#else
	return m_Nodes;
#endif
}
dynamic_array<char>& TypeTree::GetStringData() {
#ifdef UNITY_2019_1_OR_NEWER
	return Data->m_StringData;
#else
	return m_StringData;
#endif
}
dynamic_array<void*>& TypeTree::GetByteOffsets() {
#ifdef UNITY_2019_1_OR_NEWER
	return Data->m_ByteOffsets;
#else
	return m_ByteOffsets;
#endif
}
 void TypeTree::Log() {


}
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
void TypeTree::Write(FILE* file)
{
	fwrite(&GetNodes().size, 4, 1, file);
	fwrite(&GetStringData().size, 4, 1, file);
	fwrite(GetNodes().data, GetNodes().size * sizeof(TypeTreeNode), 1, file);
	fwrite(GetStringData().data, GetStringData().size, 1, file);
}