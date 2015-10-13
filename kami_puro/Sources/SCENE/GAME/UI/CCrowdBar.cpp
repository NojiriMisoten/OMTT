//=============================================================================
//
// CStaminaBarBarクラス [CStaminaBarBar.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCrowdBar.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// バーの高さ
const float BAR_WIDTH = 600;
const float BAR_HEIGHT = 40;
// バーのテクスチャ
const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// TODO 仮の量
const float CROWD_MAX = 255;
// 観客量にこの値を掛けてバーの座標を出す（適当　てか計算で出せるはず）
const float RESIST = 1.3f;
// バーの色
const D3DXCOLOR BAR_COLOR = D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
CCrowdBar::CCrowdBar(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	// レンダラーセット
	CScene::AddLinkList(CRenderer::TYPE_RENDER_UI);
}

//=============================================================================
// デストラクタ
//=============================================================================
CCrowdBar::~CCrowdBar(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CCrowdBar::Init(D3DXVECTOR2 pos)
{
	// 変数代入
	m_Value = 0;
	m_ValueMax = CROWD_MAX;
	m_Pos = pos;

	// 2D初期化
	CScene2D::Init(
		D3DXVECTOR3(pos.x,pos.y, 0),
		0, BAR_HEIGHT, BAR_TEXTURE);

	SetColorPolygon(BAR_COLOR);
}

//=============================================================================
// 終了
//=============================================================================
void CCrowdBar::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CCrowdBar::Update(void)
{
	float a = m_Value * RESIST + m_Pos.x;
	CDebugProc::Print("観客値　　%f\n", m_Value);
	CDebugProc::Print("観客座標　%f\n", a);
	
}

//=============================================================================
// 描画
//=============================================================================
void CCrowdBar::DrawUI(void)
{
	CScene2D::DrawUI();
}

//=============================================================================
// 作成
//=============================================================================
CCrowdBar* CCrowdBar::Create(
	D3DXVECTOR2 pos,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CCrowdBar* p = new CCrowdBar(pDevice);
	p->Init(pos);
	return p;
}

//=============================================================================
// 値変動
// ここ、処理を軽くするにはフラグ管理で頂点変更する回数を減らせる
//=============================================================================
void CCrowdBar::Add(float value)
{
	m_Value += value;

	// クランプ
	m_Value = min(m_Value, m_ValueMax);
	m_Value = max(m_Value, -m_ValueMax);

	// 右っかわ
	if (m_Value > 0){
		CScene2D::SetVertexPolygonLeft(m_Pos.x);
		CScene2D::SetVertexPolygonRight(m_Value * RESIST + m_Pos.x);
	}
	// ぎゃう
	else if (m_Value < 0){
		CScene2D::SetVertexPolygonRight(m_Pos.x);
		CScene2D::SetVertexPolygonLeft(m_Value * RESIST + m_Pos.x);
	}
}

//----EOF----