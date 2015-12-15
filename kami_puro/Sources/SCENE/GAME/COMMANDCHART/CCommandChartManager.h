//=============================================================================
//
//	CCommandChartManager�N���X�̐錾(CCommandChartManager.h)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CCOMMANDCHARTMANAGER_H__
#define __CCOMMANDCHARTMANAGER_H__

//-----------------------------------------------------------------------------
// �C���N���[�h
//-----------------------------------------------------------------------------
#include "../../../MAIN/main.h"
#include "CCommandChart.h"
#include "../PLAYER/CPlayerManager.h"

//-----------------------------------------------------------------------------
//	�}�N��
//-----------------------------------------------------------------------------
static const int MAX_PLAYER_NUM = 2;

//-----------------------------------------------------------------------------
//	�O���錾
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class CCommandChartManager
{
public:
	// �R���X�g���N�^
	CCommandChartManager(CPlayerManager* pPlayerManager);

	// �f�X�g���N�^
	~CCommandChartManager();

	// ������
	// �����F�f�o�C�X
	void Init(LPDIRECT3DDEVICE9* device);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	// �I��
	void Uninit(void);

	// ����
	// �����F�f�o�C�X
	static CCommandChartManager* Create(LPDIRECT3DDEVICE9* device, CPlayerManager* pPlayerManager);

	// �R�}���h�`���[�g�̎擾
	// �����F�v���C���[ID
	// �߂�l�F�R�}���h�`���[�g�̍\�������Z�̃^�C�v
	COMMAND_TYPE GetCommandChartTechnic(int nID){ return m_pCommandChart[nID]->UseTechnic(); }

	// �R�}���h���͂̕ύX
	// �����F�R�}���h���͉\�ɂ���̂Ȃ��true�s�ɂ���̂Ȃ��false
	void SetInputCommandChart(bool isInput){ m_pCommandChart[PLAYER_1]->SetCommandInputFlag(isInput);
											 m_pCommandChart[PLAYER_2]->SetCommandInputFlag(isInput);}

	// ���[�h���Z�b�g
	void SetCommandChartMode(int ID, CCommandChart::MODE_COMMAND_CHART mode);
	// ���[�h�̃Q�b�g
	CCommandChart::MODE_COMMAND_CHART GetCommandChartMode(int ID) { return m_pCommandChart[ID]->GetCommandChartMode(); };


	bool GetCanUseFinishSkill(PLAYER_ID id){ return m_pPlayerManager->GetUseFinishFlag(id); };
	bool GetCanUseRopeSkill(PLAYER_ID id){ return m_pPlayerManager->GetRopeFlag(id); };

	void SetInputCommandChart(int playerID, bool isInput) { m_pCommandChart[playerID]->SetCommandInputFlag(isInput); };

	//================================================================
	// �n���R�}���h�����̏�ԂɂȂ�
	// �Z�o������o�g�����[�h�p���Ȃ炱����
	//================================================================
	void ResetCommandList(int playerID);

	//================================================================
	// �ʒu��e�N�X�`���Ȃǂ̉�����Ăׂ鏉����
	// MODE_APPEAR�ɃZ�b�g����O�ɌĂ�łق���
	//================================================================
	void ResetAllCommand(int playerID);

	// ���[�v���̃I�[�v���A�j�������ݒ�֐�
	void StartOpenAnimeForRope(int playerID);
private:
	// �R�}���h�`���[�g�̃A�h���X�i�[�p�̃|�C���^
	CCommandChart* m_pCommandChart[MAX_PLAYER_NUM];

	CPlayerManager	*m_pPlayerManager;
};

#endif

// EOF