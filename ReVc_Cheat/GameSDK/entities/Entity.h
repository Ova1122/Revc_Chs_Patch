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
	STATUS_PLAYER,					 //״̬_���,
	STATUS_PLAYER_PLAYBACKFROMBUFFER,//״̬_���_�ӻ��������,
	STATUS_SIMPLE,					 //״̬_��,
	STATUS_PHYSICS,					 //״̬_����,
	STATUS_ABANDONED,				 //״̬_����,
	STATUS_WRECKED,					 //״̬_��,
	STATUS_TRAIN_MOVING,			 //״̬_���ƶ�,
	STATUS_TRAIN_NOT_MOVING,		 //״̬_�𳵾�ֹ,
	STATUS_HELI,					 //״̬_ֱ����,
	STATUS_PLANE,					 //״̬_�ɻ�,
	STATUS_PLAYER_REMOTE,			 //״̬_���Զ��,
	STATUS_PLAYER_DISABLED,			 //״̬_��ҽ���,
	STATUS_GHOST					 //״̬_����
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
	uint32 bUsesCollision : 1;            // ʵ���Ƿ�ʹ����ײ
	uint32 bCollisionProcessed : 1;       // �����Ƿ��ѱ� ProcessEntityCollision ��������
	uint32 bIsStatic : 1;                // ʵ���Ƿ�̬
	uint32 bHasContacted : 1;           // ʵ���Ƿ��Ѵ���һЩ�Ӵ���
	uint32 bPedPhysics : 1;              // �Ƿ�ʹ����������
	uint32 bIsStuck : 1;                 // ʵ���Ƿ�ס
	uint32 bIsInSafePosition : 1;         // ʵ���Ƿ���һ������ײ�İ�ȫλ��
	uint32 bUseCollisionRecords : 1;      // ʹ����ײ��¼

	// flagsB
	uint32 bWasPostponed : 1;            // ʵ����ƴ����Ƿ��Ƴ�
	uint32 bExplosionProof : 1;           // �Ƿ񿹱�ը
	uint32 bIsVisible : 1;               // ʵ���Ƿ�ɼ�
	uint32 bHasCollided : 1;             // �Ƿ�����ײ
	uint32 bRenderScorched : 1;          // ��Ⱦ�ս�Ч��
	uint32 bHasBlip : 1;                // �Ƿ����״��
	uint32 bIsBIGBuilding : 1;           // �����ʵ����һ�����ͽ����������ô˱�־
	uint32 bStreamBIGBuilding : 1;       // �����ƾ��� <= 2000 ʱ����

	// flagsC
	uint32 bRenderDamaged : 1;           // ���ھ��������𻵵Ķ���ʹ���𻵵� LOD ģ�ͽ�����Ⱦ
	uint32 bBulletProof : 1;             // �Ƿ��ӵ�
	uint32 bFireProof : 1;              // �Ƿ񿹻���
	uint32 bCollisionProof : 1;         // �Ƿ���ײ
	uint32 bMeleeProof : 1;             // �Ƿ񿹽�ս����
	uint32 bOnlyDamagedByPlayer : 1;     // ֻ�ܱ��������˺�
	uint32 bStreamingDontDelete : 1;    // ������ʽ����ɾ���˶���
	uint32 bRemoveFromWorld : 1;        // �´δ���ʱ�Ƴ���ʵ��

	// flagsD
	uint32 bHasHitWall : 1;              // �Ƿ��뽨���﷢����ײ��Ӱ�������ײ��
	uint32 bImBeingRendered : 1;        // ��Ҫɾ���ң���Ϊ�����ڱ���Ⱦ
	uint32 bTouchingWater : 1;          // ���� cBuoyancy::ProcessBuoyancy
	uint32 bIsSubway : 1;              // ����Ϊ�������������в�ͬ�ĺ��壿
	uint32 bDrawLast : 1;               // �����ƶ���
	uint32 bNoBrightHeadLights : 1;     // û��������ǰ��
	uint32 bDoNotRender : 1;           // -- ֻ������ CObjects ���
	uint32 bDistanceFade : 1;           // ��Ϊ̫Զ������ʵ��

	// flagsE
	uint32 m_flagE1 : 1;
	uint32 bDontCastShadowsOn : 1;      // ���ڴ˶�����Ͷ����Ӱ
	uint32 bOffscreen : 1;              // ��Ļ���־��ֻ�е�������Ϊ true ʱ�������δ˱�־
	uint32 bIsStaticWaitingForCollision : 1; // �����ڽű�������ʵ�� - �����ڼ����·�����ײ֮ǰ�Ǿ�̬��
	uint32 bDontStream : 1;             // ������ʽ���䲻Ҫ��ʽ����˶���
	uint32 bUnderwater : 1;             // �˶�����ˮ�£����Ļ���˳��
	uint32 bHasPreRenderEffects : 1;    // ���󸽼���Ԥ��ȾЧ��

	uint16 m_scanCode;                  // ɨ����
	uint16 m_randomSeed;                // �������
	int16 m_modelIndex;                 // ģ������
	int8 m_level;                       // ����
	int8 m_area;                        // ����
	CReference* m_pFirstReference;      // ��һ������ָ��

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


