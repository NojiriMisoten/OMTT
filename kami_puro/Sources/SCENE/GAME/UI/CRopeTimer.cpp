//=============================================================================
//
// CRopeTimerクラス [CRopeTimer.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CRopeTimer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const float BAR_WIDTH = 300;
static const float BAR_HEIGHT = 20;
static const float BAR_FRAME_WIDTH = 350;
static const float BAR_FRAME_HEIGHT = 35;
static const D3DXVECTOR3 BAR_POS = D3DXVECTOR3(SCREEN_WIDTH*0.5f, 360, 0);

static const float BAR_POS_LEFT = BAR_POS.x - BAR_WIDTH * 0.5f;
static const float BAR_POS_RIGHT = BAR_POS.x + BAR_WIDTH * 0.5f;

//=============================================================================
// コンストラクタ
//=============================================================================
CRopeTimer::CRopeTimer(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;
	m_pBar = NULL;
	m_pFrame = NULL;
	m_isAppearAnime = false;
	m_isMoveBar = false;
	m_PosLeft = 0;
	m_PosRight = 0;
	m_AnimeBarSize = AnimeData(0, 0);
	m_AnimeBarSizeDest = AnimeData(0, 0);
	m_AnimeFrameSize = AnimeData(0, 0);
	m_AnimeFrameSizeDest = AnimeData(0, 0);
	m_Time = 0;
	m_Count = 0;
	m_CountMax = 0;
	m_WidthOneFrame = 0;
	m_TimeOneFrame = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRopeTimer::~CRopeTimer(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CRopeTimer::Init()
{
	// バー
	m_pBar = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(BAR_POS),
		BAR_WIDTH, BAR_HEIGHT,
		TEXTURE_HP_GAGE_G);
	// 枠
	m_pFrame = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(BAR_POS),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_CROWD_GAGE_FRAME);
	// れいやー
	m_pBar->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrame->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// 最初は見えないよ
	m_pBar->SetVertexPolygon(D3DXVECTOR3(BAR_POS), 0, 0);
	m_pFrame->SetVertexPolygon(D3DXVECTOR3(BAR_POS), 0, 0);
}

//=============================================================================
// 終了
//=============================================================================
void CRopeTimer::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CRopeTimer::Update(void)
{
	// バーを出現させる更新
	if (m_isAppearAnime)
	{
		UpdateAppearAnime();
	}

	// バーを動かす更新
	if (m_isMoveBar)
	{
		UpdateBarMove();
	}
}

//=============================================================================
// バーを動かす更新
//=============================================================================
void CRopeTimer::UpdateBarMove(void)
{
	m_Count++;
	if (m_Count > m_CountMax)
	{
		// 終わったら一応まんなかにセット
		m_PosLeft = BAR_POS.x;
		m_PosRight = BAR_POS.x;
		m_pBar->SetVertexPolygonRight(m_PosRight);
		m_pBar->SetVertexPolygonLeft(m_PosLeft);
		// もう動かさない
		m_isMoveBar = false;
		// 残ったフレームを消すよ
		m_pFrame->SetVertexPolygon(D3DXVECTOR3(BAR_POS), 0, 0);
	}
	else
	{
		// 両側から真ん中にポリゴンを縮める処理
		m_PosLeft += m_WidthOneFrame;
		m_PosRight -= m_WidthOneFrame;
		m_pBar->SetVertexPolygonRight(m_PosRight);
		m_pBar->SetVertexPolygonLeft(m_PosLeft);
	}
}

//=============================================================================
// バーを出現するアニメーション更新
//=============================================================================
void CRopeTimer::UpdateAppearAnime(void)
{
	m_Time += m_TimeOneFrame;

	if (m_Time > 1.0f)
	{
		m_Time = 1.0f;
		// バーの拡縮
		float width = EasingInterpolation(m_AnimeBarSize.width, m_AnimeBarSizeDest.width, m_Time);
		float height = EasingInterpolation(m_AnimeBarSize.height, m_AnimeBarSizeDest.height, m_Time);
		m_pBar->SetVertexPolygon(D3DXVECTOR3(BAR_POS), width, height);
		// フレームの拡縮
		width = EasingInterpolation(m_AnimeFrameSize.width, m_AnimeFrameSizeDest.width, m_Time);
		height = EasingInterpolation(m_AnimeFrameSize.height, m_AnimeFrameSizeDest.height, m_Time);
		m_pFrame->SetVertexPolygon(D3DXVECTOR3(BAR_POS), width, height);
		// もう出現しない
		m_isAppearAnime = false;
		// バーを動かすよ
		m_isMoveBar = true;
	}
	else
	{
		// バーの拡縮
		float width = EasingInterpolation(m_AnimeBarSize.width, m_AnimeBarSizeDest.width, m_Time);
		float height = EasingInterpolation(m_AnimeBarSize.height, m_AnimeBarSizeDest.height, m_Time);
		m_pBar->SetVertexPolygon(D3DXVECTOR3(BAR_POS), width, height);
		// フレームの拡縮
		width = EasingInterpolation(m_AnimeFrameSize.width, m_AnimeFrameSizeDest.width, m_Time);
		height = EasingInterpolation(m_AnimeFrameSize.height, m_AnimeFrameSizeDest.height, m_Time);
		m_pFrame->SetVertexPolygon(D3DXVECTOR3(BAR_POS), width, height);
	}
}

//=============================================================================
// 描画
//=============================================================================
void CRopeTimer::DrawUI(void)
{
}

//=============================================================================
// 作成
//=============================================================================
CRopeTimer* CRopeTimer::Create(LPDIRECT3DDEVICE9 *pDevice)
{
	CRopeTimer* p = new CRopeTimer(pDevice);
	p->Init();
	return p;
}

//=============================================================================
// すたーと
//=============================================================================
void CRopeTimer::Start(int appearFrame, int reduceFrame)
{
	// １フレーム当たりに減らす幅 半分にしてるのは両方から減らすから
	m_WidthOneFrame = BAR_WIDTH / reduceFrame * 0.5f;
	// １フレーム当たりに増やすタイム
	m_TimeOneFrame = 1.0f / appearFrame;
	// 端っこの座標
	m_PosLeft = BAR_POS_LEFT;
	m_PosRight = BAR_POS_RIGHT;
	// カウント初期化
	m_CountMax = reduceFrame;
	m_Count = 0;
	// 出現アニメーション開始
	m_isAppearAnime = true;
	// サイズ指定
	m_AnimeBarSize = AnimeData(0, 0);
	m_AnimeBarSizeDest = AnimeData(BAR_WIDTH, BAR_HEIGHT);
	m_AnimeFrameSize = AnimeData(0, 0);
	m_AnimeFrameSizeDest = AnimeData(BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT);
	// タイム初期化
	m_Time = 0;
}

//----EOF----