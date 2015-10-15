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

//*****************************************************************************
// 定数
//*****************************************************************************
// 顔のテクスチャ
const TEXTURE_TYPE FACE_TEXTURE = TEXTURE_PLAYER;

//=============================================================================
// コンストラクタ
//=============================================================================
CFace::CFace(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
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
	D3DXVECTOR2 posLeft,
	D3DXVECTOR2 sizeLeft,
	D3DXVECTOR2 posRight,
	D3DXVECTOR2 sizeRight)
{
	// 2D初期化
	m_FaceLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posLeft.x, posLeft.y, 0),
		sizeLeft.x, sizeLeft.y,
		FACE_TEXTURE);
	m_FaceRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posRight.x, posRight.y, 0),
		sizeRight.x, sizeRight.y,
		FACE_TEXTURE);

	m_BackLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posLeft.x, posLeft.y, 0),
		sizeLeft.x, sizeLeft.y,
		FACE_TEXTURE);
	m_BackRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posRight.x, posRight.y, 0),
		sizeRight.x, sizeRight.y,
		FACE_TEXTURE);

//	m_FaceLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
//	m_FaceRight->AddLinkList(CRenderer::TYPE_RENDER_UI);
//	m_BackLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
//	m_BackRight->AddLinkList(CRenderer::TYPE_RENDER_UI);
}

//=============================================================================
// 終了
//=============================================================================
void CFace::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CFace::Update(void)
{
}

//=============================================================================
// 描画
//=============================================================================
void CFace::DrawUI(void)
{

}

//=============================================================================
// 作成
//=============================================================================
CFace* CFace::Create(
	D3DXVECTOR2 posLeft,
	D3DXVECTOR2 sizeLeft,
	D3DXVECTOR2 posRight,
	D3DXVECTOR2 sizeRight,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CFace* p = new CFace(pDevice);
	p->Init(posLeft, sizeLeft, posRight, sizeRight);
	return p;
}

//----EOF----