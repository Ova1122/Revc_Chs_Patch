#pragma once
//#include <common.h>

struct RwIm2DVertex
{
	float x, y, z;
	//float32 q;	// recipz no longer used because we have a vertex stage now
	float w;
	uint32 color;
	float u, v;

	
};
typedef void* RwTexture;

class CCSprite2d
{
	static float RecipNearClip;
	static float NearScreenZ;
	static float NearCamZ;	// not original
	static int nextBufferVertex;
	static int nextBufferIndex;
	static RwIm2DVertex maVertices[8];
public:
	RwTexture *m_pTexture;

	//RW
	typedef int32(F_RwRenderState)(RwRenderState state, void* value);// RwBool RwRenderStateSet(RwRenderState state, void* value)
	static F_RwRenderState* RwRenderStateSet;
	static F_RwRenderState* RwRenderStateGet;

	static int32 RwRwRenderStateSet(RwRenderState state, void* value);
	static int32 RwRwRenderStateGet(RwRenderState state, void* value);
	//typedef  void(F_DRAW_RECT)(const CRect& r, const CRGBA& col);
	static void InitPatch(DWORD exe, const char* ExeName);
	static void DrawRect(const CRect& r, const CRGBA& col);
	static void Draw(const CRect& rect, const CRGBA& col);

	static void AddToBuffer(const CRect& a1, const CRGBA& a2, float u0, float v0, float u1, float v1, float u3, float v3, float u2, float v2);
	static void RenderVertexBuffer();

	void SetRenderState(void);
	void SetTexture(const char* name);
	void SetTexture(const char* name, const char* mask);
	void Delete(void);
	/*

	static void SetRecipNearClip(void);
	static void InitPerFrame(void);

	CCSprite2d(void) : m_pTexture(nil) {};
	~CCSprite2d(void) { Delete(); };
	void Delete(void);
	void SetRenderState(void);
	void SetTexture(RwTexture *m_pTex);
	void SetTexture(const char *name);
	void SetTexture(const char *name, const char *mask);
	//void SetAddressing(RwTextureAddressMode addr);
	void Draw(float x, float y, float w, float h, const CRGBA &col);
	void Draw(const CRect &rect, const CRGBA &col);
	void Draw(const CRect &rect, const CRGBA &col,
		float u0, float v0, float u1, float v1, float u3, float v3, float u2, float v2);
	void Draw(const CRect &rect, const CRGBA &c0, const CRGBA &c1, const CRGBA &c2, const CRGBA &c3);
	void Draw(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA &col);

	static void SetVertices(const CRect &r, const CRGBA &c0, const CRGBA &c1, const CRGBA &c2, const CRGBA &c3);
	static void SetVertices(const CRect &r, const CRGBA &c0, const CRGBA &c1, const CRGBA &c2, const CRGBA &c3,
		float u0, float v0, float u1, float v1, float u3, float v3, float u2, float v2);
	static void SetVertices(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
		const CRGBA &c0, const CRGBA &c1, const CRGBA &c2, const CRGBA &c3);
	static void SetVertices(int n, float *positions, float *uvs, const CRGBA &col);
	static void SetMaskVertices(int n, float *positions);
	static void SetVertices(RwIm2DVertex *verts, const CRect &r, const CRGBA &c0, const CRGBA &c1, const CRGBA &c2, const CRGBA &c3,
		float u0, float v0, float u1, float v1, float u3, float v3, float u2, float v2);


	static void DrawLine(const float x, const float y, const float x1, const float y1, const CRGBA &col);
	static void DrawLine(const CVector2D v1, const CVector2D v2, const CRGBA &col);
	static void DrawRect(const CRect &r, const CRGBA &c0, const CRGBA &c1, const CRGBA &c2, const CRGBA &c3);
	static void DrawRect(const CRect &r, const CRGBA &col);
	static void DrawRectNoFill(const CRect &r, const CRGBA &col);
	static void DrawRectXLU(const CRect &r, const CRGBA &c0, const CRGBA &c1, const CRGBA &c2, const CRGBA &c3);
	static void DrawAnyRect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
		const CRGBA &c0, const CRGBA &c1, const CRGBA &c2, const CRGBA &c3);

	static void Draw2DPolygon(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA &color);

	static RwIm2DVertex* GetVertices() { return maVertices; };

	static bool IsVertexBufferFull();
	static void AddToBuffer(const CRect &a1, const CRGBA &a2, float u0, float v0, float u1, float v1, float u3, float v3, float u2, float v2);
	static void RenderVertexBuffer();
	*/
};
