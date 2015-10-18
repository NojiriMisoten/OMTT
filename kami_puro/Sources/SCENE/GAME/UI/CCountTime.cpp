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

	m_pFigure1st->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFigure2nd->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// �|���S���̃e�N�X�`���ύX
	Set(m_Time);
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
// �`��
//=============================================================================
void CCountTime::DrawUI(void)
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
//----EOF----