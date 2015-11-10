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

//=============================================================================
// �O���錾
//=============================================================================
class CPlayerManager;
class CUiManager;
class CFieldManager;
class CCrowdManager;
class CCameraManager;
class CJudgeManager;

typedef enum
{
	GAME_INTRO = 0,
	GAME_BATTLE,
	GAME_FINISH,
	GAME_MAX
}GAME_MODE;

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

	CManager* GetManager(void){ return m_pManager; };

	int GetBattleTimer(void) { return m_BattleTimer; };
private:
	//�@�N���X�����������悳���H
	void GameIntro( void );
	void GameBattle( void );
	void GameFinish( void );

	void InitGameIntro( void );
	void InitGameBattle( void );
	void InitGameFinish( void );

	void UninitGameIntro( void );
	void UninitGameBattle( void );
	void UninitGameFinish( void );

	// �Q�[������UI�𐶐��A�Ǘ�����N���X
	CUiManager *m_pUiManager;

	// �J�����}�l�[�W���[
	CCameraManager *m_pCameraManager;

	// �W���b�W�}�l�[�W���[
	CJudgeManager *m_pJudgeManager;
	
	// �t�B�[���h�S�̂̊Ǘ�
	CFieldManager *m_pFieldManager;
	CCrowdManager *m_pCrowdManager;

	GAME_MODE m_Mode;
	GAME_MODE m_PrevMode;

	int m_BattleTimer;
};

#endif
//----EOF----