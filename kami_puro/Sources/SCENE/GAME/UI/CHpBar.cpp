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
// バーの赤くなる部分の色
//static const D3DXCOLOR BAR_COLOR_GREEN = D3DXCOLOR(0.0f, 0.8f, 0.4f, 1.0f);
//static const D3DXCOLOR BAR_COLOR_RED = D3DXCOLOR(1, 0.4f, 0.1f, 1.0f);
// TODO 仮のHP量　プレイヤから持ってくる
static const float HP_MAX = 255;
// 赤いバーを変更するまでのカウント数
static const short RED_CHANGE_INTERVAL = 40;
// ダメージを受けた分バーを減らすまでのフレーム数(バーの速度)
static const float ERASE_INTERVAL = 20.0f;
// ダメージ分減らすときの1フレーム当たりの量 補間のtimeの計算で使う
static const float ERASE_ONE_FRAME = 1.0f / ERASE_INTERVAL;

// 枠のおおきさ
static const float BAR_FRAME_WIDTH = 800 * 0.8f;
static const float BAR_FRAME_HEIGHT = 200 * 0.8f;

// バーの座標と枠の座標のoffset
static const D3DXVECTOR2 BAR_FRAME_OFFSET = D3DXVECTOR2(30, 38);

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

	Init();

	// バーの幅
	float barWidth = posLeftBarRightX - posLeftBarLeftX;

	// 値（m_Value）１当たりのピクセル数(float)を計算
	m_WidthOneValue = barWidth / m_ValueMax;

	// バーの座標
	D3DXVECTOR3 pos[BAR_MAX] = {
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f,  posCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, posCenterY, 0),
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f , posCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, posCenterY, 0),
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

	// 色チェンジ
//	m_pBar[BAR_RED_R].m_p2D->SetColorPolygon(BAR_COLOR_RED);
//	m_pBar[BAR_RED_L].m_p2D->SetColorPolygon(BAR_COLOR_RED);
//	m_pBar[BAR_GREEN_L].m_p2D->SetColorPolygon(BAR_COLOR_GREEN);
//	m_pBar[BAR_GREEN_R].m_p2D->SetColorPolygon(BAR_COLOR_GREEN);

	// 最初はUI開始アニメーションをするから、ポリゴンをセットしたい
	// バーをStart位置に動かす
	m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_GREEN_L].m_PosRight);
	m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_GREEN_R].m_PosLeft);
	m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_RED_L].m_PosRight);
	m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_RED_R].m_PosLeft);

	// HPの枠左
	m_pFrameLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[0].x - BAR_FRAME_OFFSET.x, pos[0].y - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME);
	m_pFrameLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// HPの枠右
	m_pFrameRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[1].x + BAR_FRAME_OFFSET.x, pos[1].y - BAR_FRAME_OFFSET.y, 0),
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
	// 開始アニメーションの更新
	if (m_isAnime)
	{
		UpdateAnime();
		return;
	}

	CDebugProc::PrintL("左体力 %d / %d\n", (int)m_pBar[BAR_GREEN_L].m_Value, (int)m_ValueMax);
	CDebugProc::PrintL("右体力 %d / %d\n", (int)m_pBar[BAR_GREEN_R].m_Value, (int)m_ValueMax);

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
		m_pBar[BAR_GREEN_L].m_TimerEasing += ERASE_ONE_FRAME;
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
		m_pBar[BAR_GREEN_R].m_TimerEasing += ERASE_ONE_FRAME;
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
			m_pBar[BAR_RED_L].m_TimerEasing += ERASE_ONE_FRAME;
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
			m_pBar[BAR_RED_R].m_TimerEasing += ERASE_ONE_FRAME;
		}
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
}

//=============================================================================
// 開始アニメーションをする更新
//=============================================================================
void CHpBar::UpdateAnime()
{
	// カウント
	m_AnimeCount++;
	
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
//----EOF----