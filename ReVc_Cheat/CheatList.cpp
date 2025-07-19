#include "CheatList.h"
#include <MemUtils.h>

#include <PlayerPed.h>
#include <vector>
#include <map>
#include<PlayerInfo.h>
#include <Automobile.h>
#include <Boat.h>
#include <Bike.h>
//CPlayerPed* playped = 0;
CPlayerInfo *playinfo = 0;
//CPlayerPed* (*dFindPlayerPed)();
CPlayerPed* FindPlayerPed()
{
	//playinfo = (CPlayerInfo*)dFindPlayerPed();
	return playinfo->m_pPed;
}

//简化类名
using MU = MemUnits ;


float xspeed = 1.0f;
float tmpspeed = 0.0f;
//float setfloat(float v);
// 全局变量
BYTE g_originalBytes[5];
DWORD g_targetAddress_sp = 0;  // cvtsd2ss 指令地址
DWORD g_jmpBackAddress_sp = 0;
const float g_zero = 0.0f;
// Hook 函数
void __declspec(naked) HookSpeed() {
	__asm {
		
		cvtsd2ss xmm4, xmm0
		movss   xmm0, [esp + 14h]

		push eax
		// 保存xmm5
		sub esp, 4
		movss[esp], xmm5

		// 2. 加载目标变量地址
		mov eax, offset tmpspeed
		// 3. 检查当前值是否为0
		movss xmm5, [eax]     // 加载当前值到xmm5
		comiss xmm5, [g_zero]   // 与0比较 (zero是下面定义的0常量)
		jne skip_save         // 如果不等于0则跳过保存
		// 4. 保存xmm4到目标变量
		movss[eax], xmm4

skip_save :

		// 加载乘数到xmm5
		movss xmm5, [xspeed]

			// 执行乘法
	    mulss xmm4, xmm5

				// 恢复xmm5
		movss xmm5, [esp]
		add esp, 4
		pop eax


		// 6. 跳回原程序
		jmp[g_jmpBackAddress_sp]
		
	}
}

const char FindPlayerPed_Camera_FeatureCode[] =
"E8 ?? ?? ?? FF 0F B6 88 04 05 00 00 83 C1 2B 8D 0C 49 8B 04 C8 83 F8 1C 74 10 83 F8 1D 74 0B 83 F8 1E 75 14 6A 00"                 // test al, al
;                
const char Speed_Code[] =
"F2 0F 5A E0 F3 0F 10 44 24 14 F3 0F 5C 86 74 03 00 00 F3 0F 11 64 24 10 0F 2F C8 76 12 F3 0F 10 05 ?? ?? ?? ?? F3 0F 10 1D ?? ?? ?? ?? EB 44 F3";

const char Jump_Code[] =
"F3 0F 10 05 ?? ?? ?? ?? 83 EC 0C 8B CE F3 0F 11 44 24 08 C7 44 24 04 ?? ?? ?? ?? C7 04 24 ?? ?? ?? ?? E8 ?? ?? ?? ?? F3 0F 10 56 74 F3 0F 10 4E";
char Vehicle_Code[] =
"55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 20 A1 ?? ?? ?? ?? 33 C5 89 45 F0 53 56 57 50 8D 45 F4 64 A3 00 00 00 00 8B 75 08 B9";

const char WantedLevel_Code[] =
"55 8B EC 56 FF 75 08 8B F1 E8 ?? ?? ?? ?? 8B CE E8 ?? ?? ?? ?? 5E 5D C2 04 00";

char GiveWeapon_Code[] =
"55 8B EC 83 EC 0C 53 56 57 FF 75 08 8B F1 E8 ?? ?? ?? ?? 8B 55 08 83 C4 04 8B ?? 60 0F B6 ?? 89 ?? F4 8D 04 ?? 39 94 ?? 08 04 00 00 75 76";
//char RequestModel_Code[] =
//"55 8B EC 8B 4D 0C 56 8B 75 08 57 8D 3C B6 8A 04 BD ?? ?? ?? 00 3C 02 75 ?? F6 C1 08 74 ?? 8A 04 BD ?? ?? ?? 00 A8 08 75 ?? FF 05 ?? ?? ?? 00 0C 08 88 04 BD ?? ?? ?? 00";
//char LoadAllRequestedModels_Code[] =
//"55 8B EC A1 ?? ?? ?? 00 83 EC 10 80 3D ?? ?? ?? 00 00 53 8D 1C 85 00 00 00 00 0F 85 ?? ?? 00 00 80 7D 08 00 C6 05 ?? ?? ?? 00 01 0F 45 1D ?? ?? ?? 00 E8 ?? ?? ?? FF";
char SetModelIsDeletable_Code[] =
"55 8B EC 8B 4D 08 53 8D 14 89 8A 1C 95 ?? ?? ?? ?? 80 E3 FE 88 1C 95 ?? ?? ?? ?? 81 F9 64 19 00 00 7C 08 81 F9 CD 1E 00 00 7C 0D 8B 04 8D ?? ?? ?? ?? 80 78 19 06 74 ?? F6 C3 02 75 ?? 80 3C 95 ?? ?? ?? ?? 01 74 ?? 5B 89 4D 08 5D E9";

char Ammo_Code[] =
"48 89 46 08 8B 46 0C 85 C0 7E ?? 3D A8 61 00 00 7C ?? 84 C9 74 ?? EB ?? 84 C9 74 ?? ";


char CBoat_code[] =
"53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 53 83 EC 38 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 8B F1 89 75 E8";

//CCarCtrl::GenerateOneRandomCar
char GenerateOneRandomCar_code[] =
"53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 53 81 EC 58 01 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 EC 56 57 50 8D 45 F4 64 A3 00 00 00 00 0F B6 05 ?? ?? ?? ?? 69 F0 74 01 00 00 50 C6 85 CF FE FF FF 00 81 C6 ?? ?? ?? ?? E8 ?? ?? ?? ?? F3 0F 7E 00 66 0F D6 45 E0 8B 40 08 89 45 E8 E8 ?? ?? ?? ?? F3 0F 10 00 F3 0F 11 85 9C FE FF FF F3 0F 10 40 04 8D 85 F8 FE FF FF";
//CStreaming函数
void (*RequestModel)(int ,int);
void (*LoadAllRequestedModels)(bool);
void (*SetModelIsDeletable)(int);
void (*SetModelTxdIsDeletable)(int32 id) = NULL;//在车辆作弊里面获取

DWORD dGiveWeapon = 0;
int32 GiveWeapon(eWeaponType weaponType, uint32 ammo, bool unused)
{
	CPlayerPed* player = FindPlayerPed();
	int slot = -1;
	if (player)
	{
		if (dGiveWeapon)
		{
			slot = THIS_CALL(int, dGiveWeapon, eWeaponType, uint32, bool)((DWORD)player, weaponType, ammo, unused);
			player->m_nSelectedWepSlot = slot;
			
		}
		//player->m_nWantedLevel = level;

	}
	return 0;
}

DWORD dWantedLevel = 0;
void WantedLevel(int level)
{
	CPlayerPed* player = FindPlayerPed();
	if (player)
	{
		if (dWantedLevel)
		{
			THIS_CALL(void, dWantedLevel, int)((DWORD)player->m_pWanted, level);
		}
		//player->m_nWantedLevel = level;
		
	}
}

//刷车相关
void (*VehicleCheat)(int model);
class CStreamingInfo
{
public:
	CStreamingInfo* m_next;
	CStreamingInfo* m_prev;
	uint8 m_loadState;
	uint8 m_flags;

	int16  m_nextID;
	uint32 m_position;
	uint32 m_size;

}*ms_aInfoForModel = NULL;
void (*CWorld_Add)(CEntity* ent) = NULL;
DWORD DCAutomobile_callAddress = 0;
DWORD DCBike_callAddress = 0;
DWORD DCBoat_callAddress = 0;


//车辆new
void* (*CVehicle_operator_new)(size_t size);//重写的new方法
template <class T>
T* NewCVehicle(int32 id, uint8 CreatedBy)
{
	T* ret = NULL;
	if (DCAutomobile_callAddress)
	{
		ret = (T*)CVehicle_operator_new(sizeof(T));
		//构造函数调用
		THIS_CALL(void, DCAutomobile_callAddress, int32, uint8)((DWORD)ret, id, CreatedBy);
	}
	return ret;
}
//创建船只
CBoat* NewCBoat(int32 id, uint8 CreatedBy)
{

	CBoat* ret = NULL;
	if (DCBoat_callAddress)
	{
		ret = (CBoat*)CVehicle_operator_new(sizeof(CBoat));
		//构造函数调用
		THIS_CALL(void, DCBoat_callAddress, int32, uint8)((DWORD)ret, id, CreatedBy);
	}
	return ret;
}
//创建摩托
CBike* NewCBike(int32 id, uint8 CreatedBy)
{
	CBike* ret = NULL;
	if (DCBike_callAddress)
	{
		ret = (CBike*)CVehicle_operator_new(sizeof(CBike));
		//构造函数调用
		THIS_CALL(void, DCBike_callAddress, int32, uint8)((DWORD)ret, id, CreatedBy);
	}
	return ret;
}
//创建车辆
CAutomobile* NewCAutomobile(int32 id, uint8 CreatedBy)
{
	CAutomobile* ret = NULL;
	if (DCAutomobile_callAddress)
	{
		ret = (CAutomobile*)CVehicle_operator_new(sizeof(CAutomobile));
		//构造函数调用
		THIS_CALL(void, DCAutomobile_callAddress, int32, uint8)((DWORD)ret, id, CreatedBy);
	}
	return ret;
	//return NewCVehicle<CAutomobile>(id, CreatedBy);
}



struct CheatEvent
{
    bool run = false;
    bool IsLoop = false;
    CHEAT_TYPE type;
	void* data = nullptr;
};
std::vector <CheatEvent> EventList;
std::map <CHEAT_TYPE,CheatEvent> Eventmap;

float* jumpAddr = 0;

int AmmoTotalAddr = 0;//总弹药
int AmmoInClipAddr = 0;//弹夹

void CheatList::InitPatch()
{
	const char* ExeName = "reVC.exe";
   // DWORD exe = (DWORD)GetModuleHandleA("reVC.exe");
    std::vector <ULONGLONG> vResultContainer;
    //获取CPlayerPed
    vResultContainer = MU::ScanMemory(FindPlayerPed_Camera_FeatureCode, ExeName);
    if (vResultContainer.size() > 0)
    {
        DWORD dwAddress = vResultContainer[0];
        DWORD dwFuncAddr = (DWORD)(dwAddress);
		DWORD dwFuncAddr2 = (MU::Call_addr_To_Addr(dwFuncAddr)+0xf);
		DWORD valoffset = *(DWORD*)(dwFuncAddr2);
		playinfo = (CPlayerInfo*)valoffset;
       // dFindPlayerPed = (CPlayerPed*(*)())(MU::Call_addr_To_Addr(dwFuncAddr));

    }
	vResultContainer = MU::ScanMemory(Speed_Code, ExeName);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwFuncAddr = (DWORD)(dwAddress);
		g_targetAddress_sp = dwFuncAddr;
		g_jmpBackAddress_sp = g_targetAddress_sp + 5;
		MU::SetMemWriteEnable(dwFuncAddr, 10);
		memset((void*)dwFuncAddr,MU::nop,10);
		MU::WriteCall(dwFuncAddr, (DWORD)HookSpeed, MU::HCODE::CODE_JMP);
		
		// dFindPlayerPed = (CPlayerPed*(*)())(MU::Call_addr_To_Addr(dwFuncAddr));

	}
	vResultContainer = MU::ScanMemory(Jump_Code, ExeName);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwValAddr = *(DWORD*)(dwAddress+0x4);
		jumpAddr = (float*)(dwValAddr);
		MU::SetMemWriteEnable((DWORD)jumpAddr, 4);
		
		//playinfo = (CPlayerInfo*)valoffset;
		// dFindPlayerPed = (CPlayerPed*(*)())(MU::Call_addr_To_Addr(dwFuncAddr));

	}
	vResultContainer = MU::ScanMemory(Vehicle_Code, ExeName);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwFuncAddr = (DWORD)(dwAddress);
		VehicleCheat = (void(*)(int))(dwFuncAddr);
		DWORD Address = dwFuncAddr + 0x4A;
		DWORD Address_Sub = MU::Call_addr_To_Addr(Address);
		RequestModel = (void(*)(int, int)) Address_Sub;
		Address = Address + 0x7;
		Address_Sub = MU::Call_addr_To_Addr(Address);
		LoadAllRequestedModels = (void(*)(bool))Address_Sub;

        
		Address= dwFuncAddr + 0x89;
		Address_Sub = *(DWORD*)Address;
		ms_aInfoForModel = (CStreamingInfo*)(Address_Sub-8);
		Address = Address + 0xE;
		Address_Sub = MU::Call_addr_To_Addr(Address);
		SetModelTxdIsDeletable = (void(*)(int32))Address_Sub;

		//重写的new方法
		DWORD newAddr = Address + 0x52;
		DWORD newAddr_Sub= MU::Call_addr_To_Addr(newAddr);
		CVehicle_operator_new= (void*(*)(size_t))newAddr_Sub;


		Address = Address + 0x6D;
		Address_Sub = MU::Call_addr_To_Addr(Address);
		DCAutomobile_callAddress=Address_Sub; // reinterpret_cast<void(*)(CEntity*)>(dwFuncAddr + 0x5F);
		Address = Address + 0xEE;
		Address_Sub = MU::Call_addr_To_Addr(Address);
		CWorld_Add = (void(*)(CEntity*))Address_Sub;
		

		//playinfo = (CPlayerInfo*)valoffset;
		// dFindPlayerPed = (CPlayerPed*(*)())(MU::Call_addr_To_Addr(dwFuncAddr));

	}
	vResultContainer = MU::ScanMemory(WantedLevel_Code, ExeName);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwFuncAddr = (DWORD)(dwAddress);
		//VehicleCheat = (void(*)(int))(dwFuncAddr);
		dWantedLevel = dwFuncAddr;
		

	}
	vResultContainer = MU::ScanMemory(GiveWeapon_Code, ExeName);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwFuncAddr = (DWORD)(dwAddress);
		//VehicleCheat = (void(*)(int))(dwFuncAddr);
		dGiveWeapon = dwFuncAddr;
	}
	/*
	vResultContainer = MU::ScanMemory(1, RequestModel_Code, exe, 0x7ffffff);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwFuncAddr = (DWORD)(dwAddress);
		//VehicleCheat = (void(*)(int))(dwFuncAddr);
		RequestModel = (void(*)(int,int)) dwFuncAddr;
	}
	vResultContainer = MU::ScanMemory(1, LoadAllRequestedModels_Code, exe, 0x7ffffff);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwFuncAddr = (DWORD)(dwAddress);
		//VehicleCheat = (void(*)(int))(dwFuncAddr);
		LoadAllRequestedModels = (void(*)(bool))dwFuncAddr;
	}*/
	vResultContainer = MU::ScanMemory(SetModelIsDeletable_Code, ExeName);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwFuncAddr = (DWORD)(dwAddress);
		//VehicleCheat = (void(*)(int))(dwFuncAddr);
		SetModelIsDeletable = (void(*)(int))dwFuncAddr;
	}

	vResultContainer = MU::ScanMemory(Ammo_Code, ExeName);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwFuncAddr = (DWORD)(dwAddress);
		AmmoInClipAddr = dwFuncAddr;
		AmmoTotalAddr = (dwFuncAddr + 0x39);
		
		// dFindPlayerPed = (CPlayerPed*(*)())(MU::Call_addr_To_Addr(dwFuncAddr));
		
	}

	//摩托和船的构造函数地址 也可以在这里获取车辆的
	vResultContainer = MU::ScanMemory(GenerateOneRandomCar_code, ExeName);
	if (vResultContainer.size() > 0)
	{
		DWORD dwAddress = vResultContainer[0];
		DWORD dwFuncAddr = (DWORD)(dwAddress+0x829);
		//也可以在这里获取车辆类的构造函数
		DCBoat_callAddress = MU::Call_addr_To_Addr(dwFuncAddr);
		DCBike_callAddress = MU::Call_addr_To_Addr(dwFuncAddr + 0x37);
		//AmmoTotalAddr = (dwFuncAddr + 0x39);

		// dFindPlayerPed = (CPlayerPed*(*)())(MU::Call_addr_To_Addr(dwFuncAddr));

	}

}

float setfloat(float v)
{
	// 这里可以对传入的值进行修改
	return v*xspeed; // 设置为100.0f
	// 也可以根据需要进行其他操作
}


//刷车
void PushVehicle(int model)
{
	enum StreamFlags
	{
		STREAMFLAGS_DONT_REMOVE = 0x01,
		STREAMFLAGS_SCRIPTOWNED = 0x02,
		STREAMFLAGS_DEPENDENCY = 0x04,	// Is this right?
		STREAMFLAGS_PRIORITY = 0x08,
		STREAMFLAGS_NOFADE = 0x10,
		STREAMFLAGS_20 = 0x20,	// TODO(MIAMI): what's this

		STREAMFLAGS_CANT_REMOVE = STREAMFLAGS_DONT_REMOVE | STREAMFLAGS_SCRIPTOWNED,
		STREAMFLAGS_KEEP_IN_MEMORY = STREAMFLAGS_DONT_REMOVE | STREAMFLAGS_SCRIPTOWNED | STREAMFLAGS_DEPENDENCY,
	};
	enum StreamLoadState
	{
		STREAMSTATE_NOTLOADED = 0,
		STREAMSTATE_LOADED = 1,
		STREAMSTATE_INQUEUE = 2,
		STREAMSTATE_READING = 3,	// channel is reading
		STREAMSTATE_STARTED = 4,	// first part loaded
	};

	RequestModel(model, STREAMFLAGS_DONT_REMOVE);
	LoadAllRequestedModels(false);
	//DWORD addr = (DWORD)ms_aInfoForModel;
	//CStreamingInfo* InfoForModel = (CStreamingInfo*)((addr * sizeof(CStreamingInfo)) * model);
	CStreamingInfo* InfoForModel = ms_aInfoForModel + model;
	//CStreamingInfo* InfoForModel2 = &ms_aInfoForModel[model];
	//CStreamingInfo InfoForModel3 = ms_aInfoForModel[model];
	//if (ms_aInfoForModel[model].m_loadState == STREAMSTATE_LOADED)
	if (InfoForModel->m_loadState == STREAMSTATE_LOADED)
	{
		if (!(ms_aInfoForModel[model].m_loadState & STREAMFLAGS_DONT_REMOVE)) {
			SetModelIsDeletable(model);
			SetModelTxdIsDeletable(model);
		}

		//SetModelIsDeletable(model);
		//SetModelTxdIsDeletable(model);
		CEntity* ent = NULL;

		//坐标
		CVector playerPos = FindPlayerPed()->GetPosition();
		CVector forward = FindPlayerPed()->GetForward(); // 单位向量
		float distance = 5.0f;
		CVector spawnPos = playerPos + forward * distance;
		spawnPos.z += 3.0f; // 适当抬高，避免卡地面
		

	
		//判断车辆类型
		//eVehicleType vt;
		switch (model)
		{
		case MI_PREDATOR:
		case MI_SQUALO:
		case MI_SPEEDER:
		case MI_TROPIC:
		case MI_COASTG:
		case MI_DINGHY:
		case MI_MARQUIS:
		case MI_JETMAX:
		case MI_REEFER:
		case MI_RIO:
		{
			//vt = VEHICLE_TYPE_BOAT;
			CBoat *boat = NewCBoat(model, RANDOM_VEHICLE);
			boat->SetOrientation(0.0f, 0.0f, DEGTORAD(200.0f));
			boat->SetPosition(spawnPos);
			boat->SetStatus(STATUS_ABANDONED);
			//FindPlayerPed()->SetPosition(spawnPos);
			boat->m_nDoorLock = CARLOCK_UNLOCKED;
			//vehicle->bEngineOn = 1;
			//boat->bIsHeavy = true;

			//boat->m_vehType = vt;
			ent = boat;
		}
			break;
		//case MI_CHOPPER:
		case MI_ANGEL:
		case MI_PCJ600:
		case MI_FAGGIO:
		case MI_FREEWAY:
		case MI_SANCHEZ:
		case MI_PIZZABOY:
		{
			//vt = VEHICLE_TYPE_BIKE;
			//vt = VEHICLE_TYPE_CAR;
			CBike* bike = NewCBike(model, RANDOM_VEHICLE);
			bike->SetOrientation(0.0f, 0.0f, DEGTORAD(200.0f));
			bike->SetPosition(spawnPos);
			bike->SetStatus(STATUS_ABANDONED);
			//FindPlayerPed()->SetPosition(spawnPos);
			//bike->m_nDoorLock = CARLOCK_UNLOCKED;
			//vehicle->bEngineOn = 1;
			//bike->bIsHeavy = true;

			//bike->m_vehType = vt;
			ent = bike;
			}
			break;
		default:
			{
				///vt = VEHICLE_TYPE_CAR;
				CAutomobile* vehicle = NewCAutomobile(model, RANDOM_VEHICLE);
				vehicle->SetOrientation(0.0f, 0.0f, DEGTORAD(200.0f));
				vehicle->SetPosition(spawnPos);
				vehicle->SetStatus(STATUS_ABANDONED);
				//FindPlayerPed()->SetPosition(spawnPos);
				vehicle->m_nDoorLock = CARLOCK_UNLOCKED;
				//vehicle->bEngineOn = 1;
				vehicle->bIsHeavy = true;

				//vehicle->m_vehType = vt;
				ent = vehicle;
			}
			break;
		}

		if (ent != nil) {
			//添加到世界
			CWorld_Add(ent);
			
		}
	}

}

void SetCarState(CarProof &cf, CVehicle &v)
{
	if (cf.LockedDoor)
	{
		v.m_nDoorLock = CARLOCK_LOCKED;
	}
	else
	{
		v.m_nDoorLock = CARLOCK_UNLOCKED;
	}
	v.bBulletProof = cf.bulletProof? 1 : 0;
	v.bFireProof = cf.bFireProof;
	v.bCollisionProof = cf.bCollisionProof ? 1 : 0;
	v.bMeleeProof = cf.bMeleeProof ? 1 : 0;
	v.bExplosionProof = cf.bExplosionProof ? 1 : 0;
}

#define MONEY_MAX 99999999
//CWeather::ForceWeatherNow(WEATHER_RAINY);天气
//这个函数可以简化
CarProof defcpf;
void CheatList::UpDate()
{
	CPlayerPed* player = FindPlayerPed();
	for (auto& event: Eventmap)
	{
		
		CheatEvent *eve = &event.second;
		switch (eve->type)
		{
		case CHEAT_TYPE::HEALTH:
			if (!eve->run)break;
			
			if (playinfo)
			{

				if (player)
				{
					if (eve->IsLoop)
					{
						if (eve->data)
						{
							//float hp = *(float*)EventList.at(i).data;
							//hp > 255 ? hp = 255 : hp;
							player->m_fHealth < 0 ? player->m_fHealth = 100 : player->m_fHealth;
							//player->m_fHealth = hp;
						}
						else
						{
							float hp = 255;
							hp > 255 ? hp = 255 : hp;
							hp < 0 ? hp = 0 : hp;
							player->m_fHealth = hp;
							
						}
						
					}
					else
					{
						if (eve->data)
						{
							float hp = *(float*)eve->data;
							hp > 255 ? hp = 255 : hp;
							hp < 0 ? hp = 0 : hp;
							player->m_fHealth = hp;

							
						}
						eve->run = false;

					}
				}
			}

			break;
		case CHEAT_TYPE::MONEY:
			if (!eve->run)break;
			if (playinfo)
			{
				CPlayerPed* player = FindPlayerPed();
				if (player)
				{
					if (eve->IsLoop)
					{
						if (eve->data)
						{
							int m = *(int*)eve->data;
							m > MONEY_MAX ? m = MONEY_MAX : m;
							m < 0 ? m = 0 : m;
							playinfo->m_nMoney = m;

						}
						else
						{

							playinfo->m_nMoney = MONEY_MAX;
						}
					
					}
					else
					{
						if (eve->data)
						{
							int m = *(int*)eve->data;
							m += playinfo->m_nMoney;
							m > MONEY_MAX ? m = MONEY_MAX : m;
							m < 0 ? m = 0 : m;
							playinfo->m_nMoney = m;
						}
						eve->run = false;

					}
				}
			}
			break;
			
		case CHEAT_TYPE::GODMODE:
			if (!eve->run)break;
			if (playinfo)
			{
				//CPlayerPed* player = FindPlayerPed();
				if (player)
				{

					if (eve->data)
					{
						if (eve->IsLoop)
						{
							bool god = *(bool*)eve->data;
							player->m_bCanBeDamaged = !god;
							
						}
						else
						{

							bool god = *(bool*)eve->data;
							player->m_bCanBeDamaged = !god;
							eve->run = false;
						}


					}
					

				}
			}
			break;
			
		case CHEAT_TYPE::PLAYER_SPEED:
			if (!eve->run)break;
			if (playinfo)
			{
				CPlayerPed* player = FindPlayerPed();
				if (player)
				{

					if (eve->data)
					{
						float speed = *(float*)eve->data;

						xspeed = speed;
					}
					eve->run = false;

				}
			}
			break;
		case CHEAT_TYPE::PlAYER_JUMP:
			if (!eve->run)break;
			if (playinfo)
			{
				CPlayerPed* player = FindPlayerPed();
				if (player)
				{

					if (eve->data)
					{
						float jumpb = *(float*)eve->data;
						//float val = *jumpAddr;
						if (jumpb <= 0)
						{
							*jumpAddr = 8.5f;
						}
						else
							*jumpAddr = 8.5f * jumpb;
					}
					eve->run = false;

				}
			}
			break;
		case CHEAT_TYPE::VEHICLE:
			if (!eve->run)break;
			if (playinfo)
			{
				CPlayerPed* player = FindPlayerPed();
				if (player)
				{

					if (eve->data)
					{
						int v = *(int*)eve->data;
						//float val = *jumpAddr;
						//车辆id从130开始
						//VehicleCheat(130+v);
						//int id = MI_LANDSTAL + v;
						int id = v;
						

						PushVehicle(id);
						
					}
					eve->run = false;

				}
			}
			break;
		case CHEAT_TYPE::WANTED_LEVEL:
			if (!eve->run)break;
			if (playinfo)
			{
				CPlayerPed* player = FindPlayerPed();
				if (player)
				{

					if (eve->data)
					{
						int v = *(int*)eve->data;
						if (eve->IsLoop)
						{
							WantedLevel(v);
						}
						else
						{
							WantedLevel(v);
							eve->run = false;
						}
						
					}
					

				}
			}
			break;
		case CHEAT_TYPE::GIVE_WEAPON:
			if (!eve->run)break;
			if (playinfo)
			{
				CPlayerPed* player = FindPlayerPed();
				if (player)
				{

					if (eve->data)
					{
						WeaopnStruct v = *(WeaopnStruct*)eve->data;
						RequestModel(v.mtype, 1);
						if (v.mtype2)
						{
							RequestModel(v.mtype2, 1);
						}
						LoadAllRequestedModels(false);
						GiveWeapon((eWeaponType)(v.wtype), v.ammo, v.unused);

						SetModelIsDeletable(v.mtype);
						if (v.mtype2)
						{
							SetModelIsDeletable(v.mtype2);
						}
						eve->run = false;

					}
					

				}
			}
			break;
		case CHEAT_TYPE::AMMO:
			if (!eve->run)break;
			if (playinfo)
			{
				CPlayerPed* player = FindPlayerPed();
				if (player)
				{

					if (eve->data)
					{
						bool ammo = *(bool*)eve->data;
						if (ammo)
						{
							MU::SetMemWriteEnable(AmmoInClipAddr, 1);
							MU::MemWrite<byte>(AmmoInClipAddr, MU::nop);

							MU::SetMemWriteEnable(AmmoTotalAddr, 3);
							MU::MemWrite<byte>(AmmoTotalAddr, MU::nop);
							MU::MemWrite<byte>(AmmoTotalAddr + 1, MU::nop);
							MU::MemWrite<byte>(AmmoTotalAddr + 2, MU::nop);
						}
						else
						{
							MU::SetMemWriteEnable(AmmoInClipAddr, 1);
							MU::MemWrite<byte>(AmmoInClipAddr, 0x48);

							MU::SetMemWriteEnable(AmmoTotalAddr, 3);
							MU::MemWrite<byte>(AmmoTotalAddr, 0xFF);
							MU::MemWrite<byte>(AmmoTotalAddr + 1, 0x4E);
							MU::MemWrite<byte>(AmmoTotalAddr + 2, 0x0C);
						}
						eve->run = false;

					}

				}
			}
			break;
		case CHEAT_TYPE::ARMOUR:
			if (!eve->run)break;
			if (playinfo)
			{
				CPlayerPed* player = FindPlayerPed();
				if (player)
				{
					if (eve->data)
					{
						float armour = *(float*)eve->data;
						armour > 255 ? armour = 255 : armour;
						armour < 0 ? armour = 0 : armour;
						player->m_fArmour = armour;
					}
					else
					{
						player->m_fArmour = playinfo->m_nMaxArmour;
					}
					eve->run = false;

					

				}
			}
			break;
		case CHEAT_TYPE::VehicleProof:
			if (!eve->run)break;
			if (playinfo)
			{
				//CPlayerPed* player = FindPlayerPed();
				

				if (player)
				{
					
					if (eve->data)
					{
						CarProof crf = *(CarProof*)eve->data;
						if (eve->IsLoop)
						{
							CVehicle* mycar = player->m_pMyVehicle;
							if (mycar)
							{
								if (!(player->m_nPedState == PED_DRIVING || player->m_nPedState == PED_PASSENGER)) {
									// 玩家不在车内（驾驶或乘客）
									
									SetCarState(defcpf, *mycar);
									mycar = NULL;
								}
								else
								{
									SetCarState(crf, *mycar);
								}
							}
							
						}
						
						
					}
					//eve->run = false;
					



				}
			}
			break;
		default:
			break;
		}

	}



    
}

void CheatList::AddCheat(CHEAT_TYPE type, bool isloop, void* data)
{
	CheatEvent event;
	event.type = type;
	event.IsLoop = isloop;
	event.data = data;
	event.run = true;
	Eventmap[type] = event;
	
}

void CheatList::RemoveCheat(CHEAT_TYPE type)
{
	/*
	for (int i = 0; i < EventList.size(); i++)
	{
		if (EventList[i].type == type)
		{
			EventList.erase(EventList.begin() + i);
			break;
		}
	}*/
}
//这个感觉不用也行,懒得改
void* CheatList::GetDataAddress(GET_VALUE type)
{
	if (playinfo)
	{
		CPlayerPed* player = FindPlayerPed();
		if (player)
		{
			switch (type)
			{		
			case VALUE_NONE:
				break;
			case CurrentStamina:
				return &player->m_fCurrentStamina;
				break;
			case MaxStamina:		
				return &player->m_fMaxStamina;
				break;
			case MoveSpeed:
				return &player->m_fMoveSpeed;
				break;
			case PlayerInfo:
				return player;
			case VehicleInfo:
				break;
			default:
				break;
			}
		}
	}
	return nullptr;
}
