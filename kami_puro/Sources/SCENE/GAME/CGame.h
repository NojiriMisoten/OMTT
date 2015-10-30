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

typedef enum
{
	GAME_INTRO = 0,
	GAME_BATTLE,
	GAME_FINISH,
	GAME_MAX
}GAME_MODE;

typedef enum
{
	BATTLE_MOVE = 0,
	BATTLE_FIGHT,
	BATTLE_MAX
}BATTLE_MODE;
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
	void GameIntro(void);
	void GameBattle(void);
	void GameFinish(void);

	// �Q�[������UI�𐶐��A�Ǘ�����N���X
	CUiManager *m_pUiManager;
	// �t�B�[���h�u�����̂��Ǘ�����N���X
	CFieldManager *m_pFieldManager;

	GAME_MODE m_Mode;
	GAME_MODE m_PrevMode;
	BATTLE_MODE m_BattleMode;

	int m_BattleTimer;
};

#endif
//----EOF----