//=============================================================================
//
// CCutInクラス [CCutIn.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCutIn.h"
#include "CCutIn2D.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// カットインの大きさ
static const float CUT_IN_WIDTH = SCREEN_WIDTH;
static const float CUT_IN_HEIGHT = 300.f;
static const float CUT_IN_MOVE_SPEED = 0.1f;

// カットインごとのフェードアウトするスピード
static const float FADE_SPEED[CUT_IN_MAX] = {
	0.08f,
	0.08f
};

// カットインごとの表示する長さ
static const int CUT_IN_INTERVAL[CUT_IN_MAX] =
{
	30,
	30
};

// 仮
static const int PLAYER_0 = 0;
static const int PLAYER_1 = 1;

//=============================================================================
// コンストラクタ
//=============================================================================
CCutIn::CCutIn(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;

	m_pCutInPolygon = NULL;
	m_pCutInPolygonBack = NULL;
	m_isIn = false;
	m_isOut = false;
	m_isWait = false;
	m_Time = 0;
	m_IntervalMax = 0;
	m_IntervalCount = 0;
	m_Color = D3DXCOLOR(1, 1, 1, 1);
}

//=============================================================================
// デストラクタ
//=============================================================================
CCutIn::~CCutIn(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CCutIn::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	// 2D初期化
	m_pCutInPolygonBack = CCutIn2D::Create(m_pD3DDevice,
		D3DXVECTOR3(-CUT_IN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
		CUT_IN_WIDTH, CUT_IN_HEIGHT,
		TEXTURE_HP_GAGE_G);

	m_pCutInPolygon = CCutIn2D::Create(m_pD3DDevice,
		D3DXVECTOR3(-CUT_IN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
		CUT_IN_WIDTH, CUT_IN_HEIGHT,
		TEXTURE_CROWD_SPARK);
}

//=============================================================================
// 終了
//=============================================================================
void CCutIn::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CCutIn::Update(void)
{
	if (m_isIn)		In();
	if (m_isWait)	Wait();
	if (m_isOut)	Out();
}

//=============================================================================
// 描画
//=============================================================================
void CCutIn::DrawUI(void)
{
}

//=============================================================================
// 作成
//=============================================================================
CCutIn* CCutIn::Create(
	LPDIRECT3DDEVICE9 *pDevice)
{
	CCutIn* p = new CCutIn(pDevice);
	p->Init(pDevice);
	return p;
}

//=============================================================================
// カットインスタート
//=============================================================================
void CCutIn::Start(int ID, CutInType type)
{
	m_isIn = true;
	m_isOut = false;
	m_isWait = false;
	m_IntervalCount = 0;
	m_Time = 0;
	m_CutInType = type;

	// タイプごとに初期化
	if (type == CUT_IN_JIJII)
	{
		m_pCutInPolygon->ChangeTexture(TEXTURE_JIJII);
		m_pCutInPolygonBack->ChangeTexture(TEXTURE_HP_GAGE_G);
	}
	if (type == CUT_IN_SPARK)
	{
		m_pCutInPolygon->ChangeTexture(TEXTURE_CROWD_SPARK);
		m_pCutInPolygonBack->ChangeTexture(TEXTURE_HP_GAGE_R);
	}
	// カットインを表示する長さ
	m_IntervalMax = CUT_IN_INTERVAL[type];

	// プレイヤー番号ごとに座標を設定する
	if (ID == PLAYER_0)
	{
		m_Pos = -CUT_IN_WIDTH;
		m_PosDest = SCREEN_WIDTH * 0.5f;
	}
	else
	{
		m_Pos = CUT_IN_WIDTH;
		m_PosDest = SCREEN_WIDTH * 0.5f;
	}

	// フェードアウトの白さ初期化
	m_pCutInPolygon->InitWhite();
	m_pCutInPolygonBack->InitWhite();
	
}

//=============================================================================
// 画面内に入ってくる更新
//=============================================================================
void CCutIn::In()
{
	// 座標移動
	m_Time += CUT_IN_MOVE_SPEED;
	if (m_Time <= 1.0f)
	{
		float x = EasingInterpolation(m_Pos, m_PosDest, m_Time);
		m_pCutInPolygon->SetVertexPolygonX(x);
		m_pCutInPolygonBack->SetVertexPolygonX(x);
	}
	else
	{
		m_isIn = false;
		m_isWait = true;
	}
}

//=============================================================================
// カットインして表示している状態
//=============================================================================
void CCutIn::Wait()
{
	// フェードアウトが終わりフラグ
	bool isEnd = false;

	// カットインをアウトするまでのカウント
	m_IntervalCount++;
	if (m_IntervalCount > m_IntervalMax)
	{
		isEnd = m_pCutInPolygon->AddWhite(FADE_SPEED[m_CutInType]);
		m_pCutInPolygonBack->AddWhite(FADE_SPEED[m_CutInType]);
	}

	// フェードアウトが終わったら
	if (isEnd)
	{
		m_isWait = false;
		m_isOut = true;
		m_Time = 0;
	}
}

//=============================================================================
// 画面外に出ていく更新
//=============================================================================
void CCutIn::Out()
{
//	m_pCutInPolygon->SetColorPolygon()

	m_Time += CUT_IN_MOVE_SPEED;
	if (m_Time <= 1.0f)
	{
	}
	else
	{
		m_isOut = false;
		m_pCutInPolygon->SetVertexPolygonX(-CUT_IN_WIDTH);
		m_pCutInPolygonBack->SetVertexPolygonX(-CUT_IN_WIDTH);
	}

}

//----EOF----