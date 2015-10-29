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

//-----------------------------------------------------------------------------
//	�O���錾
//-----------------------------------------------------------------------------

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
	void DethNextCommand(void);

private:
	// �R�}���h�ێ��p�z��
	BUTTON_TYPE m_aCommandKeep[MAX_COMMAND_KEEP];
	// �f�o�C�X�̕ێ�
	LPDIRECT3DDEVICE9* m_pD3DDevice;
	// �\��������͂��ꂽ�R�}���hUI�̕ێ�
	CCommandChartUI* m_apCommandUI[MAX_COMMAND_KEEP];
	// �����͌��̃R�}���hUI�̕ێ�
	CCommandChartUI* m_apNextCommandUI[MAX_NEXT_COMMAND_VIEW];
	// ���͌��UI��\������x���W
	float m_fPosX;
	// �\�����Ă���R�}���h�������܂ł̃J�E���g
	int m_nCommandDethCnt;
	// �ێ����̃R�}���h��
	int m_nKeepCommandNum;
	// ������ID
	int m_MyID;
	// �R�}���h���͉\���ǂ����̃t���O
	bool m_isCommandInput;
};

#endif

//EOF