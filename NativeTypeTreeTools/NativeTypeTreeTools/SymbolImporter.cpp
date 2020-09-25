#include "SymbolImporter.h"
#include "Util.h"
#include <string>
#include <stdexcept>
#include <regex>
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define S_OK ((HRESULT)0L)
#define S_FALSE ((HRESULT)1L)

bool DoLoadDataFromExe(
	const wchar_t* szFilename,
	IDiaDataSource** ppSource,
	IDiaSession** ppSession,
	IDiaSymbol** ppGlobal)
{
	wchar_t wszExt[MAX_PATH];
	const wchar_t* wszSearchPath = L"SRV**\\\\symbols\\symbols"; // Alternate path to search for debug data
	DWORD dwMachType = 0;
	HRESULT hr = CoInitialize(NULL);

	// Obtain access to the provider
	hr = CoCreateInstance(__uuidof(DiaSource),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IDiaDataSource),
		(void**)ppSource);

	if (FAILED(hr)) {
		Log("CoCreateInstance failed - HRESULT = %08X\n", hr);
		if (hr == 0x80040154) {
			Log("Dia needs to be registered with regsvr32 in adminstrator mode\n", hr);
		}
		wprintf(L"CoCreateInstance failed - HRESULT = %08X\n", hr);
		return false;
	}
	_wsplitpath_s(szFilename, NULL, 0, NULL, 0, NULL, 0, wszExt, MAX_PATH);
	Log("loadDataForExe()\n");
	hr = (*ppSource)->loadDataForExe(szFilename, wszSearchPath, NULL);
	if (FAILED(hr)) {
		Log("loadDataForExe failed - HRESULT = %08X\n", hr);
		wprintf(L"loadDataForExe failed - HRESULT = %08X\n", hr);
		return false;
	}
	hr = (*ppSource)->openSession(ppSession);
	if (FAILED(hr)) {
		Log("openSession failed - HRESULT = %08X\n", hr);
		wprintf(L"openSession failed - HRESULT = %08X\n", hr);
		return false;
	}
	Log("get_globalScope\n");
	hr = (*ppSession)->get_globalScope(ppGlobal);
	if (hr != S_OK) {
		Log("get_globalScope failed - HRESULT = %08X\n", hr);
		wprintf(L"get_globalScope failed\n");
		return false;
	}
	return true;
}
bool SymbolImporter::GetRVA(const char* symbolName, DWORD& rva, NameSearchOptions options) {
	if (pGlobalSymbol == NULL) {
		rva = 0;
		return false;
	}
	std::wstring wSymbolName = Widen(symbolName);
	IDiaEnumSymbols* pEnumSymbols;
	if (FAILED(pGlobalSymbol->findChildren(SymTagPublicSymbol, wSymbolName.c_str(), options, &pEnumSymbols))) {
		Log("Error finding children");
		rva = 0;
		return false;
	}
	LONG symbolCount;
	if (FAILED(pEnumSymbols->get_Count(&symbolCount)) || symbolCount == 0) {
		Log("GetRVA: No symbols matching %s found\n", symbolName);
		rva = 0;
		return false;
	};
	IDiaSymbol* pSymbol;
	ULONG celt = 0;
	while (SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1)) {
		if (pSymbol->get_relativeVirtualAddress(&rva) != S_OK) {
			Log("Error get_relativeVirtualAddress\n");
		}
		pSymbol->Release();
	}
	return true;
}
bool SymbolImporter::GetAddress(const char* symbolName, unsigned long long& address, NameSearchOptions options) {
	DWORD rva = 0;
	if (!this->GetRVA(symbolName, rva, options)) {
		address = 0;
		return false;
	}
	HMODULE baseAddress = GetModuleHandle(TEXT("Unity.exe"));
	address = (unsigned long long)baseAddress + rva;
	Log("Resolved Symbol: %s\n", symbolName);
	return true;
}
void SymbolImporter::ResolveSymbol(const char* symbolName, void*& target) {
	unsigned long long address;
	if (!GetAddress(symbolName, address, nsNone)) {
		throw std::runtime_error(std::string("Could not find symbol ") + symbolName);
	}
	target = (void*)address;
}
void SymbolImporter::ResolveSymbolStartsWith(const char* symbolName, void*& target) {
	unsigned long long address;
	std::string input = symbolName;
	std::regex specialChars{ R"([-[\]{}()*+?.,\^$|#\s])" };
	std::string sanitized = std::regex_replace(input, specialChars, R"(\$&)");
	sanitized += "*";
	if (!GetAddress(sanitized.c_str(), address, nsfRegularExpression)) {
		throw std::runtime_error(std::string("Could not find symbol ") + symbolName);
	}
	target = (void*)address;
}
void SymbolImporter::LoadFromExe(const char* filePath) {
	if (pDiaDataSource != NULL ||
		pDiaSession != NULL ||
		pGlobalSymbol != NULL) {
		throw std::runtime_error("Error initializing SymbolImporter");
	}
	std::wstring wFilePath = Widen(filePath);
	DoLoadDataFromExe(wFilePath.c_str(), &pDiaDataSource, &pDiaSession, &pGlobalSymbol);
}
SymbolImporter::~SymbolImporter() {
	if (pGlobalSymbol) {
		pGlobalSymbol->Release();
		pGlobalSymbol = NULL;
	}
	if (pDiaSession) {
		pDiaSession->Release();
		pDiaSession = NULL;
	}
	CoUninitialize();
}