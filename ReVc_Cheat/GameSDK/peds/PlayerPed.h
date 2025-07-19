#pragma once

#include "Ped.h"

class CPad;
class CCopPed;
class CWanted;

typedef uint8_t RpAtomic[84];

class CPlayerPed : public CPed
{
public:
	CWanted *m_pWanted;//通缉系统指针​​	指向玩家的通缉数据（CWanted 类），控制通缉等级、警察追捕等。
	CCopPed *m_pArrestingCop;//逮捕警察指针​​	指向正在逮捕玩家的警察（CCopPed 类）。
	float m_fMoveSpeed;//移动速度​​	当前移动速度
	float m_fCurrentStamina;//当前体力值​​	玩家当前的体力值（跑步、游泳等消耗）。
	float m_fMaxStamina;//最大体力值​​	玩家的体力上限。
	float m_fStaminaProgress;//​​体力恢复进度​​	体力恢复的进度或衰减系数（可能用于动态调整恢复速度）。
	int8 m_nSelectedWepSlot;//当前武器槽​​	玩家当前选择的武器槽（0~9，对应不同武器类型）。
	bool m_bSpeedTimerFlag;//速度计时标志​​	可能是某种速度相关计时器的开关（如冲刺冷却）。
	uint8 m_nEvadeAmount;//闪避次数/等级​​	玩家成功闪避攻击的次数或闪避能力等级。
	uint32 m_nSpeedTimer; //​​速度计时器​​	可能与移动速度相关的计时器（注释提到可能是 m_nStandStillTimer，即静止计时器）。 m_nStandStillTimer?
	uint32 m_nHitAnimDelayTimer; //受击动画延迟计时器​​	控制受击动画的延迟（注释提到可能是 m_nShotDelay，即射击延迟）。 m_nShotDelay?
	float m_fAttackButtonCounter;//攻击按钮计数器​​	记录连续按下攻击按钮的次数或时间（用于连击判定）。
	bool m_bHaveTargetSelected;	//是否有锁定目标​​	标记玩家是否锁定了目标（如瞄准敌人或车辆）。 may have better name
	CEntity *m_pEvadingFrom;	//闪避来源实体​​	指向玩家正在闪避的实体（CEntity，可能是车辆、子弹等） is this CPhysical?
	int32 m_nTargettableObjects[4];//存储玩家当前可锁定/攻击的对象的ID（如敌人、车辆等）。
	uint32 m_nAdrenalineTime;//肾上腺素效果的剩余时间（可能增强玩家能力）
	uint8 m_nDrunkenness;             // Needed to work out whether we lost target this frame
	uint8 m_nFadeDrunkenness;
	uint8 m_nDrunkCountdown; //countdown in frames when the drunk effect ends
	bool m_bAdrenalineActive;//​​肾上腺素激活状态​​	标记肾上腺素效果是否激活。
	bool m_bHasLockOnTarget;//​​是否锁定目标​​	玩家是否处于目标锁定状态（如狙击枪瞄准）。
	bool m_bCanBeDamaged;//是否可受伤​​	标记玩家当前是否可受到伤害（无敌模式时会为 false）。
	bool m_bNoPosForMeleeAttack;//无法近战攻击位置​​	标记玩家当前位置是否无法进行近战攻击（如被障碍物阻挡）。
	bool unk1;
	CVector m_vecSafePos[6]; // safe places from the player, for example behind a tree
	CPed *m_pPedAtSafePos[6];
	CPed *m_pMeleeList[6]; // reachable peds at each direction(6)
	int16 m_nAttackDirToCheck;
	float m_fWalkAngle; //angle between heading and walking direction
	float m_fFPSMoveHeading;
	RpAtomic* m_pMinigunTopAtomic; //atomic for the spinning part of the minigun model
	float m_fGunSpinSpeed; // for minigun
	float m_fGunSpinAngle;
	unsigned int m_nPadDownPressedInMilliseconds;
	unsigned int m_nLastBusFareCollected;
#ifdef FREE_CAM
	bool m_bFreeAimActive;
	CVector m_cachedCamSource;
	CVector m_cachedCamFront;
	CVector m_cachedCamUp;
#endif

	static bool bDontAllowWeaponChange;
#ifndef MASTER
	static bool bDebugPlayerInfo;
#endif

	CPlayerPed();
	~CPlayerPed();
	void SetMoveAnim() { };

	void ReApplyMoveAnims(void);
	void ClearWeaponTarget(void);
	void SetWantedLevel(int32 level);
	void SetWantedLevelNoDrop(int32 level);
	void KeepAreaAroundPlayerClear(void);
	void AnnoyPlayerPed(bool);
	void MakeChangesForNewWeapon(int32);
	void MakeChangesForNewWeapon(eWeaponType);
	void SetInitialState(void);
	void ProcessControl(void);
	void ClearAdrenaline(void);
	void UseSprintEnergy(void);
	class CPlayerInfo *GetPlayerInfoForThisPlayerPed();
	void SetRealMoveAnim(void);
	void RestoreSprintEnergy(float);
	float DoWeaponSmoothSpray(void);
	void DoStuffToGoOnFire(void);
	bool DoesTargetHaveToBeBroken(CVector, CWeapon*);
	void RunningLand(CPad*);
	bool IsThisPedAnAimingPriority(CPed*);
	void PlayerControlSniper(CPad*);
	void PlayerControlM16(CPad*);
	void PlayerControlFighter(CPad*);
	void ProcessWeaponSwitch(CPad*);
	void MakeObjectTargettable(int32);
	void PlayerControl1stPersonRunAround(CPad *padUsed);
	void EvaluateNeighbouringTarget(CEntity*, CEntity**, float*, float, float, bool, bool);
	void EvaluateTarget(CEntity*, CEntity**, float*, float, float, bool);
	bool FindNextWeaponLockOnTarget(CEntity*, bool);
	bool FindWeaponLockOnTarget(void);
	void ProcessAnimGroups(void);
	void ProcessPlayerWeapon(CPad*);
	void PlayerControlZelda(CPad*);
	bool DoesPlayerWantNewWeapon(eWeaponType, bool);
	void PlayIdleAnimations(CPad*);
	void RemovePedFromMeleeList(CPed*);
	void GetMeleeAttackCoords(CVector&, int8, float);
	int32 FindMeleeAttackPoint(CPed*, CVector&, uint32&);
	bool CanIKReachThisTarget(CVector, CWeapon*, bool);
	void RotatePlayerToTrackTarget(void);
	bool MovementDisabledBecauseOfTargeting(void);
	void FindNewAttackPoints(void);
	void SetNearbyPedsToInteractWithPlayer(void);
	void UpdateMeleeAttackers(void);

	static void SetupPlayerPed(int32);
	static void DeactivatePlayerPed(int32);
	static void ReactivatePlayerPed(int32);

#ifdef COMPATIBLE_SAVES
	virtual void Save(uint8*& buf);
	virtual void Load(uint8*& buf);
#endif

	static const uint32 nSaveStructSize;
};

//VALIDATE_SIZE(CPlayerPed, 0x5F0);
