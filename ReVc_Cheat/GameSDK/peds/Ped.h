#pragma once

//#include "RwHelper.h"
//#include "AnimManager.h"
#include "Crime.h"
#include "EventList.h"
#include "PedIK.h"
#include "PedType.h"
#include "Physical.h"
#include "Weapon.h"
#include "WeaponInfo.h"
#include "PathFind.h"
#include "Collision.h"

#define FEET_OFFSET	1.04f
#define CHECK_NEARBY_THINGS_MAX_DIST	15.0f
#define ENTER_CAR_MAX_DIST	30.0f
#define CAN_SEE_ENTITY_ANGLE_THRESHOLD	DEGTORAD(60.0f)

class CAccident;
class CObject;
class CFire;
struct AnimBlendFrameData;
class CAnimBlendAssociation;
class CPedAttractor;

struct PedAudioData
{
	int m_nFixedDelayTime;
	int m_nOverrideFixedDelayTime;
	int m_nOverrideMaxRandomDelayTime;
	int m_nMaxRandomDelayTime;
};

enum
{
	ATTACK_IN_PROGRESS,
	CANT_ATTACK,
	WATCH_UNTIL_HE_DISAPPEARS,
};

enum eFormation
{
	FORMATION_UNDEFINED,
	FORMATION_REAR,
	FORMATION_REAR_LEFT,
	FORMATION_REAR_RIGHT,
	FORMATION_FRONT_LEFT,
	FORMATION_FRONT_RIGHT,
	FORMATION_LEFT,
	FORMATION_RIGHT,
	FORMATION_FRONT
};

enum FightState {
	FIGHTSTATE_MOVE_FINISHED = -2,
	FIGHTSTATE_JUST_ATTACKED,
	FIGHTSTATE_NO_MOVE,
	FIGHTSTATE_1
};

enum
{
	ENDFIGHT_NORMAL,
	ENDFIGHT_WITH_A_STEP,
	ENDFIGHT_FAST
};

enum PedRouteType
{
	PEDROUTE_STOP_WHEN_DONE = 1,
	PEDROUTE_GO_BACKWARD_WHEN_DONE,
	PEDROUTE_GO_TO_START_WHEN_DONE
};

enum FightMoveHitLevel
{
	HITLEVEL_NULL,
	HITLEVEL_GROUND,
	HITLEVEL_LOW,
	HITLEVEL_MEDIUM,
	HITLEVEL_HIGH
};

struct FightMove
{
	AnimationId animId;
	float startFireTime;
	float endFireTime;
	float comboFollowOnTime;
	float strikeRadius;
	float extendReachMultiplier;
	uint8 hitLevel; // FightMoveHitLevel
	uint8 damage;
	uint8 flags;
};

// TODO: This is eFightState on mobile.
enum PedFightMoves
{
	FIGHTMOVE_NULL,
	// Attacker
	FIGHTMOVE_STDPUNCH,
	FIGHTMOVE_IDLE,
	FIGHTMOVE_SHUFFLE_F,
	FIGHTMOVE_KNEE,
	FIGHTMOVE_PUNCHHOOK,
	FIGHTMOVE_PUNCHJAB,
	FIGHTMOVE_PUNCH,
	FIGHTMOVE_LONGKICK,
	FIGHTMOVE_ROUNDHOUSE,
	// Directionals
	FIGHTMOVE_FWDLEFT,
	FIGHTMOVE_FWDRIGHT,
	FIGHTMOVE_BACKKICK,
	FIGHTMOVE_BACKFLIP,
	FIGHTMOVE_BACKLEFT,
	FIGHTMOVE_BACKRIGHT,
	FIGHTMOVE_RIGHTSWEEP,
	// Special
	FIGHTMOVE_GROUNDKICK,
	// Opponent
	FIGHTMOVE_HITFRONT,
	FIGHTMOVE_HITBACK,
	FIGHTMOVE_HITRIGHT,
	FIGHTMOVE_HITLEFT,
	FIGHTMOVE_HITBODY,
	FIGHTMOVE_HITCHEST,
	FIGHTMOVE_HITHEAD,
	FIGHTMOVE_HITBIGSTEP,
	FIGHTMOVE_HITONFLOOR,
	FIGHTMOVE_HITBEHIND,
	FIGHTMOVE_MELEE1,
	FIGHTMOVE_MELEE2,
	FIGHTMOVE_MELEE3,
	FIGHTMOVE_IDLE2NORM,
	NUM_FIGHTMOVES
};

enum ePedPieceTypes
{
	PEDPIECE_TORSO,
	PEDPIECE_MID,
	PEDPIECE_LEFTARM,
	PEDPIECE_RIGHTARM,
	PEDPIECE_LEFTLEG,
	PEDPIECE_RIGHTLEG,
	PEDPIECE_HEAD,
};

enum eWaitState {
	WAITSTATE_FALSE,
	WAITSTATE_TRAFFIC_LIGHTS,
	WAITSTATE_CROSS_ROAD,
	WAITSTATE_CROSS_ROAD_LOOK,
	WAITSTATE_LOOK_PED,
	WAITSTATE_LOOK_SHOP,
	WAITSTATE_LOOK_ACCIDENT,
	WAITSTATE_FACEOFF_GANG,
	WAITSTATE_DOUBLEBACK,
	WAITSTATE_HITWALL,
	WAITSTATE_TURN180,
	WAITSTATE_SURPRISE,
	WAITSTATE_STUCK,
	WAITSTATE_LOOK_ABOUT,
	WAITSTATE_PLAYANIM_DUCK,
	WAITSTATE_PLAYANIM_COWER,
	WAITSTATE_PLAYANIM_TAXI,
	WAITSTATE_PLAYANIM_HANDSUP,
	WAITSTATE_PLAYANIM_HANDSCOWER,
	WAITSTATE_PLAYANIM_CHAT,
	WAITSTATE_FINISH_FLEE,
	WAITSTATE_SIT_DOWN,
	WAITSTATE_SIT_DOWN_RVRS,
	WAITSTATE_SIT_UP,
	WAITSTATE_SIT_IDLE,
	WAITSTATE_USE_ATM,
	WAITSTATE_SUN_BATHE_PRE,
	WAITSTATE_SUN_BATHE_DOWN,
	WAITSTATE_SUN_BATHE_IDLE,
	WAITSTATE_RIOT,
	WAITSTATE_FAST_FALL,
	WAITSTATE_BOMBER,
	WAITSTATE_STRIPPER,
	WAITSTATE_GROUND_ATTACK,
	WAITSTATE_LANCESITTING,
	WAITSTATE_PLAYANIM_HANDSUP_SIMPLE,
};

enum eObjective {
	OBJECTIVE_NONE,
	OBJECTIVE_WAIT_ON_FOOT,
	OBJECTIVE_WAIT_ON_FOOT_FOR_COP,
	OBJECTIVE_FLEE_ON_FOOT_TILL_SAFE,
	OBJECTIVE_GUARD_SPOT,
	OBJECTIVE_GUARD_AREA,
	OBJECTIVE_WAIT_IN_CAR,
	OBJECTIVE_WAIT_IN_CAR_THEN_GET_OUT,
	OBJECTIVE_KILL_CHAR_ON_FOOT,
	OBJECTIVE_KILL_CHAR_ANY_MEANS,
	OBJECTIVE_FLEE_CHAR_ON_FOOT_TILL_SAFE,
	OBJECTIVE_FLEE_CHAR_ON_FOOT_ALWAYS,
	OBJECTIVE_GOTO_CHAR_ON_FOOT,
	OBJECTIVE_GOTO_CHAR_ON_FOOT_WALKING,
	OBJECTIVE_HASSLE_CHAR,
	OBJECTIVE_FOLLOW_CHAR_IN_FORMATION,
	OBJECTIVE_LEAVE_CAR,
	OBJECTIVE_ENTER_CAR_AS_PASSENGER,
	OBJECTIVE_ENTER_CAR_AS_DRIVER,
	OBJECTIVE_FOLLOW_CAR_IN_CAR,
	OBJECTIVE_FIRE_AT_OBJECT_FROM_VEHICLE,
	OBJECTIVE_DESTROY_OBJECT,
	OBJECTIVE_DESTROY_CAR,
	OBJECTIVE_GOTO_AREA_ANY_MEANS,
	OBJECTIVE_GOTO_AREA_ON_FOOT,
	OBJECTIVE_RUN_TO_AREA,
	OBJECTIVE_GOTO_AREA_IN_CAR,
	OBJECTIVE_FOLLOW_CAR_ON_FOOT_WITH_OFFSET,
	OBJECTIVE_GUARD_ATTACK,
	OBJECTIVE_SET_LEADER,
	OBJECTIVE_FOLLOW_ROUTE,
	OBJECTIVE_SOLICIT_VEHICLE,
	OBJECTIVE_HAIL_TAXI,
	OBJECTIVE_CATCH_TRAIN,
	OBJECTIVE_BUY_ICE_CREAM,
	OBJECTIVE_STEAL_ANY_CAR,
	OBJECTIVE_STEAL_ANY_MISSION_CAR,
	OBJECTIVE_MUG_CHAR,
	OBJECTIVE_LEAVE_CAR_AND_DIE,
	OBJECTIVE_GOTO_SEAT_ON_FOOT,
	OBJECTIVE_GOTO_ATM_ON_FOOT,
	OBJECTIVE_FLEE_CAR,
	OBJECTIVE_SUN_BATHE,
	OBJECTIVE_GOTO_BUS_STOP_ON_FOOT,
	OBJECTIVE_GOTO_PIZZA_ON_FOOT,
	OBJECTIVE_GOTO_SHELTER_ON_FOOT,
	OBJECTIVE_AIM_GUN_AT,
	OBJECTIVE_WANDER,
	OBJECTIVE_WAIT_ON_FOOT_AT_SHELTER,
	OBJECTIVE_SPRINT_TO_AREA,
	OBJECTIVE_KILL_CHAR_ON_BOAT,
	OBJECTIVE_SOLICIT_FOOT,
	OBJECTIVE_WAIT_ON_FOOT_AT_BUS_STOP,
	OBJECTIVE_GOTO_ICE_CREAM_VAN_ON_FOOT,
	OBJECTIVE_WAIT_ON_FOOT_AT_ICE_CREAM_VAN,
	OBJ_55,
	OBJ_56,
	OBJ_57,
	OBJ_58,
	OBJ_59

};

enum {
	RANDOM_CHAR = 1,
	MISSION_CHAR,
	UNK_CHAR,
};

enum PedLineUpPhase {
	LINE_UP_TO_CAR_START,
	LINE_UP_TO_CAR_END,
	LINE_UP_TO_CAR_2, // Buggy. Used for cops arresting you from passenger door
	LINE_UP_TO_CAR_FALL
};

enum PedOnGroundState {
	NO_PED,
	PED_IN_FRONT_OF_ATTACKER,
	PED_ON_THE_FLOOR,
	PED_DEAD_ON_THE_FLOOR
};

enum PointBlankNecessity {
	NO_POINT_BLANK_PED,
	POINT_BLANK_FOR_WANTED_PED,
	POINT_BLANK_FOR_SOMEONE_ELSE
};

enum PedState
{
	PED_NONE,
	PED_IDLE,
	PED_LOOK_ENTITY,
	PED_LOOK_HEADING,
	PED_WANDER_RANGE,
	PED_WANDER_PATH,
	PED_SEEK_POS,
	PED_SEEK_ENTITY,
	PED_FLEE_POS,
	PED_FLEE_ENTITY,
	PED_PURSUE,
	PED_FOLLOW_PATH,
	PED_SNIPER_MODE,
	PED_ROCKET_MODE,
	PED_DUMMY,
	PED_PAUSE,
	PED_ATTACK,
	PED_FIGHT,//ԭ��վ�ţ�������Ծ�ָ�
	PED_FACE_PHONE,
	PED_MAKE_CALL,
	PED_CHAT,
	PED_MUG,
	PED_AIM_GUN,
	PED_AI_CONTROL,
	PED_SEEK_CAR,
	PED_SEEK_IN_BOAT,
	PED_FOLLOW_ROUTE,
	PED_CPR,
	PED_SOLICIT,
	PED_BUY_ICECREAM,
	PED_INVESTIGATE,
	PED_STEP_AWAY,
	PED_ON_FIRE,
	PED_SUN_BATHE,
	PED_FLASH,
	PED_JOG,
	PED_ANSWER_MOBILE,

	PED_UNKNOWN,	// Same with IDLE, but also infects up to 5 peds with same pedType and WANDER_PATH, so they become stone too. HANG_OUT in Fire_Head's idb

	PED_STATES_NO_AI,

	PED_ABSEIL,
	PED_SIT,
	PED_JUMP,
	PED_FALL,
	PED_GETUP,
	PED_STAGGER,
	PED_DIVE_AWAY,

	PED_STATES_NO_ST,
	PED_ENTER_TRAIN,
	PED_EXIT_TRAIN,
	PED_ARREST_PLAYER,

	PED_DRIVING,
	PED_PASSENGER,
	PED_TAXI_PASSENGER,
	PED_OPEN_DOOR,
	PED_DIE,
	PED_DEAD,
	PED_CARJACK,
	PED_DRAG_FROM_CAR,
	PED_ENTER_CAR,
	PED_STEAL_CAR,
	PED_EXIT_CAR,
	PED_HANDS_UP,
	PED_ARRESTED,
	PED_DEPLOY_STINGER
};

enum eMoveState {
	PEDMOVE_NONE,
	PEDMOVE_STILL,
	PEDMOVE_WALK,
	PEDMOVE_JOG,
	PEDMOVE_RUN,
	PEDMOVE_SPRINT,
	PEDMOVE_THROWN
};

extern float gfTommyFatness;

class CVehicle;

class CPed : public CPhysical
{
public:
#ifdef USE_CUTSCENE_SHADOW_FOR_PED
	class CCutsceneShadow *m_pRTShadow;
#endif
	// 0x128
	CStoredCollPoly m_collPoly;         // �洢��ײ�����
	float m_fCollisionSpeed;            // ��ײ�ٶ�

	// cf. https://github.com/DK22Pac/plugin-sdk/blob/master/plugin_sa/game_sa/CPed.h from R*
	uint32 bIsStanding : 1;             // �Ƿ�վ��
	uint32 bWasStanding : 1;            // �Ƿ�����վ��
	uint32 bIsAttacking : 1;            // �Ƿ����ڹ�����ȭͷս�󲻻����ã�
	uint32 bIsPointingGunAt : 1;        // �Ƿ�ָ��ĳ�￪ǹ
	uint32 bIsLooking : 1;              // �Ƿ����ڿ�
	uint32 bKeepTryingToLook : 1;       // ������ڲ��ɴ�ĽǶ��޷�����ĳ���ط�
	uint32 bIsRestoringLook : 1;        // �Ƿ����ڻָ�����
	uint32 bIsAimingGun : 1;            // �Ƿ�������׼ǹ

	uint32 bIsRestoringGun : 1;         // �Ƿ����ڻָ�ǹ��״̬
	uint32 bCanPointGunAtTarget : 1;    // �Ƿ����ָ��Ŀ�꿪ǹ
	uint32 bIsTalking : 1;              // �Ƿ�����˵��
	uint32 bIsInTheAir : 1;            // �Ƿ��ڿ���
	uint32 bIsLanding : 1;              // �Ƿ�������½
	uint32 bIsRunning : 1;              // ��ĳЩ�������Ƿ������ܲ�
	uint32 bHitSomethingLastFrame : 1;  // ��һ֡�Ƿ������ʲô����
	uint32 bVehEnterDoorIsBlocked : 1;  // ��Ϊ���˴����������˳�����ڱ�����

	uint32 bCanPedEnterSeekedCar : 1;   // �Ƿ���Խ���������������
	uint32 bRespondsToThreats : 1;      // �Ƿ���Ӧ��в
	uint32 bRenderPedInCar : 1;         // �Ƿ��ڳ�����Ⱦ����
	uint32 bChangedSeat : 1;            // �Ƿ������λ
	uint32 bUpdateAnimHeading : 1;      // �Ƿ���¶�������
	uint32 bBodyPartJustCameOff : 1;    // ���岿λ�Ƿ�ոյ���
	uint32 bIsShooting : 1;             // �Ƿ��������
	uint32 bFindNewNodeAfterStateRestore : 1; // �ָ�״̬���Ƿ�Ѱ���½ڵ�

	uint32 bHasACamera : 1;             // �����Ƿ�ӵ������Լ�¼�漰����/��ը���¹�
	uint32 bGonnaInvestigateEvent : 1;  // �Ƿ񽫵����¼�
	uint32 bPedIsBleeding : 1;          // �����Ƿ�����Ѫ
	uint32 bStopAndShoot : 1;           // �����޷���ȭͷ����Ŀ�꣬��Ҫʹ��ǹ
	uint32 bIsPedDieAnimPlaying : 1;    // �Ƿ����ڲ���������������
	uint32 bUsePedNodeSeek : 1;         // ʹ�����˽ڵ�Ѱ·
	uint32 bObjectiveCompleted : 1;     // Ŀ���Ƿ����
	uint32 bScriptObjectiveCompleted : 1;// �ű�Ŀ���Ƿ����

	uint32 bKindaStayInSamePlace : 1;   // ����ͣ����ͬһλ��
	uint32 bBeingChasedByPolice : 1;    // �Ƿ񱻾���׷��
	uint32 bNotAllowedToDuck : 1;       // �Ƿ��������
	uint32 bCrouchWhenShooting : 1;     // ���ʱ�Ƿ����
	uint32 bIsDucking : 1;              // �Ƿ����ڶ���
	uint32 bGetUpAnimStarted : 1;       // �Ƿ�ʼ������
	uint32 bDoBloodyFootprints : 1;     // �Ƿ�����Ѫ���㼣
	uint32 bFleeAfterExitingCar : 1;    // �˳��������Ƿ�����

	uint32 bWanderPathAfterExitingCar : 1; // �˳��������Ƿ�����·��
	uint32 bIsLeader : 1;                // �Ƿ����쵼��
	uint32 bDontDragMeOutCar : 1;       // ��������Ҵӳ����ϳ�����δ��ɵĹ��ܣ�
	uint32 m_ped_flagF8 : 1;
	uint32 bWillBeQuickJacked : 1;      // �Ƿ�ᱻ��������
	uint32 bCancelEnteringCar : 1;      // �Ŵ򿪻����������޷��򿪺�ȡ����������
	uint32 bObstacleShowedUpDuringKillObjective : 1; // ��ִ��ɱ¾�����ڼ��ϰ������
	uint32 bDuckAndCover : 1;           // Ѽ�Ӻ��ڻ�

	uint32 bStillOnValidPoly : 1;       // ����������ڵĶ������Ȼ��Ч������ײ������
	uint32 bAllowMedicsToReviveMe : 1;  // �Ƿ�����ҽ��������
	uint32 bResetWalkAnims : 1;         // �������߶���
	uint32 bStartWanderPathOnFoot : 1;  // ������ڳ��ϣ����³����ҵ�·��������
	uint32 bOnBoat : 1;                 // �������Ǽ�ʻԱ��������վ�ڴ���
	uint32 bBusJacked : 1;              // ��������������
	uint32 bGonnaKillTheCarJacker : 1;  // ֻ�е����Ҳ��������������ұ���������ʱ����
	uint32 bFadeOut : 1;                // ����

	uint32 bKnockedUpIntoAir : 1;       // �Ƿ�������ײ�������
	uint32 bHitSteepSlope : 1;          // �Ƿ��붸�·�����ײ��վ�ڶ����ϣ��������ͣ�
	uint32 bCullExtraFarAway : 1;       // ��������ֻ���ڷǳ�Զ�ĵط��Żᱻ�޳�������·�ϣ�
	uint32 bClearObjective : 1;         // ���Ŀ��
	uint32 bTryingToReachDryLand : 1;   // �ո��뿪������ͼ������������
	uint32 bCollidedWithMyVehicle : 1;  // �Ƿ����ҵĳ���������ײ
	uint32 bRichFromMugging : 1;        // �����кܶ�Ǯ����Ϊ����һֱ�����ٱ���
	uint32 bChrisCriminal : 1;          // �ǿ���˹�ľ���������ɱ�����ﷸ��Ӧ����Ϊ���ࣩ

	uint32 bShakeFist : 1;              // ���Զ�׼ʵ���ȭ
	uint32 bNoCriticalHits : 1;         // ������ã�֫�岻�����
	uint32 bVehExitWillBeInstant : 1;   // �˳���������˲���
	uint32 bHasAlreadyBeenRecorded : 1; // �Ѿ���¼�ƹ�
	uint32 bFallenDown : 1;             // �Ƿ�ˤ��
	uint32 bDontAcceptIKLookAts : 1;    // ������ IK ����
	uint32 bReachedAttractorHeadingTarget : 1; // �Ƿ�ﵽ����������Ŀ��
	uint32 bTurnedAroundOnAttractor : 1;// ����������ת����

	uint32 bHasAlreadyUsedAttractor : 1; // �Ƿ��Ѿ�ʹ�ù�������
	uint32 bHasAlreadyStoleACar : 1;     // �Ƿ��Ѿ�͵��һ����
	uint32 bCarPassenger : 1;            // �Ƿ��������˿�
	uint32 bFleeWhenStanding : 1;        // վ��ʱ�Ƿ�����
	uint32 bGotUpOfMyOwnAccord : 1;      // �Ƿ�����վ����
	uint32 bMiamiViceCop : 1;            // �����ܷ��ƾ���
	uint32 bMoneyHasBeenGivenByScript : 1; // �ű��Ƿ��Ѿ������Ǯ
	uint32 bHasBeenPhotographed : 1;     // �Ƿ��Ѿ�������

	uint32 bIsDrowning : 1;              // �Ƿ���ˮ
	uint32 bDrownsInWater : 1;           // �Ƿ����ˮ����ˮ
	uint32 bWaitForLeaderToComeCloser : 1; // �ȴ��쵼�߿���
	uint32 bHeldHostageInCar : 1;        // �Ƿ��������б�Ю��
	uint32 bIsPlayerFriend : 1;          // �Ƿ�����ҵ�����
	uint32 bHeadStuckInCollision : 1;    // ͷ���Ƿ�����ײ��
	uint32 bDeadPedInFrontOfCar : 1;     // ����ǰ���Ƿ�����ʬ
	uint32 bStayInCarOnJack : 1;         // ����ʱ�Ƿ����ڳ���

	uint32 bDontFight : 1;               // ��Ҫ���
	uint32 bDoomAim : 1;                 // ��Ŀ����׼
	uint32 bCanBeShotInVehicle : 1;      // �Ƿ�����ڳ��ڱ����
	uint32 bCanGiveUpSunbathing : 1;     // �Ƿ���Է���ɹ̫��
	uint32 bMakeFleeScream : 1;          // ����ʱ�Ƿ���
	uint32 bPushedAlongByCar : 1;        // �Ƿ���������
	uint32 bRemoveMeWhenIGotIntoCar : 1; // �ҽ����������Ƴ���
	uint32 bIgnoreThreatsBehindObjects : 1; // ��������������в

	uint32 bNeverEverTargetThisPed : 1;  // ��Զ��Ҫ����������ΪĿ��
	uint32 bCrouchWhenScared : 1;        // ����ʱ�Ƿ����
	uint32 bKnockedOffBike : 1;          // �Ƿ�����г��ϱ�ײ����
	uint32 b158_8 : 1;
	uint32 bCollectBusFare : 1;          // �ռ�������������
	uint32 bBoughtIceCream : 1;          // ���˱����
	uint32 bDonePositionOutOfCollision : 1; // λ���ѵ����Ա�����ײ
	uint32 bCanAttackPlayerWithCops : 1; // �Ƿ�����þ��칥�����

#ifdef KANGAROO_CHEAT
	// �Զ����־
	uint32 m_ped_flagI80 : 1; // KANGAROO_CHEAT ����ʹ�ô���Ϊ���׿��� 
#endif

	uint8 m_gangFlags;                      // ���ɱ�־
	uint8 m_unused15D;                      // ��Щ����������ֽڣ���ʵ�����ǳ�Ա...
	uint8 m_unused15E;
	uint8 m_unused15F;
	uint8 CharCreatedBy;                   // �ַ�������
	eObjective m_objective;                 // Ŀ��
	eObjective m_prevObjective;             // ǰһ��Ŀ��
	CPed* m_pedInObjective;               // Ŀ���е�����
	CVehicle* m_carInObjective;             // Ŀ���е�����
	CVector m_nextRoutePointPos;            // ��һ��·�ߵ��λ��
	float m_attractorHeading;               // ����������
	CPed* m_leader;                       // �쵼��
	eFormation m_pedFormation;              // ���˶���
	uint32 m_fearFlags;                     // �־��־
	CEntity* m_threatEntity;                // ��вʵ��
	CVector2D m_eventOrThreat;              // �¼�����в
	uint32 m_eventType;                     // �¼�����
	CEntity* m_pEventEntity;                // �¼�ʵ��
	float m_fAngleToEvent;                  // ���¼��ĽǶ�
	AnimBlendFrameData* m_pFrames[18];      // �������֡����
	void* m_pWeaponModel;                   // ����ģ��ָ��
	AssocGroupId m_animGroup;               // ������
	CAnimBlendAssociation* m_pVehicleAnim;  // ������������
	CVector2D m_vecAnimMoveDelta;           // �����ƶ�����
	CVector m_vecOffsetSeek;                // Ѱ��ƫ����
	CPedIK m_pedIK;                         // ���� IK
	float m_actionX;                        // ���� X
	float m_actionY;                        // ���� Y
	uint32 m_nPedStateTimer;                // ����״̬��ʱ��
	PedState m_nPedState;                   // ����״̬
	PedState m_nLastPedState;               // ��һ������״̬
	eMoveState m_nMoveState;                // �ƶ�״̬
	int32 m_nStoredMoveState;               // �洢���ƶ�״̬
	int32 m_nPrevMoveState;                 // ��һ���ƶ�״̬
	eWaitState m_nWaitState;                // �ȴ�״̬
	uint32 m_nWaitTimer;                    // �ȴ���ʱ��
	CPathNode* m_pathNodesToGo[8];          // ǰ����·���ڵ�
	int16 m_nNumPathNodes;                  // ·���ڵ�����
	int16 m_nCurPathNodeId;                 // ��ǰ·���ڵ� ID
	CEntity* m_followPathWalkAroundEnt;     // ����·���ƹ���ʵ��
	CEntity* m_followPathTargetEnt;         // ����·����Ŀ��ʵ��
	uint32 m_pathNodeTimer;                 // ·���ڵ��ʱ��
	CPathNode m_pathNodeObjPool[8];         // ·���ڵ�����
	CPathNode* m_pCurPathNode;              // ��ǰ·���ڵ�ָ��
	int8 m_nPathDir;                        // ·������
	CPathNode* m_pLastPathNode;             // ��һ��·���ڵ�ָ��
	CPathNode* m_pNextPathNode;             // ��һ��·���ڵ�ָ��
	CVector m_followPathDestPos;            // ����·����Ŀ�ĵ�λ��
	float m_followPathAbortDist;            // ����·���жϾ���
	eMoveState m_followPathMoveState;       // ����·���ƶ�״̬
	float m_fHealth;                        // ����ֵ
	float m_fArmour;                        // ����ֵ
	uint32 m_nExtendedRangeTimer;           // ��չ��Χ��ʱ��
	int16 m_routeLastPoint;                 // ·�����һ����
	uint16 m_routeStartPoint;               // ·�����
	int16 m_routePointsPassed;              // ������·�ߵ�
	int16 m_routeType;                      // �� PedRouteType
	int16 m_routePointsBeingPassed;         // ���ھ�����·�ߵ�
	CVector2D m_moved;                      // �ƶ��ľ���
	float m_fRotationCur;                   // ��ǰ��ת�Ƕ�
	float m_fRotationDest;                  // Ŀ����ת�Ƕ�
	float m_headingRate;                    // ����仯��
	uint16 m_vehDoor;                       // ������
	int16 m_walkAroundType;                 // ��������
	CPhysical* m_pCurrentPhysSurface;       // ��ǰ�������
	CVector m_vecOffsetFromPhysSurface;     // ����������ƫ����
	CEntity* m_pCurSurface;                 // ��ǰ����
	CVector m_vecSeekPos;                   // Ѱ��λ��
	CEntity* m_pSeekTarget;                 // Ѱ��Ŀ��
	CVehicle* m_pMyVehicle;                 // �ҵ�����
	bool bInVehicle;                        // �Ƿ��ڳ���
	float m_distanceToCountSeekDone;        // ����Ѱ����ɵľ���
	float m_acceptableHeadingOffset;        // ���ܵĳ���ƫ��
	CVehicle* m_vehicleInAccident;          // �����¹ʵ�����
	CPedAttractor* m_attractor;             // ������
	int32 m_positionInQueue;                // �����е�λ��
	bool bRunningToPhone;                   // �Ƿ��ܵ��绰��
	int16 m_phoneId;                        // �绰ID
	eCrimeType m_crimeToReportOnPhone;      // Ҫ������绰�ķ�������
	uint32 m_phoneTalkTimer;                // �绰ͨ����ʱ��
	CAccident* m_lastAccident;              // ��һ���¹�
	uint32 m_nPedType;                      // ��������
	CPedStats* m_pedStats;                  // ����ͳ������
	CVector2D m_fleeFromPos;                // �����λ��
	CEntity* m_fleeFrom;                    // ����Ķ���
	uint32 m_fleeTimer;                     // �����ʱ��
	CEntity* m_threatEx;                    // TODO(Miami): ����ʲô��
	CEntity* m_collidingEntityWhileFleeing; // ����ʱ��ײ��ʵ��
	uint32 m_collidingThingTimer;           // ��ײ��ʱ��
	CEntity* m_pCollidingEntity;            // ��ײʵ��
	uint8 m_stateUnused;                    // δʹ�õ�״̬
	uint32 m_timerUnused;                   // δʹ�õļ�ʱ��
	class CRange2D* m_wanderRangeBounds;    // ���η�Χ�߽�
	CWeapon m_weapons[TOTAL_WEAPON_SLOTS];  // ������
	eWeaponType m_storedWeapon;             // �洢������
	eWeaponType m_delayedWeapon;            // �ӳٵ�����
	uint32 m_delayedWeaponAmmo;             // �ӳٵ�������ҩ
	uint8 m_currentWeapon;                  // ��ǰ������eWeaponType��
	uint8 m_maxWeaponTypeAllowed;           // ���������������ͣ�eWeaponType��
	uint8 m_wepSkills;                      // ��������
	uint8 m_wepAccuracy;                    // ��������
	CEntity* m_pPointGunAt;                 // ָ���ʵ��
	CVector m_vecHitLastPos;                // ���һ�δ����λ��
	uint32 m_curFightMove;                  // ��ǰս������
	uint32 m_lastFightMove;                 // ��һ��ս������
	uint8 m_fightButtonPressure;            // ս����ťѹ��
	int8 m_fightState;                      // ս��״̬
	bool m_takeAStepAfterAttack;            // �������Ƿ�����һ��
	uint8 m_bleedCounter;                   // ��Ѫ������
	CFire* m_pFire;                         // ����ָ��
	CEntity* m_pLookTarget;                 // ������Ŀ��
	float m_fLookDirection;                 // �����ķ���
	int32 m_wepModelID;                     // ����ģ��ID
	uint32 m_leaveCarTimer;                 // �뿪����ʱ��
	uint32 m_getUpTimer;                    // �����ʱ��
	uint32 m_lookTimer;                     // ������ʱ��
	uint32 m_chatTimer;                     // �Ի���ʱ��
	uint32 m_attackTimer;                   // ������ʱ��
	uint32 m_shootTimer;                    // �����ʱ��������ǹ�����һ���֣�
	uint32 m_carJackTimer;                  // ������ʱ��
	uint32 m_objectiveTimer;                // Ŀ���ʱ��
	uint32 m_duckTimer;                     // ���¼�ʱ��
	uint32 m_duckAndCoverTimer;             // Ѽ�Ӻ��ڻ���ʱ��
	uint32 m_bloodyFootprintCountOrDeathTime; // Ѫ������������ʱ�䣨��� bDoBloodyFootprints Ϊ false����Ϊ����ʱ�䣩
	uint32 m_shotTime;                      // ��ǹʱ��
	uint32 m_ceaseAttackTimer;              // ֹͣ������ʱ��
	uint8 m_panicCounter;                   // �ֻż�����
	bool m_deadBleeding;                    // ������Ѫ
	int8 m_bodyPartBleeding;                // ��Ѫ�����岿λ��PedNode�������û����Ϊ -1
	CPed* m_nearPeds[10];                 // ����������
	uint16 m_numNearPeds;                   // �������˵�����
	uint16 m_nPedMoney;                     // ����ӵ�еĽ�Ǯ
	int8 m_lastWepDam;                      // �ϴ������˺�
	CEntity* m_lastDamEntity;               // �ϴ�����˺���ʵ��
	CEntity* m_attachedTo;                  // ���ŵ�ʵ��
	CVector m_vecAttachOffset;              // ����ƫ����
	uint16 m_attachType;                    // ��������
	float m_attachRotStep;                  // ������ת����
	uint32 m_attachWepAmmo;                 // ����������ҩ
	uint32 m_threatFlags;                   // ��в��־
	uint32 m_threatCheckTimer;              // ��в����ʱ��
	uint32 m_threatCheckInterval;           // ��в�����
	int32 m_delayedSoundID;                 // �ӳٵ�����ID
	uint32 m_delayedSoundTimer;             // �ӳٵ�������ʱ��
	uint32 m_lastSoundStart;                // ��һ��������ʼʱ��
	uint32 m_soundStart;                    // ������ʼʱ��
	uint16 m_lastQueuedSound;               // ��һ���Ŷӵ�����
	uint16 m_queuedSound;                   // �Ŷӵ�����
	bool m_canTalk;                         // �Ƿ����˵��
	uint32 m_lastComment;                   // ��һ������
	CVector m_vecSpotToGuard;               // �����ĵص�
	float m_radiusToGuard;                  // �����뾶

	
};

void FinishTalkingOnMobileCB(CAnimBlendAssociation* assoc, void* arg);
void StartTalkingOnMobileCB(CAnimBlendAssociation* assoc, void* arg);
void PlayRandomAnimationsFromAnimBlock(CPed* ped, AssocGroupId animGroup, uint32 first, uint32 amount);

VALIDATE_SIZE(CPed, 0x5F4);

bool IsPedPointerValid(CPed*);
bool IsPedPointerValid_NotInWorld(CPed*);
