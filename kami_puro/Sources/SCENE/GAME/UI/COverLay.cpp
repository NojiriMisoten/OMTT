//=============================================================================
//
// COverLay�N���X [COverLay.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "COverLay.h"

//*****************************************************************************
// �萔
//*****************************************************************************
static const float WIDTH = SCREEN_WIDTH;
static const float HEIGHT = SCREEN_HEIGHT / 720.f * 300.f;
static const D3DXVECTOR3 POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
COverLay::COverLay(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;

	m_p2D = NULL;
	m_isIn = false;
	m_isOut = false;
	m_isWait = false;
	m_IntervalMax = 0;
	m_IntervalCount = 0;
	m_Color = D3DXCOLOR(1, 1, 1, 1);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
COverLay::~COverLay(void)
{

}

//=============================================================================
// ������
//=============================================================================
void COverLay::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	// 2D������
	m_p2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(POS),
		WIDTH, HEIGHT,
		TEXTURE_BLUE);
	m_p2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// �t�F�[�h�A�E�g�̔���������
	m_Color = D3DXCOLOR(1, 1, 1, 0);
	m_p2D->SetColorPolygon(m_Color);
}

//=============================================================================
// �I��
//=============================================================================
void COverLay::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void COverLay::Update(void)
{
	if (m_isIn)		In();
	if (m_isWait)	Wait();
	if (m_isOut)	Out();
}

//=============================================================================
// �`��
//=============================================================================
void COverLay::DrawUI(void)
{
}

//=============================================================================
// �쐬
//=============================================================================
COverLay* COverLay::Create(
	LPDIRECT3DDEVICE9 *pDevice)
{
	COverLay* p = new COverLay(pDevice);
	p->Init(pDevice);
	return p;
}

//=============================================================================
// �J�b�g�C���X�^�[�g
//=============================================================================
void COverLay::Start(Data *data)
{
	m_isIn = true;
	m_isOut = false;
	m_isWait = false;
	m_IntervalCount = 0;
	m_IntervalMax = data->interval;
	m_FadeInSpeed = data->fadeInSpeed;
	m_FadeOutSpeed = data->fadeOutSpeed;

	// �e�N�X�`��
	m_p2D->ChangeTexture(data->texture);

	// �t�F�[�h�A�E�g�̔���������
	m_Color = D3DXCOLOR(1, 1, 1, 0);
	m_p2D->SetColorPolygon(m_Color);
}

//=============================================================================
// ��ʓ��ɓ����Ă���X�V
//=============================================================================
void COverLay::In()
{
	m_Color.a += m_FadeInSpeed;
	if (m_Color.a <= 1.0f)
	{
		m_p2D->SetColorPolygon(m_Color);
	}
	else
	{
		m_Color.a = 1.0f;
		m_p2D->SetColorPolygon(m_Color);
		m_isIn = false;
		m_isWait = true;
		m_IntervalCount = 0;
	}
}

//=============================================================================
// �J�b�g�C�����ĕ\�����Ă�����
//=============================================================================
void COverLay::Wait()
{
	// �J�b�g�C�����A�E�g����܂ł̃J�E���g
	m_IntervalCount++;
	if (m_IntervalCount > m_IntervalMax)
	{
		m_isWait = false;
		m_isOut = true;
	}
}

//=============================================================================
// ��ʊO�ɏo�Ă����X�V
//=============================================================================
void COverLay::Out()
{
	m_Color.a -= m_FadeOutSpeed;
	if (m_Color.a >= 0.0f)
	{
		m_p2D->SetColorPolygon(m_Color);
	}
	else
	{
		m_Color.a = 0.0f;
		m_p2D->SetColorPolygon(m_Color);
		m_isOut = false;
	}
}
//----EOF----