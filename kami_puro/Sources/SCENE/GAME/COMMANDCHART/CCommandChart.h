//=============================================================================
//
//	CCommandChart�N���X�̐錾(CCommandChart.h)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CCOMMANDCHART_H__
#define __CCOMMNADCHART_H__

//-----------------------------------------------------------------------------
//	�C���N���[�h
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "../../../INPUT/CInputKeyboard.h"

//-----------------------------------------------------------------------------
//	�}�N����`
//-----------------------------------------------------------------------------
// �R�}���h�̍ő���͐�
static const int MAX_COMMAND_KEEP = 6;
static const int MAX_NEXT_COMMAND_VIEW = 4;

// �v���C���[�ԍ��̗�
typedef enum
{
	NO_PLAYER =0,
	PLAYER_NUMBER_1,
	PLAYER_NUMBER_2,
	MAX_PLAYER_NUM
}PLAYER_NUMBER;

//-----------------------------------------------------------------------------
//	�O���錾
//-----------------------------------------------------------------------------
class CCommandChartUI;

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class CCommandChart
{
public:
	// �R���X�g���N�^
	// �����F�f�o�C�X�A�v���C���[�ԍ�
	CCommandChart(LPDIRECT3DDEVICE9* pDevice, PLAYER_NUMBER PlayerNumber);

	// �f�X�g���N�^
	~CCommandChart();

	// ������
	void Init(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �I��
	void Uninit(void);

	// ���g�̐���
	// �����F�f�o�C�X�̃|�C���^�A�����̃v���C���[�ԍ�
	// �߂�l�F���g�̃|�C���^�A�v���C���[�ԍ�
	static CCommandChart* Create(LPDIRECT3DDEVICE9* pDevice, PLAYER_NUMBER PlayerNumber);

	// ���ɓ��͂�����̃R�}���h�̕\��
	// �����F���ڂ̃R�}���h�\�L��
	void CreateNextCommand(int nNumCommand);

	// ���ɕ\������R�}���h���폜
	void DethNextCommand(void);

private:
	// �f�o�C�X�̕ێ�
	LPDIRECT3DDEVICE9* m_pD3DDevice;
	// �ێ����̃R�}���h��
	int m_nKeepCommandNum;
	// �\��������͂��ꂽ�R�}���hUI�̕ێ�
	CCommandChartUI* m_apCommandUI[MAX_COMMAND_KEEP];
	// �����͌��̃R�}���hUI�̕ێ�
	CCommandChartUI* m_apNextCommandUI[MAX_NEXT_COMMAND_VIEW];
	// ���͌��UI��\������x���W
	float m_fPosX;
	// �\�����Ă���R�}���h�������܂ł̃J�E���g
	int m_nCommandDethCnt;
	// �R�}���h���͉\���ǂ����̃t���O
	bool m_isCommandInput;
	// �R�}���h�ێ��p�z��
	KEYBOARD_CODE m_aCommandKeep[MAX_COMMAND_KEEP];
	// �����̃v���C���[�ԍ�
	PLAYER_NUMBER m_MyNumber;
};

#endif

//EOF