#include "CommonStrings.h"

char** CommonString_BufferBegin;
char** CommonString_BufferEnd;

void CommonString::InitBindings(SymbolImporter &importer) {
#if VERSION_GE(5, 0)
	importer.ResolveSymbol("?BufferBegin@CommonString@Unity@@3QEBDEB", (void*&)CommonString_BufferBegin);
	importer.ResolveSymbol("?BufferEnd@CommonString@Unity@@3QEBDEB", (void*&)CommonString_BufferEnd);
#endif
}
char* CommonString::GetBufferBegin() {
	if (CommonString_BufferBegin == NULL) {
		throw new std::runtime_error("CommonString_BufferBegin is null");
	}
	return *CommonString_BufferBegin;
}
char* CommonString::GetBufferEnd() {
	if (CommonString_BufferBegin == NULL) {
		throw new std::runtime_error("CommonString_BufferEnd is null");
	}
	return *CommonString_BufferEnd;
}
char* CommonString::GetPtr() {
#if VERSION_GE(5, 0)
	return *CommonString_BufferBegin;
#else
	return NULL;
#endif
}