#include<Font.h>
#include "MemUtils.h"
#include<memory>
#include <TxdStore.h>
#include <FileMgr.h>

//简化类名
using MU = MemUnits;

#ifdef MORE_LANGUAGES

bool16 CCFont::NewLine = 0;
uint8* CCFont::LanguageSet = NULL;
int32 CCFont::Slot = -1;
int32 chs_Slot = -1;


//三种字体的配置(强制)
enum FONT_SET
{
	FT_NONE = 0,
	FT_normal,
	FT_slant,
	FT_regular
};
FONT_SET current_font_set = FT_NONE;


#define JAP_TERMINATION (0x8000 | '~')

int16 CCFont::Size[LANGSET_MAX][MAX_FONTS][210] = {
	{
#else
int16 CCFont::Size[MAX_FONTS][210] = {
#endif
		{// FONT2 EFIGS
		 // SPC,!,         $,  %,  &,  ',  [,  ],      +,  ,   -,  .,
		 12, 9, 22, 17, 19, 19, 25, 4, 33, 33, 25, 35, 11, 10, 6, 33,
		 // 0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  :,                 ??,
		 18, 10, 17, 17, 17, 17, 17, 15, 12, 16, 5, 30, 30, 30, 30, 30,
		 //   A,  B,  C,  D,  E,  F,  G,  H,  I,  J,  K,  L,  M,  N,  O,
		 12, 16, 19, 16, 19, 18, 18, 17, 22, 11, 17, 18, 18, 30, 22, 19,
		 // P, Q,  R,  S,  T,  U,  V,  W,  X,  Y,  Z, ??, ??, ??,  ?  \,
		 #ifdef FIX_BUGS
				  22, 19, 19, 20, 18, 19, 19, 29, 19, 18, 19, 19, 33, 33, 10, 19,
		 #else
			  22, 19, 19, 20, 18, 19, 19, 29, 19, 18, 19, 19, 33, 33, 19, 19,
		 #endif
			  //??,a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,
			  12, 14, 11, 11, 16, 11, 12, 14, 14, 10, 13, 12, 10, 19, 18, 12,
			  // p, q,  r,  s,  t,  u,  v,  w,  x,  y,  z, ??, ??, ??, ??, ??,
			  16, 13, 13, 11, 12, 15, 12, 15, 13, 12, 12, 37, 33, 37, 35, 37,
			  //? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
			  16, 16, 16, 16, 33, 17, 18, 18, 18, 18, 11, 11, 11, 11, 19, 19,
			  //? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
			  19, 19, 19, 19, 19, 19, 15, 14, 14, 14, 14, 20, 14, 11, 11, 11,
			  //? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
			  #ifdef FIX_BUGS
					   11, 10, 10, 10, 10, 12, 12, 12, 12, 15, 15, 15, 15, 22, 18, 21,
			  #else
				   11, 10, 10, 10, 10, 12, 12, 12, 12, 15, 15, 15, 15, 24, 18, 21,
			  #endif
				   // i,BLANKS
				   10, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
				   19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
				   // space, unprop
				   19, 16},
				  {// FONT1 EFIGS
				   // Characters with a '2' refer to the Pricedown font.
				   // Characters that are referred as '*I' are characters that contain icons for PS2/XBOX, but contain regular characters on PC
				   // in order to display them properly in the Keyboard controls menu.
				   //! 2,!, *I,(R),  $,  %,  &,  ',  [,  ], *I,  +,  ,   -,  ., *I,
				   15, 7, 31, 25, 20, 23, 21, 7, 11, 10, 26, 14, 6, 12, 6, 26,
				   // 0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  :, *I, *I, *I, *I,  ?,
				   20, 7, 20, 20, 21, 20, 20, 19, 21, 20, 8, 30, 24, 30, 24, 19,
				   // TM,A,  B,  C,  D,  E,  F,  G,  H,  I,  J,  K,  L,  M,  N,  O,
				   20, 22, 22, 21, 22, 18, 18, 22, 22, 9, 14, 21, 18, 27, 21, 24,
				   // P, Q,  R,  S,  T,  U,  V,  W,  X,  Y,  Z, *I,  \, *I,  ?  ?
				   #ifdef FIX_BUGS
							22, 22, 23, 20, 19, 23, 22, 31, 23, 23, 21, 25, 13, 30, 7, 19,
				   #else
						22, 22, 23, 20, 19, 23, 22, 31, 23, 23, 21, 25, 13, 30, 10, 19,
				   #endif
						//(C),a, b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,
						10, 17, 17, 16, 17, 17, 11, 17, 17, 7, 7, 18, 7, 25, 17, 17,
						// p, q,  r,  s,  t,  u,  v,  w,  x,  y,  z, *I, *I, $2, (2, )2,
						17, 17, 11, 17, 11, 17, 18, 25, 19, 18, 17, 28, 26, 20, 15, 15,
						//? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
						20, 20, 20, 20, 29, 22, 19, 19, 19, 19, 9, 9, 9, 9, 23, 23,
						//? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
						23, 23, 24, 24, 24, 24, 20, 19, 17, 17, 17, 30, 16, 17, 17, 17,
						//? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
						#ifdef FIX_BUGS
								 17, 11, 11, 15, 12, 17, 17, 17, 17, 17, 17, 17, 17, 21, 17, 19,
						#else
							 17, 11, 11, 15, 12, 17, 17, 17, 17, 17, 17, 17, 17, 19, 20, 20,
						#endif
							 // 02,12,22, 32, 42, 52, 62, 72, 82, 92, :2, A2, B2, C2, D2, E2,
							 20, 18, 19, 19, 21, 19, 19, 19, 19, 19, 16, 19, 19, 19, 20, 19,
							 // F2,G2,H2, I2, J2, K2, L2, M2, N2, O2, P2, Q2, R2, S2, T2, U2,
							 16, 19, 19, 9, 19, 20, 14, 29, 19, 19, 19, 19, 19, 19, 21, 19,
							 // V2,W2,X2, Y2, Z2, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,
							 20, 32, 20, 19, 19, 19, 19, 19, 19, 29, 19, 19, 19, 19, 19, 9,
							 //?,?,?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, '2, .2,
							 #ifdef FIX_BUGS
									  9, 9, 9, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 10, 9,
							 #else
								  9, 9, 9, 19, 19, 19, 19, 19, 19, 19, 19, 19, 21, 21, 10, 9,
							 #endif
								  // space, unprop
								  10, 20}

						 #ifdef MORE_LANGUAGES
							 },
							 {
								 {5,  9,  9,  0,  17, 17, 23, 3,  21, 18, 0,  8,  3,  8,  3,  0,  16, 9,  16, 16, 15, 19, 15, 14, 17, 17, 4,  4,  0,  0,  0,  17, 19, 17, 19,
								  15, 21, 18, 19, 16, 21, 13, 15, 21, 20, 28, 21, 18, 22, 17, 21, 20, 18, 18, 20, 26, 22, 18, 18, 0,  8,  0,  9,  8,  0,  14, 11, 12, 16, 11,
								  13, 13, 15, 10, 14, 15, 11, 21, 17, 10, 20, 15, 12, 12, 16, 17, 13, 16, 13, 21, 11, 0,  0,  0,  0,  0,  20, 19, 19, 22, 27, 15, 18, 18, 20,
								  26, 21, 23, 17, 22, 21, 17, 26, 25, 26, 17, 20, 26, 17, 16, 11, 12, 13, 21, 11, 17, 17, 12, 21, 17, 17, 15, 24, 16, 10, 20, 23, 16, 7,  9,
								  16, 23, 12, 11, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
								  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  19, 19, 16},
								 {11, 5,  10, 15, 19, 22, 20, 5,  9,  8,  11, 12, 5,  12, 6,  12, 19, 5,  18, 19, 20, 18, 19, 18, 20, 19, 5,  6,  26, 12, 30, 19, 23, 21, 20,
								  20, 20, 16, 16, 21, 19, 5,  13, 19, 16, 24, 20, 21, 20, 21, 20, 19, 17, 20, 21, 30, 22, 21, 20, 25, 13, 30, 5,  9,  10, 15, 15, 14, 15, 16,
								  10, 15, 15, 5,  5,  15, 5,  23, 15, 16, 15, 15, 9,  16, 10, 15, 17, 24, 18, 15, 15, 27, 5,  19, 2,  2,  20, 20, 16, 23, 30, 19, 20, 20, 21,
								  24, 19, 19, 20, 23, 22, 19, 27, 29, 25, 20, 20, 28, 24, 16, 16, 14, 19, 25, 16, 16, 16, 17, 19, 16, 16, 17, 25, 19, 15, 23, 26, 21, 16, 14,
								  22, 20, 16, 19, 15, 14, 15, 16, 17, 15, 15, 15, 15, 15, 7,  15, 15, 15, 15, 15, 13, 15, 15, 7,  15, 16, 13, 23, 15, 15, 15, 15, 15, 15, 17,
								  15, 16, 24, 17, 17, 17, 15, 15, 13, 20, 23, 15, 17, 17, 16, 24, 15, 15, 15, 23, 18, 15, 23, 26, 23, 16, 15, 23, 15, 15, 19, 2,  2,  10, 20},
							 },
							 {{// FONT2 EFIGS
									 // SPC,!,         $,  %,  &,  ',  [,  ],      +,  ,   -,  .,
									 12, 9, 22, 17, 19, 19, 25, 4, 33, 33, 25, 35, 11, 10, 6, 33,
									 // 0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  :,                 ??,
									 18, 10, 17, 17, 17, 17, 17, 15, 12, 16, 5, 30, 30, 30, 30, 30,
									 //   A,  B,  C,  D,  E,  F,  G,  H,  I,  J,  K,  L,  M,  N,  O,
									 12, 16, 19, 16, 19, 18, 18, 17, 22, 11, 17, 18, 18, 30, 22, 19,
									 // P, Q,  R,  S,  T,  U,  V,  W,  X,  Y,  Z, ??, ??, ??,  ?  \,
									 22, 19, 19, 20, 18, 19, 19, 29, 19, 18, 19, 19, 33, 33, 10, 19,
									 //??,a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,
									 12, 14, 11, 11, 16, 11, 12, 14, 14, 10, 13, 12, 10, 19, 18, 12,
									 // p, q,  r,  s,  t,  u,  v,  w,  x,  y,  z, ??, ??, ??, ??, ??,
									 16, 13, 13, 11, 12, 15, 12, 15, 13, 12, 12, 37, 33, 37, 35, 37,
									 //? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
									 16, 16, 16, 16, 33, 17, 18, 18, 18, 18, 11, 11, 11, 11, 19, 19,
									 //? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
									 19, 19, 19, 19, 19, 19, 15, 14, 14, 14, 14, 20, 14, 11, 11, 11,
									 //? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
									 11, 10, 10, 10, 10, 12, 12, 12, 12, 15, 15, 15, 15, 22, 18, 21,
									 // i,BLANKS
									 10, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
									 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
									 // space, unprop
									 19, 16},
									{// FONT1 EFIGS
									 // Characters with a '2' refer to the Pricedown font.
									 // Characters that are referred as '*I' are characters that contain icons for PS2/XBOX, but contain regular characters on PC
									 // in order to display them properly in the Keyboard controls menu.
									 //! 2,!, *I,(R),  $,  %,  &,  ',  [,  ], *I,  +,  ,   -,  ., *I,
									 15, 7, 31, 25, 20, 23, 21, 7, 11, 10, 26, 14, 6, 12, 6, 26,
									 // 0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  :, *I, *I, *I, *I,  ?,
									 20, 7, 20, 20, 21, 20, 20, 19, 21, 20, 8, 30, 24, 30, 24, 19,
									 // TM,A,  B,  C,  D,  E,  F,  G,  H,  I,  J,  K,  L,  M,  N,  O,
									 20, 22, 22, 21, 22, 18, 18, 22, 22, 9, 14, 21, 18, 27, 21, 24,
									 // P, Q,  R,  S,  T,  U,  V,  W,  X,  Y,  Z, *I,  \, *I,  ?  ?
									 22, 22, 23, 20, 19, 23, 22, 31, 23, 23, 21, 25, 13, 30, 7, 19,
									 //(C),a, b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,
									 10, 17, 17, 16, 17, 17, 11, 17, 17, 7, 7, 18, 7, 25, 17, 17,
									 // p, q,  r,  s,  t,  u,  v,  w,  x,  y,  z, *I, *I, $2, (2, )2,
									 17, 17, 11, 17, 11, 17, 18, 25, 19, 18, 17, 28, 26, 20, 15, 15,
									 //? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
									 20, 20, 20, 20, 29, 22, 19, 19, 19, 19, 9, 9, 9, 9, 23, 23,
									 //? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
									 23, 23, 24, 24, 24, 24, 20, 19, 17, 17, 17, 30, 16, 17, 17, 17,
									 //? ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?  ?
									 17, 11, 11, 15, 12, 17, 17, 17, 17, 17, 17, 17, 17, 21, 17, 19,
									 // 02,12,22, 32, 42, 52, 62, 72, 82, 92, :2, A2, B2, C2, D2, E2,
									 20, 18, 19, 19, 21, 19, 19, 19, 19, 19, 16, 19, 19, 19, 20, 19,
									 // F2,G2,H2, I2, J2, K2, L2, M2, N2, O2, P2, Q2, R2, S2, T2, U2,
									 16, 19, 19, 9, 19, 20, 14, 29, 19, 19, 19, 19, 19, 19, 21, 19,
									 // V2,W2,X2, Y2, Z2, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,
									 20, 32, 20, 19, 19, 19, 19, 19, 19, 29, 19, 19, 19, 19, 19, 9,
									 //?,?,?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, '2, .2,
									 9, 9, 9, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 10, 9,
									 // space, unprop
									 10, 20}}
							   #endif
							   };
#ifdef MORE_LANGUAGES
							   int16 Size_jp[] = { 15, 14, 16, 20, 19, 26, 22, 11, 18, 18, 27, 26, 13,     //; 0
												  19, 20, 27, 19, 15, 19, 19, 21, 19, 20, 18, 19, 15,     //; 13
												  13, 28, 15, 32, 15, 35, 15, 19, 19, 19, 19, 17, 16,     //; 26
												  19, 20, 15, 19, 20, 14, 17, 19, 19, 19, 19, 19, 19,     //; 39
												  19, 19, 20, 25, 20, 19, 19, 33, 31, 39, 37, 39, 37,     //; 52
												  21, 21, 21, 19, 17, 15, 23, 21, 15, 19, 20, 16, 19,     //; 65
												  19, 19, 20, 20, 17, 22, 19, 22, 22, 19, 22, 22, 23,     //; 78
												  35, 35, 35, 35, 37, 19, 19, 19, 19, 29, 19, 19, 19,     //; 91
												  19, 19, 9,  9,  9,  9,  19, 19, 19, 19, 19, 19, 19, 19, //; 104
												  19, 19, 19, 19, 19, 30, 19, 19, 19, 19, 19, 10, 10,     //; 118
												  10, 10, 19, 19, 19, 19, 19, 19, 19, 19, 19, 23, 35,     //; 131
												  12, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,     //; 144
												  19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,     //; 157
												  19, 19, 19, 11, 19, 19, 19, 19, 19, 19, 19, 19, 19,     //; 170
												  19, 19, 19, 19, 19, 19, 19, 19, 19, 21 };
#endif
union tFontRenderStatePointer {
	CFontRenderState *pRenderState;
	wchar *pStr;

	void Align()
	{
		if((uintptr)pStr % 4) pStr++;
	}
};

tFontRenderStatePointer FontRenderStatePointer;
uint8 FontRenderStateBuf[1024] ;

static const short iMaxCharWidth = 28;;
static const float fMaxCharWidth = iMaxCharWidth;;

static CCSprite2d Sprite[3];//英文数字图标
static CCSprite2d Sprite_C[3];//中文
static CFontDetails *Details = NULL;
static CFontRenderState RenderState;

#ifdef BUTTON_ICONS
static int32 ButtonsSlot = -1;
static CCSprite2d ButtonSprite[MAX_BUTTON_ICONS];
static int PS2Symbol;

//static void LoadButtons(const char* txdPath);
//static void DrawButton(float x, float y);
#endif // BUTTON_ICONS

DWORD d_PrintString = 0;
DWORD d_PrintString2 = 0;
DWORD d_RenderFontBuffer = 0;
DWORD d_GetStringWidth = 0;
DWORD d_InitPerFrame = 0;
DWORD d_GetCharacterWidth = 0;
DWORD d_Initialise = 0;
DWORD d_ReloadFonts = 0;
DWORD d_Shutdown = 0;

DWORD d_LoadButton = 0;
//CFont


//CTime
//typedef uint32&(F_GetTimeInMilliseconds)(void);
//F_GetTimeInMilliseconds* GetTimeInMilliseconds = NULL;
uint32* GetTimeInMilliseconds = NULL;

//int32* CCFont::RsGlobal_width = 0;
//int32* CCFont::RsGlobal_height = 0;




const char InitPerFrame_code[] = "83 C8 FF C7 ?? ?? ?? ?? ?? 00 00 00 00 66 ?? ?? ?? ?? ?? 33C0C7 ?? ?? ?? ?? ?? ?? ?? ?? ?? 66";
const char RenderFontBuffer_code[] = "558BEC83EC30813D????????????????0F84";
const char GetStringWidth_code[] = "558BEC51803D????????03";
const char GetCharacterWidth_code[] = "558BEC51A0????????3C03";
const char PrintString1_code[] = "558BEC 83 EC 2C 8D ?? ?? 57 E8";
const char PrintString2_code[] = "558BEC668B????????016639????????01";

const char LoadButtons_code[] = "558BEC 56 68 ?? ?? ?? ?? FF ?? ?? E8 ?? ?? ?? ?? 83 ?? ?? 85 C0";

const char Initialise_code[] = "56 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B F0 83 C4 04 0F";
const char ReloadFonts_code[] = "558BEC 83 ?? ?? ?? ?? ?? FF 0F ?? ?? ?? ?? ?? 53 8A ?? ?? 38";
const char Shutdown_code[] = "83 ?? ?? ?? ?? ?? FF 74 ?? 56 BE ?? ?? ?? ?? 90";

const char GetTimeInMilliseconds_code[] = "B8 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC CC CC B8 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC CC CC D9";

//const char* GetStringWidth_code = "83C8FFC705????????????????66????????0133C0C705????????????????66";
void CCFont::InitPatch(DWORD exe)
{
	const char* ExeName = "reVC.exe";

	//Sprite = (CCSprite2d*)(exe + 0x8167E0);//0x14A67E0;
    //std::vector <DWORD> vResultContainer = MU::ScanMemory(InitPerFrame_code, 0x400000, 0x7fffffff);
	std::vector <ULONGLONG> vResultContainer;

	vResultContainer = MU::ScanMemory(InitPerFrame_code, ExeName);
	CHECK_ERROR(vResultContainer);
	d_InitPerFrame = vResultContainer[0];
	
	
	
	//Details
	DWORD CFontDetails_offset = (DWORD)(d_InitPerFrame + 0x5);
	DWORD CFontDetails_addr = *(DWORD*)(CFontDetails_offset);
	Details = (CFontDetails*)((CFontDetails_addr)-offsetof(struct CFontDetails, anonymous_25));//0x14A68E8;


	vResultContainer = MU::ScanMemory(RenderFontBuffer_code, ExeName);
	CHECK_ERROR(vResultContainer);
	d_RenderFontBuffer= vResultContainer.at(0);
	
	

	vResultContainer = MU::ScanMemory(GetStringWidth_code, ExeName);
	CHECK_ERROR(vResultContainer);
	d_GetStringWidth = vResultContainer.at(0);
	
	

	vResultContainer = MU::ScanMemory(PrintString1_code, ExeName);
	CHECK_ERROR(vResultContainer);
	d_PrintString = vResultContainer.at(0);
	
	vResultContainer = MU::ScanMemory(LoadButtons_code, ExeName);
	CHECK_ERROR(vResultContainer);
	d_LoadButton = vResultContainer.at(0);
	

	//vResultContainer = MU::ScanMemory(PrintString2_code, ExeName);
	//CHECK_ERROR(vResultContainer);
	//d_PrintString2 = vResultContainer.at(0);
	
	

	vResultContainer = MU::ScanMemory(GetCharacterWidth_code, ExeName);
	CHECK_ERROR(vResultContainer);
	d_GetCharacterWidth = vResultContainer.at(0);
	
	
	
	//LanguageSet
	DWORD LanguageSet_offset = (DWORD)(d_GetCharacterWidth + 0x5);
	DWORD LanguageSet_addr = *(DWORD*)(LanguageSet_offset);
	LanguageSet = (uint8*)(LanguageSet_addr);//0x14A68E8;

	vResultContainer = MU::ScanMemory(Initialise_code, ExeName);
	CHECK_ERROR(vResultContainer);

	d_Initialise = vResultContainer.at(0);

	DWORD RsGlobal_offset = (DWORD)d_Initialise + 0xBC;
	DWORD RsGlobal_addr = *(DWORD*)(RsGlobal_offset);
	//RsGlobal_width = (int32*)(exe+0x85F3BC);
	///RsGlobal_height = (int32*)(exe + 0x85F3C0);
	
	RsGlobal_height = (int32*)(RsGlobal_addr);
	RsGlobal_width = RsGlobal_height++;
	//printf_s("RsGlobal_width = %d || RsGlobal_height = %d\n", *RsGlobal_width, *RsGlobal_height);
	

	vResultContainer = MU::ScanMemory(ReloadFonts_code, ExeName);
	CHECK_ERROR(vResultContainer);
	d_ReloadFonts = vResultContainer.at(0);
	
	
	//printf_s("ccccc\n");
	
	vResultContainer = MU::ScanMemory(Shutdown_code, ExeName);
	CHECK_ERROR(vResultContainer);
	d_Shutdown = vResultContainer.at(0);
	
	
	//printf_s("ccccc\n");
	//time
	vResultContainer = MU::ScanMemory(GetTimeInMilliseconds_code, ExeName);
	CHECK_ERROR(vResultContainer);
	DWORD GetTime_offset = vResultContainer.at(0);
	DWORD GetTime_addr = *(DWORD*)(GetTime_offset+ 0x1);
	//GetTimeInMilliseconds = (F_GetTimeInMilliseconds*)(exe + 0x1C1DA0);
	//GetTimeInMilliseconds =(uint32*) (vResultContainer[0] + 0x1);
	GetTimeInMilliseconds = (uint32*)(GetTime_addr);
	
}

void CCFont::InitValue(DWORD exe)
{
	//Sprite = (CCSprite2d*)(exe + 0x8167E0);//0x14A67E0;
	//CFont
	//dPrintString = (F_PrintString*)(exe + 0x198DB0);
	//dPrintChar = (F_PrintChar*)(exe+0x1989D0);
	//dParseToken = (F_ParseToken*)(exe + 0x1986D0);
	//dParseToken1= (F_ParseToken1*)(exe + 0x198320);
	//dFindNewCharacter = (F_FindNewCharacter*)(exe + 0x196EB0);


	//Details = (CFontDetails*)(exe + 0x816818);//0x14A68E8;
	
	//RenderState = (CFontRenderState*)(exe + 0x8167E8);//0x14A67E8;
	//FontRenderStateBuf = (uint8*)0x14A63E0;

	//FontRenderStatePointer = (tFontRenderStatePointer*)(0x14A63D8);

	//d_PrintString = (exe + 0x199230); //007D9230
	//d_RenderFontBuffer = (exe + 0x199860);
	//d_GetStringWidth = (exe + 0x197610);
	//d_InitPerFrame = (exe + 0x197F40);// 0x1C1D50;// 0x197F40;

	//DWORD Details11 = *(DWORD*)(d_InitPerFrame +5);//0x14A68E8;

	//Details = (CFontDetails*)(exe + 0x816818);//0x14A68E8;
	//CFontDetails *Details1 = (CFontDetails*)((Details11-sizeof CFontDetails)+4);//0x14A68E8;
	//InitScan(exe);
	//InitPatch(exe);
	

	
	
	//Ctime
	//GetTimeInMilliseconds = (F_GetTimeInMilliseconds*)(exe + 0x1C1DA0);

	
	//RsGlobal_width = (int32*)(exe+0x85F3BC);
	//RsGlobal_height = (int32*)(exe + 0x85F3C0);


}

void CCFont::InitHook(DWORD exe)
{
	

    //MemUnits::SetMemWrtieable(d_PrintString, 5);
    
    //匹配重载函数
    void(*printstring)(float, float, wchar*) = &CCFont::PrintString_Chs;
    MemUnits::WriteCall(d_PrintString, (DWORD)printstring, MemUnits::CODE_JMP);

	//这个好像没用
	//void(*printstring2)(float x, float y, uint32, wchar * start, wchar * end, float spwidth) = &CCFont::PrintString;
	//MemUnits::WriteCall(d_PrintString2, (DWORD)printstring2, MemUnits::CODE_JMP);

	void(*renderFontBuffer)(void) = &CCFont::RenderFontBuffer_Chs;
	MemUnits::WriteCall(d_RenderFontBuffer, (DWORD)renderFontBuffer, MemUnits::CODE_JMP);

	float(*getStringWidth)(wchar*, bool) = &CCFont::GetStringWidth_Chs;
	MemUnits::WriteCall(d_GetStringWidth, (DWORD)getStringWidth, MemUnits::CODE_JMP);

	void(*initPerFrame)(void) = &CCFont::InitPerFrame;
	MemUnits::WriteCall(d_InitPerFrame, (DWORD)initPerFrame, MemUnits::CODE_JMP);

	void(*initialise)(void) = &CCFont::Initialise;
	MemUnits::WriteCall(d_Initialise, (DWORD)initialise, MemUnits::CODE_JMP);
	
	void(*reloadFonts)(uint8) = &CCFont::ReloadFonts;
	MemUnits::WriteCall(d_ReloadFonts, (DWORD)reloadFonts, MemUnits::CODE_JMP);

	void(*shutdown)(void) = &CCFont::Shutdown;
	MemUnits::WriteCall(d_Shutdown, (DWORD)shutdown, MemUnits::CODE_JMP);

	void(*loadbutton)(const char*) = &CCFont::LoadButtons;
	MemUnits::WriteCall(d_LoadButton, (DWORD)loadbutton, MemUnits::CODE_JMP);
    // std::string ptrr = MemUnits::MakeCallStr(d_PrintString);
}


// 中文支持
#include <cstdio>
#include <cstdlib>
struct CharPos {
	unsigned char rowIndex;
	unsigned char columnIndex;
};

#include "array"
static std::array<CharPos, 0x10000> sTable;

const CharPos& GetCharPos(wchar chr)
{
	return sTable[chr];
}

// 读表
bool ReadTable()
{
	// if(sTable.size() > 0) { sTable.}

	sTable.fill({ 63, 63 });

	FILE* hfile = std::fopen("data/Chinese.dat", "rb");

	if (hfile != nullptr) {
		std::fseek(hfile, 0, SEEK_END);

		if (std::ftell(hfile) == 131072) {
			std::fseek(hfile, 0, SEEK_SET);
			std::fread(sTable.data(), 2, 0x10000, hfile);
		}

		std::fclose(hfile);
		return true;
	}
	else {
		//std::fclose(hfile);
		return false;
	}
}
bool AddMenu = false;
void CCFont::Initialise(void)
{
	int slot,chsslot;
	printf_s("Initialise RUN\n");
	//添加菜单
	if (!AddMenu)
	{
		Add_Memu_Chs();
		AddMenu = true;
	}
	
	slot = CCTxdStore::AddTxdSlot("fonts");
	chsslot = CCTxdStore::AddTxdSlot("chsfonts");
#ifdef MORE_LANGUAGES
	Slot = slot;
	chs_Slot = chsslot;
	switch (*LanguageSet) {
	case FONT_LANGSET_EFIGS:
	default: CCTxdStore::LoadTxd(slot, "MODELS/FONTS.TXD"); break;
	case FONT_LANGSET_POLISH: CCTxdStore::LoadTxd(slot, "MODELS/FONTS_P.TXD"); break;
	case FONT_LANGSET_RUSSIAN: CCTxdStore::LoadTxd(slot, "MODELS/FONTS_R.TXD"); break;
	case FONT_LANGSET_JAPANESE: CCTxdStore::LoadTxd(slot, "MODELS/FONTS_J.TXD"); break;
	case FONT_LANGSET_CHINESE: CCTxdStore::LoadTxd(slot, "MODELS/FONTS.TXD"); break;
	}
	if (IsChinese()) {
		
		if (!ReadTable()) {
			LanguageSet = 0;
			//CCTxdStore::RemoveTxd(slot);
		}
		
	}

#else
	CTxdStore::LoadTxd(slot, "MODELS/FONTS.TXD");
#endif
	CCTxdStore::AddRef(slot);
	CCTxdStore::PushCurrentTxd();
	CCTxdStore::SetCurrentTxd(slot);

#ifdef MORE_LANGUAGES
	
	Sprite[0].SetTexture("font2", "font2m");
	Sprite[1].SetTexture("font1", "font1m");

	if (IsJapanese()) {
		Sprite[1].SetTexture("FONTJAP", "FONTJAP_mask");
		Sprite[3].SetTexture("FONTJAP", "FONTJAP_mask");
	}

#endif // MORE_LANGUAGES

	// Sprite[0].SetTexture("font2", "font2m");
	// Sprite[1].SetTexture("font1", "font1m");

	
	SetScale(1.0f, 1.0f);
	SetSlantRefPoint(SCREEN_WIDTH, 0.0f);
	SetSlant(0.0f);
	SetColor(CRGBA(255, 255, 255, 0));
	SetJustifyOff();
	SetCentreOff();
	SetWrapx(SCREEN_WIDTH);
	SetCentreSize(SCREEN_WIDTH);
	SetBackgroundOff();
	SetBackgroundColor(CRGBA(128, 128, 128, 128));
	SetBackGroundOnlyTextOff();
	SetPropOn();
	SetFontStyle(FONT_BANK);
	SetRightJustifyWrap(0.0f);
	SetAlphaFade(255.0f);
	SetDropShadowPosition(0);
	CCTxdStore::PopCurrentTxd();

	if (IsChinese()) {
		CCTxdStore::LoadTxd(chs_Slot, "MODELS/CHINESE.TXD");
		CCTxdStore::AddRef(chs_Slot);
		CCTxdStore::PushCurrentTxd();
		CCTxdStore::SetCurrentTxd(chs_Slot);
		Sprite_C[0].SetTexture("normal", "normalm");
		Sprite_C[1].SetTexture("slant", "slantm");
		Sprite_C[2].SetTexture("regular", "regularm");
		CCTxdStore::PopCurrentTxd();

		
	}

#if !defined(GAMEPAD_MENU) && defined(BUTTON_ICONS)
	// loaded in CMenuManager with GAMEPAD_MENU defined
	LoadButtons("MODELS/X360BTNS.TXD");
#endif
}


#ifdef BUTTON_ICONS

void
CCFont::LoadButtons(const char* txdPath)
{
	if (int file = CCFileMgr::OpenFile(txdPath, "rb")) {
		CCFileMgr::CloseFile(file);
		if (ButtonsSlot == -1)
			ButtonsSlot = CCTxdStore::AddTxdSlot("buttons");
		else {
			for (int i = 0; i < MAX_BUTTON_ICONS; i++) ButtonSprite[i].Delete();
			CCTxdStore::RemoveTxd(ButtonsSlot);
		}
		CCTxdStore::LoadTxd(ButtonsSlot, txdPath);
		CCTxdStore::AddRef(ButtonsSlot);
		CCTxdStore::PushCurrentTxd();
		CCTxdStore::SetCurrentTxd(ButtonsSlot);
		ButtonSprite[BUTTON_UP].SetTexture("thumblyu");
		ButtonSprite[BUTTON_DOWN].SetTexture("thumblyd");
		ButtonSprite[BUTTON_LEFT].SetTexture("thumblxl");
		ButtonSprite[BUTTON_RIGHT].SetTexture("thumblxr");
		ButtonSprite[BUTTON_CROSS].SetTexture("cross");
		ButtonSprite[BUTTON_CIRCLE].SetTexture("circle");
		ButtonSprite[BUTTON_SQUARE].SetTexture("square");
		ButtonSprite[BUTTON_TRIANGLE].SetTexture("triangle");
		ButtonSprite[BUTTON_L1].SetTexture("l1");
		ButtonSprite[BUTTON_L2].SetTexture("l2");
		ButtonSprite[BUTTON_L3].SetTexture("l3");
		ButtonSprite[BUTTON_R1].SetTexture("r1");
		ButtonSprite[BUTTON_R2].SetTexture("r2");
		ButtonSprite[BUTTON_R3].SetTexture("r3");
		ButtonSprite[BUTTON_RSTICK_UP].SetTexture("thumbryu");
		ButtonSprite[BUTTON_RSTICK_DOWN].SetTexture("thumbryd");
		ButtonSprite[BUTTON_RSTICK_LEFT].SetTexture("thumbrxl");
		ButtonSprite[BUTTON_RSTICK_RIGHT].SetTexture("thumbrxr");
		CCTxdStore::PopCurrentTxd();
	}
	else {
		if (ButtonsSlot != -1) {
			for (int i = 0; i < MAX_BUTTON_ICONS; i++) ButtonSprite[i].Delete();
			CCTxdStore::RemoveTxdSlot(ButtonsSlot);
			ButtonsSlot = -1;
		}
	}
}
#endif // BUTTON_ICONS

#ifdef MORE_LANGUAGES
void
CCFont::ReloadFonts(uint8 set)
{
	if (Slot != -1 && *LanguageSet != set) {
		*LanguageSet = set;
		Sprite[0].Delete();
		Sprite[1].Delete();

		if (IsJapanese()) Sprite[2].Delete();

		
		// CTxdStore::PushCurrentTxd();
		CCTxdStore::RemoveTxd(Slot);
		CCTxdStore::RemoveTxd(chs_Slot);
		switch (set) {
		case FONT_LANGSET_EFIGS:
		default: CCTxdStore::LoadTxd(Slot, "MODELS/FONTS.TXD"); break;
		case FONT_LANGSET_POLISH: CCTxdStore::LoadTxd(Slot, "MODELS/FONTS_P.TXD"); break;
		case FONT_LANGSET_RUSSIAN: CCTxdStore::LoadTxd(Slot, "MODELS/FONTS_R.TXD"); break;
		case FONT_LANGSET_JAPANESE: CCTxdStore::LoadTxd(Slot, "MODELS/FONTS_J.TXD"); break;
		case FONT_LANGSET_CHINESE: CCTxdStore::LoadTxd(Slot, "MODELS/FONTS.TXD"); break;
		}
		CCTxdStore::PushCurrentTxd();
		CCTxdStore::SetCurrentTxd(Slot);
		Sprite[0].SetTexture("font2", "font2m");
		Sprite[1].SetTexture("font1", "font1m");
		

		if (IsChinese()) {
			if (!ReadTable())
			{
				LanguageSet = 0;
			}
			else
			{
				CCTxdStore::PopCurrentTxd();

				//Sprite_C[0].Delete();
				//Sprite_C[1].Delete();
				CCTxdStore::RemoveTxd(chs_Slot);
				CCTxdStore::LoadTxd(chs_Slot, "MODELS/CHINESE.TXD");
				CCTxdStore::PushCurrentTxd();
				CCTxdStore::SetCurrentTxd(chs_Slot);
				Sprite_C[0].SetTexture("normal", "normalm");
				Sprite_C[1].SetTexture("slant", "slantm");
				Sprite_C[2].SetTexture("regular", "regularm");
				//CCTxdStore::PopCurrentTxd();
			}


			
		}
		if (IsChinese()) {
			

			//CCTxdStore::PopCurrentTxd();

			//CCTxdStore::LoadTxd(Slot, "MODELS/FONTS.TXD");
			//CCTxdStore::PushCurrentTxd();
			//CCTxdStore::SetCurrentTxd(Slot);

			// Sprite[2].SetTexture("slant", "slantm");
		}
		else {
			//clickNUM = 0;
		}
		

		//if (set == FONT_LANGSET_JAPANESE) { Sprite[2].SetTexture("FONTJAP", "FONTJAP_mask"); }

		CCTxdStore::PopCurrentTxd();
	}
	*LanguageSet = set;
	//if (clickNUM < 6) { clickNUM++; }
}
#endif

void CCFont::Shutdown(void)
{
#ifdef BUTTON_ICONS
	if (ButtonsSlot != -1) {
		for (int i = 0; i < MAX_BUTTON_ICONS; i++) ButtonSprite[i].Delete();
		CCTxdStore::RemoveTxdSlot(ButtonsSlot);
		ButtonsSlot = -1;
	}
#endif
	Sprite[0].Delete();
	Sprite[1].Delete();
#ifdef MORE_LANGUAGES

	if (IsJapanese()) Sprite[2].Delete();
	if (IsChinese()) {

		Sprite_C[0].Delete();
		Sprite_C[1].Delete();
		Sprite_C[2].Delete();
		//CCTxdStore::SetCurrentTxd(chs_Slot);
		CCTxdStore::RemoveTxdSlot(chs_Slot);
		chs_Slot = -1;
	}
	//CCTxdStore::SetCurrentTxd(Slot);
	CCTxdStore::RemoveTxdSlot(Slot);
		
	
	Slot = -1;
	
#else
	CTxdStore::RemoveTxdSlot(CTxdStore::FindTxdSlot("fonts"));
#endif
}



#ifdef BUTTON_ICONS
void
CCFont::DrawButton(float x, float y)
{
	if (x <= 0.0f || x > SCREEN_WIDTH || y <= 0.0f || y > SCREEN_HEIGHT) return;

	if (PS2Symbol != BUTTON_NONE) {
		CRect rect;
		rect.left = x;
		rect.top = RenderState.scaleY + RenderState.scaleY + y;
		rect.right = RenderState.scaleY * 17.0f + x;
		rect.bottom = RenderState.scaleY * 19.0f + y;

		int vertexAlphaState;
		void* raster;
		
		CCSprite2d::RwRwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, &vertexAlphaState);
		CCSprite2d::RwRwRenderStateGet(rwRENDERSTATETEXTURERASTER, &raster);
		CCSprite2d::RwRwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
		if (RenderState.bIsShadow)
			ButtonSprite[PS2Symbol].Draw(rect, RenderState.color);
		else
			ButtonSprite[PS2Symbol].Draw(rect, CRGBA(255, 255, 255, RenderState.color.a));
		CCSprite2d::RwRwRenderStateSet(rwRENDERSTATETEXTURERASTER, raster);
		CCSprite2d::RwRwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)vertexAlphaState);
	}
}
#endif


void CCFont::PrintString(float x, float y, wchar* s)
{
    
	PrintString_Chs(x, y, s);
}




void /*__declspec(naked)*/ CCFont::InitPerFrame(void)
{
	
	RenderState.style = -1;
	Details->anonymous_25 = 0;
	FontRenderStatePointer.pRenderState = (CFontRenderState*)FontRenderStateBuf;
	//SetDropShadowPosition(0);
	Details->dropShadowPosition = 0;
	NewLine = false;
	/*__asm
	{
		or eax, 0xFFFFFFFF 
		mov dword ptr ds : [0xAF686C] , 0 
		mov word ptr ds : [0xAF6814] , ax
		xor eax, eax 
		//mov dword ptr ds : [FontRenderStatePointer] , FontRenderStateBuf
		
	};

	FontRenderStatePointer.pRenderState = (CFontRenderState*)FontRenderStateBuf;

	__asm 
	{
		mov word ptr ds : [0xAF6858] , ax
		//mov word ptr ds : [CCFont::NewLine] , ax 
		//mov dword ptr ds : [<public: static int CFont::PS2Symbol>] , FFFFFFFF //| Font.cpp : 485
		ret
	}*/

}
void CCFont::RenderFontBuffer_Chs()
{
	bool bBold = false;
	bool bFlash = false;

	CRGBA bColor;

	CVector2D pos;

	wchar var_char;

	// FontBufferPointer pbuffer;

	// if(FontBufferIter->addr == FontBuffer.addr) { return; }
	if (FontRenderStatePointer.pRenderState == (CFontRenderState *)FontRenderStateBuf)return;

	RenderState = *(CFontRenderState*)&FontRenderStateBuf[0];

	// Sprite[RenderState.style].SetRenderState();
	// RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);

	bColor = RenderState.color;
	// var_14 = FontBuffer.pdata->Color;

	// pos = RenderState.Pos;
	pos.x = RenderState.fTextPosX;
	pos.y = RenderState.fTextPosY;

	// pbuffer.addr = FontBuffer.addr + 0x30;
	tFontRenderStatePointer pRenderStateBufPointer;
	pRenderStateBufPointer.pRenderState = (CFontRenderState*)&FontRenderStateBuf[0];
	
	CCSprite2d::RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

	for (++pRenderStateBufPointer.pRenderState; pRenderStateBufPointer.pStr < FontRenderStatePointer.pStr; pRenderStateBufPointer.pStr++) {
		if (*pRenderStateBufPointer.pStr == '\0') {
			//++pbuffer.ptext;

			// if((pbuffer.addr & 3) != 0) { ++pbuffer.ptext; }

			// if(pbuffer.addr >= FontBufferIter->addr) { break; }
			tFontRenderStatePointer tmpPointer = pRenderStateBufPointer;
			tmpPointer.pStr++;
			tmpPointer.Align();
			if (tmpPointer.pStr >= FontRenderStatePointer.pStr) break;

			//*RenderState = *pbuffer.pdata;
			// var_14 = RenderState.Color;
			// pos = RenderState.Pos;
			RenderState = *(tmpPointer.pRenderState++);

			pRenderStateBufPointer = tmpPointer;

			pos.x = RenderState.fTextPosX;
			pos.y = RenderState.fTextPosY;
			bColor = RenderState.color;

			// pbuffer.addr += 0x30;
		}
		
		if (*pRenderStateBufPointer.pStr == '~') {
			// pbuffer.ptext = fpParseTokenEPtR5CRGBARbRb.fun(pbuffer.ptext, var_14, var_E, var_D);
#ifdef BUTTON_ICONS
			PS2Symbol = BUTTON_NONE;
#endif
			//std::wstring ss((wchar_t*)pRenderStateBufPointer.pStr);
			//wprintf_s(ss.c_str());
			//wprintf_s(L"\n");
			//if(pRenderStateBufPointer.pStr!=NULL)
			pRenderStateBufPointer.pStr = ParseToken(pRenderStateBufPointer.pStr, bColor, bFlash, bBold);
#ifdef BUTTON_ICONS
			if (PS2Symbol != BUTTON_NONE) {
				DrawButton(pos.x, pos.y);
				pos.x += RenderState.scaleY * 17.0f;
				PS2Symbol = BUTTON_NONE;
			}
#endif

			if (bFlash) {

				// if((*CTimer::m_nTimeInMilliseconds - Details->BlipStartTime) > 300) {
				//	Details->IsBlip = true;
				//	Details->BlipStartTime = *CTimer::m_nTimeInMilliseconds;
				// }
				if (*GetTimeInMilliseconds - Details->nFlashTimer > 300) {
					Details->bFlashState = !Details->bFlashState;
					Details->nFlashTimer = *GetTimeInMilliseconds;
				}

				// if(Details->IsBlip) {
				//	Details->Color.alpha = 0;
				// } else {
				//	Details->Color.alpha = 255;
				// }
				Details->color.alpha = Details->bFlashState ? 0 : 255;
			}

			// if(!RenderState.KeepColor) { RenderState.Color = var_14; }
			if (!RenderState.bIsShadow) RenderState.color = bColor;
		}

		wchar c = *pRenderStateBufPointer.pStr;

		// c -= ' ';
		// if(RenderState.bFontHalfTexture)
		//	c = FindNewCharacter(c);
		// else if(c > 155)
		//	c = '\0';

		// if(RenderState.Slant != 0.0f) { pos.y = (RenderState.SlantRefPoint.x - pos.x) * RenderState.Slant + RenderState.SlantRefPoint.y; }
		if (RenderState.slant != 0.0f) pos.y = (RenderState.slantRefX - pos.x) * RenderState.slant + RenderState.slantRefY;

		// var_char = *pbuffer.ptext;
		var_char = *pRenderStateBufPointer.pStr;

		if (var_char < 0x80) {
			// CSprite2d::SetRenderState(&Sprite[RenderState.FontStyle], 0);
			Sprite[RenderState.style].SetRenderState();
			// pos.x += 2.0f;
			/*
			CRect rct = CRect();
			rct.left = 400;
			rct.top = 200;
			rct.right = 800;
			rct.bottom = 600;
			CCSprite2d::AddToBuffer(rct, CRGBA(0, 0, 0, 255), 0, 0, 1, 0, 0, 1, 1, 1);*/
			
			// RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);
		}
		else {
			//判断是否强制使用字体
			if (current_font_set!=FT_NONE)
			{
				switch (current_font_set)
				{
				case FT_normal: Sprite_C[0].SetRenderState();break;
				case FT_slant: Sprite_C[1].SetRenderState(); break;
				case FT_regular: Sprite_C[2].SetRenderState(); break;

				}
			}else
			if (RenderState.style != 0) {
				// CSprite2d::fpSetRenderState.fun(&ChsSprite, 0);
				//暂时注释
				//Sprite[2].SetRenderState();
				Sprite_C[0].SetRenderState();
			}
			else {
				// CSprite2d::fpSetRenderState.fun(&ChsSlantSprite, 0);
				//暂时注释
				//Sprite[3].SetRenderState();
				Sprite_C[1].SetRenderState();
			}
		}

		// rwFunc::fpRwRenderStateSet.fun(RwRenderState::rwRENDERSTATEVERTEXALPHAENABLE, (void *)1);

		PrintCharDispatcher(pos.x, pos.y, c);

		if (bBold) {
			PrintCharDispatcher(pos.x + 1.0f, pos.y, c);
			PrintCharDispatcher(pos.x + 2.0f, pos.y, c);
			pos.x += 2.0f;
		}

		// CSprite2d::fpRenderVertexBuffer.fun();

		pos.x += GetCharacterSizeDrawing(c);

		if (c == '\0') { pos.x += RenderState.fExtraSpace; }

		++*pRenderStateBufPointer.pStr;

#ifdef FIX_BUGS
		// PS2 uses different chars for some symbols
		if (!RenderState.bFontHalfTexture && c == 30) c = 61; // wanted star
#endif
		CCSprite2d::RenderVertexBuffer();
		
	}
	
	// FontBufferIter->addr = FontBuffer.addr;
	FontRenderStatePointer.pRenderState = (CFontRenderState*)FontRenderStateBuf;
	current_font_set = FT_NONE;//恢复默认
}
void CCFont::PrintCharDispatcher(float arg_x, float arg_y, wchar arg_char)
{
	// std::wstring rr;
	//  std::map<int, Character> *d = f;

	// rr.push_back(arg_char);
	// CFontRenderState ftt = RenderState;
	if (arg_char < 0x80) {
		//wchar s[2] = { arg_char, '\0' };
		// if((arg_char >= '0' && arg_char <= ':')) arg_char += 128;

		// Details
		// RenderState.style = Details->style;
		if (RenderState.style == 0) {
			// if((arg_char >= '0' && arg_char <= ':'))
			// arg_char += 128;
		}
		arg_char -= 0x20;
		if (RenderState.bFontHalfTexture) { arg_char = FindNewCharacter(arg_char); }


		PrintChar(arg_x, arg_y, arg_char); // 加128钱数显示文本正常

	}
	else {
		//dPrintChar(arg_x, arg_y, arg_char); // 加128钱数显示文本正常
		PrintCHSChar(arg_x, arg_y, arg_char);
	}
}
void CCFont::PrintCHSChar(float arg_x, float arg_y, wchar arg_char)
{
	static const float rRowsCount = 1.0f / 64.0f;
	static const float rColumnsCount = 1.0f / 64.0f;
	static const float ufix = 0.001f / 4.0f;
	// static const float vfix = 0.0021f / 4.0f;
	static const float vfix = 0.001f / 4.0f;
	static const float vfix1_slant = 0.00055f / 4.0f;
	// static const float vfix2_slant = 0.01f / 4.0f;
	static const float vfix2_slant = 0.007f / 4.0f;
	static const float vfix3_slant = 0.009f / 4.0f;

	CRect rect;

	float yOffset;

	float u1, v1, u2, v2, u3, v3, u4, v4;

	CharPos pos;

	// if(arg_x >= rw::UserDataGlobals || arg_x <= 0.0f || arg_y <= 0.0f || arg_y >= SCREEN_HEIGHT) { return; }
	if (arg_x <= 0.0f || arg_x > SCREEN_WIDTH || arg_y <= 0.0f || arg_y > SCREEN_HEIGHT) // BUG: game uses SCREENW again
		return;

	pos = GetCharPos(arg_char);

	yOffset = RenderState.scaleY * 2.0f;

	if (RenderState.slant == 0.0f) {
		rect.left = arg_x;
		rect.top = arg_y + yOffset;
		rect.right = RenderState.scaleX * 32.0f + arg_x;
		rect.bottom = RenderState.scaleY * 16.0f + arg_y + yOffset;

		u1 = pos.columnIndex * rColumnsCount;
		v1 = pos.rowIndex * rRowsCount;

		u2 = (pos.columnIndex + 1) * rColumnsCount - ufix;
		v2 = v1;

		u3 = u1;
		v3 = (pos.rowIndex + 1) * rRowsCount - vfix;

		u4 = u2;
		v4 = v3;
	}
	else {
		rect.left = arg_x;
		rect.top = arg_y + 0.015f + yOffset;
		rect.right = RenderState.scaleX * 32.0f + arg_x;
		rect.bottom = RenderState.scaleY * 16.0f + arg_y + yOffset;

		u1 = pos.columnIndex * rColumnsCount;
		v1 = pos.rowIndex * rRowsCount + vfix1_slant;
		u2 = (pos.columnIndex + 1) * rColumnsCount - ufix;
		v2 = pos.rowIndex * rRowsCount + vfix + vfix2_slant;
		u3 = pos.columnIndex * rColumnsCount;
		v3 = (pos.rowIndex + 1) * rRowsCount - vfix3_slant;
		u4 = (pos.columnIndex + 1) * rColumnsCount - ufix;
		v4 = (pos.rowIndex + 1) * rRowsCount + vfix2_slant - vfix;
	}

	// CSprite2d::fpAddToBuffer.fun(rect, RenderState.Color, u1, v1, u2, v2, u3, v3, u4, v4);
	//CCSprite2d::AddToBuffer(rect, RenderState.color, u2, v1, u1, v2, u4, v3, u3, v4);
	CCSprite2d::AddToBuffer(rect, RenderState.color, u1, v1, u2, v2, u3, v3, u4, v4);
	// CSprite2d::AddToBuffer(rect, RenderState.color, 0, 0, 1, 0, 0, 1, 1, 1);
}

static bool IsJapaneseFont() 
{ 
	return CCFont::IsJapanese() && (Details->style == FONT_JAPANESE);
}
void CCFont::PrintChar(float x, float y, wchar c)
{
	bool bDontPrint = false;
	if (x <= 0.0f || x > SCREEN_WIDTH || y <= 0.0f || y > SCREEN_HEIGHT) // BUG: game uses SCREENW again
		return;

	bDontPrint = c == '\0';
	float w = GetCharacterWidth(c) / 32.0f;
	if (Details->bFontHalfTexture && c == 208) c = '\0';
	float xoff = c % 16;
	float yoff = c / 16;
#ifdef MORE_LANGUAGES
	if (IsJapaneseFont()) {
		w = 21.0f;
		xoff = (float)(c % 48);
		yoff = c / 48;
	}

#endif

	if (RenderState.style == FONT_BANK || RenderState.style == FONT_STANDARD) {
		if (bDontPrint) return;
		if (RenderState.slant == 0.0f) {
#ifdef FIX_BUGS
			if (c < 192) {
#else
			if (c < 193) {
#endif
				CCSprite2d::AddToBuffer(CRect(x, y, x + 32.0f * RenderState.scaleX * 1.0f, y + 40.0f * RenderState.scaleY * 0.5f),
					RenderState.color, xoff / 16.0f, yoff / 12.8f + 0.0021f, (xoff + 1.0f) / 16.0f - 0.001f,
					yoff / 12.8f + 0.0021f, xoff / 16.0f, (yoff + 1.0f) / 12.8f - 0.0021f, (xoff + 1.0f) / 16.0f - 0.001f,
					(yoff + 1.0f) / 12.8f - 0.0021f);
			}
			else {
				CCSprite2d::AddToBuffer(CRect(x, y, x + 32.0f * RenderState.scaleX * 1.0f, y + 33.0f * RenderState.scaleY * 0.5f),
					RenderState.color, xoff / 16.0f, yoff / 12.8f + 0.0021f, (xoff + 1.0f) / 16.0f - 0.001f,
					yoff / 12.8f + 0.0021f, xoff / 16.0f, (yoff + 1.0f) / 12.8f - 0.017f, (xoff + 1.0f) / 16.0f - 0.001f,
					(yoff + 1.0f) / 12.8f - 0.017f);
			}
			}
		else
			CCSprite2d::AddToBuffer(CRect(x, y, x + 32.0f * RenderState.scaleX * 1.0f, y + 40.0f * RenderState.scaleY * 0.5f), RenderState.color,
				xoff / 16.0f, yoff / 12.8f + 0.00055f, (xoff + 1.0f) / 16.0f - 0.001f, yoff / 12.8f + 0.0021f + 0.01f,
				xoff / 16.0f, (yoff + 1.0f) / 12.8f - 0.009f, (xoff + 1.0f) / 16.0f - 0.001f,
				(yoff + 1.0f) / 12.8f - 0.0021f + 0.01f);
#ifdef MORE_LANGUAGES

		/*}else if (IsJapaneseFont()) {
				if (Details->dropShadowPosition != 0) {
						CSprite2d::AddSpriteToBank(Details->bank + Details->style,	// BUG: game doesn't add bank
								CRect(x + SCREEN_SCALE_X(Details->dropShadowPosition),
										y + SCREEN_SCALE_Y(Details->dropShadowPosition),
										x + SCREEN_SCALE_X(Details->dropShadowPosition) + 32.0f * Details->scaleX * 1.0f,
										y + SCREEN_SCALE_Y(Details->dropShadowPosition) + 40.0f * Details->scaleY / 2.75f),
								Details->dropColor,
								xoff * w / 1024.0f, yoff / 25.6f,
								xoff * w / 1024.0f + (1.0f / 48.0f) - 0.001f, yoff / 25.6f,
								xoff * w / 1024.0f, (yoff + 1.0f) / 25.6f,
								xoff * w / 1024.0f + (1.0f / 48.0f) - 0.001f, (yoff + 1.0f) / 25.6f - 0.0001f);
				}
				CSprite2d::AddSpriteToBank(Details->bank + Details->style,	// BUG: game doesn't add bank
						CRect(x, y,
								x + 32.0f * Details->scaleX * 1.0f,
								y + 40.0f * Details->scaleY / 2.75f),
						Details->color,
						xoff * w / 1024.0f, yoff / 25.6f,
						xoff * w / 1024.0f + (1.0f / 48.0f) - 0.001f, yoff / 25.6f,
						xoff * w / 1024.0f, (yoff + 1.0f) / 25.6f - 0.002f,
						xoff * w / 1024.0f + (1.0f / 48.0f) - 0.001f, (yoff + 1.0f) / 25.6f - 0.0001f);*/
#endif
		}
	else {
		if (bDontPrint) return;
		CCSprite2d::AddToBuffer(CRect(x, y, x + 32.0f * RenderState.scaleX * w, y + 32.0f * RenderState.scaleY * 0.5f), RenderState.color, xoff / 16.0f,
			yoff / 16.0f, (xoff + w) / 16.0f, yoff / 16.0f, xoff / 16.0f, (yoff + 1.0f) / 16.0f, (xoff + w) / 16.0f - 0.0001f,
			(yoff + 1.0f) / 16.0f - 0.0001f);
	}
	}


void CCFont::PrintString_Chs(float arg_x, float arg_y, wchar* arg_text)
{
	//std::wstring ss((wchar_t*)arg_text);
	//wprintf_s(ss.c_str());
	//wprintf_s(L"\n");
	//printf_s("RsGlobal_width = %d || RsGlobal_height = %d\n", *RsGlobal_width, *RsGlobal_height);
	CRect textBoxRect;

	float xBound;
	float yBound = arg_y;
	float strWidth, widthLimit;
	float var_38 = 0.0f;
	float print_x;
	float justifyWrap;

	wchar* ptext = arg_text;
	wchar* strHead = arg_text;

	bool emptyLine = true;

	short numSpaces = 0;

	Details->anonymous_23 = false;

	if (*arg_text == '*') { return; }
	//std::wstring tm = NewFont::WcharTOWstring(arg_text);

	++Details->anonymous_25;

	if (Details->background) {
		GetTextRect_Chs(&textBoxRect, arg_x, arg_y, arg_text);
		CCSprite2d::DrawRect(textBoxRect, Details->backgroundColor);
	}

	if (Details->centre || Details->rightJustify) {
		xBound = 0.0f;
	}
	else {
		xBound = arg_x;
	}

	while (*ptext != 0) {
		strWidth = GetStringWidth_Chs(ptext, false);
		std::wstring ss((wchar_t*)ptext);
		if (Details->centre) {
			widthLimit = Details->centreSize;
		}
		else if (Details->rightJustify) {
			widthLimit = arg_x - Details->rightJustifyWrap;
		}
		else {
			widthLimit = Details->wrapX;
		}

		if (((xBound + strWidth) <= widthLimit) || emptyLine) {
			ptext = GetNextSpace_Chs(ptext);
			xBound += strWidth;

			if (*ptext != 0) {
				if (*ptext == ' ') {
					if (*(ptext + 1) == 0) {
						*ptext = 0;
					}
					else {
						if (!emptyLine) { ++numSpaces; }

						xBound += GetCharacterSizeNormal(' ');
						++ptext;
					}
				}

				emptyLine = false;

				var_38 = xBound;
			}
			else {
				if (Details->centre) {
					print_x = arg_x - xBound * 0.5f;
				}
				else if (Details->rightJustify) {
					print_x = arg_x - xBound;
				}
				else {
					print_x = arg_x;
				}

				PrintString(print_x, yBound, 0, strHead, ptext, 0.0f);
			}
		}
		else {
			if (Details->justify && !(Details->centre)) {
				justifyWrap = (Details->wrapX - var_38) / numSpaces;
			}
			else {
				justifyWrap = 0.0f;
			}

			if (Details->centre) {
				print_x = arg_x - xBound * 0.5f;
			}
			else if (Details->rightJustify) {
				print_x = arg_x - xBound;
			}
			else {
				print_x = arg_x;
			}

			PrintString(print_x, yBound, 0, strHead, ptext, justifyWrap);

			strHead = ptext;

			if (Details->centre || Details->rightJustify) {
				xBound = 0.0f;
			}
			else {
				xBound = arg_x;
			}

			yBound += Details->scaleY * 18.0f;
			var_38 = 0.0f;
			numSpaces = 0;
			emptyLine = true;
		}
	}
}

void CCFont::PrintString(float x, float y, uint32 a, wchar* start, wchar* end, float spwidth)
{
	wchar* s;

	if (RenderState.style != Details->style) {
		RenderFontBuffer_Chs();
		RenderState.style = Details->style;
	}
	//std::wstring ss((wchar_t*)start);
	//wprintf_s(ss.c_str());
	//wprintf_s(L"\n");
	float dropShadowPosition = Details->dropShadowPosition;
	if (dropShadowPosition != 0.0f && (Details->style == FONT_BANK || Details->style == FONT_STANDARD)) {
		CRGBA color = Details->color;
		Details->color = Details->dropColor;
		Details->dropShadowPosition = 0;
		Details->bIsShadow = true;
		if (Details->slant != 0.0f) {
			Details->slantRefX += SCREEN_SCALE_X(dropShadowPosition);
			Details->slantRefY += SCREEN_SCALE_Y(dropShadowPosition);
			PrintString(SCREEN_SCALE_X(dropShadowPosition) + x, SCREEN_SCALE_Y(dropShadowPosition) + y, Details->anonymous_25, start, end, spwidth);
			Details->slantRefX -= SCREEN_SCALE_X(dropShadowPosition);
			Details->slantRefY -= SCREEN_SCALE_Y(dropShadowPosition);
		}
		else {
			PrintString(SCREEN_SCALE_X(dropShadowPosition) + x, SCREEN_SCALE_Y(dropShadowPosition) + y, Details->anonymous_25, start, end, spwidth);
		}
		Details->color = color;
		Details->dropShadowPosition = dropShadowPosition;
		Details->bIsShadow = false;
	}
	if (FontRenderStatePointer.pStr >= (wchar*)&FontRenderStateBuf[ARRAY_SIZE(FontRenderStateBuf)] - (end - start + 26)) // why 26?
		RenderFontBuffer_Chs();
	CFontRenderState* pRenderState = FontRenderStatePointer.pRenderState;
	pRenderState->fTextPosX = x;
	pRenderState->fTextPosY = y;
	pRenderState->scaleX = Details->scaleX;
	pRenderState->scaleY = Details->scaleY;
	pRenderState->color = Details->color;
	pRenderState->fExtraSpace = spwidth;
	pRenderState->slant = Details->slant;
	pRenderState->slantRefX = Details->slantRefX;
	pRenderState->slantRefY = Details->slantRefY;
	pRenderState->bFontHalfTexture = Details->bFontHalfTexture;
	pRenderState->proportional = Details->proportional;
	pRenderState->style = Details->style;
	pRenderState->bIsShadow = Details->bIsShadow;
	FontRenderStatePointer.pRenderState++;

	for (s = start; s < end;) {
		if (*s == '~') {
			for (wchar* i = ParseToken(s); s != i; FontRenderStatePointer.pStr++) { *FontRenderStatePointer.pStr = *(s++); }
			if (Details->bFlash) {
				if (*GetTimeInMilliseconds - Details->nFlashTimer > 300) {
					Details->bFlashState = !Details->bFlashState;
					Details->nFlashTimer = *GetTimeInMilliseconds;
				}
				Details->color.a = Details->bFlashState ? 0 : 255;
			}
		}
		else
			*(FontRenderStatePointer.pStr++) = *(s++);
	}
	*(FontRenderStatePointer.pStr++) = '\0';
	FontRenderStatePointer.Align();
}

void CCFont::GetTextRect_Chs(CRect* rect, float xstart, float ystart, wchar* s)
{
	short numLines = GetNumberLines_Chs(xstart, ystart, s);

	if (Details->centre) {
		if (Details->backgroundOnlyText) {
			rect->left = xstart - 4.0f;
			rect->right = xstart + 4.0f;
			rect->bottom = (18.0f * Details->scaleY) * numLines + ystart + 2.0f;
			rect->top = ystart - 2.0f;
		}
		else {
			rect->left = xstart - (Details->centreSize * 0.5f) - 4.0f;
			rect->right = xstart + (Details->centreSize * 0.5f) + 4.0f;
			rect->bottom = ystart + (18.0f * Details->scaleY * numLines) + 2.0f;
			rect->top = ystart - 2.0f;
		}
	}
	else {
		rect->left = xstart - 4.0f;
		rect->right = Details->wrapX;
		rect->bottom = ystart;
		rect->top = (18.0f * Details->scaleY) * numLines + ystart + 4.0f;
	}
}

int CCFont::GetNumberLines_Chs(float xstart, float ystart, wchar* s)
{
	int result = 0;
	float xBound;
	float yBound = ystart;
	float strWidth, widthLimit;

	if (Details->centre || Details->rightJustify) {
		xBound = 0.0f;
	}
	else {
		xBound = xstart;
	}

	while (*s != 0) {
		strWidth = GetStringWidth_Chs(s, false);

		if (Details->centre) {
			widthLimit = Details->centreSize;
		}
		else {
			widthLimit = Details->wrapX;
		}

		if ((xBound + strWidth) <= widthLimit) {
			xBound += strWidth;
			s = GetNextSpace_Chs(s);

			if (*s == ' ') {
				xBound += GetCharacterSizeNormal(' ');
				++s;
			}
			else if (*s == 0) {
				++result;
			}
		}
		else {
			if (Details->centre || Details->rightJustify) {
				xBound = 0.0f;
			}
			else {
				xBound = xstart;
			}

			++result;
			yBound += Details->scaleY * 18.0f;
		}
	}

	return result;
}

float CCFont::GetStringWidth_Chs(wchar* s, bool spaces)
{
	float result = 0.0f;

	
	while (*s != '\0') {
		if (*s == ' ') {
			if (spaces) {
				result += GetCharacterSizeNormal(' ');
			}
			else {
				break;
			}
		}
		else if (*s == '~') {
			if (result == 0.0f || spaces) {
				do {
					++s;
				} while (*s != '~');
			}
			else {
				break;
			}
		}
		else if (*s < 0x80) {
			result += GetCharacterSizeNormal(*s);
		}
		else {
			if (result == 0.0f || spaces) { result += GetCharacterSizeNormal(*s); }

			if (!spaces)
			{ break; }
		}

		++s;
	}

	return result;
}
wchar* CCFont::GetNextSpace_Chs(wchar* s)
{

	wchar* temp = s;
	while (*temp != ' ' && *temp != '\0') {
		if (*temp == '~') {
			if (temp == s) {
				do {
					++temp;
				} while (*temp != '~');

				++temp;
				s = temp;
				continue;
			}
			else {
				break;
			}
		}
		else if (*temp >= 0x80) {
			if (temp == s) { ++temp; }

			break;
		}

		++temp;
	}

	return temp;
}
//const short CCFont::iMaxCharWidth = 28;
//const float CCFont::fMaxCharWidth = CFont::iMaxCharWidth;
float CCFont::GetCharacterSize_Chs(wchar arg_char, uint16 nFontStyle, bool FontHalfTexture, bool bProp, float fScaleX)
{
	float charWidth;
	if (nFontStyle> MAX_FONTS)
	{
		return 0;
	}

	if (arg_char >= 0x80) {
		charWidth = iMaxCharWidth + 1;
	}
	else {

		arg_char -= 0x20;
		// std::wstring rr;
		//  std::map<int, Character> *d = f;

		// rr.push_back(arg_char);

		if (!FontHalfTexture && arg_char == 30) arg_char = 61;
		/**/
		if (FontHalfTexture) { arg_char = FindNewCharacter(arg_char); }

		if (bProp) {

			charWidth = Size[0][nFontStyle][arg_char];

		}
		else {
			charWidth = Size[0][nFontStyle][209];
		}
	}

	return (charWidth * fScaleX);
}

float CCFont::GetCharacterSizeNormal(wchar arg_char)
{
	return GetCharacterSize_Chs(arg_char, Details->style, Details->bFontHalfTexture, Details->proportional, Details->scaleX);
}

float CCFont::GetCharacterSizeDrawing(wchar arg_char)
{
	return GetCharacterSize_Chs(arg_char, RenderState.style, RenderState.bFontHalfTexture, RenderState.proportional, RenderState.scaleX);
}

float CCFont::GetCharacterWidth(wchar c)
{
	/*if (RenderState.style>2)
	{
		return 0;
	}*/
	uint8 tmp = *LanguageSet;
	float w = 0.0f;
#ifdef MORE_LANGUAGES
	if (IsChinese()) { *LanguageSet = 0; }

	if (IsJapanese()) {
		if (!RenderState.proportional) return Size[0][Details->style][192];
		if (c <= 94 || Details->style == FONT_HEADING || RenderState.style == FONT_BANK) {
			switch (RenderState.style) {
			case FONT_JAPANESE: return Size_jp[c];
			default: return Size[0][RenderState.style][c];
			}
		}

		switch (RenderState.style) {
		case FONT_JAPANESE: return 29.4f;
		case FONT_BANK: return 10.0f;
		default: return Size[0][RenderState.style][c];
		}
	}

	else if (RenderState.proportional) {
		w = Size[*LanguageSet][RenderState.style][c];
		*LanguageSet = tmp;
		return w;
	}
	else {
		w = Size[*LanguageSet][RenderState.style][209];
		*LanguageSet = tmp;
		return w;
	}

#else

	if (RenderState.proportional)
		return Size[RenderState.style][c];
	else
		return Size[RenderState.style][209];
#endif // MORE_LANGUAGES
}

float CCFont::GetCharacterSize(wchar c)
{
	uint8 tmp = *LanguageSet;
	float w = 0.0f;
#ifdef MORE_LANGUAGES

	if (IsChinese()) { *LanguageSet = 0; }

	if (IsJapanese()) {
		if (!Details->proportional) return Size[0][Details->style][209] * Details->scaleX;
		if (c <= 94 || Details->style == FONT_HEADING || Details->style == FONT_BANK) {
			switch (Details->style) {
			case FONT_JAPANESE: return Size_jp[c] * Details->scaleX;
			default: return Size[0][Details->style][c] * Details->scaleX;
			}
		}

		switch (Details->style) {
		case FONT_JAPANESE: return 29.4f * Details->scaleX;
		case FONT_BANK: return 10.0f * Details->scaleX;
		default: return Size[0][Details->style][c] * Details->scaleX;
		}
	}
	else {

		if (!Details->bFontHalfTexture && c == 30) c = 61; // wanted star
		if (Details->bFontHalfTexture) c = FindNewCharacter(c);
		if (Details->proportional) {
			w = Size[*LanguageSet][Details->style][c] * Details->scaleX;
			*LanguageSet = tmp;
			return w;
		}
		else {
			w = Size[*LanguageSet][Details->style][209] * Details->scaleX;
			*LanguageSet = tmp;
			return w;
		}
	}
#else

#ifdef FIX_BUGS
	// PS2 don't call FindNewCharacter in here at all, and also uses different chars for some symbols
	if (!Details->bFontHalfTexture && c == 30) c = 61; // wanted star
#endif
	if (Details->bFontHalfTexture) c = FindNewCharacter(c);
	if (Details->proportional)
		return Size[Details->style][c] * Details->scaleX;
	else
		return Size[Details->style][209] * Details->scaleX;
#endif // MORE_LANGUAGES
}

wchar CCFont::FindNewCharacter(wchar c)
{
	if (c >= 16 && c <= 26) return c + 128;
	if (c >= 8 && c <= 9) return c + 86;
	if (c == 4) return c + 89;
	if (c == 7) return 206;
	if (c == 14) return 207;
	if (c >= 33 && c <= 58) return c + 122;
	if (c >= 65 && c <= 90) return c + 90;
	if (c >= 96 && c <= 118) return c + 85;
	if (c >= 119 && c <= 140) return c + 62;
	if (c >= 141 && c <= 142) return 204;
	if (c == 143) return 205;
	if (c == 1) return 208;
	return c;
}

wchar* CCFont::ParseToken(wchar* s)
{
	Details->anonymous_23 = false;
	s++;
	if (Details->color.r || Details->color.g || Details->color.b) switch (*s) {
		//新加值
	case '+': current_font_set = FT_normal; break;
	case '-': current_font_set = FT_slant; break;
	case '#': current_font_set = FT_regular; break;

	case 'B': Details->bBold = !Details->bBold; break;
	case 'N':
	case 'n': NewLine = true; break;
	case 'b':
		Details->color.r = 27;
		Details->color.g = 89;
		Details->color.b = 130;
		Details->anonymous_23 = true;
		break;
	case 'f':
		Details->bFlash = !Details->bFlash;
		if (!Details->bFlash) Details->color.a = 255;
		break;
	case 'g':
		Details->color.r = 255;
		Details->color.g = 150;
		Details->color.b = 225;
		Details->anonymous_23 = true;
		break;
	case 'h':
		Details->color.r = 225;
		Details->color.g = 225;
		Details->color.b = 225;
		Details->anonymous_23 = true;
		break;
	case 'l':
		Details->color.r = 0;
		Details->color.g = 0;
		Details->color.b = 0;
		Details->anonymous_23 = true;
		break;
	case 'o':
		Details->color.r = 229;
		Details->color.g = 125;
		Details->color.b = 126;
		Details->anonymous_23 = true;
		break;
	case 'p':
		Details->color.r = 168;
		Details->color.g = 110;
		Details->color.b = 252;
		Details->anonymous_23 = true;
		break;
	case 'q':
		Details->color.r = 199;
		Details->color.g = 144;
		Details->color.b = 203;
		Details->anonymous_23 = true;
		break;
	case 'r':
		Details->color.r = 255;
		Details->color.g = 150;
		Details->color.b = 225;
		Details->anonymous_23 = true;
		break;
	case 't':
		Details->color.r = 86;
		Details->color.g = 212;
		Details->color.b = 146;
		Details->anonymous_23 = true;
		break;
	case 'w':
		Details->color.r = 175;
		Details->color.g = 175;
		Details->color.b = 175;
		Details->anonymous_23 = true;
		break;
	case 'x':
#ifdef FIX_BUGS
		Details->color.r = 0;
		Details->color.g = 255;
		Details->color.b = 255;
#else
		Details->color.r = 132;
		Details->color.g = 146;
		Details->color.b = 197;
#endif
		Details->anonymous_23 = true;
		break;
	case 'y':
		Details->color.r = 255;
		Details->color.g = 227;
		Details->color.b = 79;
		Details->anonymous_23 = true;
		break;
#ifdef BUTTON_ICONS
	case 'U': PS2Symbol = BUTTON_UP; break;
	case 'D': PS2Symbol = BUTTON_DOWN; break;
	case '<': PS2Symbol = BUTTON_LEFT; break;
	case '>': PS2Symbol = BUTTON_RIGHT; break;
	case 'X': PS2Symbol = BUTTON_CROSS; break;
	case 'O': PS2Symbol = BUTTON_CIRCLE; break;
	case 'Q': PS2Symbol = BUTTON_SQUARE; break;
	case 'T': PS2Symbol = BUTTON_TRIANGLE; break;
	case 'K': PS2Symbol = BUTTON_L1; break;
	case 'M': PS2Symbol = BUTTON_L2; break;
	case 'A': PS2Symbol = BUTTON_L3; break;
	case 'J': PS2Symbol = BUTTON_R1; break;
	case 'V': PS2Symbol = BUTTON_R2; break;
	case 'C': PS2Symbol = BUTTON_R3; break;
	case 'H': PS2Symbol = BUTTON_RSTICK_UP; break;
	case 'L': PS2Symbol = BUTTON_RSTICK_DOWN; break;
	case '(': PS2Symbol = BUTTON_RSTICK_LEFT; break;
	case ')': PS2Symbol = BUTTON_RSTICK_RIGHT; break;
#endif
	}
	while (*s != '~') s++;
	if (*(++s) == '~') s = ParseToken(s);
	return s;
}
wchar* CCFont::ParseToken(wchar* str, CRGBA& color, bool& flash, bool& bold)
{
	Details->anonymous_23 = false;
	wchar* s = str + 1;
	if (Details->color.r || Details->color.g || Details->color.b) {
		switch (*s) {
			//新加值
		case '+': current_font_set = FT_normal; break;
		case '-': current_font_set = FT_slant; break;
		case '#': current_font_set = FT_regular; break;

		case 'B': bold = !bold; break;
		case 'b':
			color.r = 27;
			color.g = 89;
			color.b = 130;
			break;
		case 'f': flash = !flash; break;
		case 'g':
			color.r = 255;
			color.g = 150;
			color.b = 225;
			break;
		case 'h':
			color.r = 225;
			color.g = 225;
			color.b = 225;
			break;
		case 'l':
			color.r = 0;
			color.g = 0;
			color.b = 0;
			break;
		case 'o':
			color.r = 229;
			color.g = 125;
			color.b = 126;
			break;
		case 'p':
			color.r = 168;
			color.g = 110;
			color.b = 252;
			break;
		case 'q':
			color.r = 199;
			color.g = 144;
			color.b = 203;
			break;
		case 'r':
			color.r = 255;
			color.g = 150;
			color.b = 225;
			break;
		case 't':
			color.r = 86;
			color.g = 212;
			color.b = 146;
			break;
		case 'w':
			color.r = 175;
			color.g = 175;
			color.b = 175;
			break;
#ifdef FIX_BUGS
		case 'x':
			color.r = 0;
			color.g = 255;
			color.b = 255;
			break;
#else
		case 'x':
			color.r = 132;
			color.g = 146;
			color.b = 197;
			break;
#endif
		case 'y':
			color.r = 255;
			color.g = 227;
			color.b = 79;
			break;
#ifdef BUTTON_ICONS
		case 'U': PS2Symbol = BUTTON_UP; break;
		case 'D': PS2Symbol = BUTTON_DOWN; break;
		case '<': PS2Symbol = BUTTON_LEFT; break;
		case '>': PS2Symbol = BUTTON_RIGHT; break;
		case 'X': PS2Symbol = BUTTON_CROSS; break;
		case 'O': PS2Symbol = BUTTON_CIRCLE; break;
		case 'Q': PS2Symbol = BUTTON_SQUARE; break;
		case 'T': PS2Symbol = BUTTON_TRIANGLE; break;
		case 'K': PS2Symbol = BUTTON_L1; break;
		case 'M': PS2Symbol = BUTTON_L2; break;
		case 'A': PS2Symbol = BUTTON_L3; break;
		case 'J': PS2Symbol = BUTTON_R1; break;
		case 'V': PS2Symbol = BUTTON_R2; break;
		case 'C': PS2Symbol = BUTTON_R3; break;
		case 'H': PS2Symbol = BUTTON_RSTICK_UP; break;
		case 'L': PS2Symbol = BUTTON_RSTICK_DOWN; break;
		case '(': PS2Symbol = BUTTON_RSTICK_LEFT; break;
		case ')': PS2Symbol = BUTTON_RSTICK_RIGHT; break;
#endif
		default: break;
		}
	}
	while (*s != '~') ++s;
	if (*(++s) == '~') s = ParseToken(s, color, flash, bold);
	return s;
}

void CCFont::SetScale(float x, float y)
{
#ifdef MORE_LANGUAGES
	/*if (IsJapanese()) {
			x *= 1.35f;
			y *= 1.25f;
	}*/
#endif
	Details->scaleX = x;
	Details->scaleY = y;
}

void CCFont::SetSlantRefPoint(float x, float y)
{
	Details->slantRefX = x;
	Details->slantRefY = y;
}

void CCFont::SetSlant(float s)
{
	Details->slant = s;
}

void CCFont::SetColor(CRGBA col)
{
	Details->color = col;
	if (Details->alphaFade < 255.0f) Details->color.a *= Details->alphaFade / 255.0f;
}

void CCFont::SetJustifyOn(void)
{
	Details->justify = true;
	Details->centre = false;
	Details->rightJustify = false;
}

void CCFont::SetJustifyOff(void)
{
	Details->justify = false;
	Details->rightJustify = false;
}

void CCFont::SetCentreOn(void)
{
	Details->centre = true;
	Details->justify = false;
	Details->rightJustify = false;
}

void CCFont::SetCentreOff(void)
{
	Details->centre = false;
}

void
CCFont::SetWrapx(float x)
{
	Details->wrapX = x;
}

void
CCFont::SetCentreSize(float s)
{
	Details->centreSize = s;
}

void
CCFont::SetBackgroundOn(void)
{
	Details->background = true;
}

void
CCFont::SetBackgroundOff(void)
{
	Details->background = false;
}

void
CCFont::SetBackgroundColor(CRGBA col)
{
	Details->backgroundColor = col;
}

void
CCFont::SetBackGroundOnlyTextOn(void)
{
	Details->backgroundOnlyText = true;
}

void
CCFont::SetBackGroundOnlyTextOff(void)
{
	Details->backgroundOnlyText = false;
}

void
CCFont::SetRightJustifyOn(void)
{
	Details->rightJustify = true;
	Details->justify = false;
	Details->centre = false;
}

void
CCFont::SetRightJustifyOff(void)
{
	Details->rightJustify = false;
	Details->justify = false;
	Details->centre = false;
}

void
CCFont::SetPropOff(void)
{
	Details->proportional = false;
}

void
CCFont::SetPropOn(void)
{
	Details->proportional = true;
}
void
CCFont::SetFontStyle(int16 style)
{
	if (style == FONT_HEADING) {
		Details->style = FONT_STANDARD;
		Details->bFontHalfTexture = true;
	}
	else {
		Details->style = style;
		Details->bFontHalfTexture = false;
	}
}

void
CCFont::SetRightJustifyWrap(float wrap)
{
	Details->rightJustifyWrap = wrap;
}

void
CCFont::SetAlphaFade(float fade)
{
	Details->alphaFade = fade;
}

void
CCFont::SetDropColor(CRGBA col)
{
	Details->dropColor = col;
	if (Details->alphaFade < 255.0f) Details->dropColor.a *= Details->alphaFade / 255.0f;
}

void
CCFont::SetDropShadowPosition(int16 pos)
{
	Details->dropShadowPosition = pos;
}