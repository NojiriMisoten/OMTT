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

//*****************************************************************************
// 定数
//*****************************************************************************
// 二つの数字の幅（中心座標からのオフセット）
const float FIGURE_SPACE_WIDTH = 30;
// 数字の大きさ
const float FIGURE_WIDTH = 30;
const float FIGURE_HEIGHT = 50;
// 数字のテクスチャ
const TEXTURE_TYPE FIGURE_TEXTURE = TEXTURE_NUMBER;
// タイムの最大数
const int TIME_MAX = 99;
// 一秒のカウント
const short SECOND_FRAME = 60;

//=============================================================================
// コンストラクタ
//=============================================================================
CCountTime::CCountTime(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{

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
void CCountTime::Init(D3DXVECTOR2 pos, int time)
{
	
	m_Time = time;

	m_Time = max(m_Time, TIME_MAX);

	// カウント初期化
	m_TimeCount = 0;

	// 二つの数字を生成
	m_Figure1st = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x - FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		FIGURE_TEXTURE);

	m_Figure2nd = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x + FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		FIGURE_TEXTURE);

	m_Figure1st->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_Figure2nd->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// ポリゴンのテクスチャ変更
	Set(m_Time);
}

//=============================================================================
// 終了
//=============================================================================
void CCountTime::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CCountTime::Update(void)
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
// 描画
//=============================================================================
void CCountTime::DrawUI(void)
{
	CScene2D::DrawUI();
}

//=============================================================================
// 作成
//=============================================================================
CCountTime* CCountTime::Create(
	D3DXVECTOR2 pos, int time, LPDIRECT3DDEVICE9 *pDevice)
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
	m_Figure1st->SetUV(&uv1);
	m_Figure2nd->SetUV(&uv2);
}
//----EOF----