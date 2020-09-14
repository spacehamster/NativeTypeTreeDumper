#include "PdbSymbolImporter.h"
#include "Util.h"
#include <string>
#include <stdexcept>
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define S_OK                                   ((HRESULT)0L)
#define S_FALSE                                ((HRESULT)1L)

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
bool PdbSymbolImporter::GetRVA(const char* symbolName, DWORD& rva ) {
    if (pGlobalSymbol == NULL) {
        rva = 0;
        return false;
    }
    std::wstring wSymbolName = Widen(symbolName);
    IDiaEnumSymbols* pEnumSymbols;
    if (FAILED(pGlobalSymbol->findChildren(SymTagPublicSymbol, wSymbolName.c_str(), nsNone, &pEnumSymbols))) {
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
bool PdbSymbolImporter::GetAddress(const char* symbolName, unsigned long long& address) {
    DWORD rva = 0;
    if (!this->GetRVA(symbolName, rva)) {
        address = 0;
        return false;
    }
    HMODULE baseAddress = GetModuleHandle(L"Unity.exe");
    address = (unsigned long long)baseAddress + rva;
    return true;
}
void PdbSymbolImporter::AssignAddress(const char* symbolName, void*& target) {
    unsigned long long address;
    if (!GetAddress(symbolName, address)) {
        throw std::runtime_error(std::string("Could not find symbol ") + symbolName);
    }
    target = (void*)address;
}
void PdbSymbolImporter::LoadFromExe(const char* filePath) {
    if (pDiaDataSource != NULL ||
        pDiaSession != NULL ||
        pGlobalSymbol != NULL) {
        throw std::runtime_error("Error initializing SymbolImporter");
    }
    std::wstring wFilePath = Widen(filePath);
    DoLoadDataFromExe(wFilePath.c_str(), &pDiaDataSource, &pDiaSession, &pGlobalSymbol);
}
PdbSymbolImporter::~PdbSymbolImporter() {
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