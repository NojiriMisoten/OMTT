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
static const float COMMAND_ERROR_RANGE = 0.1f;			// �R�}���hUI���ړI�̍��W���ӂœ������~�߂�ۂ̌덷
static const float COMMAND_MOVEMENT_COEFFICIENT = 0.7f;	// �ړI�̍��W�ɍs���Ƃ��̈ړ��ʂ̌W��
static const float COMMAND_MAX_ALPHA = 0.9f;			// �A���t�@�l�̍ő�l
static const UV_INDEX DEFAULT_BUTTON_RIGHT_UP = UV_INDEX(0.5f, 0.75f, 0.5f, 1.0f);	// �E���̏�����̃{�^�������������̃e�N�X�`����UV
static const UV_INDEX DEFAULT_BUTTON_RIGHT_DOWN = UV_INDEX(0.75f, 1.0f, 0.5f, 1.0f);	// �E���̉������̃{�^�������������̃e�N�X�`����UV
static const UV_INDEX DEFAULT_BUTTON_LEFT_UP = UV_INDEX(0.5f, 0.75f, 0.0f, 0.5f);		// �����̏�����̃{�^�������������̃e�N�X�`����UV
static const UV_INDEX DEFAULT_BUTTON_LEFT_DOWN = UV_INDEX(0.75f, 1.0f, 0.0f, 0.5f);	// �����̉������̃{�^�������������̃e�N�X�`����UV
static const UV_INDEX AFTER_BUTTON_RIGHT_UP = UV_INDEX(0.0f, 0.25f, 0.5f, 1.0f);	// �E���̏�����̃{�^�������������̃e�N�X�`����UV
static const UV_INDEX AFTER_BUTTON_RIGHT_DOWN = UV_INDEX(0.25f, 0.5f, 0.5f, 1.0f);	// �E���̉������̃{�^�������������̃e�N�X�`����UV
static const UV_INDEX AFTER_BUTTON_LEFT_UP = UV_INDEX(0.0f, 0.25f, 0.0f, 0.5f);		// �����̏�����̃{�^�������������̃e�N�X�`����UV
static const UV_INDEX AFTER_BUTTON_LEFT_DOWN = UV_INDEX(0.25f, 0.5f, 0.0f, 0.5f);	// �����̉������̃{�^�������������̃e�N�X�`����UV


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
void CCommandChartUI::Init(BUTTON_TYPE ButtonType, D3DXVECTOR3 pos, TEXTURE_TYPE Texture)
{
	// �|���S���̍쐬
	CScene2D::Init(pos, COMMAND_POLYGON_WIDTH, COMMAND_POLYGON_HEIGHT, Texture);

	// ���X�g�ɒǉ�
	CScene2D::AddLinkList(CRenderer::TYPE_RENDER_UI);

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//*************************************************************************
	//	�e�N�X�`����\��t������S�Ẵ|���S���̐F�𔒂ɂ����
	//*************************************************************************
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// �{�^���̎�ނɂ���ĐF��ς��Ă���
	switch (ButtonType)
	{
	case BUTTON_TYPE_1:
		SetUV((UV_INDEX*)&DEFAULT_BUTTON_RIGHT_UP);
		break;
	case BUTTON_TYPE_2:
		SetUV((UV_INDEX*)&DEFAULT_BUTTON_RIGHT_DOWN);
		break;
	case BUTTON_TYPE_3:
		SetUV((UV_INDEX*)&DEFAULT_BUTTON_LEFT_UP);
		break;
	case BUTTON_TYPE_4:
		SetUV((UV_INDEX*)&DEFAULT_BUTTON_LEFT_DOWN);
		break;
	default:
		break;
	}
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
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
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
CCommandChartUI* CCommandChartUI::Create(LPDIRECT3DDEVICE9 *pDevice, BUTTON_TYPE ButtonType, D3DXVECTOR3 pos, TEXTURE_TYPE Texture)
{
	// new
	CCommandChartUI* temp = new CCommandChartUI(pDevice);

	// ������
	temp->Init(ButtonType, pos, Texture);

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

			float fAlpha = 0;

			fAlpha = abs(COMMAND_MAX_ALPHA / fMovement);

			m_Color.a += fAlpha;

			if (m_Color.a > COMMAND_MAX_ALPHA)
			{
				m_Color.a = COMMAND_MAX_ALPHA;
			}

			// �ړ�������
			SetPos(m_Pos);

			// �F��ύX
			SetColorPolygon(m_Color);
		}
		else
		{
			// ���݂̍��W��ړI�̍��W�ɍ��킹��
			m_Pos.x = m_DestPos.x;
			SetPos(m_Pos);
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

			float fAlpha = 0;

			fAlpha = abs(COMMAND_MAX_ALPHA / fMovement);

			m_Color.a += fAlpha;

			if (m_Color.a > COMMAND_MAX_ALPHA)
			{
				m_Color.a = COMMAND_MAX_ALPHA;
			}

			// �ړ�������
			SetPos(m_Pos);

			// �F��ύX
			SetColorPolygon(m_Color);
		}
		else
		{
			// ���݂̍��W��ړI�̍��W�ɍ��킹��
			m_Pos.y = m_DestPos.y;
			SetPos(m_Pos);
			m_isMoveY = false;
		}
	}
	// �����̈ړ����I����Ă��鎞���S�Ɍ����Ă��Ȃ���΂Ȃ�Ȃ�
	else if (m_Color.a != COMMAND_MAX_ALPHA)
	{
		m_Color.a = COMMAND_MAX_ALPHA;
		SetColorPolygon(m_Color);
	}
}

//-----------------------------------------------------------------------------
// ���͂��ꂽ���̃R�}���h�̃e�N�X�`����UV�l�ύX
//-----------------------------------------------------------------------------
void CCommandChartUI::InputUIUVChange(BUTTON_TYPE ButtonType, bool isPush)
{
	// �����̃{�^������ۑ�
	m_ButtonType = ButtonType;

	// �{�^���̎�ނɂ���ĐF��ς��Ă���
	switch (ButtonType)
	{
	case BUTTON_TYPE_1:
		if (!isPush)
		{
			SetUV((UV_INDEX*)&DEFAULT_BUTTON_RIGHT_UP);
			break;
		}
		SetUV((UV_INDEX*)&AFTER_BUTTON_RIGHT_UP);
		break;

	case BUTTON_TYPE_2:
		if (!isPush)
		{
			SetUV((UV_INDEX*)&DEFAULT_BUTTON_RIGHT_DOWN);
			break;
		}
		SetUV((UV_INDEX*)&AFTER_BUTTON_RIGHT_DOWN);
		break;

	case BUTTON_TYPE_3:
		if (!isPush)
		{
			SetUV((UV_INDEX*)&DEFAULT_BUTTON_LEFT_UP);
			break;
		}
		SetUV((UV_INDEX*)&AFTER_BUTTON_LEFT_UP);
		break;

	case BUTTON_TYPE_4:
		if (!isPush)
		{
			SetUV((UV_INDEX*)&DEFAULT_BUTTON_LEFT_DOWN);
			break;
		}
		SetUV((UV_INDEX*)&AFTER_BUTTON_LEFT_DOWN);
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//	�R�}���h���͏I�����̏���
//-----------------------------------------------------------------------------
void CCommandChartUI::EndOfInputCommand(void)
{
	// �R�}���h���͏I�����Ƀ��X�g���珜�O
	CScene2D::UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
}

//-----------------------------------------------------------------------------
//	�R�}���h���͊J�n���̏���
//	�����F���W
//-----------------------------------------------------------------------------
void CCommandChartUI::RestartOfInputCommand(D3DXVECTOR3 pos)
{
	// �R�}���h���͊J�n���Ƀ��X�g�ɒǉ�
	CScene2D::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// ���W��ݒ�
	m_Pos = pos;
	SetPos(m_Pos);
}

// EOF