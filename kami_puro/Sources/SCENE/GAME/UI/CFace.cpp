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
// 顔のテクスチャ
static const TEXTURE_TYPE FACE_TEXTURE = TEXTURE_PLAYER;

//=============================================================================
// コンストラクタ
//=============================================================================
CFace::CFace(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;
	m_pFaceLeft = NULL;
	m_pBackLeft = NULL;
	m_pFaceRight = NULL;
	m_pBackRight = NULL;
	m_PosLeft = D3DXVECTOR2(0, 0);
	m_PosRight = D3DXVECTOR2(0, 0);
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
	D3DXVECTOR2 &sizeLeft,
	D3DXVECTOR2 &posRight,
	D3DXVECTOR2 &sizeRight)
{
	// 2D初期化
	m_pFaceLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posLeft.x, posLeft.y, 0),
		sizeLeft.x, sizeLeft.y,
		FACE_TEXTURE);
	m_pFaceRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posRight.x, posRight.y, 0),
		sizeRight.x, sizeRight.y,
		FACE_TEXTURE);

	m_pBackLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posLeft.x, posLeft.y, 0),
		sizeLeft.x, sizeLeft.y,
		FACE_TEXTURE);
	m_pBackRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posRight.x, posRight.y, 0),
		sizeRight.x, sizeRight.y,
		FACE_TEXTURE);

	m_pFaceLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFaceRight->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBackLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBackRight->AddLinkList(CRenderer::TYPE_RENDER_UI);
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
	D3DXVECTOR2 &sizeLeft,
	D3DXVECTOR2 &posRight,
	D3DXVECTOR2 &sizeRight,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CFace* p = new CFace(pDevice);
	p->Init(posLeft, sizeLeft, posRight, sizeRight);
	return p;
}

//----EOF----