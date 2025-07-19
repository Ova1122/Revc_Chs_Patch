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
//UTF-8��GB2312��ת��
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
//GB2312��UTF-8��ת��
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
  { MI_LANDSTAL,   "½������" },
  { MI_IDAHO,      "����ɽγ�" },
  { MI_STINGER,    "�����ܳ�" },
  { MI_LINERUN,    "���˿���" },
  { MI_PEREN,      "�����̶�" },
  { MI_SENTINEL,   "�ڱ��γ�" },
  { MI_RIO,        "��Լ�γ�" },
  { MI_FIRETRUCK,  "������" },
  { MI_TRASH,      "������" },
  { MI_STRETCH,    "�ӳ���" },
  { MI_MANANA,     "�����Ƚγ�" },
  { MI_INFERNUS,   "�������ܳ�" },
  { MI_VOODOO,     "�׶���ү��" },
  { MI_PONY,       "С�����" },
  { MI_MULE,       "���ӻ���" },
  { MI_CHEETAH,    "�Ա��ܳ�" },
  { MI_AMBULAN,    "�Ȼ���" },
  { MI_FBICAR,     "FBIר�ó�" },
  { MI_MOONBEAM,   "�¹����" },
  { MI_ESPERANT,   "ϣ��֮�ǽγ�" },
  { MI_TAXI,       "���⳵" },
  { MI_WASHING,    "��ʢ�ٽγ�" },
  { MI_BOBCAT,     "ɽèƤ��" },
  { MI_MRWHOOP,    "����ܳ�" },
  { MI_BFINJECT,   "BF Injectionɳ̲��" },
  { MI_HUNTER,     "������ֱ����" },
  { MI_POLICE,     "����" },
  { MI_ENFORCER,   "�ؾ���" },
  { MI_SECURICA,   "������" },
  { MI_BANSHEE,    "Ů���ܳ�" },
  { MI_PREDATOR,   "��ʳ�߿�ͧ" },
  { MI_BUS,        "��ʿ" },
  { MI_RHINO,      "Ϭţ̹��" },
  { MI_BARRACKS,   "���ÿ���" },
  { MI_CUBAN,      "�ŰͰﳵ��" },
 // { MI_CHOPPER,    "����Ħ�г�(������)" },
  { MI_ANGEL,      "��ʹĦ�г�" },
  { MI_COACH,      "��;�ͳ�" },
  { MI_CABBIE,     "��ʽ���⳵" },
  { MI_STALLION,   "�����ܳ�" },
  { MI_RUMPO,      "��������" },
  { MI_RCBANDIT,   "ң�ط�ͽ��" },
  { MI_ROMERO,     "��÷���鳵" },
  { MI_PACKER,     "���񿨳�" },
  { MI_SENTXS,     "�����ڱ��γ�" },
  { MI_ADMIRAL,    "�����Ͻ��γ�" },
  { MI_SQUALO,     "˹�����ͧ" },
  { MI_SEASPAR,    "������ȸֱ����" },
  { MI_PIZZABOY,   "�������ͳ�" },
  { MI_GANGBUR,    "���ɻ���" },
  { MI_AIRTRAIN,   "�����г�" },
  { MI_DEADDODO,   "����к�" },
  { MI_SPEEDER,    "���ٿ�ͧ" },
  { MI_REEFER,     "��ش�" },
  { MI_TROPIC,     "�ȴ���ͧ" },
  { MI_FLATBED,    "ƽ�忨��" },
  { MI_YANKEE,     "�������" },
  { MI_CADDY,      "�߶�����" },
  { MI_ZEBRA,      "������⳵" },
  { MI_TOPFUN,     "���ֳ�����" },
  { MI_SKIMMER,    "ˮ�Ϸɻ�" },
  { MI_PCJ600,     "PCJ-600Ħ�г�" },
  { MI_FAGGIO,     "С����̤�峵" },
  { MI_FREEWAY,    "���ٹ�·Ħ��" },
  { MI_RCBARON,    "ң���о��ɻ�" },
  { MI_RCRAIDER,   "ң��ͻϮ��" },
  { MI_GLENDALE,   "���״����γ�" },
  { MI_OCEANIC,    "����γ�" },
  { MI_SANCHEZ,    "ʥ����ԽҰĦ��" },
  { MI_SPARROW,    "��ȸֱ����" },
  { MI_PATRIOT,    "�����߼���" },
  { MI_LOVEFIST,   "��֮ȭ�ֶӳ�" },
  { MI_COASTG,     "��������ͧ" },
  { MI_DINGHY,     "��Ƥͧ" },
  { MI_HERMES,     "�ն�ī˹�γ�" },
  { MI_SABRE,      "�����γ�" },
  { MI_SABRETUR,   "���������ܳ�" },
  { MI_PHEONIX,    "����ܳ�" },
  { MI_WALTON,     "�ֶ��ٻ���" },
  { MI_REGINA,     "Ů���γ�" },
  { MI_COMET,      "�����ܳ�" },
  { MI_DELUXO,     "�����γ�" },
  { MI_BURRITO,    "ī��������" },
  { MI_SPAND,      "˹�˵»���" },
  { MI_MARQUIS,    "�����ͧ" },
  { MI_BAGGAGE,    "���" },
  { MI_KAUFMAN,    "���������⳵" },
  { MI_MAVERICK,   "������ֱ����" },
  { MI_VCNMAV,     "VCN����ֱ����" },
  { MI_RANCHER,    "����������" },
  { MI_FBIRANCH,   "FBI����������" },
  { MI_VIRGO,      "��Ů���γ�" },
  { MI_GREENWOO,   "������½γ�" },
  { MI_JETMAX,     "������ͧ" },
  { MI_HOTRING,    "�Ȼ�����" },
  { MI_SANDKING,   "ɳ����ԽҰ��" },
  { MI_BLISTAC,    "����˹��������" },
  { MI_POLMAV,     "����ֱ����" },
  { MI_BOXVILLE,   "���ͻ���" },
  { MI_BENSON,     "��ɭ����" },
  { MI_MESA,       "̨�ؼ���" },
  { MI_RCGOBLIN,   "ң��С��ֱ����" },
  { MI_HOTRINA,    "�Ȼ�����A" },
  { MI_HOTRINB,    "�Ȼ�����B" },
  { MI_BLOODRA,    "Ѫ������A" },
  { MI_BLOODRB,    "Ѫ������B" },
  { MI_VICECHEE,   "������Ա�" }
};

const char* gtaVC_VehicleModels[(sizeof(gtaVC_VehModels) / sizeof(gtaVC_VehModels[0]))][2];

// ���˺�ĳ����б���̬���£�
std::vector<std::pair<const char*,const char*>> filteredVehicles;
char searchText[128] = ""; // �������ı�

const char* weaponTypes[][2] = {
	//{"WEAPONTYPE_UNARMED",          "ͽ��"},
	{"WEAPONTYPE_BRASSKNUCKLE",     "��ָͭ��"},
	{"WEAPONTYPE_SCREWDRIVER",      "��˿��"},
	{"WEAPONTYPE_GOLFCLUB",         "�߶������"},
	{"WEAPONTYPE_NIGHTSTICK",       "����"},
	{"WEAPONTYPE_KNIFE",            "ذ��"},
	{"WEAPONTYPE_BASEBALLBAT",      "�����"},
	{"WEAPONTYPE_HAMMER",           "����"},
	{"WEAPONTYPE_CLEAVER",          "���⵶"},
	{"WEAPONTYPE_MACHETE",          "����"},
	{"WEAPONTYPE_KATANA",           "��ʿ��"},
	{"WEAPONTYPE_CHAINSAW",         "���"},
	{"WEAPONTYPE_GRENADE",          "����"},
	{"WEAPONTYPE_DETONATOR_GRENADE","ң����������"},
	{"WEAPONTYPE_TEARGAS",          "������˹"},
	{"WEAPONTYPE_MOLOTOV",          "ȼ��ƿ"},
	{"WEAPONTYPE_ROCKET",           "�����"},
	{"WEAPONTYPE_COLT45",           "�¶���45��ǹ"},
	{"WEAPONTYPE_PYTHON",           "��������"},
	{"WEAPONTYPE_SHOTGUN",          "����ǹ"},
	{"WEAPONTYPE_SPAS12_SHOTGUN",   "SPAS-12����ǹ"},
	{"WEAPONTYPE_STUBBY_SHOTGUN",   "��������ǹ"},
	{"WEAPONTYPE_TEC9",             "TEC-9���ǹ"},
	{"WEAPONTYPE_UZI",              "���ȳ��ǹ"},
	{"WEAPONTYPE_SILENCED_INGRAM",  "Mac10"},
	{"WEAPONTYPE_MP5",              "MP5���ǹ"},
	{"WEAPONTYPE_M4",               "M4����ǹ"},
	{"WEAPONTYPE_RUGER",            "³��ǹ"},
	{"WEAPONTYPE_SNIPERRIFLE",      "��׼�ѻ�ǹ"},
	{"WEAPONTYPE_LASERSCOPE",       "����ѻ�ǹ"},
	{"WEAPONTYPE_ROCKETLAUNCHER",   "���������"},
	{"WEAPONTYPE_FLAMETHROWER",     "����������"},
	{"WEAPONTYPE_M60",              "M60��ǹ"},
	{"WEAPONTYPE_MINIGUN",          "ת�ֻ�ǹ"},
	//{"WEAPONTYPE_DETONATOR",        "ң��������(����)"},
	//{"WEAPONTYPE_HELICANNON",       "����(����)"},
	{"WEAPONTYPE_CAMERA",           "�����"}
	
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
// ���˺�������б���̬���£�
std::vector<std::pair<const char*, const char*>> filteredWeapon;
char searchText_weapon[128] = ""; // �������ı�

// ��ʼ�����״����ȫ������
void InitVehicleList() {
	filteredVehicles.clear();
	int arrlen = sizeof(gtaVC_VehModels) / sizeof(gtaVC_VehModels[0]);
	for (size_t i = 0; i < arrlen; i++)
	{
		//std::string s = std::to_string(gtaVC_VehModels[i].id);
		gtaVC_VehicleModels[i][0] = { "ռλ"};
		gtaVC_VehicleModels[i][1] = {gtaVC_VehModels[i].name};
	}
	for (auto& model : gtaVC_VehicleModels) {
		filteredVehicles.emplace_back(model[0], model[1]);
	}
}
// ��ʼ�����״����ȫ������
void InitWeaponList() {
	filteredWeapon.clear();
	for (auto& model : weaponTypes) {
		filteredWeapon.emplace_back(model[0], model[1]);
	}
}

// ���¹����б����������ı���
void UpdateFilteredList(const char* key,
	std::vector<std::pair<const char*, const char*>>& vec, // ��Ϊ���ô���
	const char* (*list)[2],                                 // ��ά����ָ��
	size_t listSize                                         // ���鳤��
) {
	vec.clear();
	std::string searchLower = key;
	std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);

	// �ֶ��������飨������ڷ�Χ��for��
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
	const std::vector<std::pair<const char*, const char*>>& vec, // ��Ϊ const ���ñ��⿽��
	const char* (*list)[2],                                      // ԭʼ����ָ��
	size_t listSize,                                             // ԭʼ���鳤��
	int filteredIndex                                            // ���˺������
) {
	// 1. �����������Ƿ���Ч
	if (filteredIndex < 0 || filteredIndex >= static_cast<int>(vec.size())) {
		return -1; // ������Ч����
	}

	// 2. ��ȡ���˺����
	const auto& selectedItem = vec[filteredIndex];

	// 3. ��ԭʼ�����в���ƥ����
	auto it = std::find_if(
		list,
		list + listSize,
		[&](const auto& modelRow) {
			return strcmp(modelRow[0], selectedItem.first) == 0 &&
				strcmp(modelRow[1], selectedItem.second) == 0;
		}
	);

	// 4. ���������� -1��δ�ҵ���
	return (it != list + listSize) ? std::distance(list, it) : -1;
}


void CheatMenu::InitMenu()
{
	InitVehicleList();
	InitWeaponList();
}
void CheatMenu::Show()
{
	ImGui::Begin(u8"CheatMenu(��F5����/�ر�)",0,ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoScrollbar);
	//ImGui::Text("Test");

	CPlayerPed* player = (CPlayerPed*)CheatList::GetDataAddress(GET_VALUE::PlayerInfo);

	
	
	
	if (ImGui::BeginTabBar("#061")) {
		if (ImGui::BeginTabItem(u8"����"))
		{
			//ImGui::Text(u8"����1");
			
			
				
			
			
			//ImGui::Checkbox(u8"�޵�ģʽ", NULL);
			if (ImGui::CollapsingHeader(u8"����״̬"))
			{
				static bool god = false;
				if (ImGui::Checkbox(u8"�޵�ģʽ", &god))
				{
					CheatList::AddCheat(CHEAT_TYPE::GODMODE, true, &god);
				}
				//player->m_bCanBeDamaged = god;
				

				if (ImGui::Button(u8"�ָ�Ѫ��"))
				{
					static float hp = 100;
					CheatList::AddCheat(CHEAT_TYPE::HEALTH, false, &hp);
				}
				ImGui::SameLine();
				if (ImGui::Button(u8"�ָ�������"))
				{
					static float armour = 100;
					CheatList::AddCheat(CHEAT_TYPE::ARMOUR, false, &armour);
				}
				ImGui::Separator();
				if (ImGui::Button(u8"���10000����")) {
					static int m = 10000;
					CheatList::AddCheat(CHEAT_TYPE::MONEY, false, &m);

				}
				float* stamina = (float*)CheatList::GetDataAddress(GET_VALUE::CurrentStamina);
				float* maxstamina = (float*)CheatList::GetDataAddress(GET_VALUE::MaxStamina);
				//static bool isstamina = false;
				if (stamina)
				{
					ImGui::SliderFloat(u8"����ֵ", stamina, 1, *maxstamina, "%.1f");
					ImGui::SliderFloat(u8"�������ֵ", maxstamina, 1, 1000, "%.1f");
				}

				static float playerspeed = 1;
				if (ImGui::SliderFloat(u8"����ٶ�ֵ", &playerspeed, 1, 10, "%.1f"))
				{
					CheatList::AddCheat(CHEAT_TYPE::PLAYER_SPEED, false, &playerspeed);
				}
				

				static float playerjump = 1;
				if (ImGui::SliderFloat(u8"�����Ծ�߶�", &playerjump, 1, 10, "%.1f"))
				{
					CheatList::AddCheat(CHEAT_TYPE::PlAYER_JUMP, false, &playerjump);
				}
			
				
				

				//ImGui::Text(u8"����Addresss:%X", (Reader::SpeedAddress + 0x65c));
				/*if ((Reader::SpeedAddress + 0x65c)>0x65c)
				{
					ImGui::SliderFloat(u8"�ٶ�ֵ", &(*(FLOAT*)(Reader::SpeedAddress + 0x65c)), 1, 20, "%.1f");
				}*/
				//ImGui::SliderFloat(u8"����ٶ�ֵ", &Reader::PlayerSpeed, -10, 20, "%.1f");
				//ImGui::SameLine();
				

				//ImGui::Separator();

				//ImGui::SliderFloat(u8"�����ٶ�ֵ", &Reader::NpcSpeed, -10, 20, "%.1f");
				//ImGui::SameLine();

				//char('/');
				//ImGui::TreePop();
				//ImGui::Separator();
			}
			if (ImGui::CollapsingHeader(u8"����"))
			{
				static bool wlocked = false;
				static int m = 0;

				ImGui::BeginDisabled(wlocked);
				if (ImGui::SliderInt(u8"���ǵȼ�", &m, 0, 6))
				{
					CheatList::AddCheat(CHEAT_TYPE::WANTED_LEVEL, false, &m);
				}
				ImGui::EndDisabled();
				ImGui::SameLine();
				if (ImGui::Checkbox(u8"����", &wlocked))
				{
					CheatList::AddCheat(CHEAT_TYPE::WANTED_LEVEL, wlocked, &m);
				}
				
				//ImGui::SliderInt(u8"���ǵȼ�", &Reader::WantedLevel, 0, 6);
				//ImGui::SliderFloat(u8"�߶�", &Reader::SuperJump, 0, 50);
				//ImGui::TreePop();
				//ImGui::Separator();
			}

			ImGui::EndTabItem();
		}
		
		
		CVehicle* mycar = player->m_pMyVehicle;
		if (!(player->m_nPedState == PED_DRIVING || player->m_nPedState == PED_PASSENGER)) {
			// ��Ҳ��ڳ��ڣ���ʻ��˿ͣ�
			mycar = NULL;
		}
		
		if (ImGui::BeginTabItem(u8"����"))
		{
			if (ImGui::CollapsingHeader(u8"��ǰ�����༭"))
			{
				ImGui::Text(mycar? u8"�Ѿ����복��":u8"��ǰ���ڳ���");
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
					if (ImGui::Checkbox(carLocked? u8"����:������":u8"����:�ѽ���", &carLocked))
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
					
					
					
					
					if (ImGui::Checkbox(u8"ȫ��", &AllProof))
					{
						bulletProof = AllProof;
						bFireProof = AllProof;
						bCollisionProof = AllProof;
						bMeleeProof = AllProof;
						bExplosionProof = AllProof;
						bTyresDontBurst = AllProof;

						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						
						
						
					}

					if (ImGui::Checkbox(u8"����", &bulletProof))
					{
						//mycar->bBulletProof = bulletProof; // Update the original value if the checkbox is toggled.
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
					}
					ImGui::SameLine();
				
					if (ImGui::Checkbox(u8"����", &bFireProof))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bFireProof = bFireProof; // Update the original value if the checkbox is toggled.  
					}
					
					if (ImGui::Checkbox(u8"����ײ", &bCollisionProof))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bCollisionProof = bCollisionProof; // Update the original value if the checkbox is toggled.  
					}
					ImGui::SameLine();
					
					if (ImGui::Checkbox(u8"����ս", &bMeleeProof))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bMeleeProof = bMeleeProof; // Update the original value if the checkbox is toggled.  
					}
					if (ImGui::Checkbox(u8"����ը", &bExplosionProof))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bExplosionProof = bExplosionProof; // Update the original value if the checkbox is toggled.  
					}
					ImGui::SameLine();
					if (ImGui::Checkbox(u8"����̥��ը", &bTyresDontBurst))
					{
						CheatList::AddCheat(CHEAT_TYPE::VehicleProof, true, &cpf);
						//mycar->bTyresDontBurst = bTyresDontBurst; // Update the original value if the checkbox is toggled.  
					}
				}
				
               
             
			}
			if (ImGui::CollapsingHeader(u8"����ˢ��"))
			{
				// ѡ���е�������-1��ʾδѡ�У�
				static int selectedRowIndex = -1;
				// ������
				if (ImGui::InputText(u8"����", searchText, sizeof(searchText))) {
					//UpdateFilteredVehicles(); // ����仯ʱ���¹���
					UpdateFilteredList(searchText,
						filteredVehicles, // ��������
						gtaVC_VehicleModels, // ��ά����
						sizeof(gtaVC_VehicleModels) / sizeof(gtaVC_VehicleModels[0])); // ���鳤��
				}

				//��������
				ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true); // ���ù���

				// ����һ�����ڰ�ť
				if (ImGui::BeginTable("VehicleTable", 3,
					ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
				{
					ImGui::TableSetupColumn(u8"ģ��ID", ImGuiTableColumnFlags_WidthFixed, 60.0f);
					ImGui::TableSetupColumn(u8"��������", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn(u8"����", ImGuiTableColumnFlags_WidthFixed, 60.0f); // ��ť��
					ImGui::TableHeadersRow();

					ImGuiListClipper clipper;
					clipper.Begin(filteredVehicles.size());
					while (clipper.Step()) {
						for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
							ImGui::TableNextRow();

							// ��һ�У�ģ��ID���ɵ����������ڱ��У�
							ImGui::TableSetColumnIndex(0);
							ImGui::PushID(row * 3);
							if (ImGui::Selectable(G2U(filteredVehicles[row].first), selectedRowIndex == row,
								ImGuiSelectableFlags_None,
								ImVec2(0, ImGui::GetTextLineHeight()))) {
								selectedRowIndex = row;
							}
							ImGui::PopID();

							// �ڶ��У�������
							ImGui::TableSetColumnIndex(1);
							if (ImGui::Selectable(G2U(filteredVehicles[row].second), selectedRowIndex == row,
								ImGuiSelectableFlags_None,
								ImVec2(0, ImGui::GetTextLineHeight()))) {
								selectedRowIndex = row;
							}
							//ImGui::TextUnformatted(G2U(filteredVehicles[row].second));

							// �����У���ť����������
							ImGui::TableSetColumnIndex(2);
							ImGui::PushID(row * 3 + 1);

							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
							if (ImGui::Button(u8"ˢ��")) {
								int originalIndex = GetOriginalIndex(
									filteredVehicles,
									gtaVC_VehicleModels,
									sizeof(gtaVC_VehicleModels) / sizeof(gtaVC_VehicleModels[0]), // ����ԭʼ���鳤��
									row                                                           // ���˺������
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

				// ��ʾѡ���������
				//if (selectedRowIndex != -1) {
				//	ImGui::Separator();
				//	ImGui::Text(u8"ѡ�еĳ�������:");
				//	ImGui::BulletText(u8"ģ��ID: %s", G2U(filteredVehicles[selectedRowIndex].first));
				//	ImGui::BulletText(u8"������: %s", G2U(filteredVehicles[selectedRowIndex].second));

				//	ImGui::BulletText(u8"ԭʼ��������: %d",
				//		// ����ԭʼ�����е���������Ҫ����ƥ�䣩
				//		// ����ԭʼ�����е�����
				//		std::distance(gtaVC_VehicleModels,
				//			std::find_if(gtaVC_VehicleModels,
				//				gtaVC_VehicleModels + sizeof(gtaVC_VehicleModels) / sizeof(gtaVC_VehicleModels[0]),
				//				[&](const auto& modelRow) {
				//					// �Ƚ�ģ��ID���������Ƿ�ƥ��
				//					return strcmp(modelRow[0], filteredVehicles[selectedRowIndex].first) == 0 &&
				//						strcmp(modelRow[1], filteredVehicles[selectedRowIndex].second) == 0;
				//				})));
				//}

			}

			
			
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"����"))
		{
			ImGui::Text(u8"����2");
			//ImGui::Checkbox(u8"�޺�����", &RecoilForce);
			
			if (ImGui::CollapsingHeader(u8"��ҩ�༭"))
			{
				static bool Ammo = false;
				//static bool Ammo2 = false;
				if(ImGui::Checkbox(u8"��ҩ����", &Ammo))
				{
					CheatList::AddCheat(CHEAT_TYPE::AMMO, false, &Ammo);
				}


				

				
				
			}
			//ImGui::Separator();
			/*
			if (ImGui::Button(u8"�������"))
			{
				static CheatList::WeaopnStruct w;
				w.wtype =22;
				w.ammo = 999;
				CheatList::AddCheat(CheatList::CHEAT_TYPE::GIVE_WEAPON, false, &w);
			}*/
			ImGui::Separator();
			ImGui::Text(u8"��������");
			// ѡ���е�������-1��ʾδѡ�У�
			static int selectedRowIndex = -1;
			// ������
			if (ImGui::InputText(u8"����", searchText_weapon, sizeof(searchText_weapon))) {
				//UpdateFilteredVehicles(); // ����仯ʱ���¹���
				UpdateFilteredList(searchText_weapon,
					filteredWeapon, // ��������
					weaponTypes, // ��ά����
					sizeof(weaponTypes) / sizeof(weaponTypes[0])); // ���鳤��
			}

			//��������
			ImGui::BeginChild("ScrollingRegion1", ImVec2(0, 0), true); // ���ù���

			// ����һ�����ڰ�ť
			if (ImGui::BeginTable("VehicleTable1", 3,
				ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
			{
				ImGui::TableSetupColumn(u8"ģ��ID", ImGuiTableColumnFlags_WidthFixed, 200.0f);
				ImGui::TableSetupColumn(u8"��������", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn(u8"����", ImGuiTableColumnFlags_WidthFixed, 80.0f); // ��ť��
				ImGui::TableHeadersRow();

				ImGuiListClipper clipper;
				clipper.Begin(filteredWeapon.size());
				while (clipper.Step()) {
					for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
						ImGui::TableNextRow();

						// ��һ�У�ģ��ID���ɵ����������ڱ��У�
						ImGui::TableSetColumnIndex(0);
						ImGui::PushID(row * 3);
						if (ImGui::Selectable(G2U(filteredWeapon[row].first), selectedRowIndex == row,
							ImGuiSelectableFlags_None,
							ImVec2(0, ImGui::GetTextLineHeight()))) {
							selectedRowIndex = row;
						}
						ImGui::PopID();

						// �ڶ��У�������
						ImGui::TableSetColumnIndex(1);
						if (ImGui::Selectable(G2U(filteredWeapon[row].second), selectedRowIndex == row,
							ImGuiSelectableFlags_None,
							ImVec2(0, ImGui::GetTextLineHeight()))) {
							selectedRowIndex = row;
						}
						//ImGui::TextUnformatted(G2U(filteredVehicles[row].second));

						// �����У���ť����������
						ImGui::TableSetColumnIndex(2);
						ImGui::PushID(row * 3 + 1);

						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));
						if (ImGui::Button(u8"ˢ��")) {
							int originalIndex = GetOriginalIndex(
								filteredWeapon,
								weaponTypes,
								sizeof(weaponTypes) / sizeof(weaponTypes[0]), // ����ԭʼ���鳤��
								row                                                           // ���˺������
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

		if (ImGui::BeginTabItem(u8"����"))
		{
			ImGui::Text(u8"����3");
			//ImGui::Button(u8"�й�����");
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
