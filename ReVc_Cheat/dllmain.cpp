// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "CallList.h"
#include "d3d9.h"
#include <vector>
#include <MemUtils.h>
//#include <imgui_internal.h>
#include "imgui.h"

#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "CheatList.h"
DrawCallList* pDrawCallList = NULL;
DrawTextCallList* TextCallList = NULL;
void SetDrawCallList(DrawCallList *CallList)
{
    pDrawCallList = CallList;
}
void SetDrawTextCallList(DrawTextCallList* CallList)
{
    TextCallList = CallList;
}
WNDPROC oWndProc = nullptr;
bool* MenuActive = NULL;
bool DisableMouse = false;
bool Is_run = false;
bool show_window = false;

HWND* window = NULL;
DWORD* deviceaddr = NULL;
IDirect3DDevice9* device = NULL;
// imgui 窗体消息用的 
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT(CALLBACK* GameWndProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
// Imgui窗体消息循环
LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    *window = hWnd;
   
    switch (uMsg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
            // 按下F9弹窗
        case VK_F5:
            if (Is_run)
            {
                show_window = !show_window;
                DisableMouse = show_window;
            }
            //MessageBox(0, 0, 0, 0);
            
            break;
        }
        break;
    }
    if (show_window)
    {
        //消息 操作imgui用的
        if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return true;
    }

    return GameWndProc(hWnd, uMsg, wParam, lParam); 
}

/*
struct D3d9Globals
{
    HWND window;
    IDirect3D9* d3d9;
}*d3dGlobals;
*/



#include <dinput.h>

//禁用鼠标锁定
const char MenuCamera_Feature_Code[] =
"80 3D ?? ?? ?? ?? 00 "       // cmp FrontEndMenuManager.m_bMenuActive, 0
"0F 85 ?? ?? ?? ?? "          // jnz loc_F0D2E0
"B9 ?? ?? ?? ?? "             // mov ecx, offset CCamera::TheCamera
"E8 ?? ?? ?? ?? "             // call CCamera::GetScreenFadeStatus
"83 F8 02 "                   // cmp eax, 2
"0F 84 ?? ?? ?? ?? "          // jz loc_F0D2E0
"66 0F 6E 05 ?? ?? ?? ?? "    // movd xmm0, _RsGlobal.width
"8D 45 ?? "                   // lea eax, [ebp+pos]
"0F 5B C0 "                   // cvtdq2ps xmm0, xmm0
"50 "                         // push eax
"F3 0F 59 05 ?? ?? ?? ?? "    // mulss xmm0, [__real@3f000000]
"F3 0F 11 45 ?? "             // movss [ebp+pos], xmm0
"66 0F 6E 05 ?? ?? ?? ?? "    // movd xmm0, _RsGlobal.height
"0F 5B C0 "                   // cvtdq2ps xmm0, xmm0
"F3 0F 59 05 ?? ?? ?? ??";    // mulss xmm0, [__real@3f000000]
//禁用鼠标
const char MouseUpdate_Feature_Code[] =
"50 "                   // push eax
"E8 ?? ?? ?? ?? "       // call __InputInitialiseMouse
"A1 ?? ?? ?? ?? "       // mov eax, _RsGlobal.ps
"83 C4 04 "             // add esp, 4
"83 78 1C 00 "          // cmp dword ptr [eax+1Ch], 0
"0F 84 ?? ?? ?? ?? "    // jz loc_31A71F
"8D 45 ?? "             // lea eax, [ebp+state]
"50 "                   // push eax
"E8 ?? ?? ?? ?? "       // call __InputGetMouseState
"83 C4 04 "             // add esp, 4
"85 C0 "                // test eax, eax
"0F 88 ?? ?? ?? ?? "    // js loc_31A71F
"80 3D ?? ?? ?? ?? 00 " // cmp FrontEndMenuManager.m_bMenuActive, 0
"B9 01 00 00 00";       // mov ecx, 1


// 原始汇编代码特征码（覆盖全部指令）
const char DEVICEINIT_code[] =
"E8 ?? ?? ?? ??"
"85 C0"   // 
"74 ??"   // 
"E8 ?? ?? ?? ??"   //
"85 C0"   // 
"74 ??"
"E9";            // 

const char startD3D_Code[] =
"FF 35 ?? ?? ?? ?? "   // push d3d9Globals.window
"8B 08 "               // mov ecx, [eax]
"6A 01 "               // push 1
"FF 35 ?? ?? ?? ?? "   // push d3d9Globals.adapter
"50 "                  // push eax
"FF 51 40 "            // call [ecx+40h]
"5F 5E 5B "            // pop edi; pop esi; pop ebx
"85 C0 "               // test eax, eax
"79 ?? "               // jns short (偏移通配)
"68 ?? ?? ?? ?? "      // push 65Ah (若需要通配立即数)
"68 ?? ?? ?? ?? "      // push string_addr1
"68 ?? ?? ?? ?? "      // push string_addr2
"6A 02 "               // push 2
"C7 45 ?? 00 00 00 00";// mov [ebp+var],0 (ebp偏移通配)

const char Render2D_Feature_Code[] =
"E8 ?? ?? ?? ?? "   // call CHud::Draw
"E8 ?? ?? ?? ?? "   // call CSpecialFX::Render2DFXs
"B9 ?? ?? ?? ?? "   // mov ecx, offset COnscreenTimer
"E8 ?? ?? ?? ?? "   // call COnscreenTimer::ProcessForDisplay
"E8 ?? ?? ?? ?? "   // call CMessages::Display
"E8 ?? ?? ?? ?? "   // call CDarkel::DrawMessages
"E8 ?? ?? ?? ?? "   // call CGarages::PrintMessages
"E8 ?? ?? ?? ?? "   // call CPad::PrintErrorMessage
"E8 ?? ?? ?? ?? "   // call CFont::DrawFonts
"E8 ?? ?? ?? ?? "   // call COcclusion::Render
"E8 ?? ?? ?? ?? "   // call DebugMenuRender
"E8 ?? ?? ?? ?? "   // call PopRendergroup
"8B E5 "            // mov esp, ebp
"5D "               // pop ebp
"C3";               // retn

const char WndProc_Code[] =
"C7 45 D8 ?? ?? ?? ?? C7 45 DC 00 00 00 00 C7 45 E0 00 00 00 00 89 5D E4 FF 15 ?? ?? ?? ?? 68 00 7F 00 00 6A 00 89 45 E8 FF 15 ?? ?? ?? ?? 89 45";

const char CloseIm3d_Code[] =
"E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 50 8B 08 FF 51 08 85 C0 74 0D 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04";

HRESULT(*dInputGetMouseState)(DIMOUSESTATE2* state) = NULL;
HRESULT _InputGetMouseState(DIMOUSESTATE2* state)
{
	HRESULT ret = dInputGetMouseState(state);
    //判断游戏菜单是否激活
    if (*MenuActive)
    {
        DisableMouse = false;
    }
    if (DisableMouse)
    {
        state->lX = 0;
        state->lY = 0;
        state->rgbButtons[0] = 0;
        state->rgbButtons[1] = 0;
        state->rgbButtons[2] = 0;
        state->rgbButtons[3] = 0;
        state->rgbButtons[4] = 0;
    }
    return ret;
}
struct V2d
{
    float x, y;
  
};
void(*dRsMouseSetPos)(V2d* pos) = NULL;
void RsMouseSetPos(V2d* pos)
{
    if (!(*MenuActive))
    {
        if (!show_window)
        {
            dRsMouseSetPos(pos);
        }
        
        //DisableMouse = false;
    }
	
}
//bool inin_imgui = false;
void InitImgui()
{
   
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        //io.MouseDrawCursor = true; // Enable Docking
        io.IniFilename = "CheatMenu.ini"; // 不生成配置文件
        io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        //ImGui::StyleColorsLight();

        // 设置窗口循环消息
       // oWndProc = (WNDPROC)SetWindowLong(*window, GWL_WNDPROC, (LONG)hkWndProc);
        ImGui_ImplWin32_Init(*window);
        ImGui_ImplDX9_Init(device);
       
    
   

}
void (*Closem3d)() = 0;
void CloseD3d()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    Closem3d();

}

//Hook个函数用来初始化
int(*dinitD3d)() = 0;
int InitD3D()
{
    int ret = 0;
    if (dinitD3d)
    {
        ret = dinitD3d();
        if (ret)
        {
            device = (IDirect3DDevice9*)*deviceaddr;

           // d3dGlobals = (D3d9Globals*)((DWORD)d3dGlobals->window);
            //initImgui
            InitImgui();
            
        }
    }
   
   
	return ret;
}

#include "CheatMenu.h"
void(*HudDraw)() = NULL;
void Render()
{
    if (HudDraw)
    {
        


            if (show_window)
            {
                DisableMouse = true;
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                ImGui_ImplDX9_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();

                //光标
                ImGui::GetForegroundDrawList()->AddCircleFilled(ImGui::GetIO().MousePos, 5,
                    ImColor(255, 0, 0, 255), 0);
                ImGui::GetForegroundDrawList()->AddLine(ImVec2(0, 0), ImGui::GetIO().MousePos,
                    ImColor(255, 0, 0, 255));
               
               // ImGui::ShowDemoWindow(&show_window);
                CheatMenu::Show();

                ImGui::EndFrame();
                ImGui::Render();
                ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            }
            else
            {
                DisableMouse = false;
            }
          
          
        
      
        CheatList::UpDate();
        HudDraw();
        Is_run = true;
    }
    
}

void initPacth(const char* ExeName)
{
    //简化类名
    using MU = MemUnits;
    if (!ExeName)
    {
        return;
    }
  

    DWORD exe = (DWORD)GetModuleHandleA(ExeName);
    std::vector <ULONGLONG> vResultContainer;
    //获取hwnd
    vResultContainer =  MU::ScanMemory(startD3D_Code, ExeName);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		//DWORD dwOffset = *(DWORD*)(dwAddress + 0x1);
       // DWORD offset = (DWORD)vResultContainer.at(0);
        DWORD d3dGlobals_addr = *(DWORD*)(dwAddress + 0x2);
        DWORD d3ddevice_addr = *(DWORD*)(dwAddress + 0x93);
        //DWORD d3ddevice_addr2 = (DWORD)(DWORD*)(d3ddevice_addr);
        window = (HWND*)(d3dGlobals_addr);
        deviceaddr = (DWORD*)(d3ddevice_addr);

	}
    //hook初始化函数
    vResultContainer =  MU::ScanMemory(DEVICEINIT_code, ExeName);
    if (vResultContainer.size() > 0)
    {
        DWORD dwAddress = vResultContainer[0];
        DWORD dwFuncAddr = (DWORD)(dwAddress + 0x9);
        dinitD3d = (int(*)())(MemUnits::Call_addr_To_Addr(dwFuncAddr));
        
        MemUnits::WriteCall(dwFuncAddr, (DWORD)&InitD3D, MemUnits::CODE_CALL);
    }

    //hook初始化函数reVc.rw::d3d::startD3D+218 - A3 482DAB00           - mov [reVc.rw::d3d::d3ddevice],eax
	//hookHUD绘制函数
    vResultContainer =  MU::ScanMemory(Render2D_Feature_Code, ExeName);
    if (vResultContainer.size() > 0)
    {
        DWORD dwAddress = vResultContainer[0];
        DWORD dwFuncAddr = (DWORD)(dwAddress);
        HudDraw = (void(*)())(MemUnits::Call_addr_To_Addr(dwFuncAddr));

        MemUnits::WriteCall(dwFuncAddr, (DWORD)&Render, MemUnits::CODE_CALL);
    }
    vResultContainer =  MU::ScanMemory(MouseUpdate_Feature_Code, ExeName);
    if (vResultContainer.size() > 0)
    {
        DWORD dwAddress = vResultContainer[0];
       // DWORD dwValAddr = (DWORD)(dwAddress+0x3);
        DWORD dwFuncAddr = (DWORD)(dwAddress + 0x1C);
        //HRESULT _InputGetMouseState(DIMOUSESTATE2* state)
        dInputGetMouseState = (HRESULT(*)(DIMOUSESTATE2*))(MemUnits::Call_addr_To_Addr(dwFuncAddr));

        MemUnits::WriteCall(dwFuncAddr, (DWORD)&_InputGetMouseState, MemUnits::CODE_CALL);
       // HudDraw = (void(*)())(MemUnits::Call_addr_To_Addr(dwFuncAddr));

       // MemUnits::WriteCall(dwFuncAddr, (DWORD)&Render, MemUnits::CODE_CALL);
    }
    //MenuCamera_Feature_Code
    vResultContainer =  MU::ScanMemory(MenuCamera_Feature_Code, ExeName);
    if (vResultContainer.size() > 0)
    {
        DWORD dwAddress = vResultContainer[0];
        DWORD dwValAddr = *(DWORD*)(dwAddress+0x2);
		MenuActive = (bool*)(dwValAddr);

        DWORD dwFuncAddr = (DWORD)(dwAddress + 0x54);
        //HRESULT _InputGetMouseState(DIMOUSESTATE2* state)
        dRsMouseSetPos = (void(*)(V2d*))(MemUnits::Call_addr_To_Addr(dwFuncAddr));

        MemUnits::WriteCall(dwFuncAddr, (DWORD)&RsMouseSetPos, MemUnits::CODE_CALL);
        // HudDraw = (void(*)())(MemUnits::Call_addr_To_Addr(dwFuncAddr));

        // MemUnits::WriteCall(dwFuncAddr, (DWORD)&Render, MemUnits::CODE_CALL);
    }
    vResultContainer =  MU::ScanMemory(WndProc_Code, ExeName);
    if (vResultContainer.size() > 0)
    {
        DWORD dwAddress = vResultContainer[0];
        DWORD dwFunAddr = (DWORD)(dwAddress + 0x3);
        DWORD dwValAddr = *(DWORD*)(dwAddress + 0x3);
        GameWndProc = (LRESULT(_stdcall *)(HWND,UINT,WPARAM,LPARAM))(dwValAddr);
		MemUnits::SetMemWriteEnable(dwFunAddr, 4);
		MemUnits::MemWrite<DWORD>(dwFunAddr, (DWORD)hkWndProc);

       
    }
    //解决崩溃
    vResultContainer =  MU::ScanMemory(CloseIm3d_Code, ExeName);
    if (vResultContainer.size() > 0)
    {
        DWORD dwAddress = vResultContainer[0];
        // DWORD dwValAddr = (DWORD)(dwAddress+0x3);
        DWORD dwFuncAddr = (DWORD)(dwAddress);
        //HRESULT _InputGetMouseState(DIMOUSESTATE2* state)
        Closem3d = (void(*)())(MemUnits::Call_addr_To_Addr(dwFuncAddr));

        MemUnits::WriteCall(dwFuncAddr, (DWORD)&CloseD3d, MemUnits::CODE_CALL);
     
    }
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        printf_s("初始化修改器\n");
        const char* ExeName = "reVC.exe";

        initPacth(ExeName);
		CheatList::InitPatch(ExeName);
        CheatMenu::InitMenu();
        printf_s("初始化完毕\n");
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

