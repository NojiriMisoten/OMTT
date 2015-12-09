//=============================================================================
//
// CStaminaBarBarクラス [CStaminaBarBar.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCountTime.h"
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../../../MANAGER/CManager.h"
#include "../CGame.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// 二つの数字の幅（中心座標からのオフセット）
static const float FIGURE_SPACE_WIDTH = 30;
// 数字の大きさ
static const float FIGURE_WIDTH = 30;
static const float FIGURE_HEIGHT = 50;
// タイムの最大数
static const int TIME_MAX = 99;
// 一秒のカウント
static const short SECOND_FRAME = 60;
// 背景の大きさ
static const float FIGURE_BACK_WIDTH = 70;
static const float FIGURE_BACK_HEIGHT = 60;
// 止めているときにグレーにする
static const D3DXCOLOR GRAY_COLOR = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.5f);
// 拡縮のタイマの1フレーム当たりの量 (1 / x の場合、xフレームでアニメーションが完成する)
static const float GRAY_SCALE_SPEED = 1.0f / 20.0f;

//=============================================================================
// コンストラクタ
//=============================================================================
CCountTime::CCountTime(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame* pGame)
{
	m_pManager = pManager;
	m_pGame = pGame;
	m_pD3DDevice = pDevice;
	m_pFigure1st = NULL;
	m_pFigure2nd = NULL;
	m_pBack = NULL;
	m_pGray = NULL;
	m_Time = 0;
	m_TimeCount = 0;
	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeOneFrameAlpha = 0;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);	// 最初のアニメーションでαを透明にするため
	m_GrayCount = 0;
	m_GrayCountMax = 0;
	m_isGray = false;
	m_GrayWidth = 0;
	m_GrayWidthDest = 0;
	m_GrayHeight = 0;
	m_GrayHeightDest = 0;
	m_GrayPos = D3DXVECTOR3(0, 0, 0);
	m_GrayTime = 0;
	m_isScale = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCountTime::~CCountTime(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CCountTime::Init(D3DXVECTOR2 &pos, int time)
{
	m_Time = time;

	m_Time = max(m_Time, TIME_MAX);

	// カウント初期化
	m_TimeCount = 0;

	m_GrayPos.x = pos.x;
	m_GrayPos.y = pos.y;

	// 背景
	m_pBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x, pos.y, 0),
		FIGURE_BACK_WIDTH, FIGURE_BACK_HEIGHT,
		TEXTURE_UI_TIME_BACK);

	// 二つの数字を生成
	m_pFigure1st = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x - FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		TEXTURE_NUMBER);
	m_pFigure2nd = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x + FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		TEXTURE_NUMBER);

	// グレーの重ねる奴
	m_pGray = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x, pos.y, 0),
		FIGURE_BACK_WIDTH, FIGURE_BACK_HEIGHT,
		TEXTURE_CHAIN);
	// 最初は大きさ０だよ
	m_pGray->SetVertexPolygon(m_GrayPos, 0, 0);

	// レイヤー
	m_pBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFigure1st->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFigure2nd->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pGray->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// ポリゴンのテクスチャ変更
	Set(m_Time);

	// 最初のアニメーション用に2Dを透過させる
	m_pFigure1st->SetColorPolygon(m_Anime2DColor);
	m_pFigure2nd->SetColorPolygon(m_Anime2DColor);
}

//=============================================================================
// 終了
//=============================================================================
void CCountTime::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CCountTime::Update(void)
{
	// 時間のカウントとテクスチャ切り替え
	UpdateTime();

	// 開始アニメーション更新
	UpdateAnime();

	// 止めてぐれーになってるか
	if (m_isGray)
	{
		m_GrayCount++;
		if (m_GrayCount > m_GrayCountMax)
		{
			// グレー終わり
			m_isGray = false;
			GrayScaleClose();
		}
	}

	// スケールの更新をするか
	if (m_isScale)
	{
		UpdateScale();
	}
}

//=============================================================================
// 時間をカウントする更新
//=============================================================================
void CCountTime::UpdateTime()
{
	// ゲームから取得に変更
	m_Time = m_pGame->GetBattleTimer() / TARGET_FPS;
	Set(m_Time);
	return;
}

//=============================================================================
// 開始アニメーションをする更新
//=============================================================================
void CCountTime::UpdateAnime()
{
	// 開始アニメーションするよー
	if (!m_isAnime)	return;

	// カウント
	m_AnimeCount++;
	// アルファ値更新
	m_Anime2DColor.a += m_AnimeOneFrameAlpha;
	m_pFigure1st->SetColorPolygon(m_Anime2DColor);
	m_pFigure2nd->SetColorPolygon(m_Anime2DColor);
	// 開始アニメーション終了
	if (m_AnimeCount > m_AnimeCountMax){
		// ここでアルファ値が1.0になるはずだけど一応！少数とか！
		m_Anime2DColor.a = 1.0f;
		m_pFigure1st->SetColorPolygon(m_Anime2DColor);
		m_pFigure2nd->SetColorPolygon(m_Anime2DColor);
		m_isAnime = false;
	}
}

//=============================================================================
// 作成
//=============================================================================
CCountTime* CCountTime::Create(
	D3DXVECTOR2 &pos, int time,
	LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame* pGame)
{
	CCountTime* p = new CCountTime(pDevice, pManager, pGame);
	p->Init(pos, time);
	return p;
}

//=============================================================================
// 時間を二つの2Dのテクスチャに反映させる
//=============================================================================
void CCountTime::Set(int time)
{
	// 各桁の計算
	float u1 = (m_Time % 100 / 10) * 0.1f;
	float u2 = (m_Time % 10) * 0.1f;
	UV_INDEX uv1 = { u1, u1 + 0.1f, 0.0f, 1.0f };
	UV_INDEX uv2 = { u2, u2 + 0.1f, 0.0f, 1.0f };
	m_pFigure1st->SetUV(&uv1);
	m_pFigure2nd->SetUV(&uv2);
}

//=============================================================================
// タイマーをストップ状態にする(グレーにする)
//=============================================================================
void CCountTime::Stop(int frame)
{
	m_GrayCount = 0;
	m_GrayCountMax = frame;
	m_isGray = true;
	GrayScaleOpen();
}

//=============================================================================
// 開始アニメーションをする　引数↓
// 終了するまでのカウント(何フレームアニメーションするか)
//=============================================================================
void CCountTime::StartAnimation(int endCount)
{
	assert(endCount > 0 && "endCountはマイナスの値入れないで！");

	// アニメーションが終了するフレーム数
	m_AnimeCountMax = endCount;

	// アニメーションするための変数初期化
	m_AnimeCount = 0;
	m_isAnime = true;
	m_AnimeOneFrameAlpha = 1.0f / endCount;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);

	// 2Dを透過させる
	m_pFigure1st->SetColorPolygon(m_Anime2DColor);
	m_pFigure2nd->SetColorPolygon(m_Anime2DColor);
}


//=============================================================================
// 開始アニメーションをする　引数↓
// 終了するまでのカウント(何フレームアニメーションするか)
//=============================================================================
void CCountTime::UpdateScale()
{
	m_GrayTime += GRAY_SCALE_SPEED;

	if (m_GrayTime > 1.0f)
	{
		m_GrayTime = 1.0f;
		float width = EasingInterpolation(m_GrayWidth, m_GrayWidthDest, m_GrayTime);
		float height = EasingInterpolation(m_GrayHeight, m_GrayHeightDest, m_GrayTime);
		m_pGray->SetVertexPolygon(m_GrayPos, width, height);

		// もう出現しない
		m_isScale = false;
	}
	else
	{
		float width = EasingInterpolation(m_GrayWidth, m_GrayWidthDest, m_GrayTime);
		float height = EasingInterpolation(m_GrayHeight, m_GrayHeightDest, m_GrayTime);
		m_pGray->SetVertexPolygon(m_GrayPos, width, height);
	}
}

//=============================================================================
// グレーの開く
//=============================================================================
void CCountTime::GrayScaleOpen()
{
	m_GrayWidth = 0;
	m_GrayHeight = 0;
	m_GrayWidthDest = FIGURE_BACK_WIDTH;
	m_GrayHeightDest = FIGURE_BACK_HEIGHT;
	m_isScale = true;
	m_GrayTime = 0;
}

//=============================================================================
// グレーの閉じる
//=============================================================================
void CCountTime::GrayScaleClose()
{
	m_GrayWidth = FIGURE_BACK_WIDTH;
	m_GrayHeight = FIGURE_BACK_HEIGHT;
	m_GrayWidthDest = 0;
	m_GrayHeightDest = 0;
	m_isScale = true;
	m_GrayTime = 0;
}

//=============================================================================
// 非表示
//=============================================================================
void CCountTime::SetImvisible(void)
{
	m_pBack->SetDrawFlag(false);
	m_pFigure1st->SetDrawFlag(false);
	m_pFigure2nd->SetDrawFlag(false);
	m_pGray->SetDrawFlag(false);
}

//=============================================================================
// 表示
//=============================================================================
void CCountTime::SetVisible(void)
{
	m_pBack->SetDrawFlag(true);
	m_pFigure1st->SetDrawFlag(true);
	m_pFigure2nd->SetDrawFlag(true);
	m_pGray->SetDrawFlag(true);
}
//----EOF----