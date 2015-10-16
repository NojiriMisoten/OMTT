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
static const float BAR_WIDTH = 600;
static const float BAR_HEIGHT = 30;
// �o�[�̃e�N�X�`��
static const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// TODO ���̗�
static const float CROWD_MAX = 255;
// �ϋq�ʂɂ��̒l���|���ăo�[�̍��W���o���i�K���@�Ă��v�Z�ŏo����͂��j
static const float RESIST = 1.2f;
// �o�[�̐F
static const D3DXCOLOR BAR_COLOR_LEFT = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
static const D3DXCOLOR BAR_COLOR_RIGHT = D3DXCOLOR(0.0f, 0.1f, 1.0f, 1.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCrowdBar::CCrowdBar(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	m_pBarLeft = NULL;
	m_pBarRight = NULL;
	m_Pos = D3DXVECTOR2(0, 0);
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
void CCrowdBar::Init(D3DXVECTOR2 &pos, float height)
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
	m_pBarLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(left_center, pos.y, 0),
		width, height,
		BAR_TEXTURE);
	m_pBarRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(right_center, pos.y, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBarRight->AddLinkList(CRenderer::TYPE_RENDER_UI);

	m_pBarLeft->SetColorPolygon(BAR_COLOR_LEFT);
	m_pBarRight->SetColorPolygon(BAR_COLOR_RIGHT);
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
	CDebugProc::Print("�ϋq�l�@�@%f\n", m_Value);
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
	D3DXVECTOR2 &pos,
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

	m_pBarLeft->SetVertexPolygonRight(m_Value * RESIST + m_Pos.x);
	m_pBarRight->SetVertexPolygonLeft(m_Value * RESIST + m_Pos.x);
}

//=============================================================================
// �o�[�̒��_��^�񒆂ɏW�߂�
//=============================================================================
void CCrowdBar::Reset()
{
	m_Value = 0;
	mValueBase = 0;

	m_pBarLeft->SetVertexPolygonLeft(m_Value * RESIST + m_Pos.x);
	m_pBarRight->SetVertexPolygonRight(m_Value * RESIST + m_Pos.x);
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

	m_pBarLeft->SetVertexPolygonLeft(-mValueBase * RESIST + m_Pos.x);
	m_pBarRight->SetVertexPolygonRight(mValueBase * RESIST + m_Pos.x);
}

//----EOF----