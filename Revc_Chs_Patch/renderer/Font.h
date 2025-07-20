#pragma once
#include <common.h>

#include "Sprite2d.h"




void AsciiToUnicode(const char *src, wchar *dst);
void UnicodeStrcpy(wchar *dst, const wchar *src);
void UnicodeStrcat(wchar *dst, wchar *append);
int UnicodeStrlen(const wchar *str);
void UnicodeMakeUpperCase(wchar *dst, const wchar *src);

struct CFontDetails
{
	CRGBA color;//��ɫ
	float scaleX;//����X
	float scaleY;//����Y
	float slant;//��б
	float slantRefX;//��бx
	float slantRefY;//��бy
	bool8 justify;//�ı�����
	bool8 centre;//���� ?����?
	bool8 rightJustify;//�Ҷ���
	bool8 background;//����
	bool8 backgroundOnlyText;//�������ı�
	bool8 proportional;// ����?
	bool8 bIsShadow;//�Ƿ���ʾ��Ӱ?
	bool8 bFlash;
	bool8 bBold;
	float alphaFade;
	CRGBA backgroundColor;//������ɫ
	float wrapX;
	float centreSize;
	float rightJustifyWrap;
	int16 style;
	bool8 bFontHalfTexture;
	uint32 bank;
	int16 dropShadowPosition;
	CRGBA dropColor;
	bool8 bFlashState;
	int nFlashTimer;
	bool8 anonymous_23;
	uint32 anonymous_25;
};

struct CFontRenderState
{
	uint32 anonymous_0;
	float fTextPosX;
	float fTextPosY;
	float scaleX;
	float scaleY;
	CRGBA color;
	float fExtraSpace;
	float slant;
	float slantRefX;
	float slantRefY;
	bool8 bIsShadow;
	bool8 bFontHalfTexture;
	bool8 proportional;
	bool8 anonymous_14;
	int16 style;
};

class CSprite2d;

enum {
	FONT_BANK,
	FONT_STANDARD,
	FONT_HEADING,

	FONT_JAPANESE,
	FONT_CHINESE,

	MAX_FONTS = FONT_CHINESE
	//MAX_FONTS = FONT_HEADING
};

enum {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

#ifdef MORE_LANGUAGES
enum
{
	FONT_LANGSET_EFIGS,
	FONT_LANGSET_RUSSIAN,
	FONT_LANGSET_POLISH,
	FONT_LANGSET_JAPANESE,
	FONT_LANGSET_CHINESE,
	FONT_LANGSET_CHINESE1,
	LANGSET_MAX
};

#define FONT_LOCALE(style) (CFont::IsJapanese() ? FONT_JAPANESE : style)
#else
#define FONT_LOCALE(style) (style)
#endif

#ifdef BUTTON_ICONS
enum
{
	BUTTON_NONE = -1,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_CROSS,
	BUTTON_CIRCLE,
	BUTTON_SQUARE,
	BUTTON_TRIANGLE,
	BUTTON_L1,
	BUTTON_L2,
	BUTTON_L3,
	BUTTON_R1,
	BUTTON_R2,
	BUTTON_R3,
	BUTTON_RSTICK_UP,
	BUTTON_RSTICK_DOWN,
	BUTTON_RSTICK_LEFT,
	BUTTON_RSTICK_RIGHT,
	MAX_BUTTON_ICONS
};
#endif // BUTTON_ICONS

struct CFontSizes {
	short PropValues[209];
	short UnpropValue;
};

class CCFont
{
#ifdef MORE_LANGUAGES

	 //static CFontSizes *Size1;

	static int16 Size[LANGSET_MAX][MAX_FONTS][210];
	static uint8 *LanguageSet;
	static int32 Slot;


#else
	static int16 Size[MAX_FONTS][210];
#endif
	static bool16 NewLine;
public:
	static void Add_Memu_Chs();

#ifdef BUTTON_ICONS
	//static int32 ButtonsSlot;
	//static CCSprite2d ButtonSprite[MAX_BUTTON_ICONS];
	//static int PS2Symbol;

	static void LoadButtons(const char* txdPath);
	static void DrawButton(float x, float y);
#endif // BUTTON_ICONS

	//RW
	//static int32* RsGlobal_width;
	//static int32* RsGlobal_height;

	static void InitPatch(DWORD exe, const char* ExeName);
	//��ʼ��hook����
	static void InitValue(DWORD exe);
	//��ʼ��hook����
	static void InitHook(DWORD exe);

	static void Initialise(void);
	static void Shutdown(void);
	static void InitPerFrame(void);
	static void PrintCharDispatcher(float arg_x, float arg_y, wchar arg_char);
	static void PrintCHSChar(float arg_x, float arg_y, wchar arg_char);
	static void PrintChar(float x, float y, wchar c);
	static void PrintString_Chs(float arg_x, float arg_y, wchar *arg_text);
	static void PrintString(float x, float y, wchar *s);

	
#ifdef XBOX_SUBTITLES
	static void PrintStringFromBottom(float x, float y, wchar *str);
	static void PrintOutlinedString(float x, float y, wchar *str, float outlineStrength, bool fromBottom, CRGBA outlineColor);
#endif
	static int GetNumberLines(float xstart, float ystart, wchar *s);
	static int GetNumberLines_Chs(float xstart, float ystart, wchar *s);
	static void GetTextRect(CRect *rect, float xstart, float ystart, wchar *s);
	static void GetTextRect_Chs(CRect *rect, float xstart, float ystart, wchar *s);
	//#ifdef MORE_LANGUAGES
//	static bool PrintString(float x, float y, wchar *start, wchar* &end, float spwidth, float japX);
//#else
	static void PrintString(float x, float y, uint32, wchar *start, wchar *end, float spwidth);
//#endif
	static void PrintStringFromBottom(float x, float y, wchar *str);
	static float GetCharacterWidth(wchar c);
	static float GetCharacterSize(wchar c);
	static float GetCharacterSizeNormal(wchar arg_char);
	static float GetCharacterSize_Chs(wchar arg_char, uint16 nFontStyle, bool bBaseCharset, bool bProp, float fScaleX);
	static float GetCharacterSizeDrawing(wchar arg_char);
	static float GetStringWidth(wchar *s, bool spaces = false);
	static float GetStringWidth_Chs(wchar *s, bool spaces = false);
#ifdef MORE_LANGUAGES
	static float GetStringWidth_Jap(wchar* s);
	static float GetStringWidth_Chs(wchar *s);

#endif
	static uint16 *GetNextSpace(wchar *s);
	static uint16 *GetNextSpace_Chs(wchar *s);
	//#ifdef MORE_LANGUAGES
//	static uint16 *ParseToken(wchar *s, bool japShit = false);
//#else
	static uint16 *ParseToken(wchar *s);
	static uint16 *ParseToken(wchar *s, CRGBA &color, bool &flash, bool &bold);
//#endif
	static void DrawFonts(void);
	static void RenderFontBuffer(void);
	static void RenderFontBuffer_Chs(void);
	static uint16 character_code(uint8 c);

	static void SetScale(float x, float y);
	static void SetSlantRefPoint(float x, float y);
	static void SetSlant(float s);
	static void SetJustifyOn(void);
	static void SetJustifyOff(void);
	static void SetRightJustifyOn(void);
	static void SetRightJustifyOff(void);
	static void SetCentreOn(void);
	static void SetCentreOff(void);
	static void SetWrapx(float x);
	static void SetCentreSize(float s);
	static void SetBackgroundOn(void);
	static void SetBackgroundOff(void);
	static void SetBackGroundOnlyTextOn(void);
	static void SetBackGroundOnlyTextOff(void);
	static void SetPropOn(void);
	static void SetPropOff(void);
	static void SetFontStyle(int16 style);
	static void SetRightJustifyWrap(float wrap);
	static void SetAlphaFade(float fade);
	static void SetDropShadowPosition(int16 pos);
	static void SetBackgroundColor(CRGBA col);
	static void SetColor(CRGBA col);
	static void SetDropColor(CRGBA col);
	static wchar FindNewCharacter(wchar c);
	static void FilterOutTokensFromString(wchar*);
#ifdef MORE_LANGUAGES
	static void ReloadFonts(uint8 set);

	

	// japanese stuff
	static bool IsAnsiCharacter(wchar* s);
	static bool IsJapanesePunctuation(wchar *str);
	static bool IsJapanese() {
		//if(LanguageSet == FONT_LANGSET_CHINESE) { return true; }
		return false;
	}
	//static bool IsJapaneseFont() { return IsJapanese() && (Details->style == FONT_JAPANESE); }

	//static bool IsChinesePunctuation(wchar *str);
	static bool IsChinese()
	{
		// if(LanguageSet == FONT_LANGSET_CHINESE) { return true; }
		return *LanguageSet == FONT_LANGSET_CHINESE;
	}
	//static bool IsChineseFont() { return IsChinese() && (Details->style == FONT_CHINESE); }
#endif
};
