// ReVc_cn.cpp : 定义应用程序的入口点。
//


#include "framework.h"
#include "ReVc_cn.h"
#include <iostream>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名



//#pragma comment(lib,"Psapi.lib")  //编译这个lib文件
//#pragma comment(lib,"imagehlp.lib")  

//软件启动前注入Dll
//param1:sDllPath:dll路径，run_path:执行文件路径
bool injectDll(char sDllPath[], TCHAR run_path[])
{
    //启动目标进程
    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    //pi:创建线程返回的信息
    PROCESS_INFORMATION pi;
    BOOL bRet = ::CreateProcess(NULL, run_path, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
    //获得进程入口
    HANDLE curProcessHandle = pi.hProcess; //获得当前进程的句柄
    // 创建虚拟内存地址，放置dll路径
    LPVOID pDllPath = VirtualAllocEx(curProcessHandle, NULL, strlen(sDllPath) + 1, MEM_COMMIT, PAGE_READWRITE);

    WriteProcessMemory(curProcessHandle, pDllPath, sDllPath, strlen(sDllPath) + 1, NULL);
    // 获取LoadLibraryA地址:用于注入dll;
    PTHREAD_START_ROUTINE pfnLoadLib = (PTHREAD_START_ROUTINE)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")), "LoadLibraryA");
    // 在线程中执行dll中的入口函数：即导入dll
    HANDLE hNewThread = CreateRemoteThread(curProcessHandle, NULL, 0, pfnLoadLib, pDllPath, 0, NULL);
    // TODO: 后续可以插入命令行操作
    WaitForSingleObject(hNewThread, INFINITE);


    VirtualFreeEx(curProcessHandle, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hNewThread);
    CloseHandle(curProcessHandle);
    ResumeThread(pi.hThread);//继续
    return true;
}

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
bool MakeResourcePath()
{
    char pluginPath[32];
    char pluginPath2[32];
    char pluginPath3[32];

    //GetModuleFileNameA(hPlugin, pluginPath, 260);
    //std::strcpy(CFont::fontPath, pluginPath);
    //std::strcpy(CFont::textPath, pluginPath);
    //std::strcpy(CCharTable::datPath, pluginPath);

    strcpy_s(pluginPath, "models\\CHINESE.txd");
    strcpy_s(pluginPath2, "TEXT\\chinese.gxt");
    strcpy_s(pluginPath3, "data\\Chinese.dat");

    if (!PathFileExistsA(pluginPath))
    {
        MessageBoxW(nullptr, L"找不到资源文件: <CHINESE.txd>，请确认汉化文件是否在对应文件夹！", L"警告", MB_ICONWARNING);
        return false;
    }
    if (!PathFileExistsA(pluginPath2))
    {
        MessageBoxW(nullptr, L"找不到资源文件: <chinese.gxt>，请确认汉化文件是否在对应文件夹！", L"警告", MB_ICONWARNING);
        return false;
    }
    if (!PathFileExistsA(pluginPath3))
    {
        MessageBoxW(nullptr, L"找不到资源文件: <Chinese.dat>，请确认汉化文件是否在对应文件夹！", L"警告", MB_ICONWARNING);
        return false;
    }

    return true;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

   

    PROCESS_INFORMATION  ProcessInformation; 
    STARTUPINFO  StartupInfo;
    ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));

    memset(&StartupInfo, 0, sizeof(StartupInfo));

    char CommandLine[32]{0};
   // wcscat(CommandLine, );
    //wcscat_s(CommandLine, L"Revc.exe");
    WCHAR Buffer[256];
    memset(CommandLine, 0, 28);
    strcat_s(CommandLine, "reVc.exe");
   char dllpath[] = "revc_chs_patch.dll";
   
   if (!MakeResourcePath())
   {
       return 0;
   }
   if (!PathFileExistsA(dllpath))
   {
       MessageBox(NULL, L"补丁文件不存在", L"启动失败", MB_ICONWARNING);
       return 0;
   }

 
   WCHAR exepath[] = L"reVc.exe";
   if (!injectDll(dllpath, exepath))
   {
       MessageBox(NULL, L"补丁注入失败", L"启动失败", MB_ICONWARNING);
   }
  
  
}


