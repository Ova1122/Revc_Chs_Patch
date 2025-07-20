#include "common.h"

#include "Sprite2d.h"
#include "Font.h"
#include <MemUtils.h>
#include <vector>
#include <TxdStore.h>



typedef  void(F_DRAW_RECT)(const CRect& r, const CRGBA& col);
F_DRAW_RECT* dDrawRect = NULL;

typedef  void(F_DRAW)(const CRect& r, const CRGBA& col);
F_DRAW* dDraw = NULL;

typedef  void(F_AddToBuffer)(const CRect& a1,const CRGBA& a2, float u0, float v0, float u1,float v1 ,float u3, float v3, float u2, float v2);
F_AddToBuffer* dAddToBuffer = NULL;

typedef  void(F_SetTexture)(const char* name, const char* mask);
F_SetTexture* dSetTexture = NULL;

typedef  void(F_RenderVertexBuffer)();
F_RenderVertexBuffer* dRenderVertexBuffer = NULL;

//RW
typedef  void*(F_RpSkinGetNumBones)(void*);
F_RpSkinGetNumBones* RpSkinGetNumBones = NULL;

CCSprite2d::F_RwRenderState* CCSprite2d::RwRenderStateSet = NULL;
//DWORD* d_SetRenderState = NULL;
CCSprite2d::F_RwRenderState* CCSprite2d::RwRenderStateGet = NULL;
//DWORD* d_GetRenderState = NULL;


typedef int32(F_RwTextureDestroy)(RwTexture*);// RwBool RwRenderStateSet(RwRenderState state, void* value)
F_RwTextureDestroy* RwTextureDestroy;

typedef RwTexture*(F_RwTextureRead)(const char* name, const char* maskName);// RwBool RwRenderStateSet(RwRenderState state, void* value)
F_RwTextureRead* RwRwTextureRead;

const char RwRenderStateSet_code[] = "55 8B EC 8B 45 08 48 83 F8 1B 0F 87 ?? ?? ?? ?? 0F B6 80 ?? ?? ?? ?? FF 24 85";
const char RwRenderStateGet_code[] = "558BEC8B ?? ?? 48 83 F8 ?? 0F ?? ?? ?? ?? ?? FF";
const char RwTextureDestroy_code[] = "558BEC 8B ?? ?? E8 ?? ?? ?? ?? B8 01 00 00 00 5D C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC 558BEC 5D";
const char RwTextureRead_code[] = "558BEC 5D E9 ?? ?? ?? ?? CC CC CC CC CC CC CC 558BEC 8B ?? ?? 8B ?? ?? 8B CA 56";
		  
const char DrawRect_code[] = "558BEC 56 8B ?? ?? 56 56 56 56 FF ?? ?? E8 ?? ?? ?? ?? 6A 00 6A 01";
const char Draw_code[] = "558BEC 8B ?? ?? 56 50 50 50 50 FF ?? ?? 8B";
const char AddToBuffer_code[] = "558BEC F3 ?? ?? ?? ?? 56 8B ?? ?? 57 83 ?? ?? F3";
const char RenderVertexBuffer_code[] = "83 ?? ?? ?? ?? ?? 00 7E ?? 6A 02 6A 09 E8";
		  
const char RpSkinGetNumBones_code[] = "558BEC 8B ?? ?? 8B 00 5D C3 CC CC CC CC CC CC";
const char charSetRenderState_code[] = "8B 01 85 C0 74 ?? 50 E8 ?? ?? ?? ?? 83 C4 ?? 50 6A";



//简化类名
using MU = MemUnits;


void CCSprite2d::InitPatch(DWORD exe, const char* ExeName)
{
	//const char* ExeName = "reVC.exe";
	//RW
	std::vector <ULONGLONG> vResultContainer1;
	vResultContainer1 = MU::ScanMemory(RwRenderStateSet_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	RwRenderStateSet = (F_RwRenderState*)(vResultContainer1[0]);

	vResultContainer1 = MU::ScanMemory(RwRenderStateGet_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	RwRenderStateGet = (F_RwRenderState*)(vResultContainer1[0]);
	
	vResultContainer1 = MU::ScanMemory(RwTextureDestroy_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	RwTextureDestroy= (F_RwTextureDestroy*)(vResultContainer1[0]);
	

	vResultContainer1 = MU::ScanMemory(RwTextureRead_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	RwRwTextureRead = (F_RwTextureRead*)(vResultContainer1[0]);
	

	vResultContainer1 = MU::ScanMemory(DrawRect_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dDrawRect = (F_DRAW_RECT*)(vResultContainer1[0]);

	vResultContainer1 = MU::ScanMemory(Draw_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dDraw = (F_DRAW*)(vResultContainer1[0]);
	

	vResultContainer1 = MU::ScanMemory(AddToBuffer_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dAddToBuffer = (F_AddToBuffer*)(vResultContainer1[0]);
	

	vResultContainer1 = MU::ScanMemory(RenderVertexBuffer_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	dRenderVertexBuffer = (F_RenderVertexBuffer*)(vResultContainer1[0]);
	

	vResultContainer1 = MU::ScanMemory(charSetRenderState_code,ExeName);
	CHECK_ERROR(vResultContainer1);
	//相对地址计算 call绝对地址等于下一条指令地址+call后面地址
	DWORD hook_RpSkinGetNumBones_offset = (DWORD)vResultContainer1.at(0);
	
	RpSkinGetNumBones = (F_RpSkinGetNumBones*)MemUnits::Call_addr_To_Addr(hook_RpSkinGetNumBones_offset + 0x7);

	//RpSkinGetNumBones = (F_RpSkinGetNumBones*)(vResultContainer1[0]);

	//vResultContainer1 = MU::ScanMemory(RpSkinGetNumBones_code,ExeName);
	//CHECK_ERROR(vResultContainer1);
	//RpSkinGetNumBones = (F_RpSkinGetNumBones*)(vResultContainer1[0]);


	

	//RwRenderStateSet = (F_RwRenderStateSet*)(exe + 0x1252C0);
	//d_SetRenderState = (DWORD*)(exe + 0x1C1ED0);

	//dDrawRect = (F_DRAW_RECT*)(exe + 0x1C1CC0);
	//dAddToBuffer = (F_AddToBuffer*)(exe + 0x1C1690);
	//dRenderVertexBuffer= (F_RenderVertexBuffer*)(exe + 0x1C1DA0);

	//rp
	//RpSkinGetNumBones = (F_RpSkinGetNumBones*)(exe + 0xCD10);

}

int32 CCSprite2d::RwRwRenderStateSet(RwRenderState state, void* value)
{
	return RwRenderStateSet(state, value);
}

int32 CCSprite2d::RwRwRenderStateGet(RwRenderState state, void* value)
{
	return RwRenderStateGet(state, value);
}
void CCSprite2d::Delete(void)
{
	if (m_pTexture) {
		RwTextureDestroy(m_pTexture);
		m_pTexture = nil;
	}
}
void CCSprite2d::DrawRect(const CRect& r, const CRGBA& col)
{

	if (dDrawRect)
	{
		dDrawRect(r, col);
	}
}

void CCSprite2d::Draw(const CRect& rect, const CRGBA& col)
{
	if (dDraw)
	{
		dDraw(rect, col);
	}
}

void CCSprite2d::SetRenderState(void)
{
	//调用类函数
	//RET(void)(DWORD)FUN_ADDRESS(d_SetRenderState)((DWORD)&m_pTexture);
	void* num = 0;
	if (m_pTexture)
	{
		num = RpSkinGetNumBones(m_pTexture);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, num);
	}	
	else
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nil);
}
void CCSprite2d::SetTexture(const char* name)
{
	Delete();
	if (name)
		m_pTexture = RwRwTextureRead(name, nil);
}
void CCSprite2d::SetTexture(const char* name, const char* mask)
{
	Delete();
	if (name)
		m_pTexture = RwRwTextureRead(name, mask);
}

void CCSprite2d::AddToBuffer(const CRect& a1 ,const CRGBA& a2, float u0, float v0, float u1,float v1,float u3, float v3, float u2, float v2)
{
	dAddToBuffer(a1,a2, u0, v0, u1,v1,u3, v3, u2, v2);
}

void CCSprite2d::RenderVertexBuffer()
{
	dRenderVertexBuffer();
}
