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
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// ��̃e�N�X�`��
static const TEXTURE_TYPE FACE_TEXTURE = TEXTURE_PLAYER;

//=============================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=============================================================================
CFace::~CFace(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CFace::Init(
	D3DXVECTOR2 &posLeft,
	D3DXVECTOR2 &sizeLeft,
	D3DXVECTOR2 &posRight,
	D3DXVECTOR2 &sizeRight)
{
	// 2D������
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
// �I��
//=============================================================================
void CFace::Uninit(void)
{
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
void CFace::Draw(void)
{

}

//=============================================================================
// �쐬
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