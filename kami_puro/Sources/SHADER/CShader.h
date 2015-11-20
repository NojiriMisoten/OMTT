//=============================================================================
//
// シェーダー処理 [CShader.h]
// Author : NOJIRI NAOKI
//
//=============================================================================
#ifndef _CSHADER_H_
#define _CSHADER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

// シェーダーの種類
typedef enum
{
	VS_TYPE_DRAW_2D = 0,
	VS_TYPE_NORMAL_RENDERER,
	VS_TYPE_NORMAL_VEC_RENDERER,
	VS_TYPE_TOON_OBJECT_DEPTH,
	VS_TYPE_NORMAL_RENDERER_SKINMESH,
	VS_TYPE_NORMAL_VEC_RENDERER_SKINMESH,
	VS_TYPE_TOON_OBJECT_DEPTH_SKINMESH,
	VS_TYPE_FIELD,
	VS_TYPE_MODEL,
	VS_TYPE_POINT,
	VS_TYPE_TEX,
	VS_TYPE_MAX
}SHADER_TYPE;

typedef enum
{
	PS_TYPE_BACK_BUFFER = 0,
	PS_TYPE_NORMAL_RENDERER,
	PS_TYPE_NORMAL_VEC_RENDERER,
	PS_TYPE_TOON,
	PS_TYPE_TOON_OBJECT_DEPTH,
	PS_TYPE_FIELD,
	PS_TYPE_GAUSS_SIDE,
	PS_TYPE_GAUSS_VERTICAL,
	PS_TYPE_HIGH_LUMINANCE,
	PS_TYPE_LIGHT_BLOOM,
	PS_TYPE_MODEL,
	PS_TYPE_POINT,
	PS_TYPE_TEX,
	PS_TYPE_FADE_WHITE,
	PS_TYPE_MAX
}PS_TYPE;

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CShader
{
public:
	// コンストラクタ
	CShader(void){};

	// デストラクタ
	~CShader(void){};

	// クリエイト
	static void Create(LPDIRECT3DDEVICE9 *pDevice);

	// 終了
	static void Uninit(void);
	
	static LPDIRECT3DPIXELSHADER9* GetPS(PS_TYPE type){ return &m_ps[type]; };
	static LPD3DXCONSTANTTABLE* GetPSC(PS_TYPE type){ return &m_psc[type]; };

	static LPDIRECT3DVERTEXSHADER9* GetVS(SHADER_TYPE type){ return &m_vs[type]; };
	static LPD3DXCONSTANTTABLE* GetVSC(SHADER_TYPE type){ return &m_vsc[type]; };

private:
	//===================================================
	// 初期化
	// 引数: デバイス
	// 戻り値: 成否
	//===================================================
	static HRESULT Init(LPDIRECT3DDEVICE9 *pDevice);

	// -----------シェーダー用------------------
	static LPDIRECT3DPIXELSHADER9	m_ps[PS_TYPE_MAX];
	static LPD3DXCONSTANTTABLE		m_psc[PS_TYPE_MAX];		// 定数レジスタのテーブル
	static LPDIRECT3DVERTEXSHADER9	m_vs[VS_TYPE_MAX];
	static LPD3DXCONSTANTTABLE		m_vsc[VS_TYPE_MAX];		// 定数レジスタのテーブル
	//------------------------------------------
};

#endif
//----EOF----