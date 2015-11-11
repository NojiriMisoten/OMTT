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

//*****************************************************************************
// 定数
//*****************************************************************************
// 二つの数字の幅（中心座標からのオフセット）
static const float FIGURE_SPACE_WIDTH = 30;
// 数字の大きさ
static const float FIGURE_WIDTH = 30;
static const float FIGURE_HEIGHT = 50;
// 数字のテクスチャ
static const TEXTURE_TYPE FIGURE_TEXTURE = TEXTURE_NUMBER;
// タイムの最大数
static const int TIME_MAX = 99;
// 一秒のカウント
static const short SECOND_FRAME = 60;

//=============================================================================
// コンストラクタ
//=============================================================================
CCountTime::CCountTime(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;
	m_pFigure1st = NULL;
	m_pFigure2nd = NULL;
	m_Time = 0;
	m_TimeCount = 0;
	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeOneFrameAlpha = 0;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);	// 最初のアニメーションでαを透明にするため
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

	// 二つの数字を生成
	m_pFigure1st = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x - FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		FIGURE_TEXTURE);

	m_pFigure2nd = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x + FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		FIGURE_TEXTURE);

	m_pFigure1st->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_pFigure2nd->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

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
}

//=============================================================================
// 時間をカウントする更新
//=============================================================================
void CCountTime::UpdateTime()
{
	m_TimeCount++;

	// 1秒経過していたら
	if (m_TimeCount > SECOND_FRAME)
	{
		m_TimeCount = 0;
		// 時間経過
		m_Time--;
		// 開始時間の更新
		// ポリゴンのテクスチャ変更
		Set(m_Time);
	}
	CDebugProc::Print("\nTIME = %d\n", m_Time);
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
// 描画
//=============================================================================
void CCountTime::DrawNormalRender(void)
{
}

//=============================================================================
// 作成
//=============================================================================
CCountTime* CCountTime::Create(
	D3DXVECTOR2 &pos, int time, LPDIRECT3DDEVICE9 *pDevice)
{
	CCountTime* p = new CCountTime(pDevice);
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

//----EOF----