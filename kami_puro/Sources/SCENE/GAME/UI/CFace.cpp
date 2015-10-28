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
	
	m_FaceLeft.m_Expression = EXPRESSION_GOOD;
	m_FaceLeft.m_pBack2D = NULL;
	m_FaceLeft.m_pFace2D = NULL;
	m_FaceLeft.m_Pos = D3DXVECTOR2(0, 0);

	m_FaceRight.m_Expression = EXPRESSION_GOOD;
	m_FaceRight.m_pBack2D = NULL;
	m_FaceRight.m_pFace2D = NULL;
	m_FaceRight.m_Pos = D3DXVECTOR2(0, 0);

	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeOneFrameAlpha = 0;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);	// �ŏ��̃A�j���[�V�����œ�������n�܂邽��
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
	D3DXVECTOR2 &posRight,
	D3DXVECTOR2 &size)
{
	// ��������
	m_FaceLeft.m_pBack2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posLeft.x, posLeft.y, 0),
		size.x, size.y, FACE_TEXTURE);
	m_FaceLeft.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posLeft.x, posLeft.y, 0),
		size.x, size.y, FACE_TEXTURE);
	// �E������
	m_FaceRight.m_pBack2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posRight.x, posRight.y, 0),
		size.x, size.y, FACE_TEXTURE);
	m_FaceRight.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(posRight.x, posRight.y, 0),
		size.x, size.y, FACE_TEXTURE);
	// �����_�\�ǉ�
	m_FaceLeft.m_pBack2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceLeft.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pBack2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// �ŏ���UI�A�j���[�V�����p��2D�𓧉߂�����
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
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
	return;
	// �J�n�A�j���[�V���������[
	if (!m_isAnime)	return;

	// �J�E���g
	m_AnimeCount++;
	// �A���t�@�l�X�V
	m_Anime2DColor.a += m_AnimeOneFrameAlpha;
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	// �J�n�A�j���[�V�����I��
	if (m_AnimeCount > m_AnimeCountMax){
		// �����ŃA���t�@�l��1.0�ɂȂ�͂������ǈꉞ�I�����Ƃ��I
		m_Anime2DColor.a = 1.0f;
		m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColor);
		m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
		m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColor);
		m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
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
	D3DXVECTOR2 &posRight,
	D3DXVECTOR2 &size,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CFace* p = new CFace(pDevice);
	p->Init(posLeft, posRight, size);
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
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
}

//----EOF----