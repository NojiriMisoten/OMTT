//=============================================================================
//
// CBattleFade2Dクラス [CBattleFade2D.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CBattleFade2D.h"
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// 定数
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CBattleFade2D::CBattleFade2D(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice)
{
	m_pD3DDevice = pDevice;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBattleFade2D::~CBattleFade2D(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CBattleFade2D::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	CScene2D::Init(pos, width, height, texType);
}

//=============================================================================
// 終了
//=============================================================================
void CBattleFade2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CBattleFade2D::Update(void)
{

}

//=============================================================================
// 描画
//=============================================================================
void CBattleFade2D::DrawUIBattleFadeRender(void)
{
	// 加算合成
	(*m_pD3DDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);							// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	// ポリゴンの描画
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// プリミティブの種類
		, 0											// 描画を開始する頂点番号
		, DRAW_SQUARE_PRINITIV_NUM);				// 書きたいポリゴン数

	(*m_pD3DDevice)->SetTexture(0, NULL);

	// 加算合成を元に戻す
	(*m_pD3DDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 作成
//=============================================================================
CBattleFade2D* CBattleFade2D::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	CBattleFade2D* p = new CBattleFade2D(pDevice);
	p->Init(pos, width, height, texType);
	p->AddLinkList(CRenderer::TYPE_RENDER_UI_BATTLE_FADE);

	return p;
}

//----EOF----