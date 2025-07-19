#pragma once

//#include "templates.h"
#include <common.h>
#include <vector>

//typedef void* RwTexDictionary;
//typedef void* RwStream;



class CCTxdStore
{
	//static CPool<TxdDef,TxdDef> *ms_pTxdPool;
	//static RwTexDictionary *ms_pStoredTxd;
public:
	// 首先定义条件部分
#ifdef _DEBUG
#define DEBUG_BREAK __debugbreak()
#else
#define DEBUG_BREAK MessageBox(NULL, L"该版本无法兼容", L"错误", MB_OK)
#endif

// 然后定义主宏
#define CHECK_ERROR(v) \
    do { \
        if ((v).size() == 0) { \
            DEBUG_BREAK; \
            printf_s("警告 ::该版本无法兼容\n"); \
            exit(0); \
        } \
        if ((v).size() > 1) { \
            printf_s("警告 ::该版本可能存在兼容性问题 Address:>%d\n", (int)(v).size()); \
        } \
    } while(0)
	/*
	static void CHECK_ERROR(std::vector <DWORD> &v)
	{
		if (v.size() == 0)
		{

#ifdef _DEBUG

			__debugbreak();
			
#else
			MessageBox(NULL, L"该版本无法兼容", L"错误", MB_OK);
#endif // DEBUG
			
			printf_s("警告 ::该版本无法兼容\n");
			
			exit(0);
		}
		if (v.size() > 1)
		{
			printf_s("警告 ::该版本可能存在兼容性问题 Address:>%d\n", v.size());
		}
	}*/
	static void InitPatch(int exe);
	//static void Initialise(void);
	//static void Shutdown(void);
	//static void GameShutdown(void);
	static int AddTxdSlot(const char *name);
	static void RemoveTxdSlot(int slot);
	static int FindTxdSlot(const char *name);
	static char *GetTxdName(int slot);
	static void PushCurrentTxd(void);
	static void PopCurrentTxd(void);
	static void SetCurrentTxd(int slot);
	//static void Create(int slot);
	//static int GetNumRefs(int slot);
	static void AddRef(int slot);
	static void RemoveRef(int slot);
	//static void RemoveRefWithoutDelete(int slot);
	//static bool LoadTxd(int slot, RwStream *stream);
	static bool LoadTxd(int slot, const char *filename);
	//static bool StartLoadTxd(int slot, RwStream *stream);
	//static bool FinishLoadTxd(int slot, RwStream *stream);
	static void RemoveTxd(int slot);

	//static TxdDef* GetSlot(int slot);
		
	//static bool isTxdLoaded(int slot);
};
