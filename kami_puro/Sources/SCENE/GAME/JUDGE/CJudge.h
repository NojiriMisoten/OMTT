//=============================================================================
//
// CJudge�N���X [CJudge.h]
// Author : �r���@���
//
//=============================================================================
#ifndef _CJUDGE_H_
#define _CJUDGE_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../PLAYER/CPlayerManager.h"
#include "../COMMANDCHART/CCommandChart.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
class CPlayerManager;
class CDirectorManager;
class CCommandChartManager;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	BATTLE_MOVE = 0,
	BATTLE_FIGHT,
	BATTLE_MAX
}BATTLE_MODE;

typedef enum
{
	RPS_NONE = -1,
	RPS_ROCK = 0,
	RPS_PAPER,
	RPS_SCISSOR,
	RPS_MAX
}TYPE_RPS;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CJudge
{
public:
	// �R���X�g���N�^
	CJudge( void );

	// �f�X�g���N�^
	~CJudge( void );

	// �X�V
	void Update( void );

	// �쐬
	static CJudge* Create( CManager* pManager );

	// �I��
	void Uninit( void );

	// �퓬���[�h�̃Q�b�g
	void SetBattleMode( BATTLE_MODE battleMode ) { m_BattleMode = battleMode; };

	// �퓬���[�h�̃Z�b�g
	BATTLE_MODE GetBattleMode( void ) { return m_BattleMode; };

	// �ۑ��p�v���C���[���W�Q�b�g
	D3DXVECTOR3 GetSavePos( PLAYER_ID playerID ) { return m_SavePos[playerID]; };

	void BattleMoveUpdate( void );
	
	void BattleFightUpdate( void );

private:
	void Init( CManager* pManager );

	CManager				*m_pManager;			// �}�l�[�W���[
	CCommandChartManager	*m_pCommandChartManager;
	CPlayerManager			*m_pPlayerManager;
	CDirectorManager		*m_pDirectorManager;

	// �퓬���[�h
	BATTLE_MODE m_BattleMode;
	BATTLE_MODE m_BattleModeOld;

	// �v���C���[���W�ۑ��p
	D3DXVECTOR3 m_SavePos[PLAYER_MAX];
	
	// ���͊֘A
	int m_InputWaitFrameCount[PLAYER_MAX];
	COMMAND_TYPE m_Command[PLAYER_MAX];

	// �v���C��\���W�֘A
	D3DXVECTOR3 m_Pos[PLAYER_MAX];
	D3DXVECTOR3 m_PosCenter;
	float m_Dist[PLAYER_MAX];
	float m_DistTotal;

	// �O�񉉏o�ۑ��p
	int m_IsDirectingOld;
};

#endif
//----EOF----
