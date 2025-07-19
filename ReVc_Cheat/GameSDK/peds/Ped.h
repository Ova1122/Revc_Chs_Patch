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
	PED_FIGHT,//原地站着，可以跳跃恢复
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
	CStoredCollPoly m_collPoly;         // 存储碰撞多边形
	float m_fCollisionSpeed;            // 碰撞速度

	// cf. https://github.com/DK22Pac/plugin-sdk/blob/master/plugin_sa/game_sa/CPed.h from R*
	uint32 bIsStanding : 1;             // 是否站立
	uint32 bWasStanding : 1;            // 是否曾经站立
	uint32 bIsAttacking : 1;            // 是否正在攻击（拳头战后不会重置）
	uint32 bIsPointingGunAt : 1;        // 是否指向某物开枪
	uint32 bIsLooking : 1;              // 是否正在看
	uint32 bKeepTryingToLook : 1;       // 如果由于不可达的角度无法看向某个地方
	uint32 bIsRestoringLook : 1;        // 是否正在恢复视线
	uint32 bIsAimingGun : 1;            // 是否正在瞄准枪

	uint32 bIsRestoringGun : 1;         // 是否正在恢复枪的状态
	uint32 bCanPointGunAtTarget : 1;    // 是否可以指向目标开枪
	uint32 bIsTalking : 1;              // 是否正在说话
	uint32 bIsInTheAir : 1;            // 是否在空中
	uint32 bIsLanding : 1;              // 是否正在着陆
	uint32 bIsRunning : 1;              // 在某些条件下是否正在跑步
	uint32 bHitSomethingLastFrame : 1;  // 上一帧是否击中了什么东西
	uint32 bVehEnterDoorIsBlocked : 1;  // 因为有人从那里进入或退出，入口被阻塞

	uint32 bCanPedEnterSeekedCar : 1;   // 是否可以进入搜索到的汽车
	uint32 bRespondsToThreats : 1;      // 是否响应威胁
	uint32 bRenderPedInCar : 1;         // 是否在车内渲染行人
	uint32 bChangedSeat : 1;            // 是否更换座位
	uint32 bUpdateAnimHeading : 1;      // 是否更新动画朝向
	uint32 bBodyPartJustCameOff : 1;    // 身体部位是否刚刚掉落
	uint32 bIsShooting : 1;             // 是否正在射击
	uint32 bFindNewNodeAfterStateRestore : 1; // 恢复状态后是否寻找新节点

	uint32 bHasACamera : 1;             // 行人是否拥有相机以记录涉及火灾/爆炸的事故
	uint32 bGonnaInvestigateEvent : 1;  // 是否将调查事件
	uint32 bPedIsBleeding : 1;          // 行人是否在流血
	uint32 bStopAndShoot : 1;           // 行人无法用拳头到达目标，需要使用枪
	uint32 bIsPedDieAnimPlaying : 1;    // 是否正在播放行人死亡动画
	uint32 bUsePedNodeSeek : 1;         // 使用行人节点寻路
	uint32 bObjectiveCompleted : 1;     // 目标是否完成
	uint32 bScriptObjectiveCompleted : 1;// 脚本目标是否完成

	uint32 bKindaStayInSamePlace : 1;   // 大致停留在同一位置
	uint32 bBeingChasedByPolice : 1;    // 是否被警察追逐
	uint32 bNotAllowedToDuck : 1;       // 是否不允许蹲下
	uint32 bCrouchWhenShooting : 1;     // 射击时是否蹲下
	uint32 bIsDucking : 1;              // 是否正在蹲下
	uint32 bGetUpAnimStarted : 1;       // 是否开始起身动画
	uint32 bDoBloodyFootprints : 1;     // 是否留下血腥足迹
	uint32 bFleeAfterExitingCar : 1;    // 退出汽车后是否逃跑

	uint32 bWanderPathAfterExitingCar : 1; // 退出汽车后是否漫游路径
	uint32 bIsLeader : 1;                // 是否是领导者
	uint32 bDontDragMeOutCar : 1;       // 不允许把我从车里拖出来（未完成的功能）
	uint32 m_ped_flagF8 : 1;
	uint32 bWillBeQuickJacked : 1;      // 是否会被快速抢夺
	uint32 bCancelEnteringCar : 1;      // 门打开或因锁定而无法打开后取消进入汽车
	uint32 bObstacleShowedUpDuringKillObjective : 1; // 在执行杀戮任务期间障碍物出现
	uint32 bDuckAndCover : 1;           // 鸭子和掩护

	uint32 bStillOnValidPoly : 1;       // 如果行人所在的多边形仍然有效用于碰撞则设置
	uint32 bAllowMedicsToReviveMe : 1;  // 是否允许医生复活我
	uint32 bResetWalkAnims : 1;         // 重置行走动画
	uint32 bStartWanderPathOnFoot : 1;  // 如果他在车上，则下车，找到路径后重置
	uint32 bOnBoat : 1;                 // 不仅仅是驾驶员，可能是站在船上
	uint32 bBusJacked : 1;              // 公共汽车被抢夺
	uint32 bGonnaKillTheCarJacker : 1;  // 只有当从右侧门抢夺汽车并且被捕警逮捕时设置
	uint32 bFadeOut : 1;                // 渐隐

	uint32 bKnockedUpIntoAir : 1;       // 是否被汽车碰撞弹入空中
	uint32 bHitSteepSlope : 1;          // 是否与陡坡发生碰撞或站在陡坡上（表面类型）
	uint32 bCullExtraFarAway : 1;       // 特殊行人只有在非常远的地方才会被剔除（用于路障）
	uint32 bClearObjective : 1;         // 清除目标
	uint32 bTryingToReachDryLand : 1;   // 刚刚离开船并试图到达干燥的土地
	uint32 bCollidedWithMyVehicle : 1;  // 是否与我的车辆发生碰撞
	uint32 bRichFromMugging : 1;        // 行人有很多钱是因为他们一直在抢劫别人
	uint32 bChrisCriminal : 1;          // 是克里斯的警察任务中杀死的罪犯（应被视为此类）

	uint32 bShakeFist : 1;              // 测试对准实体挥拳
	uint32 bNoCriticalHits : 1;         // 如果设置，肢体不会掉落
	uint32 bVehExitWillBeInstant : 1;   // 退出汽车将是瞬间的
	uint32 bHasAlreadyBeenRecorded : 1; // 已经被录制过
	uint32 bFallenDown : 1;             // 是否摔倒
	uint32 bDontAcceptIKLookAts : 1;    // 不接受 IK 视线
	uint32 bReachedAttractorHeadingTarget : 1; // 是否达到吸引器朝向目标
	uint32 bTurnedAroundOnAttractor : 1;// 在吸引器处转过身

	uint32 bHasAlreadyUsedAttractor : 1; // 是否已经使用过吸引器
	uint32 bHasAlreadyStoleACar : 1;     // 是否已经偷了一辆车
	uint32 bCarPassenger : 1;            // 是否是汽车乘客
	uint32 bFleeWhenStanding : 1;        // 站立时是否逃跑
	uint32 bGotUpOfMyOwnAccord : 1;      // 是否自行站起来
	uint32 bMiamiViceCop : 1;            // 迈阿密风云警察
	uint32 bMoneyHasBeenGivenByScript : 1; // 脚本是否已经给予金钱
	uint32 bHasBeenPhotographed : 1;     // 是否已经被拍照

	uint32 bIsDrowning : 1;              // 是否溺水
	uint32 bDrownsInWater : 1;           // 是否会在水中溺水
	uint32 bWaitForLeaderToComeCloser : 1; // 等待领导者靠近
	uint32 bHeldHostageInCar : 1;        // 是否在汽车中被挟持
	uint32 bIsPlayerFriend : 1;          // 是否是玩家的朋友
	uint32 bHeadStuckInCollision : 1;    // 头部是否卡在碰撞中
	uint32 bDeadPedInFrontOfCar : 1;     // 汽车前面是否有死尸
	uint32 bStayInCarOnJack : 1;         // 抢劫时是否留在车里

	uint32 bDontFight : 1;               // 不要打架
	uint32 bDoomAim : 1;                 // 向目标瞄准
	uint32 bCanBeShotInVehicle : 1;      // 是否可以在车内被射击
	uint32 bCanGiveUpSunbathing : 1;     // 是否可以放弃晒太阳
	uint32 bMakeFleeScream : 1;          // 逃跑时是否尖叫
	uint32 bPushedAlongByCar : 1;        // 是否被汽车推走
	uint32 bRemoveMeWhenIGotIntoCar : 1; // 我进入汽车后移除我
	uint32 bIgnoreThreatsBehindObjects : 1; // 忽略物体后面的威胁

	uint32 bNeverEverTargetThisPed : 1;  // 永远不要把这行人设为目标
	uint32 bCrouchWhenScared : 1;        // 害怕时是否蹲下
	uint32 bKnockedOffBike : 1;          // 是否从自行车上被撞下来
	uint32 b158_8 : 1;
	uint32 bCollectBusFare : 1;          // 收集公共汽车费用
	uint32 bBoughtIceCream : 1;          // 买了冰淇淋
	uint32 bDonePositionOutOfCollision : 1; // 位置已调整以避免碰撞
	uint32 bCanAttackPlayerWithCops : 1; // 是否可以用警察攻击玩家

#ifdef KANGAROO_CHEAT
	// 自定义标志
	uint32 m_ped_flagI80 : 1; // KANGAROO_CHEAT 定义使用此作为作弊开关 
#endif

	uint8 m_gangFlags;                      // 帮派标志
	uint8 m_unused15D;                      // 这些不能是填充字节，但实际上是成员...
	uint8 m_unused15E;
	uint8 m_unused15F;
	uint8 CharCreatedBy;                   // 字符创建者
	eObjective m_objective;                 // 目标
	eObjective m_prevObjective;             // 前一个目标
	CPed* m_pedInObjective;               // 目标中的行人
	CVehicle* m_carInObjective;             // 目标中的汽车
	CVector m_nextRoutePointPos;            // 下一个路线点的位置
	float m_attractorHeading;               // 吸引器朝向
	CPed* m_leader;                       // 领导者
	eFormation m_pedFormation;              // 行人队形
	uint32 m_fearFlags;                     // 恐惧标志
	CEntity* m_threatEntity;                // 威胁实体
	CVector2D m_eventOrThreat;              // 事件或威胁
	uint32 m_eventType;                     // 事件类型
	CEntity* m_pEventEntity;                // 事件实体
	float m_fAngleToEvent;                  // 到事件的角度
	AnimBlendFrameData* m_pFrames[18];      // 动画混合帧数据
	void* m_pWeaponModel;                   // 武器模型指针
	AssocGroupId m_animGroup;               // 动画组
	CAnimBlendAssociation* m_pVehicleAnim;  // 车辆动画关联
	CVector2D m_vecAnimMoveDelta;           // 动画移动增量
	CVector m_vecOffsetSeek;                // 寻找偏移量
	CPedIK m_pedIK;                         // 行人 IK
	float m_actionX;                        // 动作 X
	float m_actionY;                        // 动作 Y
	uint32 m_nPedStateTimer;                // 行人状态计时器
	PedState m_nPedState;                   // 行人状态
	PedState m_nLastPedState;               // 上一个行人状态
	eMoveState m_nMoveState;                // 移动状态
	int32 m_nStoredMoveState;               // 存储的移动状态
	int32 m_nPrevMoveState;                 // 上一个移动状态
	eWaitState m_nWaitState;                // 等待状态
	uint32 m_nWaitTimer;                    // 等待计时器
	CPathNode* m_pathNodesToGo[8];          // 前往的路径节点
	int16 m_nNumPathNodes;                  // 路径节点数量
	int16 m_nCurPathNodeId;                 // 当前路径节点 ID
	CEntity* m_followPathWalkAroundEnt;     // 跟随路径绕过的实体
	CEntity* m_followPathTargetEnt;         // 跟随路径的目标实体
	uint32 m_pathNodeTimer;                 // 路径节点计时器
	CPathNode m_pathNodeObjPool[8];         // 路径节点对象池
	CPathNode* m_pCurPathNode;              // 当前路径节点指针
	int8 m_nPathDir;                        // 路径方向
	CPathNode* m_pLastPathNode;             // 上一个路径节点指针
	CPathNode* m_pNextPathNode;             // 下一个路径节点指针
	CVector m_followPathDestPos;            // 跟随路径的目的地位置
	float m_followPathAbortDist;            // 跟随路径中断距离
	eMoveState m_followPathMoveState;       // 跟随路径移动状态
	float m_fHealth;                        // 生命值
	float m_fArmour;                        // 护甲值
	uint32 m_nExtendedRangeTimer;           // 扩展范围计时器
	int16 m_routeLastPoint;                 // 路线最后一个点
	uint16 m_routeStartPoint;               // 路线起点
	int16 m_routePointsPassed;              // 经过的路线点
	int16 m_routeType;                      // 见 PedRouteType
	int16 m_routePointsBeingPassed;         // 正在经过的路线点
	CVector2D m_moved;                      // 移动的距离
	float m_fRotationCur;                   // 当前旋转角度
	float m_fRotationDest;                  // 目标旋转角度
	float m_headingRate;                    // 朝向变化率
	uint16 m_vehDoor;                       // 汽车门
	int16 m_walkAroundType;                 // 绕行类型
	CPhysical* m_pCurrentPhysSurface;       // 当前物理表面
	CVector m_vecOffsetFromPhysSurface;     // 从物理表面的偏移量
	CEntity* m_pCurSurface;                 // 当前表面
	CVector m_vecSeekPos;                   // 寻找位置
	CEntity* m_pSeekTarget;                 // 寻找目标
	CVehicle* m_pMyVehicle;                 // 我的汽车
	bool bInVehicle;                        // 是否在车内
	float m_distanceToCountSeekDone;        // 计算寻找完成的距离
	float m_acceptableHeadingOffset;        // 接受的朝向偏差
	CVehicle* m_vehicleInAccident;          // 发生事故的汽车
	CPedAttractor* m_attractor;             // 吸引器
	int32 m_positionInQueue;                // 队列中的位置
	bool bRunningToPhone;                   // 是否跑到电话旁
	int16 m_phoneId;                        // 电话ID
	eCrimeType m_crimeToReportOnPhone;      // 要报告给电话的犯罪类型
	uint32 m_phoneTalkTimer;                // 电话通话计时器
	CAccident* m_lastAccident;              // 上一个事故
	uint32 m_nPedType;                      // 行人类型
	CPedStats* m_pedStats;                  // 行人统计数据
	CVector2D m_fleeFromPos;                // 逃离的位置
	CEntity* m_fleeFrom;                    // 逃离的对象
	uint32 m_fleeTimer;                     // 逃离计时器
	CEntity* m_threatEx;                    // TODO(Miami): 这是什么？
	CEntity* m_collidingEntityWhileFleeing; // 逃离时碰撞的实体
	uint32 m_collidingThingTimer;           // 碰撞计时器
	CEntity* m_pCollidingEntity;            // 碰撞实体
	uint8 m_stateUnused;                    // 未使用的状态
	uint32 m_timerUnused;                   // 未使用的计时器
	class CRange2D* m_wanderRangeBounds;    // 漫游范围边界
	CWeapon m_weapons[TOTAL_WEAPON_SLOTS];  // 武器槽
	eWeaponType m_storedWeapon;             // 存储的武器
	eWeaponType m_delayedWeapon;            // 延迟的武器
	uint32 m_delayedWeaponAmmo;             // 延迟的武器弹药
	uint8 m_currentWeapon;                  // 当前武器（eWeaponType）
	uint8 m_maxWeaponTypeAllowed;           // 允许的最大武器类型（eWeaponType）
	uint8 m_wepSkills;                      // 武器技能
	uint8 m_wepAccuracy;                    // 武器精度
	CEntity* m_pPointGunAt;                 // 指向的实体
	CVector m_vecHitLastPos;                // 最后一次打击的位置
	uint32 m_curFightMove;                  // 当前战斗动作
	uint32 m_lastFightMove;                 // 上一个战斗动作
	uint8 m_fightButtonPressure;            // 战斗按钮压力
	int8 m_fightState;                      // 战斗状态
	bool m_takeAStepAfterAttack;            // 攻击后是否迈出一步
	uint8 m_bleedCounter;                   // 出血计数器
	CFire* m_pFire;                         // 火焰指针
	CEntity* m_pLookTarget;                 // 看望的目标
	float m_fLookDirection;                 // 看望的方向
	int32 m_wepModelID;                     // 武器模型ID
	uint32 m_leaveCarTimer;                 // 离开车计时器
	uint32 m_getUpTimer;                    // 起身计时器
	uint32 m_lookTimer;                     // 看望计时器
	uint32 m_chatTimer;                     // 对话计时器
	uint32 m_attackTimer;                   // 攻击计时器
	uint32 m_shootTimer;                    // 射击计时器（射击是攻击的一部分）
	uint32 m_carJackTimer;                  // 抢车计时器
	uint32 m_objectiveTimer;                // 目标计时器
	uint32 m_duckTimer;                     // 蹲下计时器
	uint32 m_duckAndCoverTimer;             // 鸭子和掩护计时器
	uint32 m_bloodyFootprintCountOrDeathTime; // 血迹计数或死亡时间（如果 bDoBloodyFootprints 为 false，则为死亡时间）
	uint32 m_shotTime;                      // 开枪时间
	uint32 m_ceaseAttackTimer;              // 停止攻击计时器
	uint8 m_panicCounter;                   // 恐慌计数器
	bool m_deadBleeding;                    // 死亡出血
	int8 m_bodyPartBleeding;                // 出血的身体部位（PedNode），如果没有则为 -1
	CPed* m_nearPeds[10];                 // 附近的行人
	uint16 m_numNearPeds;                   // 附近行人的数量
	uint16 m_nPedMoney;                     // 行人拥有的金钱
	int8 m_lastWepDam;                      // 上次武器伤害
	CEntity* m_lastDamEntity;               // 上次造成伤害的实体
	CEntity* m_attachedTo;                  // 附着的实体
	CVector m_vecAttachOffset;              // 附着偏移量
	uint16 m_attachType;                    // 附着类型
	float m_attachRotStep;                  // 附着旋转步长
	uint32 m_attachWepAmmo;                 // 附着武器弹药
	uint32 m_threatFlags;                   // 威胁标志
	uint32 m_threatCheckTimer;              // 威胁检查计时器
	uint32 m_threatCheckInterval;           // 威胁检查间隔
	int32 m_delayedSoundID;                 // 延迟的声音ID
	uint32 m_delayedSoundTimer;             // 延迟的声音计时器
	uint32 m_lastSoundStart;                // 上一次声音开始时间
	uint32 m_soundStart;                    // 声音开始时间
	uint16 m_lastQueuedSound;               // 上一次排队的声音
	uint16 m_queuedSound;                   // 排队的声音
	bool m_canTalk;                         // 是否可以说话
	uint32 m_lastComment;                   // 上一次评论
	CVector m_vecSpotToGuard;               // 守卫的地点
	float m_radiusToGuard;                  // 守卫半径

	
};

void FinishTalkingOnMobileCB(CAnimBlendAssociation* assoc, void* arg);
void StartTalkingOnMobileCB(CAnimBlendAssociation* assoc, void* arg);
void PlayRandomAnimationsFromAnimBlock(CPed* ped, AssocGroupId animGroup, uint32 first, uint32 amount);

VALIDATE_SIZE(CPed, 0x5F4);

bool IsPedPointerValid(CPed*);
bool IsPedPointerValid_NotInWorld(CPed*);
