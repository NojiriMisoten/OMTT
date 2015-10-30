//=============================================================================
//
// CStaminaBarBar�N���X [CStaminaBarBar.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CStaminaBar.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �o�[�̍���
static const float BAR_HEIGHT = 40;
// �o�[�̃e�N�X�`��
static const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// ���̎����X�^�~�i�񕜗�
static const float AUTO_HEAL_AMOUNT = 0.4f;

// TODO ���̃X�^�~�i�ʁ@�v���C�����玝���Ă���
static const float STAMINA_MAX = 255;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CStaminaBar::CStaminaBar(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	// �����_���[�Z�b�g
	CScene::AddLinkList(CRenderer::TYPE_RENDER_UI);

	m_PosLeft = D3DXVECTOR2(0, 0);
	m_PosRight = D3DXVECTOR2(0, 0);
	m_PositionBase = POSITIONBASE_RIGHT;
	m_Value = 0;
	m_ValueMax = 0;
	m_WidthOneValue = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CStaminaBar::~CStaminaBar(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CStaminaBar::Init(
	D3DXVECTOR2 &leftPos,
	D3DXVECTOR2 &rightPos,
	POSITIONBASE positionBase)
{
	// �ϐ����
	m_PosLeft = leftPos;
	m_PosRight = rightPos;
	m_PositionBase = positionBase;
	m_Value = STAMINA_MAX;
	m_ValueMax = STAMINA_MAX;

	// �o�[�̕�
	float barWidth = rightPos.x - leftPos.x;
	// �o�[�̒��S���W
	D3DXVECTOR3 centerPos = D3DXVECTOR3(leftPos.x + barWidth * 0.5f, leftPos.y, 0);

	// 2D������
	CScene2D::Init(centerPos, barWidth, BAR_HEIGHT, BAR_TEXTURE);
	// �X�^�~�i�̒l�im_Value�j�P������̃s�N�Z����(float)���v�Z
	m_WidthOneValue = barWidth / m_ValueMax;

}

//=============================================================================
// �I��
//=============================================================================
void CStaminaBar::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CStaminaBar::Update(void)
{
	// ������
	AutoHeal(AUTO_HEAL_AMOUNT);
}

//=============================================================================
// �`��
//=============================================================================
void CStaminaBar::DrawUI(void)
{
	// �g��Ȃ�
	return;

	CScene2D::DrawUI();
}

//=============================================================================
// �쐬
//=============================================================================
CStaminaBar* CStaminaBar::Create(
	D3DXVECTOR2 &leftPos,
	D3DXVECTOR2 &rightPos,
	POSITIONBASE positionBase,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CStaminaBar* p = new CStaminaBar(pDevice);
	p->Init(leftPos, rightPos, positionBase);
	return p;
}

//=============================================================================
// �l�ϓ�
//=============================================================================
bool CStaminaBar::UseStamina(float value)
{
	// �X�^�~�i������Ȃ��Ƃ���false�Ԃ�
	if (m_Value - value < 0){
		return false;
	}

	// �X�^�~�i����
	m_Value -= value;

	if (m_PositionBase == CStaminaBar::POSITIONBASE_RIGHT){
		float x = m_PosLeft.x + m_WidthOneValue * value;
		m_PosLeft.x = x;
		SetVertexPolygonLeft(x);
	}
	else if (m_PositionBase == CStaminaBar::POSITIONBASE_LEFT){
		float x = m_PosRight.x - m_WidthOneValue * value;
		m_PosRight.x = x;
		SetVertexPolygonRight(x);
	}
	return true;
}


//=============================================================================
// �X�^�~�i�̎�����
//=============================================================================
void CStaminaBar::AutoHeal(float value)
{
	float n = m_Value;
	n += value;
	n = min(n, m_ValueMax);
	float diff = n - m_Value;
	m_Value += diff;

//	CDebugProc::PrintL("�X�^�~�i:%f\n", m_Value);

	// �񕜂��Ă��Ȃ���ΏI��
	if (diff == 0){
		return;
	}

	if (m_PositionBase == POSITIONBASE_RIGHT){
		float x = m_PosLeft.x - m_WidthOneValue * diff;
		m_PosLeft.x = x;
		SetVertexPolygonLeft(x);
	}
	else if (m_PositionBase == POSITIONBASE_LEFT){
		float x = m_PosRight.x + m_WidthOneValue * diff;
		m_PosRight.x = x;
		SetVertexPolygonRight(x);
	}
}
//----EOF----