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

//*****************************************************************************
// �萔
//*****************************************************************************
// ��̐����̕��i���S���W����̃I�t�Z�b�g�j
static const float FIGURE_SPACE_WIDTH = 30;
// �����̑傫��
static const float FIGURE_WIDTH = 30;
static const float FIGURE_HEIGHT = 50;
// �����̃e�N�X�`��
static const TEXTURE_TYPE FIGURE_TEXTURE = TEXTURE_NUMBER;
// �^�C���̍ő吔
static const int TIME_MAX = 99;
// ��b�̃J�E���g
static const short SECOND_FRAME = 60;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCountTime::CCountTime(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;
	m_pFigure1st = NULL;
	m_pFigure2nd = NULL;
	m_Time = 0;
	m_TimeCount = 0;
	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeOneFrameAlpha = 0;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);	// �ŏ��̃A�j���[�V�����Ń��𓧖��ɂ��邽��
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

	// ��̐����𐶐�
	m_pFigure1st = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x - FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		FIGURE_TEXTURE);

	m_pFigure2nd = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x + FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		FIGURE_TEXTURE);

	m_pFigure1st->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_pFigure2nd->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

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
}

//=============================================================================
// ���Ԃ��J�E���g����X�V
//=============================================================================
void CCountTime::UpdateTime()
{
	m_TimeCount++;

	// 1�b�o�߂��Ă�����
	if (m_TimeCount > SECOND_FRAME)
	{
		m_TimeCount = 0;
		// ���Ԍo��
		m_Time--;
		// �J�n���Ԃ̍X�V
		// �|���S���̃e�N�X�`���ύX
		Set(m_Time);
	}
	CDebugProc::Print("\nTIME = %d\n", m_Time);
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
// �`��
//=============================================================================
void CCountTime::DrawNormalRender(void)
{
}

//=============================================================================
// �쐬
//=============================================================================
CCountTime* CCountTime::Create(
	D3DXVECTOR2 &pos, int time, LPDIRECT3DDEVICE9 *pDevice)
{
	CCountTime* p = new CCountTime(pDevice);
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

//----EOF----