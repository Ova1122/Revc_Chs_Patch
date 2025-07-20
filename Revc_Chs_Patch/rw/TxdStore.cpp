#include<TxdStore.h>
#include <MemUtils.h>

//代码可精简
typedef  int(F_AddTxdSlot)(const char*);
F_AddTxdSlot* dAddTxdSlot = NULL;

typedef  void(F_RemoveTxd)(int);
F_RemoveTxd* dRemoveTxd = NULL;

typedef  void(F_RemoveTxdSlot)(int);
F_RemoveTxdSlot* dRemoveTxdSlot = NULL;
typedef void* (F_GetSlot)(int slot);//判断是否NULL解决崩溃
F_GetSlot* dGetSlot = NULL;

typedef  void(F_PushCurrentTxd)();
F_PushCurrentTxd* dPushCurrentTxd = NULL;

typedef  void(F_PopCurrentTxd)();
F_PopCurrentTxd* dPopCurrentTxd = NULL;

typedef  void(F_SetCurrentTxd)(int);
F_SetCurrentTxd* dSetCurrentTxd = NULL;

typedef  bool(F_LoadTxd)(int slot, const char* filename);
F_LoadTxd* dLoadTxd = NULL;

typedef  void(F_AddRef)(int);
F_AddRef* dAddRef = NULL;

const char AddTxdSlot_code[] = "55 8B EC 53 56 8B 35 ?? ?? ?? ?? 32 C9 57 8B 5E 08 8B 46 0C";
const char RemoveTxd_code[] = "55 8B EC 56 FF ?? ?? E8 ?? ?? ?? ?? 8B F0 83 C4 04 8B 06";
const char RemoveTxdSlot_code[] = "55 8B EC 56 57 FF ?? ?? E8 ?? ?? ?? ?? 8B F8 83 C4 04";
const char PushCurrentTxd_code[] = "E8 ?? ?? ?? ?? A3 ?? ?? ?? ?? C3 CC CC CC CC CC 55 8B EC 56";
const char PopCurrentTxd_code[] = "FF 35 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C3 CC CC CC CC CC CC CC E8";
const char SetCurrentTxd_code[] = "558BEC FF ?? ?? E8 ?? ?? ?? ?? FF ?? E8 ?? ?? ?? ?? 83 C4 08";
const char LoadTxd_code[] = "558BEC 53 56 57 6A 01 E8 ?? ?? ?? ?? 8B";
const char AddRef_code[] = "558BEC FF ?? ?? E8 ?? ?? ?? ?? 83 C4 04 FF ?? ?? 5D C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 558BEC 53";

//简化类名
using MU = MemUnits;
void CCTxdStore::InitPatch(int exe, const char* ExeName)
{
	//const char* ExeName = "reVC.exe";

	std::vector <ULONGLONG> vResultContainer1;

	vResultContainer1 = MU::ScanMemory(AddTxdSlot_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dAddTxdSlot =(F_AddTxdSlot*)vResultContainer1[0];
	
	

	vResultContainer1 = MU::ScanMemory(RemoveTxd_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dRemoveTxd = (F_RemoveTxd*)vResultContainer1[0];
	
	

	vResultContainer1 = MU::ScanMemory(RemoveTxdSlot_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dRemoveTxdSlot = (F_RemoveTxdSlot*)vResultContainer1[0];
	//dGetSlot = (F_GetSlot*)(vResultContainer1[0] + 0x9);
	
	

	vResultContainer1 = MU::ScanMemory(PushCurrentTxd_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dPushCurrentTxd = (F_PushCurrentTxd*)vResultContainer1[0];
	
	

	vResultContainer1 = MU::ScanMemory(PopCurrentTxd_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dPopCurrentTxd = (F_PopCurrentTxd*)vResultContainer1[0];
	
	

	vResultContainer1 = MU::ScanMemory(SetCurrentTxd_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dSetCurrentTxd = (F_SetCurrentTxd*)vResultContainer1[0];
	
	

	vResultContainer1 = MU::ScanMemory(LoadTxd_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dLoadTxd = (F_LoadTxd*)vResultContainer1[0];
	
	

	vResultContainer1 = MU::ScanMemory(AddRef_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dAddRef = (F_AddRef*)vResultContainer1[0];

	
	

	

}

int CCTxdStore::AddTxdSlot(const char* name)
{
	return dAddTxdSlot(name);
}

void CCTxdStore::RemoveTxdSlot(int slot)
{
	dRemoveTxdSlot(slot);
	/*
	if (dGetSlot(slot) != NULL)
	{
		
	}*/
	
}

void CCTxdStore::RemoveTxd(int slot)
{
	dRemoveTxd(slot);
}

void CCTxdStore::PushCurrentTxd(void)
{
	dPushCurrentTxd();
}

void CCTxdStore::PopCurrentTxd(void)
{
	dPopCurrentTxd();
}

void CCTxdStore::SetCurrentTxd(int slot)
{
	dSetCurrentTxd(slot);
}

bool CCTxdStore::LoadTxd(int slot, const char* filename)
{

	return dLoadTxd(slot, filename);
}

void CCTxdStore::AddRef(int slot)
{
	dAddRef(slot);
}
