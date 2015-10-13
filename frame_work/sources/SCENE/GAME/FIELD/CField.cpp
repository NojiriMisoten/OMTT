//=============================================================================
//
// CField�N���X [CField.cpp]
// Author : ��K�@����
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CField.h"

//=======================================================================
// �}�N��
//=======================================================================
static const float DOWN_POS_POW = 5.0f;
static const float MOVE_POW = DOWN_POS_POW / 10.f;

//=======================================================================
// �R���X�g���N�^
//=======================================================================
CField::CField(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) :CScene2D(pDevice, nPriority, m_objType)
{

}

//=======================================================================
// �f�X�g���N�^
//=======================================================================
CField::~CField()
{
}

//=======================================================================
// �I��
//=======================================================================
void CField::Uninit(void)
{
	CScene2D::Uninit();
}

//=======================================================================
// �X�V
//=======================================================================
void CField::Update(void)
{
	if (m_vPos.y < m_vDestPos.y)
	{
		m_vPos.y += MOVE_POW;
	}
	else if (m_vPos.y > m_vDestPos.y)
	{
		m_vPos.y -= MOVE_POW * 0.5f;
	}
}

//=======================================================================
// �`��
//=======================================================================
void CField::Draw(void)
{
	CScene2D::Draw();
}

//=======================================================================
// �N���G�C�g�֐�
//=======================================================================
CField* CField::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	CField* p = new CField(pDevice);

	p->Init(pos, width, height, texType);

	return p;
}

//=======================================================================
// �������֐�
//=======================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	CScene2D::Init(pos, width, height, texType);

	// �����蔻��X�e�[�^�X�Z�b�g
	SetJudgeParam(pos, width, height, texType);
	
	m_vDestPos = m_vDefaultPos = pos;

	return TRUE;
}

//=======================================================================
// ����Ă�
//=======================================================================
void CField::Ride(bool ride)
{
	if (ride)
	{
		m_vDestPos.y = m_vDefaultPos.y + DOWN_POS_POW;
	}
	else
	{
		m_vDestPos.y = m_vDefaultPos.y;
	}
}

//=======================================================================
// �����蔻��X�e�[�^�X�Z�b�g�֐�
//=======================================================================
void CField::SetJudgeParam(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	switch (texType)
	{
		// ����
	case TEXTURE_FIELD_01:
		m_fJudgeWidth = width * 0.8f;
		m_fJudgeHeight = height * 0.6f;
		m_vJudgePos = pos;
		m_vJudgePos.y -= m_fJudgeHeight * 0.4f;
		break;

		// ��u�����
	case TEXTURE_FIELD_02:
		m_fJudgeWidth = width * 0.8f;
		m_fJudgeHeight = height * 0.6f;
		m_vJudgePos = pos;
		m_vJudgePos.y -= m_fJudgeHeight * 0.4f;
		break;

		// �X�^�[�g���S�[��
	case TEXTURE_FIELD_03:
		m_fJudgeWidth = width;
		m_fJudgeHeight = height * 0.8f;
		m_vJudgePos = pos;
		m_vJudgePos.y -= m_fJudgeHeight * 0.2f;
		break;

	default:
		m_fJudgeWidth = width;
		m_fJudgeHeight = height;
		m_vJudgePos = pos;
		break;
	}
}
//----EOF----