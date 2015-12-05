//=============================================================================
//
//	CCommandChartUI�N���X�̐錾(CCommandChartUI.h)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CCOMMANDCHARTUI_H__
#define __CCOMMANDCHARTUI_H__

//-----------------------------------------------------------------------------
//	�C���N���[�h
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//-----------------------------------------------------------------------------
//	�}�N����`
//-----------------------------------------------------------------------------
//static const int MAX_BUTTON_INFO = 4;
static const float COMMAND_POLYGON_WIDTH = 30.0f * (SCREEN_HEIGHT * 0.5f / 150.f);		// �R�}���h�̃|���S���̉���
static const float COMMAND_POLYGON_HEIGHT = 30.0f * (SCREEN_HEIGHT * 0.5f / 150.f);		// �R�}���h�̃|���S���̍���

// �{�^���̎��
typedef enum
{
	BUTTON_TYPE_NONE = 0,
	BUTTON_TYPE_1,		// �E��
	BUTTON_TYPE_2,		// �E��
	BUTTON_TYPE_3,		// ����
	BUTTON_TYPE_4,		// ����
	BUTTON_TYPE_5,		// ���E�������������
	BUTTON_TYPE_6,		// ���E��������������
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
	void Init(BUTTON_TYPE ButtonType, D3DXVECTOR3 pos, TEXTURE_TYPE Texture);

	// �X�V
	void Update(void);

	// �I��
	void Uninit(void);

	// �`��
	void Draw(void);

	// ����
	// �߂�l�F����̃|�C���^
	// �����@�F�f�o�C�X�A�{�^���̎�ށA���W
	static CCommandChartUI* Create(LPDIRECT3DDEVICE9 *pDevice, BUTTON_TYPE ButtonType, D3DXVECTOR3 pos, TEXTURE_TYPE Texture);

	// ���͔��f�t���O�̏�Ԃ�Ԃ�
	// �߂�l�F���͂���Ă���(true)���͂���Ă��Ȃ�(false)
	bool GetInputFlag(void){ return m_isInputCommand; }

	// ���͔��f�t���O�̏�Ԃ�ύX����
	// �����F���͂���Ă���(true)���͂���Ă��Ȃ�(false)
	void SetInputFlag(bool isInputCommand){ m_isInputCommand = isInputCommand; }

	// �������ǂ̃{�^���Ȃ̂���Ԃ�
	// �߂�l�F�����̎��
	BUTTON_TYPE GetButtonType(void){ return m_ButtonType; }

	// �ړ������邽�߂̖ڕW���W�̐ݒ�
	// �����F�ڕW�̍��W
	void SetDestPos(D3DXVECTOR3 DestPos){ m_DestPos = DestPos; m_isMoveX = true; m_isMoveY = true; }

	// �ړ��p�֐�
	void Move(void);

	// ���͂��ꂽ���̃R�}���h�̃{�^���̃e�N�X�`�������炷
	// �����F�{�^���̎��, ������Ă���
	void InputUIUVChange(BUTTON_TYPE ButtonType, bool isPush);

	// �R�}���h���͏I���̎��̏���
	void EndOfInputCommand(void);

	// �R�}���h���͊J�n���̏���
	// �����F���W
	void RestartOfInputCommand(D3DXVECTOR3 pos);

	//********************************************
	//    2015/12/4
	//    �����@�ǋL
	//********************************************
	// �R�}���h�����͌�₾��������Ƀ|���S���`��
	void CandidateInputBackPolygonDraw(void){ m_pBackPolygon->SetDrawFlag(true); }

	// �R�}���h�����͌��Ŗ����Ȃ�������̃|���S��������
	void CandidateInputBackPolygonVanish(void){ m_pBackPolygon->SetDrawFlag(false); }
	//********************************************
	//    2015/12/4
	//    �����@�ǋL
	//********************************************


private:
	// �������ǂ̃{�^����
	BUTTON_TYPE m_ButtonType;
	// �ŏI�I�ȕ\�����W
	D3DXVECTOR3 m_DestPos;
	// �F
	D3DXCOLOR m_Color;
	// ���͂��ꂽ�R�}���h�Ƃ��ĕێ����邩�ǂ����̃t���O
	bool m_isInputCommand;
	// x�����̈ړ����s�����ǂ����̃t���O
	bool m_isMoveX;
	// y�����̈ړ����s�����ǂ����̃t���O
	bool m_isMoveY;

	//********************************************
	//    2015/12/4
	//    �����@�ǋL
	//********************************************
	// �R�}���h�̔w�i�ɕ`�悷��|���S��
	CScene2D* m_pBackPolygon;
	//********************************************
	//    2015/12/4
	//    �����@�ǋL
	//********************************************

};

#endif

//EOF