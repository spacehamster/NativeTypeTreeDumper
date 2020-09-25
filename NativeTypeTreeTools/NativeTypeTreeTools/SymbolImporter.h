#pragma once
#include "Util.h"
#include "dia2.h"
#include <stdexcept>
class SymbolImporter {
public:
	void LoadFromExe(const char* exePath) throw(std::runtime_error);
	void ResolveSymbol(const char* symbolName, void*& ptr) throw(std::runtime_error);
	void ResolveSymbolStartsWith(const char* symbolName, void*& ptr) throw(std::runtime_error);
	~SymbolImporter();
private:
	IDiaDataSource* pDiaDataSource;
	IDiaSession* pDiaSession;
	IDiaSymbol* pGlobalSymbol;
	bool GetRVA(const char* symbolName, DWORD& rva, NameSearchOptions options);
	bool GetAddress(const char* symbolName, unsigned long long& addr, NameSearchOptions options);
};