//=============================================================================
//
// CStaminaBarBar�N���X [CStaminaBarBar.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCrowdBar.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �o�[�̍���
const float BAR_WIDTH = 600;
const float BAR_HEIGHT = 30;
// �o�[�̃e�N�X�`��
const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// TODO ���̗�
const float CROWD_MAX = 255;
// �ϋq�ʂɂ��̒l���|���ăo�[�̍��W���o���i�K���@�Ă��v�Z�ŏo����͂��j
const float RESIST = 1.2f;
// �o�[�̐F
const D3DXCOLOR BAR_COLOR_LEFT = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
const D3DXCOLOR BAR_COLOR_RIGHT = D3DXCOLOR(0.0f, 0.1f, 1.0f, 1.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCrowdBar::CCrowdBar(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCrowdBar::~CCrowdBar(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CCrowdBar::Init(D3DXVECTOR2 pos, float height)
{
	// �ϐ����
	m_Value = 0;
	m_ValueMax = CROWD_MAX;
	mValueBase = 0;
	m_Pos = pos;

	// ���E�����̍��W
	float left = -m_ValueMax * RESIST + pos.x;
	float right = m_ValueMax * RESIST + pos.x;
	float width = right - pos.x;
	float left_center = left + width * 0.5f;
	float right_center = right - width * 0.5f;

	// 2D������
	m_BarLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(left_center, pos.y, 0),
		width, height,
		BAR_TEXTURE);
	m_BarRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(right_center, pos.y, 0),
		width, height,
		BAR_TEXTURE);

	m_BarLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_BarRight->AddLinkList(CRenderer::TYPE_RENDER_UI);

	m_BarLeft->SetColorPolygon(BAR_COLOR_LEFT);
	m_BarRight->SetColorPolygon(BAR_COLOR_RIGHT);
}

//=============================================================================
// �I��
//=============================================================================
void CCrowdBar::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CCrowdBar::Update(void)
{
	float a = m_Value * RESIST + m_Pos.x;
	CDebugProc::Print("�ϋq�l�@�@%f\n", m_Value);
	CDebugProc::Print("�ϋq���W�@%f\n", a);
	
}

//=============================================================================
// �`��
//=============================================================================
void CCrowdBar::DrawUI(void)
{
	CScene2D::DrawUI();
}

//=============================================================================
// �쐬
//=============================================================================
CCrowdBar* CCrowdBar::Create(
	D3DXVECTOR2 pos,
	float height,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CCrowdBar* p = new CCrowdBar(pDevice);
	p->Init(pos, height);
	return p;
}

//=============================================================================
// �l�ϓ�
// �����A�������y������ɂ̓t���O�Ǘ��Œ��_�ύX����񐔂����点��
//=============================================================================
void CCrowdBar::Add(float value)
{
	m_Value += value;

	// �N�����v
	m_Value = min(m_Value, m_ValueMax);
	m_Value = max(m_Value, -m_ValueMax);

	m_BarLeft->SetVertexPolygonRight(m_Value * RESIST + m_Pos.x);
	m_BarRight->SetVertexPolygonLeft(m_Value * RESIST + m_Pos.x);
}

//=============================================================================
// �o�[�̒��_��^�񒆂ɏW�߂�
//=============================================================================
void CCrowdBar::Reset()
{
	m_Value = 0;
	mValueBase = 0;

	m_BarLeft->SetVertexPolygonLeft(m_Value * RESIST + m_Pos.x);
	m_BarRight->SetVertexPolygonRight(m_Value * RESIST + m_Pos.x);
}

//=============================================================================
// �o�[�̒��_��[�����ɖ߂�
//=============================================================================
void CCrowdBar::Replace(float value)
{
	mValueBase += value;

	// �N�����v
	mValueBase = min(m_Value, m_ValueMax);
	mValueBase = max(m_Value, -m_ValueMax);

	m_BarLeft->SetVertexPolygonLeft(-mValueBase * RESIST + m_Pos.x);
	m_BarRight->SetVertexPolygonRight(mValueBase * RESIST + m_Pos.x);
}

//----EOF----