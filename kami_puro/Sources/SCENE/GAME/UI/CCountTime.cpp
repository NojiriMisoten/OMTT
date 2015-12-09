//=============================================================================
//
// CStaminaBarBar�N���X [CStaminaBarBar.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCountTime.h"
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../../../MANAGER/CManager.h"
#include "../CGame.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// ��̐����̕��i���S���W����̃I�t�Z�b�g�j
static const float FIGURE_SPACE_WIDTH = 30;
// �����̑傫��
static const float FIGURE_WIDTH = 30;
static const float FIGURE_HEIGHT = 50;
// �^�C���̍ő吔
static const int TIME_MAX = 99;
// ��b�̃J�E���g
static const short SECOND_FRAME = 60;
// �w�i�̑傫��
static const float FIGURE_BACK_WIDTH = 70;
static const float FIGURE_BACK_HEIGHT = 60;
// �~�߂Ă���Ƃ��ɃO���[�ɂ���
static const D3DXCOLOR GRAY_COLOR = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.5f);
// �g�k�̃^�C�}��1�t���[��������̗� (1 / x �̏ꍇ�Ax�t���[���ŃA�j���[�V��������������)
static const float GRAY_SCALE_SPEED = 1.0f / 20.0f;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCountTime::CCountTime(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame* pGame)
{
	m_pManager = pManager;
	m_pGame = pGame;
	m_pD3DDevice = pDevice;
	m_pFigure1st = NULL;
	m_pFigure2nd = NULL;
	m_pBack = NULL;
	m_pGray = NULL;
	m_Time = 0;
	m_TimeCount = 0;
	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeOneFrameAlpha = 0;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);	// �ŏ��̃A�j���[�V�����Ń��𓧖��ɂ��邽��
	m_GrayCount = 0;
	m_GrayCountMax = 0;
	m_isGray = false;
	m_GrayWidth = 0;
	m_GrayWidthDest = 0;
	m_GrayHeight = 0;
	m_GrayHeightDest = 0;
	m_GrayPos = D3DXVECTOR3(0, 0, 0);
	m_GrayTime = 0;
	m_isScale = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCountTime::~CCountTime(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CCountTime::Init(D3DXVECTOR2 &pos, int time)
{
	m_Time = time;

	m_Time = max(m_Time, TIME_MAX);

	// �J�E���g������
	m_TimeCount = 0;

	m_GrayPos.x = pos.x;
	m_GrayPos.y = pos.y;

	// �w�i
	m_pBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x, pos.y, 0),
		FIGURE_BACK_WIDTH, FIGURE_BACK_HEIGHT,
		TEXTURE_UI_TIME_BACK);

	// ��̐����𐶐�
	m_pFigure1st = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x - FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		TEXTURE_NUMBER);
	m_pFigure2nd = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x + FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		TEXTURE_NUMBER);

	// �O���[�̏d�˂�z
	m_pGray = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x, pos.y, 0),
		FIGURE_BACK_WIDTH, FIGURE_BACK_HEIGHT,
		TEXTURE_CHAIN);
	// �ŏ��͑傫���O����
	m_pGray->SetVertexPolygon(m_GrayPos, 0, 0);

	// ���C���[
	m_pBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFigure1st->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFigure2nd->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pGray->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// �|���S���̃e�N�X�`���ύX
	Set(m_Time);

	// �ŏ��̃A�j���[�V�����p��2D�𓧉߂�����
	m_pFigure1st->SetColorPolygon(m_Anime2DColor);
	m_pFigure2nd->SetColorPolygon(m_Anime2DColor);
}

//=============================================================================
// �I��
//=============================================================================
void CCountTime::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CCountTime::Update(void)
{
	// ���Ԃ̃J�E���g�ƃe�N�X�`���؂�ւ�
	UpdateTime();

	// �J�n�A�j���[�V�����X�V
	UpdateAnime();

	// �~�߂Ă���[�ɂȂ��Ă邩
	if (m_isGray)
	{
		m_GrayCount++;
		if (m_GrayCount > m_GrayCountMax)
		{
			// �O���[�I���
			m_isGray = false;
			GrayScaleClose();
		}
	}

	// �X�P�[���̍X�V�����邩
	if (m_isScale)
	{
		UpdateScale();
	}
}

//=============================================================================
// ���Ԃ��J�E���g����X�V
//=============================================================================
void CCountTime::UpdateTime()
{
	// �Q�[������擾�ɕύX
	m_Time = m_pGame->GetBattleTimer() / TARGET_FPS;
	Set(m_Time);
	return;
}

//=============================================================================
// �J�n�A�j���[�V����������X�V
//=============================================================================
void CCountTime::UpdateAnime()
{
	// �J�n�A�j���[�V���������[
	if (!m_isAnime)	return;

	// �J�E���g
	m_AnimeCount++;
	// �A���t�@�l�X�V
	m_Anime2DColor.a += m_AnimeOneFrameAlpha;
	m_pFigure1st->SetColorPolygon(m_Anime2DColor);
	m_pFigure2nd->SetColorPolygon(m_Anime2DColor);
	// �J�n�A�j���[�V�����I��
	if (m_AnimeCount > m_AnimeCountMax){
		// �����ŃA���t�@�l��1.0�ɂȂ�͂������ǈꉞ�I�����Ƃ��I
		m_Anime2DColor.a = 1.0f;
		m_pFigure1st->SetColorPolygon(m_Anime2DColor);
		m_pFigure2nd->SetColorPolygon(m_Anime2DColor);
		m_isAnime = false;
	}
}

//=============================================================================
// �쐬
//=============================================================================
CCountTime* CCountTime::Create(
	D3DXVECTOR2 &pos, int time,
	LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame* pGame)
{
	CCountTime* p = new CCountTime(pDevice, pManager, pGame);
	p->Init(pos, time);
	return p;
}

//=============================================================================
// ���Ԃ���2D�̃e�N�X�`���ɔ��f������
//=============================================================================
void CCountTime::Set(int time)
{
	// �e���̌v�Z
	float u1 = (m_Time % 100 / 10) * 0.1f;
	float u2 = (m_Time % 10) * 0.1f;
	UV_INDEX uv1 = { u1, u1 + 0.1f, 0.0f, 1.0f };
	UV_INDEX uv2 = { u2, u2 + 0.1f, 0.0f, 1.0f };
	m_pFigure1st->SetUV(&uv1);
	m_pFigure2nd->SetUV(&uv2);
}

//=============================================================================
// �^�C�}�[���X�g�b�v��Ԃɂ���(�O���[�ɂ���)
//=============================================================================
void CCountTime::Stop(int frame)
{
	m_GrayCount = 0;
	m_GrayCountMax = frame;
	m_isGray = true;
	GrayScaleOpen();
}

//=============================================================================
// �J�n�A�j���[�V����������@������
// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
//=============================================================================
void CCountTime::StartAnimation(int endCount)
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
	m_pFigure1st->SetColorPolygon(m_Anime2DColor);
	m_pFigure2nd->SetColorPolygon(m_Anime2DColor);
}


//=============================================================================
// �J�n�A�j���[�V����������@������
// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
//=============================================================================
void CCountTime::UpdateScale()
{
	m_GrayTime += GRAY_SCALE_SPEED;

	if (m_GrayTime > 1.0f)
	{
		m_GrayTime = 1.0f;
		float width = EasingInterpolation(m_GrayWidth, m_GrayWidthDest, m_GrayTime);
		float height = EasingInterpolation(m_GrayHeight, m_GrayHeightDest, m_GrayTime);
		m_pGray->SetVertexPolygon(m_GrayPos, width, height);

		// �����o�����Ȃ�
		m_isScale = false;
	}
	else
	{
		float width = EasingInterpolation(m_GrayWidth, m_GrayWidthDest, m_GrayTime);
		float height = EasingInterpolation(m_GrayHeight, m_GrayHeightDest, m_GrayTime);
		m_pGray->SetVertexPolygon(m_GrayPos, width, height);
	}
}

//=============================================================================
// �O���[�̊J��
//=============================================================================
void CCountTime::GrayScaleOpen()
{
	m_GrayWidth = 0;
	m_GrayHeight = 0;
	m_GrayWidthDest = FIGURE_BACK_WIDTH;
	m_GrayHeightDest = FIGURE_BACK_HEIGHT;
	m_isScale = true;
	m_GrayTime = 0;
}

//=============================================================================
// �O���[�̕���
//=============================================================================
void CCountTime::GrayScaleClose()
{
	m_GrayWidth = FIGURE_BACK_WIDTH;
	m_GrayHeight = FIGURE_BACK_HEIGHT;
	m_GrayWidthDest = 0;
	m_GrayHeightDest = 0;
	m_isScale = true;
	m_GrayTime = 0;
}

//=============================================================================
// ��\��
//=============================================================================
void CCountTime::SetImvisible(void)
{
	m_pBack->SetDrawFlag(false);
	m_pFigure1st->SetDrawFlag(false);
	m_pFigure2nd->SetDrawFlag(false);
	m_pGray->SetDrawFlag(false);
}

//=============================================================================
// �\��
//=============================================================================
void CCountTime::SetVisible(void)
{
	m_pBack->SetDrawFlag(true);
	m_pFigure1st->SetDrawFlag(true);
	m_pFigure2nd->SetDrawFlag(true);
	m_pGray->SetDrawFlag(true);
}
//----EOF----