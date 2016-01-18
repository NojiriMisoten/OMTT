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
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../PLAYER/CPlayer.h"
#include "CCutIn2D.h"

//*****************************************************************************
// 定数
//*****************************************************************************
const float CHpBar::JIJII_TEX_U = 1.0f / 4.0f;
const float CHpBar::JIJII_TEX_V = 1.0f / 2.0f;

// 最大HP量
static const float HP_MAX = DEFAULT_HP_PARAMETER;

// HPの割合によって表情がかわる　その値
static const float HP_EXPRESSION[CHpBar::EXPRESSION_MAX] = {
	DEFAULT_HP_PARAMETER,
	DEFAULT_HP_PARAMETER * 0.6f,
	DEFAULT_HP_PARAMETER * 0.3f,
};

// 赤いバーを変更するまでのカウント数
static const short RED_CHANGE_INTERVAL = 40;
// ダメージを受けた分バーを減らすまでのフレーム数(バーの速度)
static const float ERASE_INTERVAL_G = 20.0f;
static const float ERASE_INTERVAL_R = 20.0f;
// ダメージ分減らすときの1フレーム当たりの量 補間のtimeの計算で使う
static const float ERASE_ONE_FRAME_G = 1.0f / ERASE_INTERVAL_G;
static const float ERASE_ONE_FRAME_R = 1.0f / ERASE_INTERVAL_R;
// 震わすときの範囲
static const float SHAKE_RANGE = 10.0f;
// 震わすときの範囲の抵抗
static const float SHAKE_RANGE_RESIST = 0.8f;
// 震わすときの時間
static const int SHAKE_INTERVAL = 15;
// 枠のおおきさ
static const float BAR_FRAME_WIDTH = SCREEN_WIDTH / 1280.f * 800.f * 0.8f;
static const float BAR_FRAME_HEIGHT = SCREEN_HEIGHT / 720.f * 200.f * 0.8f;
// まるいほうの枠のおおきさ
static const float BAR_FRAME_CIRCLE_WIDTH = SCREEN_WIDTH / 1280.f * 160.f;
static const float BAR_FRAME_CIRCLE_HEIGHT = SCREEN_HEIGHT / 720.f * 160.f;
// バーの座標に対しての枠の座標のoffset
static const D3DXVECTOR2 BAR_FRAME_OFFSET = D3DXVECTOR2(SCREEN_WIDTH / 1280.f * 49.f, SCREEN_HEIGHT / 720.f * 38.f);
// バーの座標に対してのじじいの座標のoffset バーの左右の端から相対参照
static const D3DXVECTOR2 FACE_OFFSET = D3DXVECTOR2(SCREEN_WIDTH / 1280.f * 37.f, SCREEN_HEIGHT / 720.f * -38.f);
// じじいのおおきさ
static const float FACE_WIDTH = SCREEN_WIDTH / 1280.f * 130.f;
static const float FACE_HEIGHT = SCREEN_HEIGHT / 720.f * 130.f;
// じじいの背景の色の透過する最初の色
static const D3DXCOLOR FACE_BACK_ALPHA_LEFT = D3DXCOLOR(1.0f, 0.1f, 0.0f, 0.0f);
static const D3DXCOLOR FACE_BACK_ALPHA_RIGHT = D3DXCOLOR(0.0f, 0.1f, 1.0f, 0.0f);
// じじいの背景の大きさ
static const float FACE_BACK_WIDTH = BAR_FRAME_CIRCLE_WIDTH - (SCREEN_WIDTH / 1280.f * 5.f);
static const float FACE_BACK_HEIGHT = BAR_FRAME_CIRCLE_HEIGHT - (SCREEN_HEIGHT / 720.f * 5.f);
// 点滅の速さ
static const float WHITE_SPEED = 0.1f;
// 点滅する割合
static const float WHITE_CHANGE_VALUE = HP_MAX * 0.3f;
// 火のフェードスピード
static const float FIRE_FADE_SPEED = 0.1f;
// 火の座標のオフセット yは固定 xは反転して二つに使って
static const D3DXVECTOR2 FIRE_OFFSET = D3DXVECTOR2(SCREEN_WIDTH / 1280.f * 40.f, SCREEN_HEIGHT / 720.f * 30.f);
// 火の大きさ
static const float FIRE_BACK_WIDTH = SCREEN_WIDTH / 1280.f * 280.f;
static const float FIRE_BACK_HEIGHT = SCREEN_HEIGHT / 720.f * 280.f;
static const float FIRE_SIDE_WIDTH = SCREEN_WIDTH / 1280.f * 100.f;
static const float FIRE_SIDE_HEIGHT = SCREEN_HEIGHT / 720.f * 180.f;
// 火のテクスチャの一コマ分の大きさ
static const float FIRE_ONE_WIDTH = 1.0f / 10.0f;
// 火のテクスチャアニメーションスピード
static const short FIRE_TEXTURE_SPEED = 4;
// ギラのテクスチャのアニメーションするコマの最大とか
static const short FIRE_TEXTURE_MAX = 7;
static const short FIRE_TEXTURE_MIN = 1;

//=============================================================================
// コンストラクタ
//=============================================================================
CHpBar::CHpBar(LPDIRECT3DDEVICE9 *pDevice)
{
	// 補間用の時間 0だと補間が始まるため1入れとく
	for (int i = 0; i < BAR_MAX; i++){
		m_pBar[i].m_p2D = NULL;
		m_pBar[i].m_PosEasingEnd = 0;
		m_pBar[i].m_PosEasingStart = 0;
		m_pBar[i].m_PosLeft = 0;
		m_pBar[i].m_PosRight = 0;
		m_pBar[i].m_TimerEasing = 1;
		m_pBar[i].m_Value = 0;
	}
	m_ValueMax = 0;
	m_WidthOneValue = 0;
	m_RedResetCountLeft = 0;
	m_RedResetCountRight = 0;
	m_isRedResetLeft = false;
	m_isRedResetRight = false;
	m_isRedEasingLeft = false;
	m_isRedEasingRight = false;

	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeEasingOneFrame = 0;
	m_AnimeTimerEasing = 0;

	m_pFrameLeft = NULL;
	m_pFrameRight = NULL;
	m_pFrameLeftBar = NULL;
	m_pFrameRightBar = NULL;

	m_isShakeLeft = false;
	m_isShakeRight = false;
	m_ShakeCountLeft = 0;
	m_ShakeCountRight = 0;
	m_PosCenterY = 0;
	m_ShakePosYLeft = 0;
	m_ShakePosYRight = 0;
	m_ShakeRangeLeft = 0;
	m_ShakeRangeRight = 0;

	m_FaceLeft.m_Expression = EXPRESSION_GOOD;
	m_FaceLeft.m_pFace2D = NULL;
	m_FaceLeft.m_Pos = D3DXVECTOR2(0, 0);
	m_FaceLeft.m_UV = UV_INDEX(0, JIJII_TEX_U, 0, JIJII_TEX_V);
	m_FaceRight.m_Expression = EXPRESSION_GOOD;
	m_FaceRight.m_pFace2D = NULL;
	m_FaceRight.m_Pos = D3DXVECTOR2(0, 0);
	m_FaceRight.m_UV = UV_INDEX(0, JIJII_TEX_U, JIJII_TEX_V, 1);
	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeOneFrameAlpha = 0;
	// 最初のアニメーションで透明から始まるため
	m_Anime2DColor = D3DXCOLOR(1, 1, 1,0);
	m_Anime2DColorJijiiLeft = FACE_BACK_ALPHA_LEFT;
	m_Anime2DColorJijiiRight = FACE_BACK_ALPHA_RIGHT;
	m_pD3DDevice = pDevice;
	m_isChangeWhiteLeft = false;
	m_isChangeWhiteRight = false;
	m_isAddWhiteLeft = true;
	m_isAddWhiteRight = true;

	m_pFireLeftBack = NULL;
	m_pFireRightBack = NULL;
	m_FireColorRight = D3DXCOLOR(1, 1, 1, 0);
	m_FireColorLeft = D3DXCOLOR(1, 1, 1, 0);
	m_FireTextureLeft = 0;
	m_FireTextureRight = 0;
	m_FireTextureCountLeft = 0;
	m_FireTextureCountRight = 0;
	m_isFireLeft = false;
	m_isFireRight = false;
	m_isFireAppearLeft = false;
	m_isFireAppearRight = false;
	m_isFireDisappearLeft = false;
	m_isFireDisappearRight = false;

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
	float height,
	float posCenterY,
	float posLeftBarLeftX,
	float posLeftBarRightX,
	float posRightBarLeftX,
	float posRightBarRightX)
{
	// 変数代入
	m_ValueMax = HP_MAX;
	m_AnimeTimerEasing = 0;

	m_pBar[BAR_GREEN_L].m_PosLeft = posLeftBarLeftX;
	m_pBar[BAR_GREEN_L].m_PosRight = posLeftBarRightX;
	m_pBar[BAR_GREEN_R].m_PosLeft = posRightBarLeftX;
	m_pBar[BAR_GREEN_R].m_PosRight = posRightBarRightX;
	m_pBar[BAR_RED_L].m_PosLeft = posLeftBarLeftX;
	m_pBar[BAR_RED_L].m_PosRight = posLeftBarRightX;
	m_pBar[BAR_RED_R].m_PosLeft = posRightBarLeftX;
	m_pBar[BAR_RED_R].m_PosRight = posRightBarRightX;

	// 初期か
	Init();
	// バーの幅
	float barWidth = posLeftBarRightX - posLeftBarLeftX;
	// 値（m_Value）１当たりのピクセル数(float)を計算
	m_WidthOneValue = barWidth / m_ValueMax;
	// 中心座標
	m_PosCenterY = posCenterY;

	// バーの座標
	D3DXVECTOR3 pos[BAR_MAX] = {
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
	};

	for (int i = 0; i < BAR_MAX; i++)
	{
		// バーの2Dの作成
		m_pBar[i].m_p2D = CCutIn2D::Create(m_pD3DDevice,
			pos[i],
			barWidth, height,
			i < 2 ? TEXTURE_HP_GAGE_R : TEXTURE_HP_GAGE_G);
		// バーの変数
		m_pBar[i].m_Value = m_ValueMax;
		m_pBar[i].m_TimerEasing = 1;
	}

	// 最初はUI開始アニメーションをするから、ポリゴンをセットしたい
	// バーをStart位置に動かす
	m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_GREEN_L].m_PosRight);
	m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_GREEN_R].m_PosLeft);
	m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_RED_L].m_PosRight);
	m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_RED_R].m_PosLeft);

	// 顔の座標
	D3DXVECTOR3 facePosL = D3DXVECTOR3(posLeftBarLeftX - FACE_OFFSET.x, m_PosCenterY + FACE_OFFSET.y, 0);
	D3DXVECTOR3 facePosR = D3DXVECTOR3(posRightBarRightX + FACE_OFFSET.x, m_PosCenterY + FACE_OFFSET.y, 0);

	// 枠
	m_pFrameLeftBar = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[0].x - BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_BAR);
	m_pFrameRightBar = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[1].x + BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_BAR);
	// 反転
	m_pFrameRightBar->SetUVMirror();

	// 丸い枠
	m_pFrameLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		BAR_FRAME_CIRCLE_WIDTH, BAR_FRAME_CIRCLE_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_CIRCLE);
	m_pFrameRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		BAR_FRAME_CIRCLE_WIDTH, BAR_FRAME_CIRCLE_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_CIRCLE);

	// 顔生成
	m_FaceLeft.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		FACE_WIDTH, FACE_HEIGHT, TEXTURE_JIJII);
	// 右初期化
	m_FaceRight.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		FACE_WIDTH, FACE_HEIGHT, TEXTURE_JIJII);
	// じじいの背景
	m_FaceLeft.m_pBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		FACE_BACK_WIDTH, FACE_BACK_HEIGHT, TEXTURE_JIJII_BACK);
	// 右初期化
	m_FaceRight.m_pBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		FACE_BACK_WIDTH, FACE_BACK_HEIGHT, TEXTURE_JIJII_BACK);
	// 最初はVの値をセットするから4つの引数の方
	m_FaceLeft.m_pFace2D->SetUV(&(m_FaceLeft.m_UV));
	m_FaceRight.m_pFace2D->SetUV(&(m_FaceRight.m_UV));
	// 最初は見えないから透明化
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pBack->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceRight.m_pBack->SetColorPolygon(m_Anime2DColorJijiiRight);

	// うしろがわ
	m_pFireLeftBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y - FIRE_OFFSET.y, 0),
		FIRE_BACK_WIDTH, FIRE_BACK_HEIGHT, TEXTURE_FIRE_BACK);
	m_pFireRightBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y - FIRE_OFFSET.y, 0),
		FIRE_BACK_WIDTH, FIRE_BACK_HEIGHT, TEXTURE_FIRE_BACK);
	// 透過するよ
	m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
	m_pFireRightBack->SetColorPolygon(m_FireColorRight);

	// 最初はVの値をセットするから4つの引数の方
	m_pFireLeftBack->SetUV(0, FIRE_ONE_WIDTH);
	m_pFireRightBack->SetUV(0, FIRE_ONE_WIDTH);

	// 描画順を決定
	for (int i = 0; i < BAR_MAX; i++)
	{
		m_pBar[i].m_p2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	}
	m_FaceLeft.m_pBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrameLeftBar->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrameRightBar->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrameLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrameRight->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFireLeftBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFireRightBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceLeft.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
}

//=============================================================================
// 終了
//=============================================================================
void CHpBar::Uninit(void)
{
}

//=============================================================================
//=============================================================================
void CHpBar::CBarBase::SetImvisible(void)
{
	m_p2D->SetDrawFlag(false);
}
//=============================================================================
//=============================================================================
void CHpBar::CBarBase::SetVisible(void)
{
	m_p2D->SetDrawFlag(true);
}

//=============================================================================
// 更新
//=============================================================================
void CHpBar::Update(void)
{
#ifdef _DEBUG
	CDebugProc::PrintL("左体力：%+10.3f / %+10.3f\n", (float)m_pBar[BAR_GREEN_L].m_Value, (float)m_ValueMax);
	CDebugProc::PrintL("右体力：%+10.3f / %+10.3f\n", (float)m_pBar[BAR_GREEN_R].m_Value, (float)m_ValueMax);
	if (m_FaceLeft.m_Expression == EXPRESSION_GOOD)
		CDebugProc::PrintL("左表情：GOOD\n");
	else if (m_FaceLeft.m_Expression == EXPRESSION_NORAML)
		CDebugProc::PrintL("左表情：NOAML\n");
	else
		CDebugProc::PrintL("左表情：BAD\n");
	
	if (m_FaceRight.m_Expression == EXPRESSION_GOOD)
		CDebugProc::PrintL("右表情：GOOD\n");
	else if (m_FaceRight.m_Expression == EXPRESSION_NORAML)
		CDebugProc::PrintL("右表情：NOAML\n");
	else
		CDebugProc::PrintL("右表情：BAD\n");
#endif
	// 開始アニメーションの更新
	if (m_isAnime)
	{
		UpdateAnime();
		return;
	}

	// 震えの更新
	UpdateShake();

	// 左側みどりの補間を行うなら
	if (m_pBar[BAR_GREEN_L].m_TimerEasing < 1.0f)
	{
		float posX = EasingInterpolation(
			m_pBar[BAR_GREEN_L].m_PosEasingStart,
			m_pBar[BAR_GREEN_L].m_PosEasingEnd,
			m_pBar[BAR_GREEN_L].m_TimerEasing);

		// 頂点動かす
		m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(posX);
		// 補間の時間更新
		m_pBar[BAR_GREEN_L].m_TimerEasing += ERASE_ONE_FRAME_G;
	}
	// 右側みどりの補間を行うなら
	if (m_pBar[BAR_GREEN_R].m_TimerEasing < 1.0f)
	{
		float posX = EasingInterpolation(
			m_pBar[BAR_GREEN_R].m_PosEasingStart,
			m_pBar[BAR_GREEN_R].m_PosEasingEnd,
			m_pBar[BAR_GREEN_R].m_TimerEasing);

		// 頂点動かす
		m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(posX);
		// 補間の時間更新
		m_pBar[BAR_GREEN_R].m_TimerEasing += ERASE_ONE_FRAME_G;
	}

	// 左の赤いバーを緑に合わせる
	if (m_isRedResetLeft){
		m_RedResetCountLeft++;
		if (m_RedResetCountLeft > RED_CHANGE_INTERVAL){
			// 赤いバーの値を緑に合わせる
			m_pBar[BAR_RED_L].m_Value = m_pBar[BAR_GREEN_L].m_Value;
			m_pBar[BAR_RED_L].m_TimerEasing = 0;
			// 補間で使う移動前と移動後の座標を保存
			if (m_isRedEasingLeft)
				m_pBar[BAR_RED_L].m_PosEasingStart = m_pBar[BAR_RED_L].m_PosEasingEnd;
			else
				m_pBar[BAR_RED_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosEasingStart;
			m_pBar[BAR_RED_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosEasingEnd;

			// フラグ初期化
			m_isRedResetLeft = false;
			// 赤いバーの線形補間をする更新フラグtrue
			m_isRedEasingLeft = true;
		}
	}
	// 左赤いバーの線形補間更新
	if (m_isRedEasingLeft){
		// 左あか補間を行うなら
		if (m_pBar[BAR_RED_L].m_TimerEasing < 1.0f)
		{
			float posX = EasingInterpolation(
				m_pBar[BAR_RED_L].m_PosEasingStart,
				m_pBar[BAR_RED_L].m_PosEasingEnd,
				m_pBar[BAR_RED_L].m_TimerEasing);

			// 頂点動かす
			m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(posX);
			// 補間の時間更新
			m_pBar[BAR_RED_L].m_TimerEasing += ERASE_ONE_FRAME_R;
		}
	}

	// 右の赤いバーを緑に合わせる
	if (m_isRedResetRight){
		m_RedResetCountRight++;
		if (m_RedResetCountRight > RED_CHANGE_INTERVAL){
			// フラグ初期化
			m_isRedResetRight = false;
			// 赤いバーの線形補間をする更新フラグtrue
			m_isRedEasingRight = true;
			// 赤いバーの値を緑に合わせる
			m_pBar[BAR_RED_R].m_Value = m_pBar[BAR_GREEN_R].m_Value;
			m_pBar[BAR_RED_R].m_TimerEasing = 0;
			// 補間で使う移動前と移動後の座標を保存
			if (m_isRedEasingLeft)
				m_pBar[BAR_RED_R].m_PosEasingStart = m_pBar[BAR_RED_R].m_PosEasingEnd;
			else
				m_pBar[BAR_RED_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosEasingStart;
			m_pBar[BAR_RED_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosEasingEnd;

		}
	}
	// 右赤いバーの線形補間更新
	if (m_isRedEasingRight){
		// 左あか補間を行うなら
		if (m_pBar[BAR_RED_R].m_TimerEasing < 1.0f)
		{
			float posX = EasingInterpolation(
				m_pBar[BAR_RED_R].m_PosEasingStart,
				m_pBar[BAR_RED_R].m_PosEasingEnd,
				m_pBar[BAR_RED_R].m_TimerEasing);

			// 頂点動かす
			m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(posX);
			// 補間の時間更新
			m_pBar[BAR_RED_R].m_TimerEasing += ERASE_ONE_FRAME_R;
		}
	}

	// 白くするかもの更新
	UpdateWhite();

	// 炎のテクスチャアニメーション
	UpdateFire();
}

// 左炎でる
void CHpBar::FireAppearLeft(void)
{
	m_isFireAppearLeft = true;
	m_FireColorLeft.a = 0.0f;
	m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
}
// 左炎消える
void CHpBar::FireDisappearLeft(void)
{
	m_isFireDisappearLeft = true;
	m_FireColorLeft.a = 1.0f;
	m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
}

// みぎ炎でる
void CHpBar::FireAppearRight(void)
{
	m_isFireAppearRight = true;
	m_FireColorRight.a = 0.0f;
	m_pFireRightBack->SetColorPolygon(m_FireColorRight);
}
// みぎ炎消える
void CHpBar::FireDisappearRight(void)
{
	m_isFireDisappearRight = true;
	m_FireColorRight.a = 1.0f;
	m_pFireRightBack->SetColorPolygon(m_FireColorRight);
}
//=============================================================================
// 左の炎を出す
//=============================================================================
void CHpBar::FireLeft(bool isAppear)
{
	if (isAppear)
	{
		// 炎をまだだしていなかったら
		if (!m_isFireLeft)
		{
			m_isFireLeft = true;
			FireAppearLeft();
		}
	}
	else
	{
		// 炎がでていたら
		if (m_isFireLeft)
		{
			m_isFireLeft = false;
			FireDisappearLeft();
		}
	}
}
//=============================================================================
// 右の炎を出す
//=============================================================================
void CHpBar::FireRight(bool isAppear)
{
	if (isAppear)
	{
		// 炎をまだだしていなかったら
		if (!m_isFireRight)
		{
			m_isFireRight = true;
			FireAppearRight();
		}
	}
	else
	{
		// 炎がでていたら
		if (m_isFireRight)
		{
			m_isFireRight = false;
			FireDisappearRight();
		}
	}
}

//=============================================================================
// 炎の更新
//=============================================================================
void CHpBar::UpdateFire(void)
{
	// 炎だす更新　ひだり
	if (m_isFireAppearLeft)
	{
		m_FireColorLeft.a += FIRE_FADE_SPEED;
		if (m_FireColorLeft.a >= 1.0f)
		{
			m_FireColorLeft.a = 1;
			m_isFireAppearLeft = false;
		}
		m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
	}
	if (m_isFireDisappearLeft)
	{
		m_FireColorLeft.a -= FIRE_FADE_SPEED;
		if (m_FireColorLeft.a <= 0.0f)
		{
			m_FireColorLeft.a = 0;
			m_isFireDisappearLeft = false;
		}
		m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
	}
	// 炎だす更新　みぎ
	if (m_isFireAppearRight)
	{
		m_FireColorRight.a += FIRE_FADE_SPEED;
		if (m_FireColorRight.a >= 1.0f)
		{
			m_FireColorRight.a = 1;
			m_isFireAppearRight = false;
		}
		m_pFireRightBack->SetColorPolygon(m_FireColorRight);
	}
	if (m_isFireDisappearRight)
	{
		m_FireColorRight.a -= FIRE_FADE_SPEED;
		if (m_FireColorRight.a <= 0.0f)
		{
			m_FireColorRight.a = 0;
			m_isFireDisappearRight = false;
		}
		m_pFireRightBack->SetColorPolygon(m_FireColorRight);
	}

	// 炎のアニメーション
	if (m_isFireLeft)
	{
		m_FireTextureCountLeft++;
		if (m_FireTextureCountLeft > FIRE_TEXTURE_SPEED)
		{
			m_FireTextureCountLeft = 0;
			m_FireTextureLeft += FIRE_ONE_WIDTH;
			if (m_FireTextureLeft > FIRE_TEXTURE_MAX * FIRE_ONE_WIDTH)
			{
				m_FireTextureLeft = FIRE_TEXTURE_MIN * FIRE_ONE_WIDTH;
			}
			m_pFireLeftBack->SetUV(0 + m_FireTextureLeft, 0 + m_FireTextureLeft + FIRE_ONE_WIDTH);
		}
	}
	if (m_isFireRight)
	{
		m_FireTextureCountRight++;
		if (m_FireTextureCountRight > FIRE_TEXTURE_SPEED)
		{
			m_FireTextureCountRight = 0;
			m_FireTextureRight += FIRE_ONE_WIDTH;
			if (m_FireTextureRight > FIRE_TEXTURE_MAX * FIRE_ONE_WIDTH)
			{
				m_FireTextureRight = FIRE_TEXTURE_MIN * FIRE_ONE_WIDTH;
			}
			m_pFireRightBack->SetUV(0 + m_FireTextureRight, 0 + m_FireTextureRight + FIRE_ONE_WIDTH);
		}
	}
}

//=============================================================================
// 白くする判定
//=============================================================================
void CHpBar::WhiteJudge(void)
{
	m_isChangeWhiteLeft = m_pBar[BAR_GREEN_L].m_Value < WHITE_CHANGE_VALUE;
	m_isChangeWhiteRight = m_pBar[BAR_GREEN_R].m_Value < WHITE_CHANGE_VALUE;

	// 白さを初期化するよ
	if (!m_isChangeWhiteLeft)
	{
		m_pBar[BAR_GREEN_L].m_p2D->InitWhite();
	}
	if (!m_isChangeWhiteRight)
	{
		m_pBar[BAR_GREEN_R].m_p2D->InitWhite();
	}
}

//=============================================================================
// 白くするよの更新
//=============================================================================
void CHpBar::UpdateWhite(void)
{
	// 白くするなら白くするよ
	if (m_isChangeWhiteLeft){
		if (m_isAddWhiteLeft){
			bool isEnd = m_pBar[BAR_GREEN_L].m_p2D->AddWhite(WHITE_SPEED);
			if (isEnd){
				m_isAddWhiteLeft = false;
			}
		}
		else{
			bool isEnd = m_pBar[BAR_GREEN_L].m_p2D->AddWhite(-WHITE_SPEED);
			if (isEnd){
				m_isAddWhiteLeft = true;
			}
		}
	}
	if (m_isChangeWhiteRight){
		if (m_isAddWhiteRight){
			bool isEnd = m_pBar[BAR_GREEN_R].m_p2D->AddWhite(WHITE_SPEED);
			if (isEnd){
				m_isAddWhiteRight = false;
			}
		}
		else{
			bool isEnd = m_pBar[BAR_GREEN_R].m_p2D->AddWhite(-WHITE_SPEED);
			if (isEnd){
				m_isAddWhiteRight = true;
			}
		}
	}
}

//=============================================================================
// 震わす更新
//=============================================================================
void CHpBar::UpdateShake(void)
{
	// 左側の震わす
	if (m_isShakeLeft)
	{
		m_ShakeCountLeft++;
		if (m_ShakeCountLeft > SHAKE_INTERVAL)
		{
			// フラグ切る
			m_isShakeLeft = false;
			// 元の座標に戻す
			m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonY(m_PosCenterY);
			m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonY(m_PosCenterY);
			m_pFrameLeft->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_pFrameLeftBar->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_FaceLeft.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
			JudgeExpressionLeft();
		}
		// 奇数偶数で座標をかえる
		if (m_ShakeCountLeft % 2 == 0)
			m_ShakePosYLeft = m_ShakeRangeLeft;
		else
			m_ShakePosYLeft = -m_ShakeRangeLeft;
		// バーの座標を動かす
		m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonY(m_PosCenterY + m_ShakePosYLeft);
		m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonY(m_PosCenterY + m_ShakePosYLeft);
		m_pFrameLeft->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYLeft);
		m_pFrameLeftBar->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYLeft);
		m_FaceLeft.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
		
		// 動かす範囲を減衰させる
		m_ShakeRangeLeft *= SHAKE_RANGE_RESIST;
	}
	// 右側の震わす
	if (m_isShakeRight)
	{
		m_ShakeCountRight++;
		if (m_ShakeCountRight > SHAKE_INTERVAL)
		{
			// フラグ切る
			m_isShakeRight = false;
			// 元の座標に戻す
			m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonY(m_PosCenterY);
			m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonY(m_PosCenterY);
			m_pFrameRight->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_pFrameRightBar->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_FaceRight.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
			// 表情を現在のHPに応じて変更
			JudgeExpressionRight();
		}
		// 奇数偶数で座標をかえる
		if (m_ShakeCountRight % 2 == 0)
			m_ShakePosYRight = m_ShakeRangeRight;
		else
			m_ShakePosYRight = -m_ShakeRangeRight;
		// バーの座標を動かす
		m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonY(m_PosCenterY + m_ShakePosYRight);
		m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonY(m_PosCenterY + m_ShakePosYRight);
		m_pFrameRight->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYRight);
		m_pFrameRightBar->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYRight);
		m_FaceRight.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYRight);
		// 動かす範囲を減衰させる
		m_ShakeRangeRight *= SHAKE_RANGE_RESIST;		
	}
}

//=============================================================================
// 描画
//=============================================================================
void CHpBar::DrawUI(void)
{
}

//=============================================================================
// 作成
//=============================================================================
CHpBar* CHpBar::Create(
	float height,
	float posCenterY,
	float posLeftBarLeftX,
	float posLeftBarRightX,
	float posRightBarLeftX,
	float posRightBarRightX,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CHpBar* p = new CHpBar(pDevice);
	p->Init(
		height,
		posCenterY,
		posLeftBarLeftX, posLeftBarRightX,
		posRightBarLeftX, posRightBarRightX);
	return p;
}

//=============================================================================
// 左サイドのHPを引数の値で加算
//=============================================================================
void CHpBar::AddLeft(float value)
{
	// valueが範囲外の時はクランプした値を利用
	if (m_pBar[BAR_GREEN_L].m_Value + value > m_ValueMax)
	{
		value = m_ValueMax - m_pBar[BAR_GREEN_L].m_Value;
	}

	// 補間で使う移動前と移動後の座標を保存
	m_pBar[BAR_GREEN_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_pBar[BAR_GREEN_L].m_Value;
	m_pBar[BAR_GREEN_L].m_Value += value;
	m_pBar[BAR_GREEN_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_pBar[BAR_GREEN_L].m_Value;

	// 一応クランプ
	m_pBar[BAR_GREEN_L].m_Value = min(m_pBar[BAR_GREEN_L].m_Value, m_ValueMax);

	// 補間するときのタイマを初期化
	m_pBar[BAR_GREEN_L].m_TimerEasing = 0;

	// 赤バー変更するフラグ
	m_isRedResetLeft = true;
	m_RedResetCountLeft = 0;

	// HPが増えた場合には即座に赤いバーをみどりと同じ幅にする
	m_RedResetCountLeft = RED_CHANGE_INTERVAL;

	// 表情変更
	JudgeExpressionLeft();

	// 白くするか判定
	WhiteJudge();
}

//=============================================================================
// 左サイドのHPを引数の値で減算
//=============================================================================
void CHpBar::SubLeft(float value)
{
	value *= -1;

	// valueが範囲外の時はクランプした値を利用
	if (m_pBar[BAR_GREEN_L].m_Value + value < 0)
	{
		value = -m_pBar[BAR_GREEN_L].m_Value;
	}

	// 補間で使う移動前と移動後の座標を保存
	m_pBar[BAR_GREEN_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_pBar[BAR_GREEN_L].m_Value;
	m_pBar[BAR_GREEN_L].m_Value += value;
	m_pBar[BAR_GREEN_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_pBar[BAR_GREEN_L].m_Value;

	// 一応クランプ
	m_pBar[BAR_GREEN_L].m_Value = max(m_pBar[BAR_GREEN_L].m_Value, 0);

	// 赤バー変更するフラグ
	m_isRedResetLeft = true;
	m_RedResetCountLeft = 0;

	// 補間するときのタイマを初期化
	m_pBar[BAR_GREEN_L].m_TimerEasing = 0;

	// 震わす
	ShakeLeft();

	// 白くするか判定
	WhiteJudge();
}

//=============================================================================
// 右サイドのHPを引数の値で加算
//=============================================================================
void CHpBar::AddRight(float value)
{
	// valueが範囲外の時はクランプした値を利用
	if (m_pBar[BAR_GREEN_R].m_Value + value > m_ValueMax)
	{
		value = m_ValueMax - m_pBar[BAR_GREEN_R].m_Value;
	}

	// 補間で使う移動前と移動後の座標を保存
	m_pBar[BAR_GREEN_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_pBar[BAR_GREEN_R].m_Value;
	m_pBar[BAR_GREEN_R].m_Value += value;
	m_pBar[BAR_GREEN_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_pBar[BAR_GREEN_R].m_Value;

	// 一応クランプ
	m_pBar[BAR_GREEN_R].m_Value = min(m_pBar[BAR_GREEN_R].m_Value, m_ValueMax);

	// 補間するときのタイマを初期化
	m_pBar[BAR_GREEN_R].m_TimerEasing = 0;

	// 赤バー変更するフラグ
	m_isRedResetRight = true;
	m_RedResetCountRight = 0;

	// HPが増えた場合には即座に赤いバーをみどりと同じ幅にする
	m_RedResetCountRight = RED_CHANGE_INTERVAL;
	
	// 表情変更
	JudgeExpressionRight();

	// 白くするか判定
	WhiteJudge();
}

//=============================================================================
// 右サイドのHPを引数の値で減算
//=============================================================================
void CHpBar::SubRight(float value)
{
	value *= -1;

	// valueが範囲外の時はクランプした値を利用
	if (m_pBar[BAR_GREEN_R].m_Value + value < 0)
	{
		value = -m_pBar[BAR_GREEN_R].m_Value;
	}
	// 補間で使う移動前と移動後の座標を保存
	m_pBar[BAR_GREEN_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_pBar[BAR_GREEN_R].m_Value;
	m_pBar[BAR_GREEN_R].m_Value += value;
	m_pBar[BAR_GREEN_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_pBar[BAR_GREEN_R].m_Value;

	// 一応クランプ
	m_pBar[BAR_GREEN_R].m_Value = max(m_pBar[BAR_GREEN_R].m_Value, 0);

	// 補間するときのタイマを初期化
	m_pBar[BAR_GREEN_R].m_TimerEasing = 0;

	// 赤バー変更するフラグ
	m_isRedResetRight = true;
	m_RedResetCountRight = 0;

	// 震わす
	ShakeRight();

	// 白くするか判定
	WhiteJudge();
}

//=============================================================================
// 開始アニメーションをする　引数↓
// 終了するまでのカウント(何フレームアニメーションするか)
//=============================================================================
void CHpBar::StartAnimation(int endCount)
{
#ifdef _DEBUG
	assert(endCount > 0 && "endCountはマイナスの値入れないで！");
#endif
	// アニメーションが終了するフレーム数
	m_AnimeCountMax = endCount;

	// アニメーションするための変数初期化
	m_AnimeCount = 0;
	m_isAnime = true;
	m_AnimeEasingOneFrame = 1.0f / static_cast<float>(endCount);
	m_AnimeOneFrameAlpha = 1.0f / endCount;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);

	// これいらなさそ
	m_pBar[BAR_GREEN_L].m_TimerEasing = 0;
	m_pBar[BAR_GREEN_R].m_TimerEasing = 0;
	// アニメーション用の保管タイマ
	m_AnimeTimerEasing = 0;

	m_pBar[BAR_GREEN_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosRight;
	m_pBar[BAR_GREEN_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_ValueMax;
	m_pBar[BAR_GREEN_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosLeft;
	m_pBar[BAR_GREEN_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_ValueMax;

	// バーをStart位置に動かす
	m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_GREEN_L].m_PosRight);
	m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_GREEN_R].m_PosLeft);
	m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_RED_L].m_PosRight);
	m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_RED_R].m_PosLeft);

	// じじい
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);
	m_Anime2DColorJijiiLeft = FACE_BACK_ALPHA_LEFT;
	m_Anime2DColorJijiiRight = FACE_BACK_ALPHA_RIGHT;
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pBack->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceRight.m_pBack->SetColorPolygon(m_Anime2DColorJijiiRight);

}

//=============================================================================
// 開始アニメーションをする更新
//=============================================================================
void CHpBar::UpdateAnime()
{
	// カウント
	m_AnimeCount++;
	
	// バーとかの位置
	if (m_AnimeTimerEasing < 1.0f){

		float leftX = EasingInterpolation(
			m_pBar[BAR_GREEN_L].m_PosEasingStart,
			m_pBar[BAR_GREEN_L].m_PosEasingEnd,
			m_AnimeTimerEasing);

		float rightX = EasingInterpolation(
			m_pBar[BAR_GREEN_R].m_PosEasingStart,
			m_pBar[BAR_GREEN_R].m_PosEasingEnd,
			m_AnimeTimerEasing);

		// 頂点動かす
		m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(leftX);
		m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(leftX);
		m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(rightX);
		m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(rightX);

		// 補間の時間更新 今回は左みどりのタイマだけ利用
		m_AnimeTimerEasing += m_AnimeEasingOneFrame;
	}

	// アルファ値更新
	m_Anime2DColor.a += m_AnimeOneFrameAlpha;
	m_Anime2DColorJijiiLeft.a += m_AnimeOneFrameAlpha;
	m_Anime2DColorJijiiRight.a += m_AnimeOneFrameAlpha;
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pBack->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceRight.m_pBack->SetColorPolygon(m_Anime2DColorJijiiRight);

	// 開始アニメーション終了
	if (m_AnimeCount > m_AnimeCountMax){
		// ここでアルファ値が1.0になるはずだけど一応！少数とか！
		m_Anime2DColor.a = 1.0f;
		m_Anime2DColorJijiiLeft.a = 1.0f;
		m_Anime2DColorJijiiLeft.a = 1.0f;
		m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
		m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
		m_FaceLeft.m_pBack->SetColorPolygon(m_Anime2DColorJijiiLeft);
		m_FaceRight.m_pBack->SetColorPolygon(m_Anime2DColorJijiiRight);
		m_isAnime = false;
	}


	// 開始アニメーション終了
	if (m_AnimeCount > m_AnimeCountMax){
		m_isAnime = false;
		Init();
	}
}

//=============================================================================
// 各値の初期化　開始アニメションの後で呼ぶ
//=============================================================================
void CHpBar::Init(){
	
	for (int i = 0; i < BAR_MAX; i++){
		m_pBar[i].m_Value = m_ValueMax;
		m_pBar[i].m_TimerEasing = 1;
	}
	// バーの値
	m_pBar[BAR_RED_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosLeft;
	m_pBar[BAR_RED_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosLeft;
	m_pBar[BAR_RED_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosRight;
	m_pBar[BAR_RED_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosRight;
}

//=============================================================================
// 左側のHPバーを震わす
// 引数：震わす時間
//=============================================================================
void CHpBar::ShakeLeft()
{
	// 震わす処理の初期化
	m_isShakeLeft = true;
	m_ShakeCountLeft = 0;
	m_ShakeRangeLeft = SHAKE_RANGE;

	// 攻撃されている表情にセット
	m_FaceLeft.m_Expression = EXPRESSION_ATTACKED;
	m_FaceLeft.SetUV();
}

//=============================================================================
// 右側のHPバーを震わす
// 引数：震わす時間
//=============================================================================
void CHpBar::ShakeRight()
{
	// 震わす処理の初期化
	m_isShakeRight = true;
	m_ShakeCountRight = 0;
	m_ShakeRangeRight = SHAKE_RANGE;
	
	// 攻撃されている表情にセット
	m_FaceRight.m_Expression = EXPRESSION_ATTACKED;
	m_FaceRight.SetUV();
}

//=============================================================================
// 現在のHPから表情を変更する。UVもセットする
//=============================================================================
void CHpBar::JudgeExpressionLeft()
{
	// 今の表情を算出
	for (int i = 0; i < EXPRESSION_MAX; i++)
	{
		if (HP_EXPRESSION[i] > m_pBar[BAR_GREEN_L].m_Value)
		{
			m_FaceLeft.m_Expression = static_cast<Expression>(i);
		}
	}
	// 表情をテクスチャにセット
	m_FaceLeft.SetUV();
}

//=============================================================================
// 現在のHPから表情を変更する。UVもセットする
//=============================================================================
void CHpBar::JudgeExpressionRight()
{
	// 今の表情を算出
	for (int i = 0; i < EXPRESSION_MAX; i++)
	{
		if (HP_EXPRESSION[i] > m_pBar[BAR_GREEN_R].m_Value)
		{
			m_FaceRight.m_Expression = static_cast<Expression>(i);
		}
	}
	// 表情をテクスチャにセット
	m_FaceRight.SetUV();
}

//=============================================================================
// 非表示
//=============================================================================
void CHpBar::SetImvisible(void)
{
	for (int i = 0; i < BAR_MAX; i++)
	{
		m_pBar[i].SetImvisible();
	}
	m_pFrameLeft->SetDrawFlag(false);
	m_pFrameLeftBar->SetDrawFlag(false);
	m_pFrameRight->SetDrawFlag(false);
	m_pFrameRightBar->SetDrawFlag(false);
	m_FaceLeft.SetImvisible();
	m_FaceRight.SetImvisible();
	m_pFireLeftBack->SetDrawFlag(false);
	m_pFireRightBack->SetDrawFlag(false);
}

//=============================================================================
// 表示
//=============================================================================
void CHpBar::SetVisible(void)
{
	for (int i = 0; i < BAR_MAX; i++)
	{
		m_pBar[i].SetVisible();
	}
	m_pFrameLeft->SetDrawFlag(true);
	m_pFrameLeftBar->SetDrawFlag(true);
	m_pFrameRight->SetDrawFlag(true);
	m_pFrameRightBar->SetDrawFlag(true);
	m_FaceLeft.SetVisible();
	m_FaceRight.SetVisible();
	m_pFireLeftBack->SetDrawFlag(true);
	m_pFireRightBack->SetDrawFlag(true);
}

//=============================================================================
// HPゲージの最小から最大の2D座標
//=============================================================================
float CHpBar::GetPosHpCenter(int playerNum)
{
	// 自分の値のまんなか
	if (playerNum == 0)
	{
		// はしっこ
		return m_pBar[BAR_GREEN_L].m_PosEasingEnd;
		// まんなか
		return (m_pBar[BAR_GREEN_L].m_PosRight - m_pBar[BAR_GREEN_L].m_PosEasingEnd) * 0.5f + m_pBar[BAR_GREEN_L].m_PosEasingEnd;
	}
	else
	{
		// はしっこ
		return m_pBar[BAR_GREEN_R].m_PosEasingEnd;
		// まんなか
		return (m_pBar[BAR_GREEN_R].m_PosEasingEnd - m_pBar[BAR_GREEN_R].m_PosLeft) * 0.5f + m_pBar[BAR_GREEN_R].m_PosLeft;
	}
}
//----EOF----