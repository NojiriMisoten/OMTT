//=============================================================================
//
// CRendererクラス [CRenderer.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CRENDERER_H_
#define _CRENDERER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

class CScene2D;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRenderer
{
public:
	// レンダーターゲットの種類
	typedef enum
	{
		TYPE_RENDER_TOON_OBJECT_DEPTH = 0,	// トゥーンかけるオブジェクトの深度値かく
		TYPE_RENDER_NORMAL,				// 書くところ
		TYPE_RENDER_NORMAL_VEC,			// 法線書く
		TYPE_RENDER_SHADOW,				// 影書く
		TYPE_RENDER_BASE_HIGH_LUMINANCE,// 輝度抽出したいやつ書く
		TYPE_RENDER_UI,					// UI書く
		TYPE_RENDER_FADE,

		// 以下はリスト登録したオブジェクトは書かないこと
		TYPE_RENDER_TOON,			// トゥーン書く
		TYPE_RENDER_HALF_SIDE,		// 1/2サイズ
		TYPE_RENDER_HALF_GAUS,		// 1/2サイズガウス
		TYPE_RENDER_QUARTER_SIDE,	// 1/4サイズ
		TYPE_RENDER_QUARTER_GAUS,	// 1/4サイズガウス
		TYPE_RENDER_HIGH_LUMINANCE,	// 高輝度抽出
		TYPE_RENDER_LIGHT_BLOOM,	// ライトブルーム
		TYPE_RENDER_MAX
	}TYPE_RENDER_TEX;

	// コンストラクタ
	CRenderer();

	// デストラクタ
	~CRenderer();

	//================================================
	// 初期化
	// 引数: ウインドウハンドル、ウインドウモードで表示するか
	// 戻り値: 成否
	//================================================
	HRESULT Init(HWND hWnd, BOOL bWindow);

	// 終了
	void Uninit(void);

	// 更新
	void Update();

	//================================================
	// 指定プライオリティのみ更新
	// 引数: プライオリティ
	//================================================
	void Update(int priority);
	
	// 描画
	void Draw();

	//================================================
	// デバイスゲッター
	// 戻り値: デバイスのアドレス
	//================================================
	static LPDIRECT3DDEVICE9 *GetDevice(void){return &m_pD3DDevice;};

	//================================================
	// レンダーターゲット変更
	// 引数: レンダーターゲットの種類
	//================================================
	static void ChangeRenderTarget(TYPE_RENDER_TEX idx);

	//================================================
	// 元のレンダーターゲットに変更
	//================================================
	static void ChangeDefaultRenderTarget(void);

	//================================================
	// レンダーテクスチャーゲット
	// 引数: レンダーターゲットの種類
	// 戻り値: テクスチャのポインタ
	//================================================
	static LPDIRECT3DTEXTURE9* GetRenderTexture(TYPE_RENDER_TEX idx);

	//================================================
	// レンダーターゲットのクリア
	// 引数: レンダーターゲットの種類
	//================================================
	static void ClearRenderTarget(TYPE_RENDER_TEX idx);

	static void TeachCameraRot(float rot){ m_fCameraRot = rot; };
	static void TeachProjMtx(D3DXMATRIX& proj){ m_ProjMtx = proj; };
	static void TeachViewMtx(D3DXMATRIX& view){ m_ViewMtx = view; };
	#ifdef _DEBUG
	//================================================
	// FPSセット
	// 引数: FPS
	//================================================
	void		SetFPS(int FPS){m_nCountFPS = FPS;};
	#endif
private:
	// バックバッファーに書く
	void DrawBackBuffer(void);

	// トゥーンにする
	void DrawToon(void);

	// ライトテクスチャ合成する
	void DrawMixLight(void);

	// ガウス書く
	void DrawGauss(TYPE_RENDER_TEX orijinTex, TYPE_RENDER_TEX sideRender, TYPE_RENDER_TEX gausRender);

	// 高輝度抽出
	void DrawBrightness(void);

	// ライトブルーム
	void DrawLightBloom(void);

	LPDIRECT3D9					m_pD3D;									// Direct3Dオブジェクト
	static LPDIRECT3DDEVICE9	m_pD3DDevice;							// デバイスオブジェクト(描画に必要)
	static LPDIRECT3DTEXTURE9	m_pRenderTexture[TYPE_RENDER_MAX];		// レンダーターゲット用のテクスチャ
	static LPDIRECT3DSURFACE9	m_pRenderSurface[TYPE_RENDER_MAX];		// レンダーターゲット用のサーフェス
	static LPDIRECT3DSURFACE9	m_pRenderSurfaceDepth[TYPE_RENDER_MAX];	// レンダーターゲット用のサーフェス
	static LPDIRECT3DSURFACE9	m_pDefaultSurface;						// 元のサーフェス
	static LPDIRECT3DSURFACE9	m_pDefaultSurfaceDepth;					// 元のサーフェス
	static LPDIRECT3DSURFACE9	m_pKeepSurface;							// 入れ替え用のサーフェス
	static LPDIRECT3DSURFACE9	m_pKeepSurfaceDepth;					// 入れ替え用のサーフェス
	static LPDIRECT3DTEXTURE9	m_pKeepTexture;							// テクスチャの入れえよう
	static D3DVIEWPORT9			m_DefaultViewPort;
	static D3DVIEWPORT9			m_ChangeViewPort[TYPE_RENDER_MAX];
	static LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	static VF					m_Vertex[];
	static IDirect3DVertexBuffer9 *m_pVertexBuff;
	static bool					m_bBeginScene;
	static float				m_fCameraRot;
	static D3DXMATRIX			m_ProjMtx;
	static D3DXMATRIX			m_ViewMtx;

	#ifdef _DEBUG
	int							m_nCountFPS;			// FPSカウンタ
	#endif
};

#endif
//----EOF----