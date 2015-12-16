//=============================================================================
//
// CForcusLineクラス [CForcusLine.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CForcusLine.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// テクスチャアニメ―ションのスピード
static const int ANIME_SPEED = 2;
// テクスチャの一コマのUV値
static const float ANIME_ONE_WIDTH = 1.0f / 4.0f;
// フェードのスピード
static const float FADE_SPEED = 0.2f;

//=============================================================================
// コンストラクタ
//=============================================================================
CForcusLine::CForcusLine(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;

	m_pPolygon = NULL;
	m_pBack = NULL;
	m_isIn = false;
	m_isOut = false;
	m_isWait = false;
	m_IntervalMax = 0;
	m_IntervalCount = 0;
	m_Color = D3DXCOLOR(1, 1, 1, 0);
	m_ColorBack = D3DXCOLOR(1, 1, 1, 0);
	m_BackUV = UV_INDEX(0, 1, 0, 1);
	m_TextureCount = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CForcusLine::~CForcusLine(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CForcusLine::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_FORCUS_LINE_BACK);
	m_pBack->SetColorPolygon(m_ColorBack);
	m_pBack->AddLinkList(CRenderer::TYPE_RENDER_UI);

	m_pPolygon = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_FORCUS_LINE);
	m_pPolygon->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pPolygon->SetColorPolygon(m_Color);
	m_BackUV.left = 0.0f;
	m_BackUV.right = ANIME_ONE_WIDTH;
	m_pPolygon->SetUV(&m_BackUV);
}

//=============================================================================
// 終了
//=============================================================================
void CForcusLine::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CForcusLine::Update(void)
{
	if (m_isIn)		In();
	if (m_isWait)	Wait();
	if (m_isOut)	Out();
}

//=============================================================================
// 描画
//=============================================================================
void CForcusLine::DrawUI(void)
{
}

//=============================================================================
// 作成
//=============================================================================
CForcusLine* CForcusLine::Create(
	LPDIRECT3DDEVICE9 *pDevice)
{
	CForcusLine* p = new CForcusLine(pDevice);
	p->Init(pDevice);
	return p;
}

//=============================================================================
// カットインスタート
//=============================================================================
void CForcusLine::Start(int interval, bool isFinish)
{
	m_isIn = true;
	m_isOut = false;
	m_isWait = false;
	m_IntervalCount = 0;

	// カットインを表示する長さ
	m_IntervalMax = interval;

	m_Color.a = 0.0f;
	m_pPolygon->SetColorPolygon(m_Color);
	m_ColorBack.a = 0.0f;
	m_pBack->SetColorPolygon(m_ColorBack);

	if (!isFinish)
		m_pPolygon->ChangeTexture(TEXTURE_FORCUS_LINE);
	else
		m_pPolygon->ChangeTexture(TEXTURE_FORCUS_LINE_RED);
}

//=============================================================================
// 画面内に入ってくる更新
//=============================================================================
void CForcusLine::In()
{
	m_ColorBack.a += FADE_SPEED;
	m_Color.a += FADE_SPEED;
	if (m_Color.a > 1.0f){
		m_isIn = false;
		m_isWait = true;
		m_Color.a = 1.0f;
		m_ColorBack.a = 1.0f;
	}
	m_pPolygon->SetColorPolygon(m_Color);
	m_pBack->SetColorPolygon(m_ColorBack);
}

//=============================================================================
// カットインして表示している状態
//=============================================================================
void CForcusLine::Wait()
{
	// カットインをアウトするまでのカウント
	m_IntervalCount++;
	if (m_IntervalCount > m_IntervalMax)
	{
		m_isWait = false;
		m_isOut = true;
	}

	// アニメーション
	m_TextureCount++;
	if (m_TextureCount > ANIME_SPEED){
		m_TextureCount = 0;

		m_BackUV.left += ANIME_ONE_WIDTH;
		m_BackUV.right += ANIME_ONE_WIDTH;
		if (m_BackUV.left >= 1.0f){
			m_BackUV.left = 0.0f;
			m_BackUV.right = ANIME_ONE_WIDTH;
		}
		m_pPolygon->SetUV(&m_BackUV);
	}
}

//=============================================================================
// 画面外に出ていく更新
//=============================================================================
void CForcusLine::Out()
{
	m_Color.a -= FADE_SPEED;
	m_ColorBack.a -= FADE_SPEED;
	if (m_Color.a < 0.0f){
		m_isOut = false;
		m_Color.a = 0.0f;
		m_ColorBack.a = 0.0f;
	}
	m_pPolygon->SetColorPolygon(m_Color);
	m_pBack->SetColorPolygon(m_ColorBack);
}

//----EOF----