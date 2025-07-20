// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <common.h>
#include <Font.h>
#include <FileMgr.h>

#include "MemUtils.h"
#include <TxdStore.h>
//#include <aobscan.h>
  //简化类名
using MU = MemUnits;



//#define CALASS_CALL(FUNAddress,ReturnType,Class) RET(ReturnType)

/*
(*(int(__thiscall*)(DWORD, const char*))((DWORD)Fun_65CA80))(
    (DWORD)Class_6,
    aRa2mdIni);*/

DWORD EXE = 0;
CCFont* font = NULL;
/*
void PrintString(float xstart, float ystart, wchar* s)
{
   // RET(void)(DWORD,int)FUN_ADDRESS(0)(1);
    std::wstring ss((wchar_t*)s);
    wprintf_s(ss.c_str());
    wprintf_s(L"\n");

}*/
static bool IsInit = false;

typedef void (*ButtonPressFunc)(int8 action); // see FEOPTION_ACTIONs above
typedef wchar* (*DrawFunc)(bool* disabled, bool userHovering); // you must return a pointer for right text.
typedef  void(F_FrontendOptionAddDynamic)(const char* gxtKey, uint16 x, uint16 y, uint8 align, DrawFunc drawFunc, int8* var, ButtonPressFunc buttonPressFunc, const char* saveCat, const char* saveName);
F_FrontendOptionAddDynamic* dFrontendOptionAddDynamic = NULL;

typedef  void(F_InitialiseChangedLanguageSettings)();
F_InitialiseChangedLanguageSettings* dInitialiseChangedLanguageSettings = NULL;
typedef  void(F_SaveSettings)();
F_SaveSettings* dSaveSettings = NULL;

//typedef  int(F_CFileMgr_OpenFile)(const char* file, const char* mode);
//F_CFileMgr_OpenFile* dCFileMgr_OpenFile = NULL;

bool* ReloadObrTxtGxt = NULL;
int32* m_PrefsLanguage = NULL;
int32 RusSelect = 0;

enum LANGUAGE
{
    LANGUAGE_AMERICAN,
    LANGUAGE_FRENCH,
    LANGUAGE_GERMAN,
    LANGUAGE_ITALIAN,
    LANGUAGE_SPANISH,
#ifdef MORE_LANGUAGES
    LANGUAGE_POLISH,
    LANGUAGE_RUSSIAN,
    LANGUAGE_JAPANESE,
    LANGUAGE_CHINESE,
    LANGUAGE_CHINESE_A,
#endif
};
void Lang_Call_Back()
{
    //printf_s("被调用 语言为:%d\n", *m_PrefsLanguage);
    switch (*m_PrefsLanguage) {
    case LANGUAGE_POLISH:
        CCFont::ReloadFonts(FONT_LANGSET_POLISH);
        break;
    case LANGUAGE_RUSSIAN:
        CCFont::ReloadFonts(FONT_LANGSET_RUSSIAN);
        break;
    case LANGUAGE_JAPANESE:
        CCFont::ReloadFonts(FONT_LANGSET_RUSSIAN);
        break;
    case LANGUAGE_CHINESE:
        CCFont::ReloadFonts(FONT_LANGSET_CHINESE);
        break;
    case LANGUAGE_CHINESE_A:
        CCFont::ReloadFonts(FONT_LANGSET_CHINESE1);
        break;
    default:
        CCFont::ReloadFonts(FONT_LANGSET_EFIGS);
        break;
    }

}

static int CText_Load_CallBack(const char* file, const char* mode)
{
   // printf_s("文本加载被调用:文件为:%s\n", file);
    char filename[48]{0};
    int ret = 0;
    //sprintf(filename, file);
    uint8 nn = *m_PrefsLanguage;
    switch (nn) {
    case LANGUAGE_AMERICAN:
        ret = CCFileMgr::OpenFile(file, mode);
        break;
    case LANGUAGE_FRENCH:
        ret = CCFileMgr::OpenFile(file, mode);
        break;
    case LANGUAGE_GERMAN:
        ret = CCFileMgr::OpenFile(file, mode);
        break;
    case LANGUAGE_ITALIAN:
        ret = CCFileMgr::OpenFile(file, mode);
        break;
    case LANGUAGE_SPANISH:
        ret = CCFileMgr::OpenFile(file, mode);
        break;
#ifdef MORE_LANGUAGES
    case LANGUAGE_POLISH:
        ret = CCFileMgr::OpenFile(file, mode);
        break;
    case LANGUAGE_RUSSIAN:
        ret = CCFileMgr::OpenFile(file, mode);
        break;
    case LANGUAGE_JAPANESE:
        ret = CCFileMgr::OpenFile(file, mode);
        break;
    case LANGUAGE_CHINESE:
        sprintf(filename, "CHINESE.GXT");
        ret = CCFileMgr::OpenFile(filename, mode);
        //printf_s("文本加载:%s\n",ret? "成功" : "失败");
        break;
    case LANGUAGE_CHINESE_A:
        sprintf(filename, "CHINESE1.GXT");
        ret = CCFileMgr::OpenFile(filename, mode);
        //printf_s("文本加载:%s\n",ret? "成功" : "失败");
        break;
#endif
    }
   
    return ret;
}
const char FrontendOptionAddDynamic_code[] = "558BEC 56 E8 ?? ?? ?? ?? 0F BE C8 69 ?? ?? ?? ?? ?? ?? ?? ?? ?? C1 E1 05 6A 08";
const char InitialiseChangedLanguageSettings_code[] = "558BEC 51568B??80";
const char LangRusSelect_code[] = "558BEC 80 ?? ?? ?? 75 ?? B9 ?? ?? ?? ?? C7";
const char SaveSettings_code[] = "8A ?? ?? 88 ?? ?? E9 ?? ?? ?? ?? CC CC CC CC CC 558BEC";

const char Load_GXT_code[] = "558BEC 83 ?? ?? A1 ?? ?? ?? ?? 33 C5 89 ?? ?? 53 8B D9 C6 ?? ?? ?? 56 57 C6";

const char Load_Mission_GXT_code[] = "558BEC 83 ?? ?? A1 ?? ?? ?? ?? 33 C5 89 ?? ?? 53 56 57 8B ?? ?? 8B ?? 89 ?? ?? E8 ?? ?? ?? ?? FF";
const char FileMgr_OpenFile_code[] = "558BEC 5D E9 ?? ?? ?? ?? CC CC CC CC CC CC CC 558BEC 68 ?? ?? ?? ?? FF ?? ?? E8";

F_CFileMgr_OpenFile* CCFileMgr::dOpenFile = NULL;
F_CFileMgr_CloseFile* CCFileMgr::dCloseFile = NULL;

DWORD *VCMenuManager_Class = 0;
void HookLangUageSelect(DWORD exe, const char* ExeName)
{
    //const char* ExeName = "reVC.exe";

    //菜单选择hook
    std::vector <ULONGLONG> vResultContainer;
    vResultContainer = MU::ScanMemory(FrontendOptionAddDynamic_code, ExeName);
    CHECK_ERROR(vResultContainer);
    dFrontendOptionAddDynamic = (F_FrontendOptionAddDynamic*)vResultContainer[0];
    printf_s("开始Patch:菜单选择\n");

    //从LangRusSelect获取菜单加载相关函数
    vResultContainer = MU::ScanMemory(LangRusSelect_code, ExeName);
    CHECK_ERROR(vResultContainer);
    DWORD PrefsLanguage_offset = (DWORD)vResultContainer.at(0);
    DWORD PrefsLanguage_addr = *(DWORD*)(PrefsLanguage_offset + 0x10);
    m_PrefsLanguage = (int32*)(PrefsLanguage_addr);
    RusSelect = *(DWORD*)(PrefsLanguage_offset + 0x14);


    DWORD ReloadObrTxtGxt_offset = (DWORD)vResultContainer.at(0);
    DWORD ReloadObrTxtGxt_addr = *(DWORD*)(ReloadObrTxtGxt_offset + 0x1A);
    ReloadObrTxtGxt = (bool*)(ReloadObrTxtGxt_addr);

    VCMenuManager_Class = (DWORD*)vResultContainer[0] + 0xA;
    dInitialiseChangedLanguageSettings = (F_InitialiseChangedLanguageSettings*)MemUnits::Call_addr_To_Addr(vResultContainer[0] + 0x1F);
    dSaveSettings = (F_InitialiseChangedLanguageSettings*)MemUnits::Call_addr_To_Addr(vResultContainer[0] + 0x2A);


   // printf_s("开始Patch:Lang_Call_Back\n");
    //写入回调
    DWORD hook_Callback_offset = (DWORD)(dInitialiseChangedLanguageSettings);
    DWORD hook_Callback_addr = (hook_Callback_offset + 0x61) ;
    MU::WriteCall(hook_Callback_addr, (DWORD)Lang_Call_Back,MU::CODE_CALL);
     uint8 pop_esi = 0x5E;
     uint8 pop_ebp = 0x5D;
     uint8 mov = 0x8B;
     uint8 esp_ebp = 0xE5;
     uint8 retn = 0xC3;
     MU::SetMemWriteEnable(hook_Callback_addr + 5, 5);
     MU::MemWrite(hook_Callback_addr + 5, pop_esi);
     MU::MemWrite(hook_Callback_addr + 6, mov);
     MU::MemWrite(hook_Callback_addr + 7, esp_ebp);
     MU::MemWrite(hook_Callback_addr + 8, pop_ebp);
     MU::MemWrite(hook_Callback_addr + 9, retn);

   //开始hook加载gxt的
     printf_s("开始Patch:加载gxt\n");
     vResultContainer = MU::ScanMemory(Load_GXT_code, ExeName);
     CHECK_ERROR(vResultContainer);
     DWORD hook_OpenFile_offset = (DWORD)vResultContainer.at(0);
     DWORD hook_OpenFile_addr = (hook_OpenFile_offset + 0xF3);
     //加载gxt的hook回调 
     // HooK_Text
     //DWORD CFileMgr_OpenFile_addr1 = *(DWORD*)(hook_OpenFile_offset + 0xF4);
     //DWORD CFileMgr_OpenFile_next_addr1 = (hook_OpenFile_offset + 0xF8);
     //DWORD CFileMgr_OpenFile_addr = CFileMgr_OpenFile_next_addr1 + CFileMgr_OpenFile_addr1;
     
     CCFileMgr::dOpenFile = (F_CFileMgr_OpenFile*)MemUnits::Call_addr_To_Addr(hook_OpenFile_offset + 0xF3);
     CCFileMgr::dCloseFile = (F_CFileMgr_CloseFile*)MemUnits::Call_addr_To_Addr(hook_OpenFile_offset + 0x224);
     //dCFileMgr_OpenFile = (F_CFileMgr_OpenFile*)(CFileMgr_OpenFile_addr);
    MU::WriteCall(hook_OpenFile_addr, (DWORD)CText_Load_CallBack,MU::CODE_CALL);

     //HooK_missionText
     vResultContainer = MU::ScanMemory(Load_Mission_GXT_code, ExeName);
     CHECK_ERROR(vResultContainer);
     hook_OpenFile_addr = (vResultContainer.at(0) + 0x121);
    MU::WriteCall(hook_OpenFile_addr, (DWORD)CText_Load_CallBack,MU::CODE_CALL);
     /*
     vResultContainer = MU::ScanMemory(FileMgr_OpenFile_code, ExeName);
     CHECK_ERROR(vResultContainer);
     dCFileMgr_OpenFile = (F_CFileMgr_OpenFile*)(vResultContainer.at(0));*/
     printf_s("菜单Patch:完成\n");
}
enum
{
    MENUALIGN_LEFT = 1,
    MENUALIGN_RIGHT,
    MENUALIGN_CENTER,
};
#define FEOPTION_ACTION_LEFT 0
#define FEOPTION_ACTION_RIGHT 1
#define FEOPTION_ACTION_SELECT 2
#define FEOPTION_ACTION_FOCUSLOSS 3


void LangchaSelect(int8 action)
{
    if (action== FEOPTION_ACTION_SELECT)
    {
       // printf_s("被点击:%d\n", action);
        *m_PrefsLanguage = LANGUAGE_CHINESE;
        *ReloadObrTxtGxt = true;
        //下面两个不能直接调用 类函数
       // dInitialiseChangedLanguageSettings();
        //dSaveSettings();
        //调用类函数
        THIS_CALL(void,dInitialiseChangedLanguageSettings)((DWORD)&VCMenuManager_Class);
        THIS_CALL(void, dSaveSettings)((DWORD)&VCMenuManager_Class);
    //RET(void)(DWORD)FUN_ADDRESS(dInitialiseChangedLanguageSettings)((DWORD)&VCMenuManager_Class);
   // RET(void)(DWORD)FUN_ADDRESS(dSaveSettings)((DWORD)&VCMenuManager_Class);
    }
   
    /*
    if (action == FEOPTION_ACTION_SELECT) {
        FrontEndMenuManager.m_PrefsLanguage = CMenuManager::LANGUAGE_CHINESE;
        FrontEndMenuManager.m_bFrontEnd_ReloadObrTxtGxt = true;
        FrontEndMenuManager.InitialiseChangedLanguageSettings();
        FrontEndMenuManager.SaveSettings();
    }*/
}
void Langcha1Select(int8 action)
{
    if (action == FEOPTION_ACTION_SELECT)
    {
        // printf_s("被点击:%d\n", action);
        *m_PrefsLanguage = LANGUAGE_CHINESE_A;
        *ReloadObrTxtGxt = true;
        //下面两个不能直接调用 类函数
       // dInitialiseChangedLanguageSettings();
        //dSaveSettings();
        //调用类函数
        THIS_CALL(void, dInitialiseChangedLanguageSettings)((DWORD)&VCMenuManager_Class);
        THIS_CALL(void, dSaveSettings)((DWORD)&VCMenuManager_Class);
        //RET(void)(DWORD)FUN_ADDRESS(dInitialiseChangedLanguageSettings)((DWORD)&VCMenuManager_Class);
       // RET(void)(DWORD)FUN_ADDRESS(dSaveSettings)((DWORD)&VCMenuManager_Class);
    }

 
}
//在语言选项添加中文
void CCFont::Add_Memu_Chs()
{
   // printf_s("调用FrontendOptionAddDynamic\n");
    dFrontendOptionAddDynamic("FEL_CHS", 0, 0, MENUALIGN_CENTER, nil, nil, LangchaSelect, nil, nil);
   // dFrontendOptionAddDynamic("FEL_CHSS", 0, 0, MENUALIGN_CENTER, nil, nil, LangchaSelect, nil, nil);
   // printf_s("调用\n");
}
void Init()
{
    const char* ExeName = "reVC.exe";

    //Sleep(200);
    EXE = (DWORD)GetModuleHandleA(ExeName);
    printf_s("开始处理:CTxdStore\n");
    CCTxdStore::InitPatch(EXE, ExeName);

    printf_s("开始处理:CSprite2d\n");
    CCSprite2d::InitPatch(EXE, ExeName);

    printf_s("开始开始处理:CFont\n");
   // CCFont::InitValue(EXE);
    CCFont::InitPatch(EXE, ExeName);

    printf_s("初始化Hook:CFont\n");
    CCFont::InitHook(EXE);
    printf_s("初始化Hook:CMenu\n");
    HookLangUageSelect(EXE, ExeName);
   

    printf_s("执行完毕...\n");
    IsInit = true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        
       
#ifdef _DEBUG
            MessageBoxA(NULL, "准备调试", "提示", MB_OK);

            //创建调试窗口
            AllocConsole();
            //freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w+t", stdout);
            freopen_s(reinterpret_cast<FILE**>(stdout), "debug.txt", "w+t", stdout);
            //freopen_s(reinterpret_cast<FILE**>(stdout), "out.txt", "w+t", stdout);
            //FILE* f2=freopen("CONIN$", "r+t", stdin);
            SetConsoleTitleA("调试窗口");
#endif // DEBUG
            {
               

                printf_s("初始化\n");


                Init();
                HINSTANCE hDll = LoadLibrary(L"revc_cheat.dll");
                if (hDll == NULL)
                {
                    // 处理错误
                }
            }
           
            
        
       
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        FreeConsole();
        break;
    }
    return TRUE;
}

