//=============================================================================
//
// COverLayクラス [COverLay.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "COverLay.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const float WIDTH = SCREEN_WIDTH;
static const float HEIGHT = SCREEN_HEIGHT / 720.f * 300.f;
static const D3DXVECTOR3 POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);

//=============================================================================
// コンストラクタ
//=============================================================================
COverLay::COverLay(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;

	m_p2D = NULL;
	m_isIn = false;
	m_isOut = false;
	m_isWait = false;
	m_IntervalMax = 0;
	m_IntervalCount = 0;
	m_Color = D3DXCOLOR(1, 1, 1, 1);
}

//=============================================================================
// デストラクタ
//=============================================================================
COverLay::~COverLay(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void COverLay::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	// 2D初期化
	m_p2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(POS),
		WIDTH, HEIGHT,
		TEXTURE_BLUE);
	m_p2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// フェードアウトの白さ初期化
	m_Color = D3DXCOLOR(1, 1, 1, 0);
	m_p2D->SetColorPolygon(m_Color);
}

//=============================================================================
// 終了
//=============================================================================
void COverLay::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void COverLay::Update(void)
{
	if (m_isIn)		In();
	if (m_isWait)	Wait();
	if (m_isOut)	Out();
}

//=============================================================================
// 描画
//=============================================================================
void COverLay::DrawUI(void)
{
}

//=============================================================================
// 作成
//=============================================================================
COverLay* COverLay::Create(
	LPDIRECT3DDEVICE9 *pDevice)
{
	COverLay* p = new COverLay(pDevice);
	p->Init(pDevice);
	return p;
}

//=============================================================================
// カットインスタート
//=============================================================================
void COverLay::Start(Data *data)
{
	m_isIn = true;
	m_isOut = false;
	m_isWait = false;
	m_IntervalCount = 0;
	m_IntervalMax = data->interval;
	m_FadeInSpeed = data->fadeInSpeed;
	m_FadeOutSpeed = data->fadeOutSpeed;

	// テクスチャ
	m_p2D->ChangeTexture(data->texture);

	// フェードアウトの白さ初期化
	m_Color = D3DXCOLOR(1, 1, 1, 0);
	m_p2D->SetColorPolygon(m_Color);
}

//=============================================================================
// 画面内に入ってくる更新
//=============================================================================
void COverLay::In()
{
	m_Color.a += m_FadeInSpeed;
	if (m_Color.a <= 1.0f)
	{
		m_p2D->SetColorPolygon(m_Color);
	}
	else
	{
		m_Color.a = 1.0f;
		m_p2D->SetColorPolygon(m_Color);
		m_isIn = false;
		m_isWait = true;
		m_IntervalCount = 0;
	}
}

//=============================================================================
// カットインして表示している状態
//=============================================================================
void COverLay::Wait()
{
	// カットインをアウトするまでのカウント
	m_IntervalCount++;
	if (m_IntervalCount > m_IntervalMax)
	{
		m_isWait = false;
		m_isOut = true;
	}
}

//=============================================================================
// 画面外に出ていく更新
//=============================================================================
void COverLay::Out()
{
	m_Color.a -= m_FadeOutSpeed;
	if (m_Color.a >= 0.0f)
	{
		m_p2D->SetColorPolygon(m_Color);
	}
	else
	{
		m_Color.a = 0.0f;
		m_p2D->SetColorPolygon(m_Color);
		m_isOut = false;
	}
}
//----EOF----