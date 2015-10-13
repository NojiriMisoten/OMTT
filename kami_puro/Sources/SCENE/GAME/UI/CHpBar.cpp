//=============================================================================
//
// CHpBar�N���X [CHpBar.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CHpBar.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �o�[�̍���
const float BAR_WIDTH = 200;
const float BAR_HEIGHT = 40;
// �o�[�̃e�N�X�`��
const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// �o�[�̐Ԃ��Ȃ镔���̐F
const D3DXCOLOR BAR_HP_COLOR = D3DXCOLOR(0.0f, 0.8f, 0.2f, 1.0f);
const D3DXCOLOR BAR_LESS_COLOR = D3DXCOLOR(1, 0.2f, 0.1f, 1.0f);
// �Ԃ��o�[���ǂ�������X�s�[�h
const float BAR_RED_SPEED_RESIST = 0.01f;
// ���̎����X�^�~�i�񕜗�
const float AUTO_HEAL_AMOUNT = 0.4f;

// TODO ����HP�ʁ@�v���C�����玝���Ă���
const float HP_MAX = 255;

// �Ԃ��o�[��ύX����J�E���g��
const float RED_CHANGE_INTERVAL = 60;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CHpBar::CHpBar(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	// �����_���[�Z�b�g
	CScene::AddLinkList(CRenderer::TYPE_RENDER_UI);

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CHpBar::~CHpBar(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CHpBar::Init(
	D3DXVECTOR2 pos,
	POSITIONBASE positionBase)
{
	// �ϐ����
	m_Pos = pos;
	m_PositionBase = positionBase;
	m_Value = HP_MAX;
	m_ValueMax = HP_MAX;
	m_ValueRed = HP_MAX;
	m_RedResetCount = 0;
	m_isRedReset = false;

	// �o�[�̒��S���W
	// 2D������
	CScene2D::Init(D3DXVECTOR3(pos.x, pos.y, 0), BAR_WIDTH, BAR_HEIGHT, BAR_TEXTURE);
	// �l�im_Value�j�P������̃s�N�Z����(float)���v�Z
	m_WidthOneValue = BAR_WIDTH / m_ValueMax;
	// �o�[�̐F��ύX
	SetColorPolygon(BAR_LESS_COLOR);
	// �Œ肵�����W�@�o�[�̒��_��ύX����Ƃ��Ɏg�p
	if (positionBase == POSITIONBASE_LEFT)
	{
		m_PosBase = D3DXVECTOR2(pos.x + BAR_WIDTH * 0.5f, pos.y);
	}
	else
	{
		m_PosBase = D3DXVECTOR2(pos.x - BAR_WIDTH * 0.5f, pos.y);
	}

	// �݂ǂ�o�[
	m_HpBar = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos.x, pos.y, 0), BAR_WIDTH, BAR_HEIGHT, BAR_TEXTURE);
	m_HpBar->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_HpBar->SetColorPolygon(BAR_HP_COLOR);
}

//=============================================================================
// �I��
//=============================================================================
void CHpBar::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CHpBar::Update(void)
{
	CDebugProc::Print("�̗� %d / %d\n", (int)m_Value, (int)m_ValueRed);
	
	// �Ԃ��o�[��΂ɍ��킹�Ȃ��Ȃ烊�^�[��
	if (!m_isRedReset)
		return;
	// �Ԃ��o�[��΂ɍ��킹��
	m_RedResetCount++;
	if (m_RedResetCount > RED_CHANGE_INTERVAL){
		m_RedResetCount = 0;
		m_ValueRed = m_Value;
		m_isRedReset = false;

		if (m_PositionBase == POSITIONBASE_RIGHT)
		{
			SetVertexPolygonRight(m_Value * m_WidthOneValue + m_PosBase.x);
		}
		else
		{
			SetVertexPolygonLeft(m_PosBase.x - m_Value * m_WidthOneValue);
		}
	}

}

//=============================================================================
// �`��
//=============================================================================
void CHpBar::DrawUI(void)
{
	CScene2D::DrawUI();
}

//=============================================================================
// �쐬
//=============================================================================
CHpBar* CHpBar::Create(
	D3DXVECTOR2 pos,
	POSITIONBASE positionBase,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CHpBar* p = new CHpBar(pDevice);
	p->Init(pos, positionBase);
	return p;
}

//=============================================================================
// HP����
//=============================================================================
void CHpBar::Add(float value)
{
	m_Value += value;

	// �N�����v
	m_Value = min(m_Value, m_ValueMax);
	m_Value = max(m_Value, 0);

	// �ԃo�[�ύX����t���O
	m_isRedReset = true;

	// �݂ǂ�o�[���_�ύX
	if (m_PositionBase == POSITIONBASE_RIGHT)
	{
		m_HpBar->SetVertexPolygonRight(m_Value * m_WidthOneValue + m_PosBase.x);
	}
	else
	{
		m_HpBar->SetVertexPolygonLeft(m_PosBase.x - m_Value * m_WidthOneValue);
	}

}

//----EOF----