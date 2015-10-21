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
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// バーのテクスチャ
static const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// TODO 仮の量
static const float CROWD_MAX = 255;
// バーの色
static const D3DXCOLOR BAR_COLOR_LEFT = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
static const D3DXCOLOR BAR_COLOR_RIGHT = D3DXCOLOR(0.0f, 0.1f, 1.0f, 1.0f);
// バーの移動加速度初期値
static const float BAR_VEL_BASE = 1.0f;
// バーの移動加速度
static const float BAR_VEL_ACCE = 1.1f;

//=============================================================================
// コンストラクタ
//=============================================================================
CCrowdBar::CCrowdBar(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;
	m_PosLeftX = 0;
	m_PosRightX = 0;
	m_PosCenterX = 0;
	m_PosDestX = 0;
	m_PosCurrentX = 0;
	m_PosVel = 0;
	m_isPosMove = 0;
	m_Value = 0;
	m_ValueMax = 0;
	m_WidthOneValue = 0;
	m_pBarLeft = NULL;
	m_pBarRight = NULL;

	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeEasingOneFrame = 0;
	m_AnimeEasingTimer = 0;
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
void CCrowdBar::Init(
	float height,
	float posCenterY,
	float posLeft,
	float posRight,
	LPDIRECT3DDEVICE9 *pDevice)
{
	// 変数代入
	m_Value = 0;
	m_ValueMax = CROWD_MAX;
	m_PosLeftX = posLeft;
	m_PosRightX = posRight;

	// 一つのバーの幅 2個だから0.5f
	float width = (m_PosRightX - m_PosLeftX) * 0.5f;

	// 左右バーの中央の座標 中央だから0.5f
	float left_center = m_PosLeftX + width * 0.5f;
	float right_center = m_PosRightX - width * 0.5f;

	// 2D初期化
	m_pBarLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(left_center, posCenterY, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(right_center, posCenterY, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBarRight->AddLinkList(CRenderer::TYPE_RENDER_UI);

	m_pBarLeft->SetColorPolygon(BAR_COLOR_LEFT);
	m_pBarRight->SetColorPolygon(BAR_COLOR_RIGHT);

	// 値（m_Value）１当たりのピクセル数(float)を計算
	m_WidthOneValue = width / m_ValueMax;

	// 中心の座標
	m_PosCenterX = m_PosLeftX + width;
	m_PosCurrentX = m_PosCenterX;
}

//=============================================================================
// 終了
//=============================================================================
void CCrowdBar::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CCrowdBar::Update(void)
{
	CDebugProc::Print("観客値　　%f\n", m_Value);
	CDebugProc::Print("Cur X　　%f\n", m_PosCurrentX);
	CDebugProc::Print("Def X　　%f\n", m_PosDestX);

	// 開始アニメーション
	if (m_isAnime){
		UpdateAnime();
		return;
	}

	// バーを動かす更新
	UpdateBarMove();
}

//=============================================================================
// バーを動かす更新
//=============================================================================
void CCrowdBar::UpdateBarMove(void)
{
	// 境目の座標をセットする時
	if (m_isPosMove)
	{
		m_PosVel *= BAR_VEL_ACCE;
		m_PosCurrentX += m_PosVel;

		m_pBarLeft->SetVertexPolygonRight(m_PosCurrentX);
		m_pBarRight->SetVertexPolygonLeft(m_PosCurrentX);

		// 同じ値になったら境目の座標セットフラグfalse
		if (m_PosVel < 0){
			if (m_PosCurrentX < m_PosDestX)
			{
				m_isPosMove = false;
				m_PosCurrentX = m_PosDestX;
				m_pBarLeft->SetVertexPolygonRight(m_PosCurrentX);
				m_pBarRight->SetVertexPolygonLeft(m_PosCurrentX);
			}
		}
		else
			if (m_PosCurrentX > m_PosDestX)
			{
			m_isPosMove = false;
			m_PosCurrentX = m_PosDestX;
			m_pBarLeft->SetVertexPolygonRight(m_PosCurrentX);
			m_pBarRight->SetVertexPolygonLeft(m_PosCurrentX);
			}
	}

}

//=============================================================================
// 描画
//=============================================================================
void CCrowdBar::DrawUI(void)
{
}

//=============================================================================
// 作成
//=============================================================================
CCrowdBar* CCrowdBar::Create(
	float height,
	float posCenterY,
	float posLeft,
	float posRight,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CCrowdBar* p = new CCrowdBar(pDevice);
	p->Init(height, posCenterY, posLeft, posRight, pDevice);
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

	m_isPosMove = true;
	m_PosDestX = m_Value * m_WidthOneValue + m_PosCenterX;
	
	// 最初の境目座標の慣性移動量設定
	if (m_PosDestX < m_PosCurrentX)
		m_PosVel = -BAR_VEL_BASE;
	else
		m_PosVel = BAR_VEL_BASE;
}

//=============================================================================
// 開始アニメーションをする　引数↓
// 終了するまでのカウント(何フレームアニメーションするか)
//=============================================================================
void CCrowdBar::StartAnimation(int endCount)
{
	assert(endCount > 0 && "endCountはマイナスの値入れないで！");

	// アニメーションが終了するフレーム数
	m_AnimeCountMax = endCount;

	// アニメーションするための変数初期化
	m_AnimeCount = 0;
	m_isAnime = true;
	m_AnimeEasingOneFrame = 1.0f / static_cast<float>(endCount);
	m_AnimeEasingTimer = 0;

	// バーを消す
	m_pBarLeft->SetVertexPolygonLeft(m_PosCenterX);
	m_pBarRight->SetVertexPolygonRight(m_PosCenterX);

	m_pBarLeft->SetVertexPolygonRight(m_PosCenterX);
	m_pBarRight->SetVertexPolygonLeft(m_PosCenterX);

	// バーの基本の情報初期化
	Init();
}

//=============================================================================
// 開始アニメーションをする更新
//=============================================================================
void CCrowdBar::UpdateAnime()
{
	if (m_AnimeEasingTimer < 1){
		float leftX = EasingInterpolation(
			m_PosCenterX,
			m_PosLeftX,
			m_AnimeEasingTimer);
		float rightX = EasingInterpolation(
			m_PosCenterX,
			m_PosRightX,
			m_AnimeEasingTimer);

		// 補間のタイマを更新
		m_AnimeEasingTimer += m_AnimeEasingOneFrame;

		// カレントの値をセット
		m_pBarLeft->SetVertexPolygonLeft(leftX);
		m_pBarRight->SetVertexPolygonRight(rightX);
	}

	// カウント
	m_AnimeCount++;

	// 開始アニメーション終了
	if (m_AnimeCount > m_AnimeCountMax){
		m_isAnime = false;
	}
}

//=============================================================================
// アニメーションの初期化をするときのバー情報初期化
//=============================================================================
void CCrowdBar::Init()
{
	m_Value = 0;
	m_PosCurrentX = m_PosCenterX;
}
//----EOF----