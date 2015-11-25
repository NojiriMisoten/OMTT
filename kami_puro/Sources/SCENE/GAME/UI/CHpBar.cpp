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

//*****************************************************************************
// 定数
//*****************************************************************************
const float CHpBar::JIJII_TEX_U = 1.0f / 3.0f;
const float CHpBar::JIJII_TEX_V = 1.0f / 2.0f;

// TODO 仮のHP量　プレイヤから持ってくるかゲームからセットして
static const float HP_MAX = 100;
// HPの割合によって表情がかわる　その値
static const float HP_EXPRESSION[CHpBar::EXPRESSION_MAX] = {
	100,
	60,
	30,
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
static const float BAR_FRAME_WIDTH = 800 * 0.8f;
static const float BAR_FRAME_HEIGHT = 200 * 0.8f;
// バーの座標に対しての枠の座標のoffset
static const D3DXVECTOR2 BAR_FRAME_OFFSET = D3DXVECTOR2(49, 38);
// バーの座標に対してのじじいの座標のoffset バーの左右の端から相対参照
static const D3DXVECTOR2 FACE_OFFSET = D3DXVECTOR2(37, -44);
// じじいのおおきさ
static const float FACE_WIDTH = 120;
static const float FACE_HEIGHT = 150;
// じじいの背景おおきさ
static const float FACE_BACK_WIDTH = 160;
static const float FACE_BACK_HEIGHT = 160;
// じじいの背景の色の透過する最初の色
static const D3DXCOLOR FACE_BACK_ALPHA_LEFT = D3DXCOLOR(1.0f, 0.1f, 0.0f, 0.0f);
static const D3DXCOLOR FACE_BACK_ALPHA_RIGHT = D3DXCOLOR(0.0f, 0.1f, 1.0f, 0.0f);

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
	m_pFrameLeftTop = NULL;
	m_pFrameRightTop = NULL;

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
	m_FaceLeft.m_pBack2D = NULL;
	m_FaceLeft.m_pFace2D = NULL;
	m_FaceLeft.m_Pos = D3DXVECTOR2(0, 0);
	m_FaceLeft.m_UV = UV_INDEX(0, JIJII_TEX_U, 0, JIJII_TEX_V);
	m_FaceRight.m_Expression = EXPRESSION_GOOD;
	m_FaceRight.m_pBack2D = NULL;
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
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f,  m_PosCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f , m_PosCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
	};

	for (int i = 0; i < BAR_MAX; i++)
	{
		// バーの2Dの作成
		m_pBar[i].m_p2D = CScene2D::Create(m_pD3DDevice,
			pos[i],
			barWidth, height,
			i < 2 ? TEXTURE_HP_GAGE_R : TEXTURE_HP_GAGE_G);
		// バーの2D初期化
		m_pBar[i].m_p2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
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
	// 顔の背景
	m_FaceLeft.m_pBack2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		FACE_BACK_WIDTH, FACE_BACK_HEIGHT, TEXTURE_JIJII_BACK);
	m_FaceRight.m_pBack2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		FACE_BACK_WIDTH, FACE_BACK_HEIGHT, TEXTURE_JIJII_BACK);
	// レンダ―追加
	m_FaceLeft.m_pBack2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pBack2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// HPの枠左 上半分
	m_pFrameLeftTop = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[0].x - BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_TOP);
	m_pFrameLeftTop->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// HPの枠右 上半分
	m_pFrameRightTop = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[1].x + BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_TOP);
	m_pFrameRightTop->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// 反転
	m_pFrameRightTop->SetUVMirror();

	// 顔生成
	m_FaceLeft.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		FACE_WIDTH, FACE_HEIGHT, TEXTURE_JIJII);
	// 右初期化
	m_FaceRight.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		FACE_WIDTH, FACE_HEIGHT, TEXTURE_JIJII);
	m_FaceLeft.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// 最初はVの値をセットするから4つの引数の方
	m_FaceLeft.m_pFace2D->SetUV(&(m_FaceLeft.m_UV));
	m_FaceRight.m_pFace2D->SetUV(&(m_FaceRight.m_UV));
	// 最初は見えないから透明化
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiRight);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);

	// HPの枠左
	m_pFrameLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[0].x - BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME);
	m_pFrameLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// HPの枠右
	m_pFrameRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[1].x + BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME);
	m_pFrameRight->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// 反転
	m_pFrameRight->SetUVMirror();

}

//=============================================================================
// 終了
//=============================================================================
void CHpBar::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CHpBar::Update(void)
{
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
			m_pFrameLeftTop->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_FaceLeft.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
			m_FaceLeft.m_pBack2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
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
		m_pFrameLeftTop->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYLeft);
		m_FaceLeft.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
		m_FaceLeft.m_pBack2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
		
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
			m_pFrameRightTop->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_FaceRight.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
			m_FaceRight.m_pBack2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
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
		m_pFrameRightTop->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYRight);
		m_FaceRight.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
		m_FaceRight.m_pBack2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
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

	// 表情変更
	JudgeExpressionLeft();
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

	// 表情変更
	JudgeExpressionRight();
}

//=============================================================================
// 開始アニメーションをする　引数↓
// 終了するまでのカウント(何フレームアニメーションするか)
//=============================================================================
void CHpBar::StartAnimation(int endCount)
{
	assert(endCount > 0 && "endCountはマイナスの値入れないで！");

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
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiRight);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);

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
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiRight);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	// 開始アニメーション終了
	if (m_AnimeCount > m_AnimeCountMax){
		// ここでアルファ値が1.0になるはずだけど一応！少数とか！
		m_Anime2DColor.a = 1.0f;
		m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiLeft);
		m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
		m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiRight);
		m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
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
	m_isShakeLeft = true;
	m_ShakeCountLeft = 0;
	m_ShakeRangeLeft = SHAKE_RANGE;
}

//=============================================================================
// 右側のHPバーを震わす
// 引数：震わす時間
//=============================================================================
void CHpBar::ShakeRight()
{
	m_isShakeRight = true;
	m_ShakeCountRight = 0;
	m_ShakeRangeRight = SHAKE_RANGE;
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

//----EOF----