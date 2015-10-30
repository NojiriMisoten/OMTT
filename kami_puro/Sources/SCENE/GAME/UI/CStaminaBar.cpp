//=============================================================================
//
// CStaminaBarBarクラス [CStaminaBarBar.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CStaminaBar.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// バーの高さ
static const float BAR_HEIGHT = 40;
// バーのテクスチャ
static const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// 仮の自動スタミナ回復量
static const float AUTO_HEAL_AMOUNT = 0.4f;

// TODO 仮のスタミナ量　プレイヤから持ってくる
static const float STAMINA_MAX = 255;

//=============================================================================
// コンストラクタ
//=============================================================================
CStaminaBar::CStaminaBar(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	// レンダラーセット
	CScene::AddLinkList(CRenderer::TYPE_RENDER_UI);

	m_PosLeft = D3DXVECTOR2(0, 0);
	m_PosRight = D3DXVECTOR2(0, 0);
	m_PositionBase = POSITIONBASE_RIGHT;
	m_Value = 0;
	m_ValueMax = 0;
	m_WidthOneValue = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CStaminaBar::~CStaminaBar(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CStaminaBar::Init(
	D3DXVECTOR2 &leftPos,
	D3DXVECTOR2 &rightPos,
	POSITIONBASE positionBase)
{
	// 変数代入
	m_PosLeft = leftPos;
	m_PosRight = rightPos;
	m_PositionBase = positionBase;
	m_Value = STAMINA_MAX;
	m_ValueMax = STAMINA_MAX;

	// バーの幅
	float barWidth = rightPos.x - leftPos.x;
	// バーの中心座標
	D3DXVECTOR3 centerPos = D3DXVECTOR3(leftPos.x + barWidth * 0.5f, leftPos.y, 0);

	// 2D初期化
	CScene2D::Init(centerPos, barWidth, BAR_HEIGHT, BAR_TEXTURE);
	// スタミナの値（m_Value）１当たりのピクセル数(float)を計算
	m_WidthOneValue = barWidth / m_ValueMax;

}

//=============================================================================
// 終了
//=============================================================================
void CStaminaBar::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CStaminaBar::Update(void)
{
	// 自動回復
	AutoHeal(AUTO_HEAL_AMOUNT);
}

//=============================================================================
// 描画
//=============================================================================
void CStaminaBar::DrawUI(void)
{
	// 使わない
	return;

	CScene2D::DrawUI();
}

//=============================================================================
// 作成
//=============================================================================
CStaminaBar* CStaminaBar::Create(
	D3DXVECTOR2 &leftPos,
	D3DXVECTOR2 &rightPos,
	POSITIONBASE positionBase,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CStaminaBar* p = new CStaminaBar(pDevice);
	p->Init(leftPos, rightPos, positionBase);
	return p;
}

//=============================================================================
// 値変動
//=============================================================================
bool CStaminaBar::UseStamina(float value)
{
	// スタミナが足りないときはfalse返す
	if (m_Value - value < 0){
		return false;
	}

	// スタミナ消費
	m_Value -= value;

	if (m_PositionBase == CStaminaBar::POSITIONBASE_RIGHT){
		float x = m_PosLeft.x + m_WidthOneValue * value;
		m_PosLeft.x = x;
		SetVertexPolygonLeft(x);
	}
	else if (m_PositionBase == CStaminaBar::POSITIONBASE_LEFT){
		float x = m_PosRight.x - m_WidthOneValue * value;
		m_PosRight.x = x;
		SetVertexPolygonRight(x);
	}
	return true;
}


//=============================================================================
// スタミナの自動回復
//=============================================================================
void CStaminaBar::AutoHeal(float value)
{
	float n = m_Value;
	n += value;
	n = min(n, m_ValueMax);
	float diff = n - m_Value;
	m_Value += diff;

//	CDebugProc::PrintL("スタミナ:%f\n", m_Value);

	// 回復していなければ終了
	if (diff == 0){
		return;
	}

	if (m_PositionBase == POSITIONBASE_RIGHT){
		float x = m_PosLeft.x - m_WidthOneValue * diff;
		m_PosLeft.x = x;
		SetVertexPolygonLeft(x);
	}
	else if (m_PositionBase == POSITIONBASE_LEFT){
		float x = m_PosRight.x + m_WidthOneValue * diff;
		m_PosRight.x = x;
		SetVertexPolygonRight(x);
	}
}
//----EOF----