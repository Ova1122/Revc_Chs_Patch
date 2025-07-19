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
	int32 m_audioEntityId;            // ��Ƶʵ��ID
	float m_phys_unused1;             // ����δʹ���ֶ�1
	uint32 m_nLastTimeCollided;       // �ϴ���ײʱ��
	CVector m_vecMoveSpeed;           // �ٶ�
	CVector m_vecTurnSpeed;           // ���ٶ�
	CVector m_vecMoveFriction;        // �ƶ�Ħ����
	CVector m_vecTurnFriction;        // ת��Ħ����
	CVector m_vecMoveSpeedAvg;        // ƽ���ƶ��ٶ�
	CVector m_vecTurnSpeedAvg;        // ƽ��ת���ٶ�
	float m_fMass;                    // ����
	float m_fTurnMass;                // ת�����Ծ�
	float m_fForceMultiplier;         // ������
	float m_fAirResistance;           // ��������
	float m_fElasticity;              // ����ϵ��
	float m_fBuoyancy;                // ����
	CVector m_vecCentreOfMass;        // ����λ��
	uint32 m_entryInfoList;           // �����Ϣ�б�
	void* m_movingListNode;           // �ƶ���������ڵ�

	int8 m_phys_unused2;              // ����δʹ���ֶ�2
	uint8 m_nStaticFrames;            // ��̬֡��
	uint8 m_nCollisionRecords;        // ��ײ��¼��
	CEntity* m_aCollisionRecords[PHYSICAL_MAX_COLLISIONRECORDS]; // ��ײ��¼����

	float m_fDistanceTravelled;       // ��ʻ����

	// �𻵲���
	float m_fDamageImpulse;           // ��������
	CEntity* m_pDamageEntity;         // ����ʵ��
	CVector m_vecDamageNormal;        // ���˷���
	int16 m_nDamagePieceType;         // ���˲�������

	uint8 bIsHeavy : 1;               // �Ƿ����ͳ���
	uint8 bAffectedByGravity : 1;     // �Ƿ�������Ӱ��
	uint8 bInfiniteMass : 1;          // �Ƿ��������޴�
	uint8 m_phy_flagA08 : 1;          // �����־λA08
	uint8 bIsInWater : 1;             // �Ƿ���ˮ��
	uint8 m_phy_flagA20 : 1;          // �����־λA20��δʹ�ã�
	uint8 bHitByTrain : 1;            // �Ƿ񱻻�ײ��
	uint8 bSkipLineCol : 1;           // �Ƿ������߶���ײ���

	uint8 bIsFrozen : 1;              // �Ƿ񶳽�
	uint8 bDontLoadCollision : 1;     // �Ƿ񲻼�����ײģ��
	uint8 m_bIsVehicleBeingShifted : 1;// �������� - Ҳ�������˵���δ����
	uint8 bJustCheckCollision : 1;    // ������Ƿ�����ײ

	uint8 m_nSurfaceTouched;          // ������������
	int8 m_nZoneLevel;                // ���򼶱�

	//CPhysical(void);
	//~CPhysical(void);
	virtual int32 ProcessEntityCollision(CEntity* ent, void* colpoints) = 0;
	
};
