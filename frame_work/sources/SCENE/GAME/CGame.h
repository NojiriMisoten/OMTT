//=============================================================================
//
// CGame�N���X [CGame.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CGAME_H_
#define _CGAME_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"
#include "../../TEXTURE/CTexture.h"
#include "../PAUSE/CPause.h"
#include "CTimeManager.h"

//=============================================================================
// �O���錾
//=============================================================================
class CTimeManager;
class CTreasureManager;
class CGoalManager;
class CPlayerManager;
class CAttackManager;
class CThreadManager;
class CJudgeManager;
class CFieldManager;
class CBackGroundManager;
class CCountDown;
class CEffectManager;
class CWinDraw;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CPhase
{
	// ���J�����o
public:
	// �R���X�g���N�^
	CGame(void);

	// �f�X�g���N�^
	~CGame(void);

	//======================================================
	// ������
	// ����: �t�F�[�Y���[�h�A�f�o�C�X
	//======================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	//======================================================
	// �N���G�C�g
	// ����: �t�F�[�Y���[�h�A�f�o�C�X
	// �߂�l: �������̃|�C���^
	//======================================================
	static CGame* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// �Q�[���I���Z�b�g
	void SetGameOver(){
		m_bGameOver = true;
	}
	
	//======================================================
	// ������̓Q�[�����̃��U���g�V�[����
	// �ڂ�Ƃ��Ɉ��̂݌Ă΂�� Init�I�ȁB
	//======================================================
	// �������v���C���ԍ����Z�b�g
	void SetWinPlayer(short num);
	// ���������ɂ���
	void SetDraw();

	// ����J�����o
private:
	// �Q�[�����̃��U���g�V�[���̍X�V ���t���[���Ă΂��
	void Result();

	// �|�[�Y
	CPause *m_pPause;
	// �Q�[���̃^�C���Ǘ���
	CTimeManager *m_pTimeManager;
	// �󕨊Ǘ���
	CTreasureManager *m_pTreasureManager;
	// �S�[���Ǘ���
	CGoalManager *m_pGoalManager;
	// �v���C���}�l�[�W��
	CPlayerManager *m_pPlayerManager;
	// �U���}�l�[�W��
	CAttackManager * m_pAttackManager;
	// ���}�l�[�W��
	CThreadManager * m_pThreadManager;
	// �W���b�W�}�l�[�W���[
	CJudgeManager* m_pJudgeManager;
	// �t�B�[���h�}�l�[�W���[
	CFieldManager* m_pFieldManager;
	// �w�i�}�l�[�W���[
	CBackGroundManager *m_pBackGroundManager;
	// �G�t�F�N�g�}�l�[�W���[
	CEffectManager *m_pEffectManager;
	// �J�E���g�_�E��
	CCountDown* m_pCountDown;
	// ���������������S
	CWinDraw *m_pWinDrawLogo;

	// �Q�[���J�n�t���O(�v���C���𑀍�ł���t���O)
	bool m_bPlayerControl;
	// �Q�[���N���A�t���O
	bool m_bGameOver;
	// �������v���C���[�ԍ�
	short m_nWinPlayerNum;
	// ��񂾂�BGM��炷���߂̃t���O ���ʂ�����true
	bool m_bPlaySoundOnece;

	// �Q�[����̊ȈՃ��U���g���o���Ă���V�[���J�ڂ���܂ł̃J�E���g
	int m_nResultCount;

	// ��������v���C����
	short m_nPlayerNumManual;
	short m_nPlayerNumCpu;

	// ��������X�e�[�W�ԍ�
	int m_nStageNum;
};

#endif
//----EOF----