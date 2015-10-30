//=============================================================================
//
//	CCommandChartUI�N���X�̏���(CCommandChartUI.cpp)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
// �C���N���[�h
#include "CCommandChartUI.h"
#include "../../../RENDERER/CRenderer.h"

//-----------------------------------------------------------------------------
//	�}�N����`
//-----------------------------------------------------------------------------
static const float COMMAND_POLYGON_WIDTH = 50.0f;		// �R�}���h�̃|���S���̉���
static const float COMMAND_POLYGON_HEIGHT = 50.0f;		// �R�}���h�̃|���S���̍���
static const float COMMAND_ERROR_RANGE = 0.1f;		// �R�}���hUI���ړI�̍��W���ӂœ������~�߂�ۂ̌덷
static const float COMMAND_MOVEMENT_COEFFICIENT = 0.5f;	// �ړI�̍��W�ɍs���Ƃ��̈ړ��ʂ̌W��

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CCommandChartUI::CCommandChartUI(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) : CScene2D(pDevice, objType)
{
	m_isMoveX = true;
	m_isMoveY = true;
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CCommandChartUI::~CCommandChartUI()
{
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void CCommandChartUI::Init(BUTTON_TYPE ButtonType, D3DXVECTOR3 pos)
{
	// �|���S���̍쐬
	CScene2D::Init(pos, COMMAND_POLYGON_WIDTH, COMMAND_POLYGON_HEIGHT, TEXTURE_NULL);

	// ���X�g�ɒǉ�
	CScene2D::AddLinkList(CRenderer::TYPE_RENDER_UI);

	// �b�菈��
	// �{�^���̎�ނɂ���ĐF��ς��Ă���
	switch (ButtonType)
	{
	case BUTTON_TYPE_1:
		m_Color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	case BUTTON_TYPE_2:
		m_Color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		break;
	case BUTTON_TYPE_3:
		m_Color = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		break;
	case BUTTON_TYPE_4:
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	default:
		break;
	}
	SetColorPolygon(m_Color);

	// �����̃{�^������ۑ�
	m_ButtonType = ButtonType;

	// ���͔��f�t���O��false�ɂ���
	m_isInputCommand = false;
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void CCommandChartUI::Uninit(void)
{
	// �I��
	CScene2D::Uninit();
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void CCommandChartUI::Update(void)
{
	// �ړ�����
	Move();
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void CCommandChartUI::Draw(void)
{
	CScene2D::Draw();

#ifdef _DEBUG
	CDebugProc::PrintL("�R�}���hUI�̃��l%f",m_Color.a);
#endif
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
CCommandChartUI* CCommandChartUI::Create(LPDIRECT3DDEVICE9 *pDevice,  BUTTON_TYPE ButtonType, D3DXVECTOR3 pos)
{
	// new
	CCommandChartUI* temp = new CCommandChartUI(pDevice);

	// ������
	temp->Init(ButtonType, pos);

	// �Ԃ�
	return temp;
}

//-----------------------------------------------------------------------------
// �ړ��p�֐�
//-----------------------------------------------------------------------------
void CCommandChartUI::Move(void)
{
	// x���W�̈ړ����s����
	if (m_isMoveX)
	{
		// x���W�̈ړ����I���܂ł����W�̈ړ����s��
		if (m_Pos.x < m_DestPos.x - COMMAND_ERROR_RANGE || m_DestPos.x + COMMAND_ERROR_RANGE < m_Pos.x)
		{
			// �ړ���
			float fMovement = (m_DestPos.x - m_Pos.x)*COMMAND_MOVEMENT_COEFFICIENT;
			m_Pos.x += fMovement;

			m_Color.a += 1.0f / fMovement;

			// �ړ�������
			SetPos(m_Pos);
		}
		else
		{
			// ���݂̍��W��ړI�̍��W�ɍ��킹��
			m_Pos.x = m_DestPos.x;
			m_isMoveX = false;
		}
	}
	// x���W�̈ړ����I����Ă�����y���W�̈ړ����n�߂�
	else if (m_isMoveY)
	{
		// x���W�̈ړ����I���܂ł����W�̈ړ����s��
		if (m_Pos.y < m_DestPos.y - COMMAND_ERROR_RANGE || m_DestPos.y + COMMAND_ERROR_RANGE < m_Pos.y)
		{
			// �ړ���
			float fMovement = (m_DestPos.y - m_Pos.y)*COMMAND_MOVEMENT_COEFFICIENT;
			m_Pos.y += fMovement;

			m_Color.a += 1.0f / fMovement;

			// �ړ�������
			SetPos(m_Pos);
		}
		else
		{
			// ���݂̍��W��ړI�̍��W�ɍ��킹��
			m_Pos.y = m_DestPos.y;
			m_isMoveY = false;
		}
	}
	// �����̈ړ����I����Ă��鎞���S�Ɍ����Ă��Ȃ���΂Ȃ�Ȃ�
	else
	{
		m_Color.a = 1.0f;
		SetColorPolygon(m_Color);
	}
}

// EOF