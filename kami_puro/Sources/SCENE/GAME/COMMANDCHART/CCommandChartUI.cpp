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
static const float COMMAND_POLYGON_WIDTH = 150.0f;	// �R�}���h�̃|���S���̉���
static const float COMMAND_POLYGON_HEIGHT = 50.0f;	// �R�}���h�̃|���S���̍���

//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
CCommandChartUI::CCommandChartUI(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) : CScene2D(pDevice, objType)
{
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
		SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		break;
	case BUTTON_TYPE_2:
		SetColorPolygon(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		break;
	case BUTTON_TYPE_3:
		SetColorPolygon(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		break;
	case BUTTON_TYPE_4:
		SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	default:
		break;
	}
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
CCommandChartUI* CCommandChartUI::Create(LPDIRECT3DDEVICE9 *pDevice,  BUTTON_TYPE ButtonType, D3DXVECTOR3 pos)
{
	// new
	CCommandChartUI* temp = new CCommandChartUI(pDevice);

	// ������
	temp->Init(ButtonType, pos);

	// �Ԃ�
	return temp;
}

// EOF