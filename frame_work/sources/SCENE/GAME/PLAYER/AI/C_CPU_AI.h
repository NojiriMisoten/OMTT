//=============================================================================
//
// C_CPU_AI�N���X [C_CPU_AI.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _C_CPU_AI_H_
#define _C_CPU_AI_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../../MAIN/main.h"
#include "../../FIELD/CNaviTile.h"

class CPlayer;
class CTreasure;
class CGoal;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class C_CPU_AI
{
public:
	// �R���X�g���N�^
	C_CPU_AI();

	// �f�X�g���N�^
	~C_CPU_AI();

	// �I��
	void	Uninit(void);

	//	�X�V
	void	Update(void);

	// �N���G�C�g
	static C_CPU_AI* Create(CNaviTile* pNaviTile, CPlayer* pPlayer);
	
	// AI�̓��̓Q�b�^�[
	bool GetAIInputRight(void){ return m_AIInput.moveRight; };
	bool GetAIInputLeft(void){ return m_AIInput.moveLeft; };
	bool GetAIInputUp(void){ return m_AIInput.moveUp; };
	bool GetAIInputDown(void){ return m_AIInput.moveDown; };
	bool GetAIInputThread(void){ return m_AIInput.pushThread; };
	bool GetAIInputAtk(void){ return m_AIInput.pushAtk; };
	bool GetAIInputChangeAtk(void){ return m_AIInput.changeAtk; };
	bool GetAIInputChangeSpeed(void){ return m_AIInput.changeSpeed; };
	bool GetAIInputChangeJammer(void){ return m_AIInput.changeJammer; };
private:
	// AI�̎v�l
	typedef enum
	{
		AI_STATE_THINK = 0,						// �s�����l����
		AI_STATE_MOVE_TO_TREASURE,				// ��Ɍ�����
		AI_STATE_ATTACK_WHO,					// �N�����U������
		AI_STATE_ATTACK_HAVE_TREASURE_PLAYER,	// ��������Ă����U������
		AI_STATE_MOVE_GOAL,						// �S�[���Ɍ�����
		AI_STATE_SAVE_TREASURE,					// �S�[���n�_�ŕ�����
		AI_STATE_MAKE_FOOT_STEP,				// ������
		AI_STATE_MOVE_FREE,						// ���R�ړ�
		AI_STATE_MAX
	}AI_STATE;

	// AI�̉������{�^��
	typedef struct
	{
		bool moveRight;
		bool moveLeft;
		bool moveUp;
		bool moveDown;
		bool pushThread;
		bool pushAtk;
		bool changeAtk;
		bool changeSpeed;
		bool changeJammer;
	}AI_INPUT;

	// ������
	void Init(CNaviTile* pNaviTile, CPlayer* pPlayer);

	// �s�����l����
	void ThinkAI(void);

	// ��Ɍ�����
	void MoveToTreasure(void);

	// �N�����U������
	void AttackWho(void);

	// �����玝���Ă����U������
	void AttackHaveTreasurPlayer(void);

	// �S�[���Ɍ�����
	void MoveGoal(void);

	// ������
	void SaveTreasure(void);

	// ������
	void MakeFootStep(void);
	
	// �K���ړ�
	void MoveFree(void);

	// �ڕW�n�_�Ɍ�����
	bool Move(void);

	// �I�[�v�����X�g������
	void InitOpenList(void);

	// �o�H�T��
	bool SarchPath(TileIndex* start, TileIndex* goal);

	// ����̃^�C���T��
	void SarchAroundTile(TileIndex* sarch_base, TileIndex* start, TileIndex* goal);

	// �I�[�v����
	void SetOpen(TileIndex* tile, TileIndex* start, TileIndex* goal, TileIndex* sarch_base);

	// �N���[�Y��
	void SetClose(TileIndex* sarch_base, TileIndex* start, TileIndex* goal);

	// �ȂȂߔ���
	void SlantingSarch(TileIndex* sarch_base, int sarch_index_x, int sarch_index_y);

	// �^�C��������W�Q�b�g
	D3DXVECTOR2* GetOpenListPos(TileIndex* index){ return &m_apTile[index->y_block][index->x_block].centerPos; };

	// ���W����^�C���C���f�b�N�X�Q�b�g
	TileIndex GetOpenListIndex(D3DXVECTOR3* position);

	// ������ꏊ������֐�
	bool JudgeCanMove(TileIndex* sarch_tile);

	// AI�̓��͂̏�����
	void InitAIInput(void);

	// AI�̏�ԕύX
	void ChangeAIState(AI_STATE state);

	CNaviTile*	m_pDefaultNaviTile;
	NAVI_TILE**	m_apTile;		// �o�H�ۑ��p
	int			m_nXBlockNum;	// m_apTile[m_nYBlockNum][m_nXBlockNum]
	int			m_nYBlockNum;
	CPlayer*	m_pOwner;		// �v���C���[�A�h���X
	D3DXVECTOR2 m_vDestPos;		// �ڕW�n�_
	TileIndex	m_DestTile;		// �ڕW�^�C���i���݂̖ڕW�j
	TileIndex	m_GoalTile;		// �S�[���^�C���i�ŏI�ڕW�j
	AI_STATE	m_AIState;		// AI�̏��
	AI_STATE	m_AIStateOld;	// AI�̏��
	AI_INPUT	m_AIInput;		// AI�̓���
	AI_INPUT	m_AIInputOld;	// AI�̓���
	int			m_nTimer;		// �s���Ǘ��p�^�C�}�[
	int			m_nSarchTimer;	// �o�H�T�����Ă���̎���
	CTreasure*	m_pTreasure;	// ��
	CGoal*		m_pGoal;		// �S�[��
	CPlayer*	m_pAimPlayer;	// �_���Ă���v���C���[

#ifdef _DEBUG
	void Printstate(void);
#endif
};


#endif
//----EOF-----