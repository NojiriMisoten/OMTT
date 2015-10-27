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

	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeOneFrameAlpha = 0;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 1);
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
	// �J�n�A�j���[�V�����̍X�V
	UpdateAnime();
}

//=============================================================================
// �J�n�A�j���[�V����������X�V
//=============================================================================
void CFace::UpdateAnime()
{
	// �J�n�A�j���[�V���������[
	if (!m_isAnime)	return;

	// �J�E���g
	m_AnimeCount++;
	// �A���t�@�l�X�V
	m_Anime2DColor.a += m_AnimeOneFrameAlpha;
	m_pFaceLeft->SetColorPolygon(m_Anime2DColor);
	m_pBackLeft->SetColorPolygon(m_Anime2DColor);
	m_pFaceRight->SetColorPolygon(m_Anime2DColor);
	m_pBackRight->SetColorPolygon(m_Anime2DColor);
	// �J�n�A�j���[�V�����I��
	if (m_AnimeCount > m_AnimeCountMax){
		// �����ŃA���t�@�l��1.0�ɂȂ�͂������ǈꉞ�I�����Ƃ��I
		m_Anime2DColor.a = 1.0f;
		m_pFaceLeft->SetColorPolygon(m_Anime2DColor);
		m_pBackLeft->SetColorPolygon(m_Anime2DColor);
		m_pFaceRight->SetColorPolygon(m_Anime2DColor);
		m_pBackRight->SetColorPolygon(m_Anime2DColor);
		m_isAnime = false;
	}
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

//=============================================================================
// �J�n�A�j���[�V����������@������
// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
//=============================================================================
void CFace::StartAnimation(int endCount)
{
	assert(endCount > 0 && "endCount�̓}�C�i�X�̒l����Ȃ��ŁI");

	// �A�j���[�V�������I������t���[����
	m_AnimeCountMax = endCount;

	// �A�j���[�V�������邽�߂̕ϐ�������
	m_AnimeCount = 0;
	m_isAnime = true;
	m_AnimeOneFrameAlpha = 1.0f / endCount;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);

	// 2D�𓧉߂�����
	m_pFaceLeft->SetColorPolygon(m_Anime2DColor);
	m_pBackLeft->SetColorPolygon(m_Anime2DColor);
	m_pFaceRight->SetColorPolygon(m_Anime2DColor);
	m_pBackRight->SetColorPolygon(m_Anime2DColor);
}

//----EOF----