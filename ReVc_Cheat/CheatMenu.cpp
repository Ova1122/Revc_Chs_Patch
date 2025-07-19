#include "CheatMenu.h"
#include "imgui.h"

#include "CheatList.h"
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <stringapiset.h>

#include <PlayerPed.h>
#include "Vehicle.h" 
//#include<PlayerInfo.h>
//#include<ModelIndices.h>
//UTF-8到GB2312的转换
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}
//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

struct VehModel
{
	int id;
	const char* name;
	//int type;
};
VehModel gtaVC_VehModels[] =
{
  { MI_LANDSTAL,   "陆地游侠" },
  { MI_IDAHO,      "爱达荷轿车" },
  { MI_STINGER,    "毒刺跑车" },
  { MI_LINERUN,    "货运卡车" },
  { MI_PEREN,      "佩伦商队" },
  { MI_SENTINEL,   "哨兵轿车" },
  { MI_RIO,        "里约轿车" },
  { MI_FIRETRUCK,  "消防车" },
  { MI_TRASH,      "垃圾车" },
  { MI_STRETCH,    "加长礼车" },
  { MI_MANANA,     "玛娜娜轿车" },
  { MI_INFERNUS,   "地狱火跑车" },
  { MI_VOODOO,     "巫毒老爷车" },
  { MI_PONY,       "小马货车" },
  { MI_MULE,       "骡子货车" },
  { MI_CHEETAH,    "猎豹跑车" },
  { MI_AMBULAN,    "救护车" },
  { MI_FBICAR,     "FBI专用车" },
  { MI_MOONBEAM,   "月光货车" },
  { MI_ESPERANT,   "希望之星轿车" },
  { MI_TAXI,       "出租车" },
  { MI_WASHING,    "华盛顿轿车" },
  { MI_BOBCAT,     "山猫皮卡" },
  { MI_MRWHOOP,    "冰淇淋车" },
  { MI_BFINJECT,   "BF Injection沙滩车" },
  { MI_HUNTER,     "阿帕奇直升机" },
  { MI_POLICE,     "警车" },
  { MI_ENFORCER,   "特警车" },
  { MI_SECURICA,   "保安车" },
  { MI_BANSHEE,    "女妖跑车" },
  { MI_PREDATOR,   "掠食者快艇" },
  { MI_BUS,        "巴士" },
  { MI_RHINO,      "犀牛坦克" },
  { MI_BARRACKS,   "军用卡车" },
  { MI_CUBAN,      "古巴帮车辆" },
 // { MI_CHOPPER,    "哈雷摩托车(有问题)" },
  { MI_ANGEL,      "天使摩托车" },
  { MI_COACH,      "长途客车" },
  { MI_CABBIE,     "老式出租车" },
  { MI_STALLION,   "种马跑车" },
  { MI_RUMPO,      "伦帕货车" },
  { MI_RCBANDIT,   "遥控匪徒车" },
  { MI_ROMERO,     "罗梅罗灵车" },
  { MI_PACKER,     "货柜卡车" },
  { MI_SENTXS,     "豪华哨兵轿车" },
  { MI_ADMIRAL,    "海军上将轿车" },
  { MI_SQUALO,     "斯奎洛快艇" },
  { MI_SEASPAR,    "海上麻雀直升机" },
  { MI_PIZZABOY,   "披萨配送车" },
  { MI_GANGBUR,    "帮派货车" },
  { MI_AIRTRAIN,   "空中列车" },
  { MI_DEADDODO,   "嘟嘟鸟残骸" },
  { MI_SPEEDER,    "竞速快艇" },
  { MI_REEFER,     "冷藏船" },
  { MI_TROPIC,     "热带游艇" },
  { MI_FLATBED,    "平板卡车" },
  { MI_YANKEE,     "扬基货车" },
  { MI_CADDY,      "高尔夫球车" },
  { MI_ZEBRA,      "斑马出租车" },
  { MI_TOPFUN,     "游乐场卡车" },
  { MI_SKIMMER,    "水上飞机" },
  { MI_PCJ600,     "PCJ-600摩托车" },
  { MI_FAGGIO,     "小绵羊踏板车" },
  { MI_FREEWAY,    "高速公路摩托" },
  { MI_RCBARON,    "遥控男爵飞机" },
  { MI_RCRAIDER,   "遥控突袭者" },
  { MI_GLENDALE,   "格伦戴尔轿车" },
  { MI_OCEANIC,    "海洋轿车" },
  { MI_SANCHEZ,    "圣切兹越野摩托" },
  { MI_SPARROW,    "麻雀直升机" },
  { MI_PATRIOT,    "爱国者吉普" },
  { MI_LOVEFIST,   "爱之拳乐队车" },
  { MI_COASTG,     "海岸警卫艇" },
  { MI_DINGHY,     "橡皮艇" },
  { MI_HERMES,     "赫尔墨斯轿车" },
  { MI_SABRE,      "军刀轿车" },
  { MI_SABRETUR,   "军刀涡轮跑车" },
  { MI_PHEONIX,    "凤凰跑车" },
  { MI_WALTON,     "沃尔顿货车" },
  { MI_REGINA,     "女王轿车" },
  { MI_COMET,      "彗星跑车" },
  { MI_DELUXO,     "豪华轿车" },
  { MI_BURRITO,    "墨西哥卷饼车" },
  { MI_SPAND,      "斯潘德货车" },
  { MI_MARQUIS,    "侯爵游艇" },
  { MI_BAGGAGE,    "行李车" },
  { MI_KAUFMAN,    "考夫曼出租车" },
  { MI_MAVERICK,   "独行侠直升机" },
  { MI_VCNMAV,     "VCN新闻直升机" },
  { MI_RANCHER,    "牧场主吉普" },
  { MI_FBIRANCH,   "FBI牧场主吉普" },
  { MI_VIRGO,      "处女座轿车" },
  { MI_GREENWOO,   "格林伍德轿车" },
  { MI_JETMAX,     "喷气快艇" },
  { MI_HOTRING,    "热环赛车" },
  { MI_SANDKING,   "沙丘王越野车" },
  { MI_BLISTAC,    "布列斯塔紧凑型" },
  { MI_POLMAV,     "警用直升机" },
  { MI_BOXVILLE,   "箱型货车" },
  { MI_BENSON,     "本森货车" },
  { MI_MESA,       "台地吉普" },
  { MI_RCGOBLIN,   "遥控小妖直升机" },
  { MI_HOTRINA,    "热环赛车A" },
  { MI_HOTRINB,    "热环赛车B" },
  { MI_BLOODRA,    "血环赛车A" },
  { MI_BLOODRB,    "血环赛车B" },
  { MI_VICECHEE,   "罪恶都市猎豹" }
};

const char* gtaVC_VehicleModels[(sizeof(gtaVC_VehModels) / sizeof(gtaVC_VehModels[0]))][2];

// 过滤后的车辆列表（动态更新）
std::vector<std::pair<const char*,const char*>> filteredVehicles;
char searchText[128] = ""; // 搜索框文本

const char* weaponTypes[][2] = {
	//{"WEAPONTYPE_UNARMED",          "徒手"},
	{"WEAPONTYPE_BRASSKNUCKLE",     "黄铜指虎"},
	{"WEAPONTYPE_SCREWDRIVER",      "螺丝刀"},
	{"WEAPONTYPE_GOLFCLUB",         "高尔夫球杆"},
	{"WEAPONTYPE_NIGHTSTICK",       "警棍"},
	{"WEAPONTYPE_KNIFE",            "匕首"},
	{"WEAPONTYPE_BASEBALLBAT",      "棒球棒"},
	{"WEAPONTYPE_HAMMER",           "锤子"},
	{"WEAPONTYPE_CLEAVER",          "切肉刀"},
	{"WEAPONTYPE_MACHETE",          "砍刀"},
	{"WEAPONTYPE_KATANA",           "武士刀"},
	{"WEAPONTYPE_CHAINSAW",         "电锯"},
	{"WEAPONTYPE_GRENADE",          "手榴弹"},
	{"WEAPONTYPE_DETONATOR_GRENADE","遥控引爆手雷"},
	{"WEAPONTYPE_TEARGAS",          "催泪瓦斯"},
	{"WEAPONTYPE_MOLOTOV",          "燃烧瓶"},
	{"WEAPONTYPE_ROCKET",           "火箭弹"},
	{"WEAPONTYPE_COLT45",           "柯尔特45手枪"},
	{"WEAPONTYPE_PYTHON",           "蟒蛇左轮"},
	{"WEAPONTYPE_SHOTGUN",          "霰弹枪"},
	{"WEAPONTYPE_SPAS12_SHOTGUN",   "SPAS-12霰弹枪"},
	{"WEAPONTYPE_STUBBY_SHOTGUN",   "重型霰弹枪"},
	{"WEAPONTYPE_TEC9",             "TEC-9冲锋枪"},
	{"WEAPONTYPE_UZI",              "乌兹冲锋枪"},
	{"WEAPONTYPE_SILENCED_INGRAM",  "Mac10"},
	{"WEAPONTYPE_MP5",              "MP5冲锋枪"},
	{"WEAPONTYPE_M4",               "M4卡宾枪"},
	{"WEAPONTYPE_RUGER",            "鲁格步枪"},
	{"WEAPONTYPE_SNIPERRIFLE",      "精准狙击枪"},
	{"WEAPONTYPE_LASERSCOPE",       "激光狙击枪"},
	{"WEAPONTYPE_ROCKETLAUNCHER",   "火箭发射器"},
	{"WEAPONTYPE_FLAMETHROWER",     "火焰喷射器"},
	{"WEAPONTYPE_M60",              "M60机枪"},
	{"WEAPONTYPE_MINIGUN",          "转轮机枪"},
	//{"WEAPONTYPE_DETONATOR",        "遥控引爆器(无用)"},
	//{"WEAPONTYPE_HELICANNON",       "机炮(无用)"},
	{"WEAPONTYPE_CAMERA",           "摄像机"}
	
};
const int weaponENUM[][3]
{
	{ WEAPONTYPE_BRASSKNUCKLE,MI_BRASS_KNUCKLES,0},
	{ WEAPONTYPE_SCREWDRIVER, MI_SCREWDRIVER ,0},
	{ WEAPONTYPE_GOLFCLUB, MI_GOLFCLUB ,0},
	{ WEAPONTYPE_NIGHTSTICK, MI_NIGHTSTICK,0 },
	{ WEAPONTYPE_KNIFE,  MI_KNIFE,0},
	{ WEAPONTYPE_BASEBALLBAT,  MI_BASEBALL_BAT,0},
	{ WEAPONTYPE_HAMMER,  MI_HAMMER,0},
	{ WEAPONTYPE_CLEAVER,  MI_MEAT_CLEAVER,0},
	{ WEAPONTYPE_MACHETE,  MI_MACHETE,0},
	{ WEAPONTYPE_KATANA,MI_KATANA,0},
	{ WEAPONTYPE_CHAINSAW,MI_CHAINSAW,0},
	{ WEAPONTYPE_GRENADE,MI_GRENADE,0},
	{ WEAPONTYPE_DETONATOR_GRENADE, MI_GRENADE, MI_BOMB},//
	{ WEAPONTYPE_TEARGAS,MI_TEARGAS,0},//
	{ WEAPONTYPE_MOLOTOV,MI_MOLOTOV,0},
	{ WEAPONTYPE_ROCKET,MI_ROCKETLAUNCHER,0},
	{ WEAPONTYPE_COLT45,MI_COLT45,0},
	{ WEAPONTYPE_PYTHON,MI_PYTHON,0},
	{ WEAPONTYPE_SHOTGUN,MI_SHOTGUN,0},
	{ WEAPONTYPE_SPAS12_SHOTGUN,MI_SPAS12_SHOTGUN,0},
	{ WEAPONTYPE_STUBBY_SHOTGUN,MI_STUBBY_SHOTGUN,0},
	{ WEAPONTYPE_TEC9,MI_TEC9,0},
	{ WEAPONTYPE_UZI,MI_UZI,0},
	{ WEAPONTYPE_SILENCED_INGRAM,MI_SILENCEDINGRAM,0},
	{ WEAPONTYPE_MP5,MI_MP5,0},
	{ WEAPONTYPE_M4,MI_M4,0},
	{ WEAPONTYPE_RUGER,MI_RUGER,0},
	{ WEAPONTYPE_SNIPERRIFLE,MI_SNIPERRIFLE,0},
	{ WEAPONTYPE_LASERSCOPE,MI_LASERSCOPE,0},
	{ WEAPONTYPE_ROCKETLAUNCHER,MI_ROCKETLAUNCHER,0},
	{ WEAPONTYPE_FLAMETHROWER,MI_FLAMETHROWER,0},
	{ WEAPONTYPE_M60,MI_M60,0},
	{ WEAPONTYPE_MINIGUN,MI_MINIGUN,MI_MINIGUN2},
	{ WEAPONTYPE_CAMERA,MI_CAMERA,0},
	
};
// 过滤后的武器列表（动态更新）
std::vector<std::pair<const char*, const char*>> filteredWeapon;
char searchText_weapon[128] = ""; // 搜索框文本

// 初始化：首次填充全部数据
void InitVehicleList() {
	filteredVehicles.clear();
	int arrlen = sizeof(gtaVC_VehModels) / sizeof(gtaVC_VehModels[0]);
	for (size_t i = 0; i < arrlen; i++)
	{
		//std::string s = std::to_string(gtaVC_VehModels[i].id);
		gtaVC_VehicleModels[i][0] = { "占位"};
		gtaVC_VehicleModels[i][1] = {gtaVC_VehModels[i].name};
	}
	for (auto& model : gtaVC_VehicleModels) {
		filteredVehicles.emplace_back(model[0], model[1]);
	}
}
// 初始化：首次填充全部数据
void InitWeaponList() {
	filteredWeapon.clear();
	for (auto& model : weaponTypes) {
		filteredWeapon.emplace_back(model[0], model[1]);
	}
}

// 更新过滤列表（根据搜索文本）
void UpdateFilteredList(const char* key,
	std::vector<std::pair<const char*, const char*>>& vec, // 改为引用传递
	const char* (*list)[2],                                 // 二维数组指针
	size_t listSize                                         // 数组长度
) {
	vec.clear();
	std::string searchLower = key;
	std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);

	// 手动遍历数组（避免基于范围的for）
	for (size_t i = 0; i < listSize; ++i) {
		const char** model = list[i];
		std::string idLower = model[0];
		std::string nameLower = model[1];
		std::transform(idLower.begin(), idLower.end(), idLower.begin(), ::tolower);
		std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

		if (idLower.find(searchLower) != std::string::npos ||
			nameLower.find(searchLower) != std::string::npos) {
			vec.emplace_back(model[0], model[1]);
		}
	}
}
int GetOriginalIndex(
	const std::vector<std::pair<const char*, const char*>>& vec, // 改为 const 引用避免拷贝
	const char* (*list)[2],                                      // 原始数组指针
	size_t listSize,                                             // 原始数组长度
	int filteredIndex                                            // 过滤后的索引
) {
	// 1. 检查过滤索引是否有效
	if (filteredIndex < 0 || filteredIndex >= static_cast<int>(vec.size())) {
		return -1; // 返回无效索引
	}

	// 2. 获取过滤后的项
	const auto& selectedItem = vec[filteredIndex];

	// 3. 在原始数组中查找匹配项
	auto it = std::find_if(
		list,
		list + listSize,
		[&](const auto& modelRow) {
			return strcmp(modelRow[0], selectedItem.first) == 0 &&
				strcmp(modelRow[1], selectedItem.second) == 0;
		}
	);

	// 4. 返回索引或 -1（未找到）
	return (it != list + listSize) ? std::distance(list, it) : -1;
}


void CheatMenu::InitMenu()
{
	InitVehicleList();
	InitWeaponList();
}
void CheatMenu::Show()
{
	ImGui::Begin(u8"CheatMenu(按F5启动/关闭)",0,ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoScrollbar);
	//ImGui::Text("Test");

	CPlayerPed* player = (CPlayerPed*)CheatList::GetDataAddress(GET_VALUE::PlayerInfo);

	
	
	
	if (ImGui::BeginTabBar("#061")) {
		if (ImGui::BeginTabItem(u8"人物"))
		{
			//ImGui::Text(u8"测试1");
			
			
				
			
			
			//ImGui::Checkbox(u8"无敌模式", NULL);
			if (ImGui::CollapsingHeader(u8"人物状态"))
			{
				static bool god = false;
				if (ImGui::Checkbox(u8"无敌模式", &god))
				{
					CheatList::AddCheat(CHEAT_TYPE::GODMODE, true, &god);
				}
				//player->m_bCanBeDamaged = god;
				

				if (ImGui::Button(u8"恢复血量"))
				{
					static float hp = 100;
					CheatList::AddCheat(CHEAT_TYPE::HEALTH, false, &hp);
				}
				ImGui::SameLine();
				if (ImGui::Button(u8"恢复防弹衣"))
				{
					static float armour = 100;
					CheatList::AddCheat(CHEAT_TYPE::ARMOUR, false, &armour);
				}
				ImGui::Separator();
				if (ImGui::Button(u8"获得10000补助")) {
					static int m = 10000;
					CheatList::AddCheat(CHEAT_TYPE::MONEY, false, &m);

				}
				float* stamina = (float*)CheatList::GetDataAddress(GET_VALUE::CurrentStamina);
				float* maxstamina = (float*)CheatList::GetDataAddress(GET_VALUE::MaxStamina);
				//static bool isstamina = false;
				if (stamina)
				{
					ImGui::SliderFloat(u8"体力值", stamina, 1, *maxstamina, "%.1f");
					ImGui::SliderFloat(u8"最大体力值", maxstamina, 1, 1000, "%.1f");
				}

				static float playerspeed = 1;
				if (ImGui::SliderFloat(u8"玩家速度值", &playerspeed, 1, 10, "%.1f"))
				{
					CheatList::AddCheat(CHEAT_TYPE::PLAYER_SPEED, false, &playerspeed);
				}
				

				static float playerjump = 1;
				if (ImGui::SliderFloat(u8"玩家跳跃高度", &playerjump, 1, 10, "%.1f"))
				{
					CheatList::AddCheat(CHEAT_TYPE::PlAYER_JUMP, false, &playerjump);
				}
			
				
				

				//ImGui::Text(u8"测试Addresss:%X", (Reader::SpeedAddress + 0x65c));
				/*if ((Reader::SpeedAddress + 0x65c)>0x65c)
				{
					ImGui::SliderFloat(u8"速度值", &(*(FLOAT*)(Reader::SpeedAddress + 0x65c)), 1, 20, "%.1f");
				}*/
				//ImGui::SliderFloat(u8"玩家速度值", &Reader::PlayerSpeed, -10, 20, "%.1f");
				//ImGui::SameLine();
				

				//ImGui::Separator();

				//ImGui::SliderFloat(u8"敌人速度值", &Reader::NpcSpeed, -10, 20, "%.1f");
				//ImGui::SameLine();

				//char('/');
				//ImGui::TreePop();
				//ImGui::Separator();
			}
			if (ImGui::CollapsingHeader(u8"警星"))
			{
				static bool wlocked = false;
				static int m = 0;

				ImGui::BeginDisabled(wlocked);
				if (ImGui::SliderInt(u8"警星等级", &m, 0, 6))
				{
					CheatList::AddCheat(CHEAT_TYPE::WANTED_LEVEL, false, &m);
				}
				ImGui::EndDisabled();
				ImGui::SameLine();
				if (ImGui::Checkbox(u8"锁定", &wlocked))
				{
					CheatList::AddCheat(CHEAT_TYPE::WANTED_LEVEL, wlocked, &m);
				}
				
				//ImGui::SliderInt(u8"警星等级", &Reader::WantedLevel, 0, 6);
				//ImGui::SliderFloat(u8"高度", &Reader::SuperJump, 0, 50);
				//ImGui::TreePop();
				//ImGui::Separator();
			}

			ImGui::EndTabItem();
		}
		
		
		CVehicle* mycar = player->m_pMyVehicle;
		if (!(player->m_nPedState == PED_DRIVING || player->m_nPedState == PED_PASSENGER)) {
			// 玩家不在车内（驾驶或乘客）
			mycar = NULL;
		}
		
		if (ImGui::BeginTabItem(u8"车辆"))
		{
			if (ImGui::CollapsingHeader(u8"当前车辆编辑"))
			{
				ImGui::Text(mycar? u8"已经进入车辆":u8"当前不在车内");
				if (mycar)
				{

					static bool carLocked = false;

					static bool AllProof = false;

					static bool bulletProof;
					static bool bFireProof;
					static bool bCollisionProof;
					static bool bMeleeProof;
					static bool bExplosionProof;
					static bool bTyresDontBurst;

					CarProof cpf =
					{
						carLocked,
						bulletProof,
						bFireProof,
						bCollisionProof,
						bMeleeProof,
						bExplosionProof,
						bTyresDontBurst
					};
					
					//bool bulletProof = mycar->bBulletProof;
					if (ImGui::Checkbox(carLocked? u8"门锁:已锁定":u8"门锁:已解锁", &carLocked))
					{
						if (carLocked)
						{
							CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						}
						else
						{
							CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						}
					}
					ImGui::Separator();
					
					
					
					
					if (ImGui::Checkbox(u8"全防", &AllProof))
					{
						bulletProof = AllProof;
						bFireProof = AllProof;
						bCollisionProof = AllProof;
						bMeleeProof = AllProof;
						bExplosionProof = AllProof;
						bTyresDontBurst = AllProof;

						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						
						
						
					}

					if (ImGui::Checkbox(u8"防弹", &bulletProof))
					{
						//mycar->bBulletProof = bulletProof; // Update the original value if the checkbox is toggled.
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
					}
					ImGui::SameLine();
				
					if (ImGui::Checkbox(u8"防火", &bFireProof))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bFireProof = bFireProof; // Update the original value if the checkbox is toggled.  
					}
					
					if (ImGui::Checkbox(u8"防碰撞", &bCollisionProof))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bCollisionProof = bCollisionProof; // Update the original value if the checkbox is toggled.  
					}
					ImGui::SameLine();
					
					if (ImGui::Checkbox(u8"防近战", &bMeleeProof))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bMeleeProof = bMeleeProof; // Update the original value if the checkbox is toggled.  
					}
					if (ImGui::Checkbox(u8"防爆炸", &bExplosionProof))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bExplosionProof = bExplosionProof; // Update the original value if the checkbox is toggled.  
					}
					ImGui::SameLine();
					if (ImGui::Checkbox(u8"防车胎爆炸", &bTyresDontBurst))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bTyresDontBurst = bTyresDontBurst; // Update the original value if the checkbox is toggled.  
					}
				}
				
               
             
			}
			if (ImGui::CollapsingHeader(u8"车辆刷出"))
			{
				// 选中行的索引（-1表示未选中）
				static int selectedRowIndex = -1;
				// 搜索框
				if (ImGui::InputText(u8"搜索", searchText, sizeof(searchText))) {
					//UpdateFilteredVehicles(); // 输入变化时更新过滤
					UpdateFilteredList(searchText,
						filteredVehicles, // 传递引用
						gtaVC_VehicleModels, // 二维数组
						sizeof(gtaVC_VehicleModels) / sizeof(gtaVC_VehicleModels[0])); // 数组长度
				}

				//滚动区域
				ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true); // 启用滚动

				// 增加一列用于按钮
				if (ImGui::BeginTable("VehicleTable", 3,
					ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn(u8"模型ID", ImGuiTableColumnFlags_WidthFixed, 60.0f);
					ImGui::TableSetupColumn(u8"中文译名", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn(u8"操作", ImGuiTableColumnFlags_WidthFixed, 60.0f); // 按钮列
					ImGui::TableHeadersRow();

					ImGuiListClipper clipper;
					clipper.Begin(filteredVehicles.size());
					while (clipper.Step()) {
						for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
							ImGui::TableNextRow();

							// 第一列：模型ID（可点击区域仅限于本列）
							ImGui::TableSetColumnIndex(0);
							ImGui::PushID(row * 3);
							if (ImGui::Selectable(G2U(filteredVehicles[row].first), selectedRowIndex == row,
								ImGuiSelectableFlags_None,
								ImVec2(0, ImGui::GetTextLineHeight()))) {
								selectedRowIndex = row;
							}
							ImGui::PopID();

							// 第二列：中文名
							ImGui::TableSetColumnIndex(1);
							if (ImGui::Selectable(G2U(filteredVehicles[row].second), selectedRowIndex == row,
								ImGuiSelectableFlags_None,
								ImVec2(0, ImGui::GetTextLineHeight()))) {
								selectedRowIndex = row;
							}
							//ImGui::TextUnformatted(G2U(filteredVehicles[row].second));

							// 第三列：按钮（独立区域）
							ImGui::TableSetColumnIndex(2);
							ImGui::PushID(row * 3 + 1);

							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
							if (ImGui::Button(u8"刷出")) {
								int originalIndex = GetOriginalIndex(
									filteredVehicles,
									gtaVC_VehicleModels,
									sizeof(gtaVC_VehicleModels) / sizeof(gtaVC_VehicleModels[0]), // 计算原始数组长度
									row                                                           // 过滤后的索引
								);;
								static int modelID = 0;
								
								//modelID = originalIndex;
								modelID = gtaVC_VehModels[originalIndex].id;
								CheatList::AddCheat(CHEAT_TYPE::VEHICLE, false, &modelID);
								//ShowVehicleDetails(originalIndex);
							}
							ImGui::PopStyleVar();
							ImGui::PopID();
						}
					}
					ImGui::EndTable();
				}

				ImGui::EndChild();

				// 显示选中项的数据
				//if (selectedRowIndex != -1) {
				//	ImGui::Separator();
				//	ImGui::Text(u8"选中的车辆数据:");
				//	ImGui::BulletText(u8"模型ID: %s", G2U(filteredVehicles[selectedRowIndex].first));
				//	ImGui::BulletText(u8"中文名: %s", G2U(filteredVehicles[selectedRowIndex].second));

				//	ImGui::BulletText(u8"原始数组索引: %d",
				//		// 计算原始数组中的索引（需要遍历匹配）
				//		// 计算原始数组中的索引
				//		std::distance(gtaVC_VehicleModels,
				//			std::find_if(gtaVC_VehicleModels,
				//				gtaVC_VehicleModels + sizeof(gtaVC_VehicleModels) / sizeof(gtaVC_VehicleModels[0]),
				//				[&](const auto& modelRow) {
				//					// 比较模型ID和中文名是否匹配
				//					return strcmp(modelRow[0], filteredVehicles[selectedRowIndex].first) == 0 &&
				//						strcmp(modelRow[1], filteredVehicles[selectedRowIndex].second) == 0;
				//				})));
				//}

			}

			
			
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"武器"))
		{
			ImGui::Text(u8"测试2");
			//ImGui::Checkbox(u8"无后坐力", &RecoilForce);
			
			if (ImGui::CollapsingHeader(u8"弹药编辑"))
			{
				static bool Ammo = false;
				//static bool Ammo2 = false;
				if(ImGui::Checkbox(u8"弹药不减", &Ammo))
				{
					CheatList::AddCheat(CHEAT_TYPE::AMMO, false, &Ammo);
				}


				

				
				
			}
			//ImGui::Separator();
			/*
			if (ImGui::Button(u8"获得武器"))
			{
				static CheatList::WeaopnStruct w;
				w.wtype =22;
				w.ammo = 999;
				CheatList::AddCheat(CheatList::CHEAT_TYPE::GIVE_WEAPON, false, &w);
			}*/
			ImGui::Separator();
			ImGui::Text(u8"搜索武器");
			// 选中行的索引（-1表示未选中）
			static int selectedRowIndex = -1;
			// 搜索框
			if (ImGui::InputText(u8"搜索", searchText_weapon, sizeof(searchText_weapon))) {
				//UpdateFilteredVehicles(); // 输入变化时更新过滤
				UpdateFilteredList(searchText_weapon,
					filteredWeapon, // 传递引用
					weaponTypes, // 二维数组
					sizeof(weaponTypes) / sizeof(weaponTypes[0])); // 数组长度
			}

			//滚动区域
			ImGui::BeginChild("ScrollingRegion1", ImVec2(0, 0), true); // 启用滚动

			// 增加一列用于按钮
			if (ImGui::BeginTable("VehicleTable1", 3,
				ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
			{
				ImGui::TableSetupColumn(u8"模型ID", ImGuiTableColumnFlags_WidthFixed, 200.0f);
				ImGui::TableSetupColumn(u8"中文译名", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn(u8"操作", ImGuiTableColumnFlags_WidthFixed, 80.0f); // 按钮列
				ImGui::TableHeadersRow();

				ImGuiListClipper clipper;
				clipper.Begin(filteredWeapon.size());
				while (clipper.Step()) {
					for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
						ImGui::TableNextRow();

						// 第一列：模型ID（可点击区域仅限于本列）
						ImGui::TableSetColumnIndex(0);
						ImGui::PushID(row * 3);
						if (ImGui::Selectable(G2U(filteredWeapon[row].first), selectedRowIndex == row,
							ImGuiSelectableFlags_None,
							ImVec2(0, ImGui::GetTextLineHeight()))) {
							selectedRowIndex = row;
						}
						ImGui::PopID();

						// 第二列：中文名
						ImGui::TableSetColumnIndex(1);
						if (ImGui::Selectable(G2U(filteredWeapon[row].second), selectedRowIndex == row,
							ImGuiSelectableFlags_None,
							ImVec2(0, ImGui::GetTextLineHeight()))) {
							selectedRowIndex = row;
						}
						//ImGui::TextUnformatted(G2U(filteredVehicles[row].second));

						// 第三列：按钮（独立区域）
						ImGui::TableSetColumnIndex(2);
						ImGui::PushID(row * 3 + 1);

						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
						if (ImGui::Button(u8"刷出")) {
							int originalIndex = GetOriginalIndex(
								filteredWeapon,
								weaponTypes,
								sizeof(weaponTypes) / sizeof(weaponTypes[0]), // 计算原始数组长度
								row                                                           // 过滤后的索引
							);;
							static int modelID = 0;
							modelID = originalIndex;
							static WeaopnStruct w;
							w.wtype = weaponENUM[modelID][0];
							w.mtype = weaponENUM[modelID][1];
							w.mtype2 = weaponENUM[modelID][2];
							w.ammo = 9;
							CheatList::AddCheat(CHEAT_TYPE::GIVE_WEAPON, false, &w);
							//CheatList::AddCheat(CheatList::CHEAT_TYPE::GIVE_WEAPON, false, &modelID);
							//ShowVehicleDetails(originalIndex);
						}
						ImGui::PopStyleVar();
						ImGui::PopID();
					}
				}
				ImGui::EndTable();
			}

			ImGui::EndChild();
		
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"杂项"))
		{
			ImGui::Text(u8"测试3");
			//ImGui::Button(u8"中国哈哈");
			//ImGui::Checkbox(u8"Log Window", &isLogOpen);
			ImGui::EndTabItem();
		}


		ImGui::EndTabBar();
	}
	////
	ImGui::SameLine();
	ImGui::Text("Index:%d Weapon Name:%s", 0, "gg");
	ImGui::End();
}
