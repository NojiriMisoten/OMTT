//=============================================================================
//
// CLogoクラス [CLogo.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CLogo.h"
#include "../../../SHADER/CShader.h"
#include "../../../MATH/math.h"
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CLogo::CLogo(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) :CScene2D(pDevice, objType)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CLogo ::~CLogo(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CLogo::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	CScene2D::Init(pos, width, height, texType);
	m_Alpha = 1.f;
	m_FadeTime = 0.f;
	m_isFade = false;
	m_FadeSpd = 0.f;
	m_GrayScal = D3DXVECTOR3(0.298912f, 0.586611f, 0.114478f) * 0.5f;
	m_isFadeIN = false;
	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CLogo::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CLogo::Update(void)
{
	UpdateFade();
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CLogo::DrawUIRender(void)
{
	// 自動描画ONなら
	if (!m_AutoDraw)
	{
		return;
	}

	PS_TYPE type = PS_TYPE_2D_GRAY;

	LPDIRECT3DPIXELSHADER9	*ps;
	LPD3DXCONSTANTTABLE		*psc;
	ps = CShader::GetPS(type);
	psc = CShader::GetPSC(type);
	(*m_pD3DDevice)->SetPixelShader(*ps);
	HRESULT hr = 0;
	UINT texSampler = (*psc)->GetSamplerIndex("texSampler");
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	(*psc)->SetFloatArray((*m_pD3DDevice), "gMonochromeScale", (float*)&m_GrayScal, 3);
	(*psc)->SetFloat((*m_pD3DDevice), "gAlpha", m_Alpha);

	// ポリゴンの描画
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(texSampler, m_pD3DTexBuff);							// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// プリミティブの種類
									, 0											// 描画を開始する頂点番号
									, DRAW_SQUARE_PRINITIV_NUM);				// 書きたいポリゴン数

	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CLogo* CLogo::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	// 作成
	CLogo* p = new CLogo(pDevice);

	// 初期化
	p->Init(pos, width, height, texType);

	return p;
}

//*****************************************************************************
// フェードインスタート関数
//*****************************************************************************
void CLogo::StartFadeIN(int maxFadeInFrame)
{
	m_isFade = true;
	m_FadeSpd = 1.f / (float)maxFadeInFrame;
	m_FadeTime = 0.f;
	m_Alpha = 0.f;
	m_isFadeIN = true;
}

//*****************************************************************************
// フェードアウトスタート関数
//*****************************************************************************
void CLogo::StartFadeOUT(int maxFadeInFrame)
{
	m_isFade = true;
	m_FadeSpd = 1.f / (float)maxFadeInFrame;
	m_FadeTime = 0.f;
	m_Alpha = 1.f;
	m_isFadeIN = false;
}

//*****************************************************************************
// フェード更新関数
//*****************************************************************************
void CLogo::UpdateFade(void)
{
	if (!m_isFade)
	{
		return;
	}

	if (m_isFadeIN)
	{
		m_Alpha = EasingInterpolation(0.f, 1.f, m_FadeTime);
	}
	else
	{
		m_Alpha = EasingInterpolation(1.f, 0.f, m_FadeTime);
	}
	m_FadeTime += m_FadeSpd;
	if (m_FadeTime > 1.f)
	{
		m_isFade = false;
	}
}
//----EOF----