#pragma once

//#include "Lists.h"
//#include "Timer.h"
#include "Entity.h"

enum {
	PHYSICAL_MAX_COLLISIONRECORDS = 6
};

#define GRAVITY (0.008f)

//class CTreadable;

class CPhysical : public CEntity
{
public:
	int32 m_audioEntityId;            // 音频实体ID
	float m_phys_unused1;             // 物理未使用字段1
	uint32 m_nLastTimeCollided;       // 上次碰撞时间
	CVector m_vecMoveSpeed;           // 速度
	CVector m_vecTurnSpeed;           // 角速度
	CVector m_vecMoveFriction;        // 移动摩擦力
	CVector m_vecTurnFriction;        // 转动摩擦力
	CVector m_vecMoveSpeedAvg;        // 平均移动速度
	CVector m_vecTurnSpeedAvg;        // 平均转动速度
	float m_fMass;                    // 质量
	float m_fTurnMass;                // 转动惯性矩
	float m_fForceMultiplier;         // 力乘数
	float m_fAirResistance;           // 空气阻力
	float m_fElasticity;              // 弹性系数
	float m_fBuoyancy;                // 浮力
	CVector m_vecCentreOfMass;        // 质心位置
	uint32 m_entryInfoList;           // 入口信息列表
	void* m_movingListNode;           // 移动物体链表节点

	int8 m_phys_unused2;              // 物理未使用字段2
	uint8 m_nStaticFrames;            // 静态帧数
	uint8 m_nCollisionRecords;        // 碰撞记录数
	CEntity* m_aCollisionRecords[PHYSICAL_MAX_COLLISIONRECORDS]; // 碰撞记录数组

	float m_fDistanceTravelled;       // 行驶距离

	// 损坏部件
	float m_fDamageImpulse;           // 损伤脉冲
	CEntity* m_pDamageEntity;         // 损伤实体
	CVector m_vecDamageNormal;        // 损伤法线
	int16 m_nDamagePieceType;         // 损伤部件类型

	uint8 bIsHeavy : 1;               // 是否重型车辆
	uint8 bAffectedByGravity : 1;     // 是否受重力影响
	uint8 bInfiniteMass : 1;          // 是否质量无限大
	uint8 m_phy_flagA08 : 1;          // 物理标志位A08
	uint8 bIsInWater : 1;             // 是否在水中
	uint8 m_phy_flagA20 : 1;          // 物理标志位A20（未使用）
	uint8 bHitByTrain : 1;            // 是否被火车撞击
	uint8 bSkipLineCol : 1;           // 是否跳过线段碰撞检测

	uint8 bIsFrozen : 1;              // 是否冻结
	uint8 bDontLoadCollision : 1;     // 是否不加载碰撞模型
	uint8 m_bIsVehicleBeingShifted : 1;// 错误名称 - 也用于行人但从未设置
	uint8 bJustCheckCollision : 1;    // 仅检查是否有碰撞

	uint8 m_nSurfaceTouched;          // 触摸表面类型
	int8 m_nZoneLevel;                // 区域级别

	//CPhysical(void);
	//~CPhysical(void);
	virtual int32 ProcessEntityCollision(CEntity* ent, void* colpoints) = 0;
	
};
