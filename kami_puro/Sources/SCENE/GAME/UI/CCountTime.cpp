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

//*****************************************************************************
// �萔
//*****************************************************************************
// ��̐����̕��i���S���W����̃I�t�Z�b�g�j
const float FIGURE_SPACE_WIDTH = 30;
// �����̑傫��
const float FIGURE_WIDTH = 30;
const float FIGURE_HEIGHT = 50;
// �����̃e�N�X�`��
const TEXTURE_TYPE FIGURE_TEXTURE = TEXTURE_NUMBER;
// �^�C���̍ő吔
const int TIME_MAX = 99;
// ��b�̃J�E���g
const short SECOND_FRAME = 60;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCountTime::CCountTime(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{

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
void CCountTime::Init(D3DXVECTOR2 pos, int time)
{
	
	m_Time = time;

	m_Time = max(m_Time, TIME_MAX);

	// �J�E���g������
	m_TimeCount = 0;

	// ��̐����𐶐�
	m_Figure1st = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x - FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		FIGURE_TEXTURE);

	m_Figure2nd = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x + FIGURE_SPACE_WIDTH * 0.5f, pos.y, 0),
		FIGURE_WIDTH, FIGURE_HEIGHT,
		FIGURE_TEXTURE);

	m_Figure1st->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_Figure2nd->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// �|���S���̃e�N�X�`���ύX
	Set(m_Time);
}

//=============================================================================
// �I��
//=============================================================================
void CCountTime::Uninit(void)
{
	CScene2D::Uninit();
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
	CScene2D::DrawUI();
}

//=============================================================================
// �쐬
//=============================================================================
CCountTime* CCountTime::Create(
	D3DXVECTOR2 pos, int time, LPDIRECT3DDEVICE9 *pDevice)
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
	m_Figure1st->SetUV(&uv1);
	m_Figure2nd->SetUV(&uv2);
}
//----EOF----