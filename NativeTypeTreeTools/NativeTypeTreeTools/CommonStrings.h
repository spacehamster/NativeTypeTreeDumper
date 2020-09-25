#pragma once
#include "Util.h"
#include "SymbolImporter.h"
class CommonString {
public:
	static void InitBindings(SymbolImporter& importer);
	static char* GetBufferBegin();
	static char* GetBufferEnd();
	static char* GetPtr();
};