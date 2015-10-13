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
const float BAR_HEIGHT = 40;
// �o�[�̃e�N�X�`��
const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// TODO ���̗�
const float CROWD_MAX = 255;
// �ϋq�ʂɂ��̒l���|���ăo�[�̍��W���o���i�K���@�Ă��v�Z�ŏo����͂��j
const float RESIST = 1.3f;
// �o�[�̐F
const D3DXCOLOR BAR_COLOR = D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCrowdBar::CCrowdBar(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	// �����_���[�Z�b�g
	CScene::AddLinkList(CRenderer::TYPE_RENDER_UI);
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
void CCrowdBar::Init(D3DXVECTOR2 pos)
{
	// �ϐ����
	m_Value = 0;
	m_ValueMax = CROWD_MAX;
	m_Pos = pos;

	// 2D������
	CScene2D::Init(
		D3DXVECTOR3(pos.x,pos.y, 0),
		0, BAR_HEIGHT, BAR_TEXTURE);

	SetColorPolygon(BAR_COLOR);
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
	LPDIRECT3DDEVICE9 *pDevice)
{
	CCrowdBar* p = new CCrowdBar(pDevice);
	p->Init(pos);
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

	// �E������
	if (m_Value > 0){
		CScene2D::SetVertexPolygonLeft(m_Pos.x);
		CScene2D::SetVertexPolygonRight(m_Value * RESIST + m_Pos.x);
	}
	// ���Ⴄ
	else if (m_Value < 0){
		CScene2D::SetVertexPolygonRight(m_Pos.x);
		CScene2D::SetVertexPolygonLeft(m_Value * RESIST + m_Pos.x);
	}
}

//----EOF----