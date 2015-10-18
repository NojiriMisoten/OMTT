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
static const float BAR_WIDTH = 600;
static const float BAR_HEIGHT = 30;
// バーのテクスチャ
static const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// TODO 仮の量
static const float CROWD_MAX = 255;
// 観客量にこの値を掛けてバーの座標を出す（適当　てか計算で出せるはず）
static const float RESIST = 1.2f;
// バーの色
static const D3DXCOLOR BAR_COLOR_LEFT = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
static const D3DXCOLOR BAR_COLOR_RIGHT = D3DXCOLOR(0.0f, 0.1f, 1.0f, 1.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
CCrowdBar::CCrowdBar(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	m_pBarLeft = NULL;
	m_pBarRight = NULL;
	m_Pos = D3DXVECTOR2(0, 0);
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
void CCrowdBar::Init(D3DXVECTOR2 &pos, float height)
{
	// 変数代入
	m_Value = 0;
	m_ValueMax = CROWD_MAX;
	mValueBase = 0;
	m_Pos = pos;

	// 左右中央の座標
	float left = -m_ValueMax * RESIST + pos.x;
	float right = m_ValueMax * RESIST + pos.x;
	float width = right - pos.x;
	float left_center = left + width * 0.5f;
	float right_center = right - width * 0.5f;

	// 2D初期化
	m_pBarLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(left_center, pos.y, 0),
		width, height,
		BAR_TEXTURE);
	m_pBarRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(right_center, pos.y, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBarRight->AddLinkList(CRenderer::TYPE_RENDER_UI);

	m_pBarLeft->SetColorPolygon(BAR_COLOR_LEFT);
	m_pBarRight->SetColorPolygon(BAR_COLOR_RIGHT);
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
	CDebugProc::Print("観客値　　%f\n", m_Value);
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
	D3DXVECTOR2 &pos,
	float height,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CCrowdBar* p = new CCrowdBar(pDevice);
	p->Init(pos, height);
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

	m_pBarLeft->SetVertexPolygonRight(m_Value * RESIST + m_Pos.x);
	m_pBarRight->SetVertexPolygonLeft(m_Value * RESIST + m_Pos.x);
}

//=============================================================================
// バーの頂点を真ん中に集める
//=============================================================================
void CCrowdBar::Reset()
{
	m_Value = 0;
	mValueBase = 0;

	m_pBarLeft->SetVertexPolygonLeft(m_Value * RESIST + m_Pos.x);
	m_pBarRight->SetVertexPolygonRight(m_Value * RESIST + m_Pos.x);
}

//=============================================================================
// バーの頂点を端っこに戻す
//=============================================================================
void CCrowdBar::Replace(float value)
{
	mValueBase += value;

	// クランプ
	mValueBase = min(m_Value, m_ValueMax);
	mValueBase = max(m_Value, -m_ValueMax);

	m_pBarLeft->SetVertexPolygonLeft(-mValueBase * RESIST + m_Pos.x);
	m_pBarRight->SetVertexPolygonRight(mValueBase * RESIST + m_Pos.x);
}

//----EOF----