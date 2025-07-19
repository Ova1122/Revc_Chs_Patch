#pragma once

//#include "ModelInfo.h"
#include "Placeable.h"

struct CReference;
class CPtrList;

enum eEntityType
{
	ENTITY_TYPE_NOTHING = 0,
	ENTITY_TYPE_BUILDING,
	ENTITY_TYPE_VEHICLE,
	ENTITY_TYPE_PED,
	ENTITY_TYPE_OBJECT,
	ENTITY_TYPE_DUMMY,
};

enum eEntityStatus
{
	STATUS_PLAYER,					 //状态_玩家,
	STATUS_PLAYER_PLAYBACKFROMBUFFER,//状态_玩家_从缓冲区玩家,
	STATUS_SIMPLE,					 //状态_简单,
	STATUS_PHYSICS,					 //状态_物理,
	STATUS_ABANDONED,				 //状态_放弃,
	STATUS_WRECKED,					 //状态_损坏,
	STATUS_TRAIN_MOVING,			 //状态_火车移动,
	STATUS_TRAIN_NOT_MOVING,		 //状态_火车静止,
	STATUS_HELI,					 //状态_直升机,
	STATUS_PLANE,					 //状态_飞机,
	STATUS_PLAYER_REMOTE,			 //状态_玩家远程,
	STATUS_PLAYER_DISABLED,			 //状态_玩家禁用,
	STATUS_GHOST					 //状态_幽灵
};

class CEntity : public CPlaceable
{
public:
	void *m_rwObject;
protected:
	uint32 m_type : 3;
private:
	uint32 m_status : 5;
public:
	// flagsA
	uint32 bUsesCollision : 1;            // 实体是否使用碰撞
	uint32 bCollisionProcessed : 1;       // 对象是否已被 ProcessEntityCollision 函数处理
	uint32 bIsStatic : 1;                // 实体是否静态
	uint32 bHasContacted : 1;           // 实体是否已处理一些接触力
	uint32 bPedPhysics : 1;              // 是否使用行人物理
	uint32 bIsStuck : 1;                 // 实体是否卡住
	uint32 bIsInSafePosition : 1;         // 实体是否处于一个无碰撞的安全位置
	uint32 bUseCollisionRecords : 1;      // 使用碰撞记录

	// flagsB
	uint32 bWasPostponed : 1;            // 实体控制处理是否被推迟
	uint32 bExplosionProof : 1;           // 是否抗爆炸
	uint32 bIsVisible : 1;               // 实体是否可见
	uint32 bHasCollided : 1;             // 是否发生碰撞
	uint32 bRenderScorched : 1;          // 渲染烧焦效果
	uint32 bHasBlip : 1;                // 是否有雷达点
	uint32 bIsBIGBuilding : 1;           // 如果该实体是一个大型建筑，则设置此标志
	uint32 bStreamBIGBuilding : 1;       // 当绘制距离 <= 2000 时设置

	// flagsC
	uint32 bRenderDamaged : 1;           // 对于具有适用损坏的对象，使用损坏的 LOD 模型进行渲染
	uint32 bBulletProof : 1;             // 是否抗子弹
	uint32 bFireProof : 1;              // 是否抗火焰
	uint32 bCollisionProof : 1;         // 是否抗碰撞
	uint32 bMeleeProof : 1;             // 是否抗近战攻击
	uint32 bOnlyDamagedByPlayer : 1;     // 只能被玩家造成伤害
	uint32 bStreamingDontDelete : 1;    // 不让流式传输删除此对象
	uint32 bRemoveFromWorld : 1;        // 下次处理时移除此实体

	// flagsD
	uint32 bHasHitWall : 1;              // 是否与建筑物发生碰撞（影响后续碰撞）
	uint32 bImBeingRendered : 1;        // 不要删除我，因为我正在被渲染
	uint32 bTouchingWater : 1;          // 用于 cBuoyancy::ProcessBuoyancy
	uint32 bIsSubway : 1;              // 设置为地铁，但可能有不同的含义？
	uint32 bDrawLast : 1;               // 最后绘制对象
	uint32 bNoBrightHeadLights : 1;     // 没有明亮的前灯
	uint32 bDoNotRender : 1;           // -- 只适用于 CObjects 类别
	uint32 bDistanceFade : 1;           // 因为太远而淡出实体

	// flagsE
	uint32 m_flagE1 : 1;
	uint32 bDontCastShadowsOn : 1;      // 不在此对象上投射阴影
	uint32 bOffscreen : 1;              // 屏幕外标志。只有当它设置为 true 时才能信任此标志
	uint32 bIsStaticWaitingForCollision : 1; // 这用于脚本创建的实体 - 它们在加载下方的碰撞之前是静态的
	uint32 bDontStream : 1;             // 告诉流式传输不要流式传输此对象
	uint32 bUnderwater : 1;             // 此对象在水下，更改绘制顺序
	uint32 bHasPreRenderEffects : 1;    // 对象附加了预渲染效果

	uint16 m_scanCode;                  // 扫描码
	uint16 m_randomSeed;                // 随机种子
	int16 m_modelIndex;                 // 模型索引
	int8 m_level;                       // 级别
	int8 m_area;                        // 区域
	CReference* m_pFirstReference;      // 第一个引用指针

public:
	
	//CEntity(void);
	virtual ~CEntity(void);
		
	virtual void Add(void);
	virtual void Remove(void);
	virtual void SetModelIndex(uint32 id);
	virtual void SetModelIndexNoCreate(uint32 id);
	virtual void CreateRwObject(void);
	virtual void DeleteRwObject(void);
	virtual CRect GetBoundRect(void);
	virtual void ProcessControl(void) {}
	virtual void ProcessCollision(void) {}
	virtual void ProcessShift(void) {}
	virtual void Teleport(CVector v) {}
	virtual void PreRender(void);
	virtual void Render(void);
	virtual bool SetupLighting(void);
	virtual void RemoveLighting(bool);
	virtual void FlagToDestroyWhenNextProcessed(void) {};

	uint8 GetType() const { return m_type; }
	void SetType(uint8 type) { m_type = type; }
	uint8 GetStatus() const { return m_status; }
	void SetStatus(uint8 status) { m_status = status; }
	//CColModel* GetColModel(void) { return CModelInfo::GetModelInfo(m_modelIndex)->GetColModel(); }
	bool GetIsStatic(void) const { return bIsStatic || bIsStaticWaitingForCollision; }
	void SetIsStatic(bool state) { bIsStatic = state; }

	bool IsBuilding(void) { return m_type == ENTITY_TYPE_BUILDING; }
	bool IsVehicle(void) { return m_type == ENTITY_TYPE_VEHICLE; }
	bool IsPed(void) { return m_type == ENTITY_TYPE_PED; }
	bool IsObject(void) { return m_type == ENTITY_TYPE_OBJECT; }
	bool IsDummy(void) { return m_type == ENTITY_TYPE_DUMMY; }

	

	
};


