//=============================================================================
//
//	CCommandChartUI�N���X�̐錾(CCommandChartUI.h)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CCOMMANDCHARTUI_H__
#define __CCOMMNADCHARTUI_H__

//-----------------------------------------------------------------------------
//	�C���N���[�h
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//-----------------------------------------------------------------------------
//	�}�N����`
//-----------------------------------------------------------------------------
static const int MAX_BUTTON_INFO = 4;

// �{�^���̎��
typedef enum
{
	BUTTON_TYPE_NONE = 0,
	BUTTON_TYPE_1,
	BUTTON_TYPE_2,
	BUTTON_TYPE_3,
	BUTTON_TYPE_4,
	BUTTON_TYPE_MAX
}BUTTON_TYPE;

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class CCommandChartUI : public CScene2D
{
public:
	// �R���X�g���N�^
	// �����F�f�o�C�X
	CCommandChartUI(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType = OBJTYPE_2D);

	// �f�X�g���N�^
	~CCommandChartUI();

	// ������
	// �����F�{�^���̃^�C�v�A���W
	void Init(BUTTON_TYPE ButtonType, D3DXVECTOR3 pos);

	// �X�V
	void Update(void);

	// �I��
	void Uninit(void);

	// �`��
	void Draw(void);

	// ����
	// �߂�l�F����̃|�C���^
	// �����@�F�f�o�C�X�A�{�^���̎�ށA���W
	static CCommandChartUI* Create(LPDIRECT3DDEVICE9 *pDevice, BUTTON_TYPE ButtonType, D3DXVECTOR3 pos);
	
private:
	// �������ǂ̃{�^����
	BUTTON_TYPE m_ButtonType;
};

#endif

//EOF