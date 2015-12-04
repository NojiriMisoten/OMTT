//=============================================================================
//
// CCutIn2D2Dクラス [CCutIn2D2D.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCutIn2D.h"
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// 定数
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CCutIn2D::CCutIn2D(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice)
{
	m_pD3DDevice = pDevice;
	m_WhiteColor = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCutIn2D::~CCutIn2D(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CCutIn2D::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	CScene2D::Init(pos, width, height, texType);
}

//=============================================================================
// 終了
//=============================================================================
void CCutIn2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CCutIn2D::Update(void)
{

}

//=============================================================================
// 描画
//=============================================================================
void CCutIn2D::DrawUIRender(void)
{
	// ピクセルシェーダー
	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_FADE_WHITE);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_FADE_WHITE);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	// テクスチャのインデックス取得
	UINT texSampler = (*_psc)->GetSamplerIndex("texSampler");

	// 白さ加減
	(*_psc)->SetFloat(*m_pD3DDevice, "whiteColor", m_WhiteColor);

	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(texSampler, m_pD3DTexBuff);							// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	// ポリゴンの描画
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// プリミティブの種類
		, 0											// 描画を開始する頂点番号
		, DRAW_SQUARE_PRINITIV_NUM);				// 書きたいポリゴン数

	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}

//=============================================================================
// 作成
//=============================================================================
CCutIn2D* CCutIn2D::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	CCutIn2D* p = new CCutIn2D(pDevice);
	p->Init(pos, width, height, texType);
	p->AddLinkList(CRenderer::TYPE_RENDER_UI);

	return p;
}

//=============================================================================
// 白さ加減の調整
// １になったらtrueを返す
//=============================================================================
bool CCutIn2D::AddWhite(float f)
{
	m_WhiteColor += f;

	if (m_WhiteColor > 1.0f)
	{
		return true;
	}
	return false;
}
//----EOF----