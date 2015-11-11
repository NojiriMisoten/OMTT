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
static const int MAX_NEXT_COMMAND_VIEW = 4;
static const int MY_ID_1 = 0;	// ID(��)
static const int MY_ID_2 = 1;	// ID(��)
static const int MAX_PLAYER = 2;

// �R�}���h�̎��
typedef enum
{
	COMMAND_TYPE_NONE = 0,
	COMMAND_TYPE_CHOP,			// �`���b�v
	COMMAND_TYPE_ELBOW,			// �G���{�[
	COMMAND_TYPE_LARIAT,		// �����A�b�g
	COMMAND_TYPE_ROLLING_ELBOW,	// ���[�����O�G���{�[
	COMMAND_TYPE_FLYING_ELBOW,	// �t���C���O�G���{�[
	COMMAND_TYPE_DROP_KICK,		// �h���b�v�L�b�N
	COMMAND_TYPE_STANER,		// �X�^�i�[
	COMMAND_TYPE_ROPE,			// ���[�v
	COMMAND_TYPE_FINISHER,		// ���ߋZ
	COMMAND_TYPE_MAX
}COMMAND_TYPE;

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

	// ���ɓ��͂�����̃R�}���h�̕\��
	// �����F���ڂ̃R�}���h�\�L��
	void CreateNextCommand(int nNumCommand);

	// ���ɕ\������R�}���h���폜
	void DeathNextCommand(void);

	// ��ʊO�ւ̃t�F�[�h�A�E�g
	void ScreenOut(void);

	// ��ʓ��ւ̃t�F�[�h�C��
	void ScreenIn(void);

private:
	// �f�t�H���g�̏�Ԃɕϐ���߂�
	void SetDefault(void);

	// �R�}���h����
	void InputCommand(void);

	// �R�}���h�̃��Z�b�g
	void ResetCommand(void);

	// �R�}���h�ێ��p�z��
	BUTTON_TYPE m_aCommandKeep[MAX_COMMAND_KEEP];
	// �f�o�C�X�̕ێ�
	LPDIRECT3DDEVICE9* m_pD3DDevice;
	// �\��������͂��ꂽ�R�}���hUI�̕ێ�
	CCommandChartUI* m_apCommandUI[MAX_COMMAND_KEEP];
	// �����͌��̃R�}���hUI�̕ێ�
	CCommandChartUI* m_apNextCommandUI[MAX_NEXT_COMMAND_VIEW];
	// �������̋Z���\���pUI�̃|�C���^�̕ێ�
	CCommandName* m_apCommandName[MAX_NEXT_COMMAND_VIEW];
	// �R�}���h�`���[�g�̔w�ʂɐ�������|���S��
	CScene2D* m_pBackPolygon;
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