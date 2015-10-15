//=============================================================================
//
// CHpBarクラス [CHpBar.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CHpBar.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// バーの高さ
const float BAR_WIDTH = 264;
const float BAR_HEIGHT = 40;
// バーのテクスチャ
const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// バーの赤くなる部分の色
const D3DXCOLOR BAR_HP_COLOR = D3DXCOLOR(0.0f, 0.8f, 0.2f, 1.0f);
const D3DXCOLOR BAR_LESS_COLOR = D3DXCOLOR(1, 0.2f, 0.1f, 1.0f);
// 赤いバーが追っかけるスピード
const float BAR_RED_SPEED_RESIST = 0.01f;
// 仮の自動スタミナ回復量
const float AUTO_HEAL_AMOUNT = 0.4f;

// TODO 仮のHP量　プレイヤから持ってくる
const float HP_MAX = 255;

// 赤いバーを変更するカウント数
const float RED_CHANGE_INTERVAL = 40;

//=============================================================================
// コンストラクタ
//=============================================================================
CHpBar::CHpBar(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	// レンダラーセット
	CScene::AddLinkList(CRenderer::TYPE_RENDER_UI);

}

//=============================================================================
// デストラクタ
//=============================================================================
CHpBar::~CHpBar(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CHpBar::Init(
	D3DXVECTOR2 pos,
	float width,
	float height,
	float centerDist,
	POSITIONBASE positionBase)
{
	// 変数代入
	m_Pos = pos;
	m_PositionBase = positionBase;
	m_Value = HP_MAX;
	m_ValueMax = HP_MAX;
	m_ValueRed = HP_MAX;
	m_RedResetCount = 0;
	m_isRedReset = false;

	// 値（m_Value）１当たりのピクセル数(float)を計算
	m_WidthOneValue = width / m_ValueMax;

	float haji = 0, center = 0;
	if (m_PositionBase == POSITIONBASE_LEFT){
		haji = pos.x + centerDist;
		center = haji + width * 0.5f;
		// 固定した座標　バーの頂点を変更するときに使用
		m_PosBase = D3DXVECTOR2(haji, pos.y);
	}
	else{
		haji = pos.x - centerDist;
		center = haji - width * 0.5f;
		// 固定した座標　バーの頂点を変更するときに使用
		m_PosBase = D3DXVECTOR2(haji, pos.y);
	}

	// バーの中心座標
	// 2D初期化
	CScene2D::Init(D3DXVECTOR3(center, pos.y, 0), m_WidthOneValue * m_Value, height, BAR_TEXTURE);
	// バーの色を変更
	SetColorPolygon(BAR_LESS_COLOR);

	// みどりバー
	m_HpBar = CScene2D::Create(
		m_pD3DDevice,
		D3DXVECTOR3(center, pos.y, 0),
		m_WidthOneValue * m_Value, height, BAR_TEXTURE);

	m_HpBar->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_HpBar->SetColorPolygon(BAR_HP_COLOR);
}

//=============================================================================
// 終了
//=============================================================================
void CHpBar::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CHpBar::Update(void)
{
	CDebugProc::Print("体力 %d / %d\n", (int)m_Value, (int)m_ValueRed);
	
	// 赤いバーを緑に合わせないならリターン
	if (!m_isRedReset)
		return;
	// 赤いバーを緑に合わせる
	m_RedResetCount++;
	if (m_RedResetCount > RED_CHANGE_INTERVAL){
		m_RedResetCount = 0;
		m_ValueRed = m_Value;
		m_isRedReset = false;

		if (m_PositionBase == POSITIONBASE_RIGHT)
		{
			SetVertexPolygonLeft(m_PosBase.x - m_Value * m_WidthOneValue);
		}
		else
		{
			SetVertexPolygonRight(m_PosBase.x + m_Value * m_WidthOneValue);
		}
	}

}

//=============================================================================
// 描画
//=============================================================================
void CHpBar::DrawUI(void)
{
	CScene2D::DrawUI();
}

//=============================================================================
// 作成
//=============================================================================
CHpBar* CHpBar::Create(
	D3DXVECTOR2 pos,
	float width,
	float height,
	float centerDist,
	POSITIONBASE positionBase,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CHpBar* p = new CHpBar(pDevice);
	p->Init(pos, width, height, centerDist, positionBase);
	return p;
}

//=============================================================================
// HP増減
//=============================================================================
void CHpBar::Add(float value)
{
	m_Value += value;

	// クランプ
	m_Value = min(m_Value, m_ValueMax);
	m_Value = max(m_Value, 0);

	// 赤バー変更するフラグ
	m_isRedReset = true;
	m_RedResetCount = 0;

	// みどりバー頂点変更
	if (m_PositionBase == POSITIONBASE_RIGHT)
	{
		m_HpBar->SetVertexPolygonLeft(m_PosBase.x - m_Value * m_WidthOneValue);
	}
	else
	{
		m_HpBar->SetVertexPolygonRight(m_PosBase.x + m_Value * m_WidthOneValue);
	}

	// もしHPが増えた場合には赤いバーをみどりと同じ幅にする
	if (value > 0){
		m_RedResetCount = 0;
		m_ValueRed = m_Value;
		m_isRedReset = false;

		if (m_PositionBase == POSITIONBASE_RIGHT)
		{
			SetVertexPolygonLeft(m_PosBase.x - m_Value * m_WidthOneValue);
		}
		else
		{
			SetVertexPolygonRight(m_PosBase.x + m_Value * m_WidthOneValue);
		}
	}

}

//=============================================================================
// HPバーの値を０にしてバーを消す
//=============================================================================
void CHpBar::Reset()
{
	Add(-m_ValueMax);

	if (m_PositionBase == POSITIONBASE_RIGHT)
	{
		SetVertexPolygonLeft(m_PosBase.x - m_Value * m_WidthOneValue);
	}
	else
	{
		SetVertexPolygonRight(m_PosBase.x + m_Value * m_WidthOneValue);
	}
}
//----EOF----