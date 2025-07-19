#pragma once
#include "../Revc_Chs_Patch/core/common.h"
#include "../Revc_Chs_Patch/math/Rect.h"

struct DrawCallList
{
	void(*F_DRAW_RECT)(const CRect& r, const CRGBA& col) = 0;

};
struct DrawTextCallList
{
	void (*PrintString)(float x, float y, uint32, wchar* start, wchar* end, float spwidth) = 0;;
	float (*GetStringWidth)(wchar* s, bool spaces) = 0;//  spaces= false
	//void (*GetTextRect)(CRect* rect, float xstart, float ystart, wchar* s);

	void (*SetScale)(float x, float y) = 0;
	//void (*SetSlantRefPoint)(float x, float y) = 0;
	//void (*SetSlant)(float s) = 0;
	//void (*SetJustifyOn)(void) = 0;
	//void (*SetJustifyOff)(void) = 0;
	//void (*SetRightJustifyOn)(void) = 0;
	//void (*SetRightJustifyOff)(void) = 0;
	void (*SetCentreOn)(void) = 0;
	void (*SetCentreOff)(void) = 0;
	//void (*SetWrapx)(float x) = 0;
	void (*SetCentreSize)(float s) = 0;
	void (*SetBackgroundOn)(void) = 0;
	void (*SetBackgroundOff)(void) = 0;
	//void (*SetBackGroundOnlyTextOn)(void) = 0;
	//void (*SetBackGroundOnlyTextOff)(void) = 0;
	//void (*SetPropOn)(void) = 0;
	//void (*SetPropOff)(void) = 0;
	void (*SetFontStyle)(int16 style) = 0;
	//void (*SetRightJustifyWrap)(float wrap) = 0;
	//void (*SetAlphaFade)(float fade) = 0;
	//void (*SetDropShadowPosition)(int16 pos) = 0;
	void (*SetBackgroundColor)(CRGBA col) = 0;
	void (*SetColor)(CRGBA col) = 0;

};