#pragma once
#include <Windows.h>
#include <psapi.h>
//#include <string.h>
#include <string>
#include<sstream>
#include <vector>

class MemUnits
{
    static MODULEINFO GetCurrentModuleInfo(const char* moduleName);
public:
    //返回类型
#define RET(rettype) (*(rettype(__thiscall*)
//要调用的函数地址
//#define FUN_ADDRESS(Address) )((DWORD)ClassAddress))
//x86专用 调用类函数 参数1必须是类地址,后面的参数按顺序输入  THIS_CALL(rettype,funaddress,val1type,val2type,...)(class_address,val1,val2,...); 
#define THIS_CALL(return_type,fun_address,...)  RET(return_type)(DWORD,##__VA_ARGS__))(fun_address))

    // 改进后的宏定义
//#define RET(ret_type, ...) (*(ret_type(__thiscall*)(DWORD, ##__VA_ARGS__)) 
//#define THIS_CALL(ret_type, class_ptr, func_address, ...) \
    RET(ret_type, ##__VA_ARGS__)(func_address))( (DWORD)(class_ptr), ##__VA_ARGS__ )

    /*
(*(int(__thiscall*)(DWORD, const char*))((DWORD)Fun_65CA80))(
    (DWORD)Class_6,
    aRa2mdIni);*/

    typedef unsigned char byte;

    enum HCODE {
        CODE_CALL,
        CODE_JMP
    };

    //组装call文本 
    static std::string MakeCallStr(DWORD adderss)
    {
        //DWORD rel32 = func - (address + 5);
        std::ostringstream ss;
        ss << std::hex << adderss;
        std::string re = "E8";
        re.append( ss.str());
        return re;
    }
    static const byte ret = 0xC3;
    static const byte call = 0xE8;
    static const byte jmp = 0xE9;
    static const byte nop = 0x90;

    //c相对地址转绝对地址,参数是从e8 xx xx xx xx指令算的
    static DWORD Call_addr_To_Addr(DWORD address) {
        DWORD addr1 = *(DWORD*)(address + 0x1);
        DWORD next_addr = (address + 0x5);
        DWORD addr = next_addr + addr1;
        return addr;
    }

    //设置可写
    static void SetMemWriteEnable(DWORD address, SIZE_T size) {
        DWORD old;
        VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &old);
    }
    //设置只读
    static void SetMemWriteDisable(DWORD address, SIZE_T size) {
        DWORD old;
        VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READ, &old);
    }
    static void WriteCall(DWORD address, DWORD func, HCODE c)
    {
        DWORD old;
        //设置为可写
        //VirtualProtect((LPVOID)address, 5, PAGE_EXECUTE_READWRITE, &old);
        SetMemWriteEnable(address, 5);
        DWORD rel32 = func - (address + 5);
        if (c == HCODE::CODE_CALL)
            MemWrite<byte>(address, call);
        else MemWrite<byte>(address, jmp);

        MemWrite<DWORD>(address + 1, rel32);
        //恢复
        SetMemWriteDisable(address, 5);
       // VirtualProtect((LPVOID)address, 5, old, &old);
    }



    /// <summary>
    /// 读数据 通用
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="ptr">地址</param>
    /// <returns>对应数据</returns>
    template<class T>
    static T MemRead(uintptr_t ptr) {
        T buff{};
        if ((ptr > 0x1000000 - 0x8) && (ptr < 0x7FFFFFF00000 + 0x8))
        {
            if (IsBadReadPtr((void*)ptr, sizeof(T)) == 0) {
                buff = *reinterpret_cast<T*>(ptr);
            }
        }
        return buff;
    }
   
    /// <summary>
   /// 读数据 通用
   /// </summary>
   /// <typeparam name="T"></typeparam>
   /// <param name="ptr">地址</param>
   /// <returns>对应数据</returns>
    template<class T>
    static bool MemRead(uintptr_t ptr,UINT size,T* buff) {
        //T buff{0};
        bool ret = false;
        if ((ptr > 0x1000000 - 0x8) && (ptr < 0x7FFFFFF00000 + 0x8))
        {
            if (IsBadReadPtr((void*)ptr, size) == 0) {
                //buff = *reinterpret_cast<T*>(ptr);
                int err = memcpy_s(*buff, size, (void*)ptr, size);
                if (!err)
                    ret = true;
            }
        }
        return buff;
    }

    /// <summary>
    /// 写数据 通用
    /// </summary>
    /// <typeparam name="type">写入类型</typeparam>
    /// <param name="address">地址</param>
    /// <param name="val">值</param>
    /// <returns>是否修改成功</returns>
    template<class type>
    static bool MemWrite(uintptr_t address, const type& val) {
        if (reinterpret_cast<type*>(address) != nullptr)
        {
            *reinterpret_cast<type*>(address) = val;
            return true;
        }
        return false;
    }

    /// <summary>
    /// 搜索内存
    /// </summary>
    /// <param name="pattern">特征码</param>
    /// <param name="ModelName">模块名</param>
    /// <returns></returns>
    static std::vector<ULONGLONG> ScanMemory(const std::string& pattern, const char* ModelName);

};

