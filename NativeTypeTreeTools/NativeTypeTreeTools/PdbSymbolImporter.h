#pragma once
#include "dia2.h"
#include <stdexcept>
class PdbSymbolImporter {
public:
	bool LoadFromExe(const char* exePath);
	void AssignAddress(const char* symbolName, void*& ptr) throw(std::runtime_error);
	~PdbSymbolImporter();
private:
	IDiaDataSource* pDiaDataSource;
	IDiaSession* pDiaSession;
	IDiaSymbol* pGlobalSymbol;
	bool GetRVA(const char* symbolName, DWORD& rva);
	bool GetAddress(const char* symbolName, unsigned long long& addr);
};