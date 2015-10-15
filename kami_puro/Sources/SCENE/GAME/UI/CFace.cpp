//=============================================================================
//
// CFace�N���X [CFace.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CFace.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// ��̃e�N�X�`��
const TEXTURE_TYPE FACE_TEXTURE = TEXTURE_PLAYER;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFace::CFace(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFace::~CFace(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CFace::Init(
	D3DXVECTOR2 posLeft,
	D3DXVECTOR2 sizeLeft,
	D3DXVECTOR2 posRight,
	D3DXVECTOR2 sizeRight)
{
	// 2D������
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
// �I��
//=============================================================================
void CFace::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CFace::Update(void)
{
}

//=============================================================================
// �`��
//=============================================================================
void CFace::DrawUI(void)
{

}

//=============================================================================
// �쐬
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