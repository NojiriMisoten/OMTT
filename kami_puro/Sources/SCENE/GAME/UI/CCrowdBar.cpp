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

// 枠のおおきさ
static const float BAR_FRAME_WIDTH = 940;
static const float BAR_FRAME_HEIGHT = 25;
// バーと枠のオフセット
static const D3DXVECTOR2 BAR_FRAME_OFFSET = D3DXVECTOR2(0, 0);

// 観客アニメーションの2Dの大きさ
static const float CROWD_WIDTH = 140;
static const float CROWD_HEIGHT = 60;
// 観客アニメーション2Dのバーからのオフセット
static const float CROWD_OFFSET_Y = -40;
// 観客アニメーションの間隔
static const int CROWD_ANIME_INTERVAL = 30;
// 観客アニメーションのＵＶの一つ（1 / アニメ数）
static const float CROWD_ANIME_UV_ONE = 1.0f / 4.0f;

// バチバチの大きさ
static const float SPARK_WIDTH = 50;
static const float SPARK_HEIGHT = 50;
// バチバチのバーからのオフセット
static const D3DXVECTOR2 SPARK_OFFSET = D3DXVECTOR2(0, 0);
// バチバチの拡縮間隔
static const int SPARK_ANIME_INTERVAL = 30;

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
	m_pFrame = NULL;
	m_pCrowd = NULL;
	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeEasingOneFrame = 0;
	m_AnimeEasingTimer = 0;
	m_CrowdAnimeCount = 0;
	m_pSpark = NULL;
	m_SparkRot = 0;
	m_SparkCount = 0;
	m_isSparkAdd = true;
	m_SparkPos = D3DXVECTOR3(0, 0, 0);
	m_AnimeOneFrameAlpha = 0;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);	// 最初のアニメーションで透明から始まるため
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
		D3DXVECTOR3(left_center, posCenterY + BAR_FRAME_OFFSET.y, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(right_center, posCenterY + BAR_FRAME_OFFSET.y, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarLeft->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_pBarRight->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	m_pBarLeft->SetColorPolygon(BAR_COLOR_LEFT);
	m_pBarRight->SetColorPolygon(BAR_COLOR_RIGHT);

	// 値（m_Value）１当たりのピクセル数(float)を計算
	m_WidthOneValue = width / m_ValueMax;

	// 中心の座標
	m_PosCenterX = m_PosLeftX + width;
	m_PosCurrentX = m_PosCenterX;

	// 最初はUIの開始アニメーションをするためポリゴンの位置を変更
	m_pBarLeft->SetVertexPolygonLeft(m_PosCenterX);
	m_pBarRight->SetVertexPolygonRight(m_PosCenterX);
	m_pBarLeft->SetVertexPolygonRight(m_PosCenterX);
	m_pBarRight->SetVertexPolygonLeft(m_PosCenterX);

	// 枠
	m_pFrame = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, posCenterY + BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_CROWD_GAGE_FRAME);
	m_pFrame->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// 観客たちの絵
	m_pCrowd = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, posCenterY + CROWD_OFFSET_Y, 0),
		CROWD_WIDTH, CROWD_HEIGHT,
		TEXTURE_CROWD_GAGE_HUMAN);
	m_pCrowd->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// 観客たちのＵＶ値
	m_CrowdUV = UV_INDEX(0.0f, 0.25f, 0.0f, 1.0f);
	m_pCrowd->SetUV(&m_CrowdUV);

	// バチバチ
	m_SparkPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, posCenterY + SPARK_OFFSET.y, 0);
	m_pSpark = CScene2D::Create(m_pD3DDevice,
		m_SparkPos, SPARK_WIDTH, SPARK_HEIGHT,
		TEXTURE_CROWD_SPARK);
	m_pSpark->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_pSpark->SetColorPolygon(m_Anime2DColor);
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

	// 観客たちのアニメーション更新
	UpdateCrowdAnimation();

	// バーを動かす更新
	UpdateBarMove();

	// バチバチのアニメーション
	UpdateSparkAnimation();
}

//=============================================================================
// 観客たちのアニメーション更新
//=============================================================================
void CCrowdBar::UpdateCrowdAnimation(void)
{
	// アニメーションカウント
	m_CrowdAnimeCount++;

	// テクスチャを変えるタイミング
	if (m_CrowdAnimeCount > CROWD_ANIME_INTERVAL)
	{
		// カウント初期化
		m_CrowdAnimeCount = 0;
		
		// ＵＶ変更
		CrowdChangeUV();
	}

}

//=============================================================================
// 観客たちのアニメーションのテクスチャ座標を変更する
//=============================================================================
void CCrowdBar::CrowdChangeUV(void)
{
	// 右端まで行ってたらループ
	if (m_CrowdUV.right >= 1.0f)
	{
		m_CrowdUV.left = 0;
		m_CrowdUV.right = CROWD_ANIME_UV_ONE;
	}
	// 他は右にスクロール
	else
	{
		m_CrowdUV.left += CROWD_ANIME_UV_ONE;
		m_CrowdUV.right += CROWD_ANIME_UV_ONE;
	}

	// ＵＶセット
	m_pCrowd->SetUV(&m_CrowdUV);
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

		// バチバチの移動
		m_SparkPos.x = m_PosCurrentX;
		m_pSpark->SetVertexPolygon(m_SparkPos, SPARK_WIDTH, SPARK_HEIGHT);

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
// バチバチのアニメーション
//=============================================================================
void CCrowdBar::UpdateSparkAnimation(void)
{
	// 角度
	float num = m_SparkCount - static_cast<float>(SPARK_ANIME_INTERVAL)* 0.5f;
	float rot = num * 0.01f;
	m_pSpark->SetRot(D3DXVECTOR3(0, 0, rot));
	// 大きさ
	
	// 大きくすると上の観客の絵とかぶるしどうしようかなー

	// 加算と減算を繰り返す
	if (m_isSparkAdd)
	{
		m_SparkCount++;
		if (m_SparkCount > SPARK_ANIME_INTERVAL)
		{
			m_isSparkAdd = !m_isSparkAdd;
		}
	}
	else
	{
		m_SparkCount--;
		if (m_SparkCount <= 0)
		{
			m_isSparkAdd = !m_isSparkAdd;
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void CCrowdBar::DrawNormalRender(void)
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

	// バチバチの初期化
	m_AnimeOneFrameAlpha = 1.0f / endCount;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);
	m_pSpark->SetColorPolygon(m_Anime2DColor);
}

//=============================================================================
// 開始アニメーションをする更新
//=============================================================================
void CCrowdBar::UpdateAnime()
{
	if (m_AnimeEasingTimer < 1)
	{
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
	m_Anime2DColor.a += m_AnimeOneFrameAlpha;
	m_pSpark->SetColorPolygon(m_Anime2DColor);

	// 開始アニメーション終了
	if (m_AnimeCount > m_AnimeCountMax){
		m_isAnime = false;
		m_Anime2DColor.a = 1.0f;
		m_pSpark->SetColorPolygon(m_Anime2DColor);
	}
}

//=============================================================================
// アニメーションの初期化をするときのバー情報初期化
//=============================================================================
void CCrowdBar::Init()
{
	m_Value = 0;
	m_PosCurrentX = m_PosCenterX;
	m_SparkRot = 0;
	m_SparkCount = 0;
	m_isSparkAdd = true;
}
//----EOF----