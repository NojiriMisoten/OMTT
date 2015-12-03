//=============================================================================
//
// CRopeTimer�N���X [CRopeTimer.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CRopeTimer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
static const float BAR_WIDTH = 300;
static const float BAR_HEIGHT = 20;
static const float BAR_FRAME_WIDTH = 350;
static const float BAR_FRAME_HEIGHT = 35;
static const D3DXVECTOR3 BAR_POS = D3DXVECTOR3(SCREEN_WIDTH*0.5f, 360, 0);

static const float BAR_POS_LEFT = BAR_POS.x - BAR_WIDTH * 0.5f;
static const float BAR_POS_RIGHT = BAR_POS.x + BAR_WIDTH * 0.5f;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRopeTimer::CRopeTimer(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;
	m_pBar = NULL;
	m_pFrame = NULL;
	m_isAppearAnime = false;
	m_isMoveBar = false;
	m_PosLeft = 0;
	m_PosRight = 0;
	m_AnimeBarSize = AnimeData(0, 0);
	m_AnimeBarSizeDest = AnimeData(0, 0);
	m_AnimeFrameSize = AnimeData(0, 0);
	m_AnimeFrameSizeDest = AnimeData(0, 0);
	m_Time = 0;
	m_Count = 0;
	m_CountMax = 0;
	m_WidthOneFrame = 0;
	m_TimeOneFrame = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRopeTimer::~CRopeTimer(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CRopeTimer::Init()
{
	// �o�[
	m_pBar = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(BAR_POS),
		BAR_WIDTH, BAR_HEIGHT,
		TEXTURE_HP_GAGE_G);
	// �g
	m_pFrame = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(BAR_POS),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_CROWD_GAGE_FRAME);
	// �ꂢ��[
	m_pBar->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrame->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// �ŏ��͌����Ȃ���
	m_pBar->SetVertexPolygon(D3DXVECTOR3(BAR_POS), 0, 0);
	m_pFrame->SetVertexPolygon(D3DXVECTOR3(BAR_POS), 0, 0);
}

//=============================================================================
// �I��
//=============================================================================
void CRopeTimer::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CRopeTimer::Update(void)
{
	// �o�[���o��������X�V
	if (m_isAppearAnime)
	{
		UpdateAppearAnime();
	}

	// �o�[�𓮂����X�V
	if (m_isMoveBar)
	{
		UpdateBarMove();
	}
}

//=============================================================================
// �o�[�𓮂����X�V
//=============================================================================
void CRopeTimer::UpdateBarMove(void)
{
	m_Count++;
	if (m_Count > m_CountMax)
	{
		// �I�������ꉞ�܂�Ȃ��ɃZ�b�g
		m_PosLeft = BAR_POS.x;
		m_PosRight = BAR_POS.x;
		m_pBar->SetVertexPolygonRight(m_PosRight);
		m_pBar->SetVertexPolygonLeft(m_PosLeft);
		// �����������Ȃ�
		m_isMoveBar = false;
		// �c�����t���[����������
		m_pFrame->SetVertexPolygon(D3DXVECTOR3(BAR_POS), 0, 0);
	}
	else
	{
		// ��������^�񒆂Ƀ|���S�����k�߂鏈��
		m_PosLeft += m_WidthOneFrame;
		m_PosRight -= m_WidthOneFrame;
		m_pBar->SetVertexPolygonRight(m_PosRight);
		m_pBar->SetVertexPolygonLeft(m_PosLeft);
	}
}

//=============================================================================
// �o�[���o������A�j���[�V�����X�V
//=============================================================================
void CRopeTimer::UpdateAppearAnime(void)
{
	m_Time += m_TimeOneFrame;

	if (m_Time > 1.0f)
	{
		m_Time = 1.0f;
		// �o�[�̊g�k
		float width = EasingInterpolation(m_AnimeBarSize.width, m_AnimeBarSizeDest.width, m_Time);
		float height = EasingInterpolation(m_AnimeBarSize.height, m_AnimeBarSizeDest.height, m_Time);
		m_pBar->SetVertexPolygon(D3DXVECTOR3(BAR_POS), width, height);
		// �t���[���̊g�k
		width = EasingInterpolation(m_AnimeFrameSize.width, m_AnimeFrameSizeDest.width, m_Time);
		height = EasingInterpolation(m_AnimeFrameSize.height, m_AnimeFrameSizeDest.height, m_Time);
		m_pFrame->SetVertexPolygon(D3DXVECTOR3(BAR_POS), width, height);
		// �����o�����Ȃ�
		m_isAppearAnime = false;
		// �o�[�𓮂�����
		m_isMoveBar = true;
	}
	else
	{
		// �o�[�̊g�k
		float width = EasingInterpolation(m_AnimeBarSize.width, m_AnimeBarSizeDest.width, m_Time);
		float height = EasingInterpolation(m_AnimeBarSize.height, m_AnimeBarSizeDest.height, m_Time);
		m_pBar->SetVertexPolygon(D3DXVECTOR3(BAR_POS), width, height);
		// �t���[���̊g�k
		width = EasingInterpolation(m_AnimeFrameSize.width, m_AnimeFrameSizeDest.width, m_Time);
		height = EasingInterpolation(m_AnimeFrameSize.height, m_AnimeFrameSizeDest.height, m_Time);
		m_pFrame->SetVertexPolygon(D3DXVECTOR3(BAR_POS), width, height);
	}
}

//=============================================================================
// �`��
//=============================================================================
void CRopeTimer::DrawUI(void)
{
}

//=============================================================================
// �쐬
//=============================================================================
CRopeTimer* CRopeTimer::Create(LPDIRECT3DDEVICE9 *pDevice)
{
	CRopeTimer* p = new CRopeTimer(pDevice);
	p->Init();
	return p;
}

//=============================================================================
// �����[��
//=============================================================================
void CRopeTimer::Start(int appearFrame, int reduceFrame)
{
	// �P�t���[��������Ɍ��炷�� �����ɂ��Ă�̂͗������猸�炷����
	m_WidthOneFrame = BAR_WIDTH / reduceFrame * 0.5f;
	// �P�t���[��������ɑ��₷�^�C��
	m_TimeOneFrame = 1.0f / appearFrame;
	// �[�����̍��W
	m_PosLeft = BAR_POS_LEFT;
	m_PosRight = BAR_POS_RIGHT;
	// �J�E���g������
	m_CountMax = reduceFrame;
	m_Count = 0;
	// �o���A�j���[�V�����J�n
	m_isAppearAnime = true;
	// �T�C�Y�w��
	m_AnimeBarSize = AnimeData(0, 0);
	m_AnimeBarSizeDest = AnimeData(BAR_WIDTH, BAR_HEIGHT);
	m_AnimeFrameSize = AnimeData(0, 0);
	m_AnimeFrameSizeDest = AnimeData(BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT);
	// �^�C��������
	m_Time = 0;
}

//----EOF----