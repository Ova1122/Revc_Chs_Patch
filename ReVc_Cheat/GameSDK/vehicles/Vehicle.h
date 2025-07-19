#pragma once

#include "Physical.h"
//#include "AutoPilot.h"
//#include "ModelIndices.h"
//#include "AnimationId.h"
//#include "WeaponType.h"
//#include "Collision.h"
//#include "HandlingMgr.h"

class CPed;
class CPlayerPed;
class CCopPed;
class CFire;

enum {
	RANDOM_VEHICLE = 1,
	MISSION_VEHICLE = 2,
	PARKED_VEHICLE = 3,
	PERMANENT_VEHICLE = 4,
};

enum eCarNodes
{
	CAR_WHEEL_RF = 1,
	CAR_WHEEL_RM,
	CAR_WHEEL_RB,
	CAR_WHEEL_LF,
	CAR_WHEEL_LM,
	CAR_WHEEL_LB,
	CAR_BUMP_FRONT,
	CAR_BUMP_REAR,
	CAR_WING_RF,
	CAR_WING_RR,
	CAR_DOOR_RF,
	CAR_DOOR_RR,
	CAR_WING_LF,
	CAR_WING_LR,
	CAR_DOOR_LF,
	CAR_DOOR_LR,
	CAR_BONNET,
	CAR_BOOT,
	CAR_WINDSCREEN,
	NUM_CAR_NODES,
};

enum {
	CAR_DOOR_FLAG_UNKNOWN = 0x0,
	CAR_DOOR_FLAG_LF = 0x1,
	CAR_DOOR_FLAG_LR = 0x2,
	CAR_DOOR_FLAG_RF = 0x4,
	CAR_DOOR_FLAG_RR = 0x8
};

enum eCarLock {
	CARLOCK_NOT_USED,				 //CARLOCK_未使用,
	CARLOCK_UNLOCKED,				 //CARLOCK_解锁,
	CARLOCK_LOCKED,					 //CARLOCK_锁定,
	CARLOCK_LOCKOUT_PLAYER_ONLY,	 //CARLOCK_锁定（仅限玩家）,
	CARLOCK_LOCKED_PLAYER_INSIDE,	 //CARLOCK_锁定（玩家在内）,
	CARLOCK_LOCKED_INITIALLY,		 //CARLOCK_初始锁定,
	CARLOCK_FORCE_SHUT_DOORS,		 //CARLOCK_强制关闭车门,
	CARLOCK_LOCKED_BUT_CAN_BE_DAMAGED//CARLOCK_锁定但可被杀死的驾驶员?
};

enum eBombType
{
	CARBOMB_NONE,
	CARBOMB_TIMED,
	CARBOMB_ONIGNITION,
	CARBOMB_REMOTE,
	CARBOMB_TIMEDACTIVE,
	CARBOMB_ONIGNITIONACTIVE,
};

enum eDoors
{
	DOOR_BONNET = 0,
	DOOR_BOOT,
	DOOR_FRONT_LEFT,
	DOOR_FRONT_RIGHT,
	DOOR_REAR_LEFT,
	DOOR_REAR_RIGHT
};

enum ePanels
{
	VEHPANEL_FRONT_LEFT,
	VEHPANEL_FRONT_RIGHT,
	VEHPANEL_REAR_LEFT,
	VEHPANEL_REAR_RIGHT,
	VEHPANEL_WINDSCREEN,
	VEHBUMPER_FRONT,
	VEHBUMPER_REAR,
};

enum eLights
{
	VEHLIGHT_FRONT_LEFT,
	VEHLIGHT_FRONT_RIGHT,
	VEHLIGHT_REAR_LEFT,
	VEHLIGHT_REAR_RIGHT,
};

enum
{
	CAR_PIECE_BONNET = 1,
	CAR_PIECE_BOOT,
	CAR_PIECE_BUMP_FRONT,
	CAR_PIECE_BUMP_REAR,
	CAR_PIECE_DOOR_LF,
	CAR_PIECE_DOOR_RF,
	CAR_PIECE_DOOR_LR,
	CAR_PIECE_DOOR_RR,
	CAR_PIECE_WING_LF,
	CAR_PIECE_WING_RF,
	CAR_PIECE_WING_LR,
	CAR_PIECE_WING_RR,
	CAR_PIECE_WHEEL_LF,
	CAR_PIECE_WHEEL_RF,
	CAR_PIECE_WHEEL_LR,
	CAR_PIECE_WHEEL_RR,
	CAR_PIECE_WINDSCREEN,
};

enum tWheelState
{
	WHEEL_STATE_NORMAL,	// standing still or rolling normally
	WHEEL_STATE_SPINNING,	// rotating but not moving
	WHEEL_STATE_SKIDDING,
	WHEEL_STATE_FIXED,	// not rotating
};

enum eFlightModel
{
	FLIGHT_MODEL_DODO,
	FLIGHT_MODEL_RCPLANE,
	FLIGHT_MODEL_RCHELI,
	FLIGHT_MODEL_SEAPLANE,
	FLIGHT_MODEL_PLANE_UNUSED,
	FLIGHT_MODEL_PLANE,
	FLIGHT_MODEL_HELI
};

enum eVehicleAppearance
{
	VEHICLE_APPEARANCE_NONE,
	VEHICLE_APPEARANCE_CAR,
	VEHICLE_APPEARANCE_BIKE,
	VEHICLE_APPEARANCE_HELI,
	VEHICLE_APPEARANCE_BOAT,
	VEHICLE_APPEARANCE_PLANE,
};

// TODO: what is this even?
enum eBikeWheelSpecial
{
	BIKE_WHEELSPEC_0,	// both wheels on ground
	BIKE_WHEELSPEC_1,	// rear wheel on ground
	BIKE_WHEELSPEC_2,	// only front wheel on ground
	BIKE_WHEELSPEC_3,	// can't happen
};

enum
{
	ROTOR_TOP = 3,
	ROTOR_FRONT = 4,
	ROTOR_RIGHT = 5,
	ROTOR_LEFT = 7,
	ROTOR_BACK = 8,
	ROTOR_BOTTOM = 9,
};

enum eVehicleType {
	VEHICLE_TYPE_CAR,//汽车​
	VEHICLE_TYPE_BOAT,//​​船只
	VEHICLE_TYPE_TRAIN,//火车
	VEHICLE_TYPE_HELI,//直升机​
	VEHICLE_TYPE_PLANE,//飞机​
	VEHICLE_TYPE_BIKE,//摩托车/自行车
	NUM_VEHICLE_TYPES
};

class CVehicle : public CPhysical
{
public:
	void* pHandling;                      // 处理数据指针
	void* pFlyingHandling;                // 飞行处理数据指针
	uint8 AutoPilot[120];                 // 自动驾驶数据
	uint8 m_currentColour1;               // 当前颜色1
	uint8 m_currentColour2;               // 当前颜色2
	int8 m_aExtras[2];                    // 额外配置
	int16 m_nAlarmState;                  // 报警状态
	int16 m_nRouteSeed;                   // 路线种子
	CPed* pDriver;                        // 司机
	CPed* pPassengers[8];                 // 乘客数组
	uint8 m_nNumPassengers;               // 乘客数量
	int8 m_nNumGettingIn;                 // 正在上车的人数
	int8 m_nGettingInFlags;               // 上车标志
	int8 m_nGettingOutFlags;              // 下车标志
	uint8 m_nNumMaxPassengers;            // 最大乘客数量
	float field_1D0[4];                   // 未命名字段
	CEntity* m_pCurGroundEntity;          // 当前地面实体
	CFire* m_pCarFire;                    // 汽车火焰
	float m_fSteerAngle;                  // 方向盘角度
	float m_fGasPedal;                    // 加速踏板
	float m_fBrakePedal;                  // 刹车踏板
	uint8 VehicleCreatedBy;               // 创建车辆的标识

	// cf. https://github.com/DK22Pac/plugin-sdk/blob/master/plugin_sa/game_sa/CVehicle.h from R*
	uint8 bIsLawEnforcer : 1;             // 是否正在追捕玩家
	uint8 bIsAmbulanceOnDuty : 1;         // 救护车是否正在前往事故现场
	uint8 bIsFireTruckOnDuty : 1;         // 消防车是否正在前往火灾现场
	uint8 bIsLocked : 1;                  // 是否被脚本锁定（无法移除）
	uint8 bEngineOn : 1;                  // 发动机是否开启（用于声音效果，停泊的汽车发动机关闭）
	uint8 bIsHandbrakeOn : 1;             // 手刹是否开启
	uint8 bLightsOn : 1;                  // 灯光是否开启
	uint8 bFreebies : 1;                  // 车内是否有免费物品？

	uint8 bIsVan : 1;                     // 是否是货车（车门在车尾）
	uint8 bIsBus : 1;                     // 是否是公交车
	uint8 bIsBig : 1;                     // 是否是大型车辆
	uint8 bLowVehicle : 1;                // 是否需要使用低上下车动画（运动型汽车）
	uint8 bComedyControls : 1;            // 是否启用喜剧控制模式（使汽车难以控制）
	uint8 bWarnedPeds : 1;                // 是否已经扫描并警告行人注意危险
	uint8 bCraneMessageDone : 1;          // 是否已经为此汽车打印了起重机消息
	uint8 bExtendedRange : 1;             // 该车辆需要稍微远离一点才能被删除

	uint8 bTakeLessDamage : 1;            // 该车辆更强（承受约1/4的伤害）
	uint8 bIsDamaged : 1;                 // 该车辆已损坏并显示所有组件
	uint8 bHasBeenOwnedByPlayer : 1;      // 该车辆是否曾被玩家拥有（用于判断盗窃是否犯罪）
	uint8 bFadeOut : 1;                   // 渐隐车辆
	uint8 bIsBeingCarJacked : 1;          // 是否正在进行抢车
	uint8 bCreateRoadBlockPeds : 1;       // 如果该车辆接近足够近，我们将创建周围的人（警察或帮派成员）
	uint8 bCanBeDamaged : 1;              // 在过场动画期间设置为FALSE以避免爆炸
	uint8 bUsingSpecialColModel : 1;      // 玩家车辆是否使用特殊碰撞模型，存储在玩家结构中

	uint8 bOccupantsHaveBeenGenerated : 1;// 乘客是否已经生成（不应再次发生）
	uint8 bGunSwitchedOff : 1;            // 关闭船只上的枪支（关卡设计师可以使用此功能）
	uint8 bVehicleColProcessed : 1;       // 是否已处理 ProcessEntityCollision 函数
	uint8 bIsCarParkVehicle : 1;          // 使用特殊 CAR_PARK 脚本命令创建的汽车
	uint8 bHasAlreadyBeenRecorded : 1;    // 用于回放
	uint8 bPartOfConvoy : 1;              // 是否属于车队
	uint8 bHeliMinimumTilt : 1;           // 直升机应几乎没有倾斜
	uint8 bAudioChangingGear : 1;         // 发出换挡声音

	uint8 bIsDrowning : 1;                // 车辆乘员是否在水中受到伤害（即车辆在水中死亡）
	uint8 bTyresDontBurst : 1;            // 如果设置，轮胎不会爆裂
	uint8 bCreatedAsPoliceVehicle : 1;    // 是否作为执法车辆创建（巡警、警车等）
	uint8 bRestingOnPhysical : 1;         // 不要变为静态因为汽车坐在可能会被移除的物理对象上
	uint8 bParking : 1;                   // 是否正在停车
	uint8 bCanPark : 1;                   // 是否可以停车
#if (!defined GTA_PS2 || defined FIX_BUGS)
	uint8 m_bombType : 3;                 // 炸弹类型
#endif
	uint8 bDriverLastFrame : 1;           // 上一帧是否有司机

	int8 m_numPedsUseItAsCover;           // 使用该车辆作为掩护的行人数量
	uint8 m_nAmmoInClip;                  // 用于让船上的枪进行重新装填（默认20）
	uint8 m_nPacManPickupsCarried;        // 携带的吃豆人拾取物数量
	uint8 m_nRoadblockType;               // 路障类型
	float m_fHealth;                      // 健康值（1000.0f = 全健康，250.0f = 着火，0 -> 爆炸）
	uint8 m_nCurrentGear;                 // 当前档位
	float m_fChangeGearTime;              // 换挡时间
#if (!defined GTA_PS2 || defined FIX_BUGS)
	CEntity* m_pBombRigger;               // 安置炸弹的人
#endif
	uint32 m_nSetPieceExtendedRangeTime;  // 特定场景扩展范围时间
	uint32 m_nGunFiringTime;              // 车辆上的枪上次发射时间（用于船只）
	uint32 m_nTimeOfDeath;                // 死亡时间
	uint16 m_nTimeBlocked;                // 被阻挡的时间
	int16 m_nBombTimer;                   // 每帧减少
	CEntity* m_pBlowUpEntity;             // 爆炸实体
	float m_fMapObjectHeightAhead;        // 前方Z坐标？
	float m_fMapObjectHeightBehind;       // 后方Z坐标？
	eCarLock m_nDoorLock;                 // 车门锁状态
	int8 m_nLastWeaponDamage;             // 最后一次武器伤害（见 eWeaponType，-1 表示无伤害）
	CEntity* m_pLastDamageEntity;         // 最后造成伤害的实体
	uint8 m_nRadioStation;                // 收音机频道
	uint8 m_bRainAudioCounter;            // 雨声音频计数器
	uint8 m_bRainSamplesCounter;          // 雨声样本计数器
	uint32 m_nCarHornTimer;               // 汽笛计时器
	uint8 m_nCarHornPattern;              // 汽笛模式
	bool m_bSirenOrAlarm;                 // 是否有警报器或汽笛
	uint8 m_nCarHornDelay;                // 汽笛延迟
	int8 m_comedyControlState;            // 喜剧控制状态
	CStoredCollPoly m_aCollPolys[2];      // 前后部分的多边形
	float m_fSteerInput;                  // 方向盘输入
	eVehicleType m_vehType;               // 车辆类型

	//static void *operator new(size_t) throw();
	//static void *operator new(size_t sz, int slot) throw();
	//static void operator delete(void*, size_t) throw();
	//static void operator delete(void*, int) throw();

	CVehicle(void) {}	// FAKE
	//CVehicle(uint8 CreatedBy);
	static DWORD OverCVehicle_Addr;
	
	// from CEntity
	//void SetModelIndex(uint32 id);
	//bool SetupLighting(void);
	//void RemoveLighting(bool);
	//void FlagToDestroyWhenNextProcessed(void) {}

	virtual void ProcessControlInputs(uint8) {}
	virtual void GetComponentWorldPosition(int32 component, CVector &pos) {}
	virtual bool IsComponentPresent(int32 component) { return false; }
	virtual void SetComponentRotation(int32 component, CVector rotation) {}
	virtual void OpenDoor(int32, eDoors door, float) {}
	virtual void ProcessOpenDoor(uint32, uint32, float) {}
	virtual bool IsDoorReady(eDoors door) { return false; }
	virtual bool IsDoorFullyOpen(eDoors door) { return false; }
	virtual bool IsDoorClosed(eDoors door) { return false; }
	virtual bool IsDoorMissing(eDoors door) { return false; }
	virtual bool IsDoorReady(uint32 door) { return false; }
	virtual bool IsDoorMissing(uint32 door) { return false; }
	virtual bool IsOpenTopCar(void) { return false; }
	virtual void RemoveRefsToVehicle(CEntity *ent) {}
	virtual void BlowUpCar(CEntity *ent) {}
	virtual bool SetUpWheelColModel(CColModel *colModel) { return false; }
	virtual void BurstTyre(uint8 tyre, bool applyForces) {}
	virtual bool IsRoomForPedToLeaveCar(uint32 component, CVector *forcedDoorPos) { return false; }
	//virtual bool IsClearToDriveAway(void);
	//virtual float GetHeightAboveRoad(void);
	virtual void PlayCarHorn(void) {}
#ifdef COMPATIBLE_SAVES
	//virtual void Save(uint8*& buf);
	//virtual void Load(uint8*& buf);
#endif

	eVehicleAppearance GetVehicleAppearance(void);
	bool IsCar(void) { return m_vehType == VEHICLE_TYPE_CAR; }
	bool IsBoat(void) { return m_vehType == VEHICLE_TYPE_BOAT; }
	bool IsTrain(void) { return m_vehType == VEHICLE_TYPE_TRAIN; }
	bool IsHeli(void) { return m_vehType == VEHICLE_TYPE_HELI; }
	bool IsPlane(void) { return m_vehType == VEHICLE_TYPE_PLANE; }
	bool IsBike(void) { return m_vehType == VEHICLE_TYPE_BIKE; }

	void FlyingControl(eFlightModel flightModel);
	bool DoBladeCollision(CVector pos, CMatrix &matrix, int16 rotorType, float radius, float damageMult);
	bool BladeColSectorList(CPtrList &list, CColModel &rotorColModel, CMatrix &matrix, int16 rotorType, float damageMult);

	void ProcessWheel(CVector &wheelFwd, CVector &wheelRight, CVector &wheelContactSpeed, CVector &wheelContactPoint,
		int32 wheelsOnGround, float thrust, float brake, float adhesion, int8 wheelId, float *wheelSpeed, tWheelState *wheelState, uint16 wheelStatus);
	void ProcessBikeWheel(CVector &wheelFwd, CVector &wheelRight, CVector &wheelContactSpeed, CVector &wheelContactPoint,
		int32 wheelsOnGround, float thrust, float brake, float adhesion, float destabTraction, int8 wheelId, float *wheelSpeed, tWheelState *wheelState, eBikeWheelSpecial special, uint16 wheelStatus);
	void ExtinguishCarFire(void);
	void ProcessDelayedExplosion(void);
	float ProcessWheelRotation(tWheelState state, const CVector &fwd, const CVector &speed, float radius);
	int FindTyreNearestPoint(float x, float y);
	bool IsLawEnforcementVehicle(void);
	void ChangeLawEnforcerState(uint8 enable);
	bool UsesSiren(void);
	bool IsVehicleNormal(void);
	bool CarHasRoof(void);
	bool IsUpsideDown(void);
	bool IsOnItsSide(void);
	bool CanBeDeleted(void);
	bool CanPedOpenLocks(CPed *ped);
	bool CanDoorsBeDamaged(void);
	bool CanPedEnterCar(void);
	bool CanPedExitCar(bool jumpExit);
	bool CanPedJumpOutCar(void);
	bool CanPedJumpOffBike(void);
	// do these two actually return something?
	CPed *SetUpDriver(void);
	CPed *SetupPassenger(int n);
	void SetDriver(CPed *driver);
	bool AddPassenger(CPed *passenger);
	bool AddPassenger(CPed *passenger, uint8 n);
	void RemovePassenger(CPed *passenger);
	void RemoveDriver(void);
	bool IsDriver(CPed *ped);
	bool IsDriver(int32 model);
	bool IsPassenger(CPed *ped);
	bool IsPassenger(int32 model);
	void UpdatePassengerList(void);
	void ProcessCarAlarm(void);
	bool IsSphereTouchingVehicle(float sx, float sy, float sz, float radius);
	bool ShufflePassengersToMakeSpace(void);
	void MakeNonDraggedPedsLeaveVehicle(CPed *ped1, CPed *ped2, CPlayerPed *&player, CCopPed *&cop);
	void InflictDamage(CEntity *damagedBy, eWeaponType weaponType, float damage, CVector pos = CVector(0.0f, 0.0f, 0.0f));
	void DoFixedMachineGuns(void);
	void FireFixedMachineGuns(void);
	void ActivateBomb(void);
	void ActivateBombWhenEntered(void);
	void KillPedsInVehicle(void);

	void SetComponentAtomicAlpha(RpAtomic *atomic, int32 alpha);
	void UpdateClumpAlpha(void);

	static void HeliDustGenerate(CEntity *heli, float radius, float ground, int rnd);
	void DoSunGlare(void);

	static bool bWheelsOnlyCheat;
	static bool bAllDodosCheat;
	static bool bCheat3;
	static bool bCheat4;
	static bool bCheat5;
	static bool bCheat8;
	static bool bCheat9;
	static bool bCheat10;
	static bool bHoverCheat;
	static bool bAllTaxisHaveNitro;
	static bool m_bDisableMouseSteering;
	static bool bDisableRemoteDetonation;
	static bool bDisableRemoteDetonationOnContact;
#ifndef MASTER
	static bool m_bDisplayHandlingInfo;
#endif
};

void DestroyVehicleAndDriverAndPassengers(CVehicle* pVehicle);
bool IsVehiclePointerValid(CVehicle* pVehicle);

// Names of functions below are made up by us.

// Used in III and VC.
inline int8 GetCarDoorFlag(int32 carnode) {
	switch (carnode) {
	case CAR_DOOR_LF:
		return CAR_DOOR_FLAG_LF;
	case CAR_DOOR_LR:
		return CAR_DOOR_FLAG_LR;
	case CAR_DOOR_RF:
		return CAR_DOOR_FLAG_RF;
	case CAR_DOOR_RR:
		return CAR_DOOR_FLAG_RR;
	default:
		return CAR_DOOR_FLAG_UNKNOWN;
	}
}

// VC. Accounts the case numMaxPassengers == 0, only for m_nGettingInFlags.
inline int8 GetEnterCarDoorFlag(int32 carnode, uint8 numMaxPassengers) {
	switch (carnode) {
	case CAR_DOOR_RF:
		return CAR_DOOR_FLAG_RF;
	case CAR_DOOR_RR:
		return CAR_DOOR_FLAG_RR;
	case CAR_DOOR_LF:
		if (numMaxPassengers != 0)
			return CAR_DOOR_FLAG_LF;
		else
			return CAR_DOOR_FLAG_LF | CAR_DOOR_FLAG_LR;
	case CAR_DOOR_LR:
		if (numMaxPassengers != 0)
			return CAR_DOOR_FLAG_LR;
		else
			return CAR_DOOR_FLAG_LF | CAR_DOOR_FLAG_LR;
	default:
		return CAR_DOOR_FLAG_UNKNOWN;
	}
}
