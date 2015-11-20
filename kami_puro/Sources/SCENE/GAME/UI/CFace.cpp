//=============================================================================
//
// CFaceクラス [CFace.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CFace.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// じじいのテクスチャの一コマのサイズ
const float CFace::FaceBace::JIJII_TEX_U = 1.0f / 3.0f;
const float CFace::FaceBace::JIJII_TEX_V = 1.0f / 2.0f;


//=============================================================================
// コンストラクタ
//=============================================================================
CFace::CFace(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;
	
	m_FaceLeft.m_Expression = EXPRESSION_GOOD;
	m_FaceLeft.m_pBack2D = NULL;
	m_FaceLeft.m_pFace2D = NULL;
	m_FaceLeft.m_Pos = D3DXVECTOR2(0, 0);
	m_FaceLeft.m_UV = UV_INDEX(0, FaceBace::JIJII_TEX_U, 0, FaceBace::JIJII_TEX_V);

	m_FaceRight.m_Expression = EXPRESSION_GOOD;
	m_FaceRight.m_pBack2D = NULL;
	m_FaceRight.m_pFace2D = NULL;
	m_FaceRight.m_Pos = D3DXVECTOR2(0, 0);
	m_FaceRight.m_UV = UV_INDEX(0, FaceBace::JIJII_TEX_U, FaceBace::JIJII_TEX_V, 1);

	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeOneFrameAlpha = 0;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 1);	// 最初のアニメーションで透明から始まるため
}

//=============================================================================
// デストラクタ
//=============================================================================
CFace::~CFace(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CFace::Init(
	D3DXVECTOR2 &posLeft,
	D3DXVECTOR2 &posRight,
	D3DXVECTOR2 &size)
{
	// 左初期化
	m_FaceLeft.m_pBack2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posLeft.x, posLeft.y, 0),
		size.x, size.y, TEXTURE_HP_GAGE_G);
	m_FaceLeft.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posLeft.x, posLeft.y, 0),
		size.x, size.y, TEXTURE_JIJII);
	// 右初期化
	m_FaceRight.m_pBack2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posRight.x, posRight.y, 0),
		size.x, size.y, TEXTURE_HP_GAGE_G);
	m_FaceRight.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posRight.x, posRight.y, 0),
		size.x, size.y, TEXTURE_JIJII);
	// レンダ―追加
	m_FaceLeft.m_pBack2D->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_FaceLeft.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_FaceRight.m_pBack2D->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_FaceRight.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// 最初のUIアニメーション用に2Dを透過させる
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);

	// テクスチャのセット
	// 最初はVの値をセットするから4つの引数の方
	m_FaceLeft.m_pFace2D->SetUV(&(m_FaceLeft.m_UV));
	m_FaceRight.m_pFace2D->SetUV(&(m_FaceRight.m_UV));
}

//=============================================================================
// 終了
//=============================================================================
void CFace::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CFace::Update(void)
{
	// 開始アニメーションの更新
	UpdateAnime();
}

//=============================================================================
// 開始アニメーションをする更新
//=============================================================================
void CFace::UpdateAnime()
{
	// 開始アニメーションするよー
	if (!m_isAnime)	return;

	// カウント
	m_AnimeCount++;
	// アルファ値更新
	m_Anime2DColor.a += m_AnimeOneFrameAlpha;
//	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
//	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	// 開始アニメーション終了
	if (m_AnimeCount > m_AnimeCountMax){
		// ここでアルファ値が1.0になるはずだけど一応！少数とか！
		m_Anime2DColor.a = 1.0f;
//		m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColor);
		m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
//		m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColor);
		m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
		m_isAnime = false;
	}
}

//=============================================================================
// 描画
//=============================================================================
void CFace::Draw(void)
{

}

//=============================================================================
// 作成
//=============================================================================
CFace* CFace::Create(
	D3DXVECTOR2 &posLeft,
	D3DXVECTOR2 &posRight,
	D3DXVECTOR2 &size,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CFace* p = new CFace(pDevice);
	p->Init(posLeft, posRight, size);
	return p;
}

//=============================================================================
// 開始アニメーションをする　引数↓
// 終了するまでのカウント(何フレームアニメーションするか)
//=============================================================================
void CFace::StartAnimation(int endCount)
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
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
}

//----EOF----