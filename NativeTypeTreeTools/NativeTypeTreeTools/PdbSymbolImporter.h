#pragma once
#include "dia2.h"
class PdbSymbolImporter {
	IDiaDataSource* pDiaDataSource;
	IDiaSession* pDiaSession;
	IDiaSymbol* pGlobalSymbol;
public:
	bool LoadFromExe(const char* exePath);
	bool GetRVA(const char* symolName, DWORD& rva);
	bool GetAddress(const char* symolName, unsigned long long& addr);
	bool AssignAddress(const char* symolName, void*& ptr);
	~PdbSymbolImporter();
};