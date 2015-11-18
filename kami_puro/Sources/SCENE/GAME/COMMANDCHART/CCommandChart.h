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
#include "CCommandChartUI.h"

//-----------------------------------------------------------------------------
//	�}�N����`
//-----------------------------------------------------------------------------
// �R�}���h�̍ő���͐�
static const int MAX_COMMAND_KEEP = 6;
// ���̓��͌��̋Z�̍ő吔
static const int MAX_NEXT_COMMAND_VIEW = 4;
static const int MY_ID_1 = 0;	// ID
static const int MY_ID_2 = 1;	// ID
// �v���C���[�̐�
static const int MAX_PLAYER = 2;

// �R�}���h�̓��͐�
// �����Z
static const int COMMAND_INPUT_NUM_MONO = 1;
// ���Z
static const int COMMAND_INPUT_NUM_SMALL = 3;
// ���Z
static const int COMMAND_INPUT_NUM_MIDDLE = 4;
// ��Z
static const int COMMAND_INPUT_NUM_LARGE = 5;
// ���ߋZ
static const int COMMAND_INPUT_NUM_FINISHER = 6;
// �Z�̎��(�㒆���̂R��ނ̂���)
static const int COMMAND_TYPE_NUM = 3;

// �Z�̎��
typedef enum
{
	COMMAND_TYPE_CHOP = 0,		// �`���b�v
	COMMAND_TYPE_ELBOW,			// �G���{�[
	COMMAND_TYPE_LARIAT,		// �����A�b�g
	COMMAND_TYPE_ROLLING_ELBOW,	// ���[�����O�G���{�[
	COMMAND_TYPE_FLYING_ELBOW,	// �t���C���O�G���{�[
	COMMAND_TYPE_DROP_KICK,		// �h���b�v�L�b�N
	COMMAND_TYPE_FACE_SLAPPING,	// �r���^
	COMMAND_TYPE_BACKDROP,		// �o�b�N�h���b�v
	COMMAND_TYPE_STANER,		// �X�^�i�[
	COMMAND_TYPE_ROPE,			// ���[�v
	COMMAND_TYPE_FINISHER,		// ���ߋZ
	COMMAND_TYPE_MAX,			// �Z�̐�
	COMMAND_TYPE_NONE			// �R�}���h���s����������
}COMMAND_TYPE;

//-----------------------------------------------------------------------------
//	�\���̒�`
//-----------------------------------------------------------------------------
// �R�}���h���
struct COMMAND_INFO
{
	const int m_nCommandLength;
	const COMMAND_TYPE m_CommandType;
	const BUTTON_TYPE* m_Command;
};

// �R�}���hUI�̕ۑ����
struct COMMAND_UI_INFO
{
	COMMAND_UI_INFO* m_NextCommand;		// ���̌��̃{�^�����
	CCommandChartUI* m_pUIType;			// �R�}���h�`���[�gUI�̃|�C���^�̕ێ�
	bool m_isInputButton;				// ���̃{�^���̓��͊m�F
};

//-----------------------------------------------------------------------------
//	�O���錾
//-----------------------------------------------------------------------------
class CCommandName;

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class CCommandChart
{
public:
	// �R���X�g���N�^
	// �����F�f�o�C�X�A�v���C���[�ԍ�
	CCommandChart(LPDIRECT3DDEVICE9* pDevice, int nID);

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
	static CCommandChart* Create(LPDIRECT3DDEVICE9* pDevice, int nID);

	// ��ʊO�ւ̃t�F�[�h�A�E�g
	void ScreenOut(void);

	// ��ʓ��ւ̃t�F�[�h�C��
	void ScreenIn(void);

	// �ŏI�I�ɌJ��o���Z�𒲂ׂĕԂ�
	// �߂�l�F�J��o���Z�̃^�C�v
	COMMAND_TYPE UseTechnic(void);

private:
	//*************************************
	// �֐�
	//*************************************
	// �f�t�H���g�̏�Ԃɕϐ���߂�
	void SetDefault(void);

	// �R�}���h����
	void InputCommand(void);

	// �R�}���h�̃��Z�b�g
	void ResetCommand(void);

	// ���ɕ\������R�}���h���폜
	void DeathNextCommand(void);

	// ��ԏ��߂ɕ\��������͌��̃R�}���h�̍쐬
	void CreateFirstCommand(void);
	
	// ���ɓ��͂�����̃R�}���h�̃`�F�b�N
	// �߂�l�F���͌��ƂȂ�R�}���h���̔z��̃|�C���^
	COMMAND_INFO* NextCommandCheck(void);

	// ���ɓ��͂�����̃R�}���h�̍쐬
	// �����F���ڂ̃R�}���h�\�L��
	void CreateNextCommand(int nNumCommand);

	// �R�}���h�̃`�F�b�N
	// �����F�Z���̍\���̂̔z��
	// �߂�l�F�R�}���h���������������ۂ�
	bool CheckCommand(COMMAND_INFO* Technic);

	// �E��L�[�J�n�̃R�}���h�`���[�g�̐���
	void CreateRightUpTechnicCommand(int nNumCommand);

	// ����L�[�J�n�̃R�}���h�`���[�g�̐���
	void CreateLeftUpTechnicCommand(int nNumCommand);

	// �����L�[�J�n�̃R�}���h�`���[�g�̐���
	void CreateLeftDownTechnicCommand(int nNumCommand);

	// �R�}���hUI�������ꂽ��Ԃɂ��邩���肵�ĉ�����Ă����Ԃɂ���̂ł���Ή����ꂽ��Ԃɂ���
	void CommandUIInput(BUTTON_TYPE button);

	//*************************************
	// �ϐ�
	//*************************************
	// �R�}���h�ێ��p�ϐ�
	BUTTON_TYPE m_aCommandKeep;
	// �f�o�C�X�̕ێ�
	LPDIRECT3DDEVICE9* m_pD3DDevice;
	// �\��������͂��ꂽ�R�}���hUI�̕ێ�
	//CCommandChartUI* m_apCommandUI[MAX_COMMAND_KEEP];
	// �����͌��̃R�}���hUI�̕ێ�
	CCommandChartUI* m_apNextCommandUI[MAX_NEXT_COMMAND_VIEW];
	// �������̋Z���\���pUI�̃|�C���^�̕ێ�
	CCommandName* m_apCommandName[MAX_NEXT_COMMAND_VIEW];
	// �R�}���h�`���[�g�̔w�ʂɐ�������|���S��
	CScene2D* m_pBackPolygon;
	// �R�}���hUI�̃��X�g�̍ŏ��̃|�C���^
	COMMAND_UI_INFO* m_pCommandUI[COMMAND_TYPE_NUM];
	// ���͌��UI��\������x���W
	float m_fPosX;
	// �\�����Ă���R�}���h�������܂ł̃J�E���g
	int m_nCommandDeathCnt;
	// �ێ����̃R�}���h��
	int m_nKeepCommandNum;
	// ������ID
	int m_MyID;
	// �R�}���h���͉\���ǂ����̃t���O
	bool m_isCommandInput;
};

#endif

//EOF