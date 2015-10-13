//=============================================================================
//
// C_CPU_AI�N���X [C_CPU_AI.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "C_CPU_AI.h"
#include "mersenne_twister.h"
#include "../../TREASURE/CTreasure.h"
#include "../../GOAL/CGoal.h"
#include "../CPlayerManager.h"
#include "../CPlayer.h"
//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int RETRY_SERCH_PATH_TIME = 300;			// �ēx�o�H�T������܂ł̎���
static const int BEFORE_MOVE_MAKE_FOOT_STEP_TIME = 120;	// ������܂ł̓�������
static const float JUDGE_ZERO_FOR_MOVE = 3.0f;			// �ۂߌ덷
static const int FREE_MOVE_TIME = 240;					// ���R�ړ��̎���
static const int FREE_MOVE_THINK = 30;					// ���R�ړ��̍l����Ԋu����
static const int MAX_PERCENT = 100;						// �m���̍ő�
static const int HALF_PERCENT = 50;						// 50%
static const int MAX_MOVE_TIME_FOR_ATTACK = 600;		// �U���̂��߂̈ړ�����
static const int THINK_CHANGE_MODE_TIME = 300;			// �`�ԕύX���l����Ԋu

//=============================================================================
// �R���X�g���N�^
//=============================================================================
C_CPU_AI::C_CPU_AI()
{
	m_pDefaultNaviTile = NULL;
	m_pOwner = NULL;
	m_vDestPos = D3DXVECTOR2(0.f, 0.f);
	m_DestTile.x_block = m_DestTile.y_block = 0;
	m_AIStateOld = m_AIState = AI_STATE_THINK;
	InitAIInput();
	m_nTimer = 0;
	m_pTreasure = NULL;
	m_pGoal = NULL;
	m_nSarchTimer = 0;
	m_pAimPlayer = NULL;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
C_CPU_AI::~C_CPU_AI()
{

}

//=============================================================================
// ������
//=============================================================================
void C_CPU_AI::Init(CNaviTile* pNaviTile, CPlayer* pPlayer)
{
	m_pDefaultNaviTile = pNaviTile;
	m_nXBlockNum = m_pDefaultNaviTile->GetXBlockNum();
	m_nYBlockNum = m_pDefaultNaviTile->GetYBlockNum();

	// �^�C���쐬
	m_apTile = new NAVI_TILE*[m_nYBlockNum];
	for (int i = 0; i < m_nYBlockNum; ++i)
	{
		m_apTile[i] = new NAVI_TILE[m_nXBlockNum];
	}

	// �f�t�H���g�̃^�C�����R�s�[
	InitOpenList();

	// �v���C���[�̃A�h���X�ۑ�
	m_pOwner = pPlayer;

	// ��̃A�h���X�Z�b�g
	CScene *pScene, *pSceneNext;	
	int priority = TYPE_PRIORITY_TRESURE;
	// ��͈�Ȃ̂Ő擪�̂�
	pScene = CScene::GetTopAddress(priority);
	m_pTreasure = (CTreasure*)pScene;
	if (!m_pTreasure)
	{
#ifdef _DEBUG
		MessageBox(NULL, "���ɍ����\n", "�G���[", MB_OK | MB_ICONERROR);
#endif
	}

	priority = TYPE_PRIORITY_GOAL;
	pScene = CScene::GetTopAddress(priority);
	// �|�C���^��NULL�łȂ����
	while (pScene)
	{
		// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
		pSceneNext = pScene->GetNextAddress();

		CGoal* goal = (CGoal*)pScene;
		if (goal->GetPlayerNum() == m_pOwner->GetPlayerNum())
		{
			m_pGoal = goal;
			break;
		}
		// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
		pScene = pSceneNext;
	}

	if (!m_pTreasure)
	{
#ifdef _DEBUG
		MessageBox(NULL, "�S�[����ɍ����\n", "�G���[", MB_OK | MB_ICONERROR);
#endif
	}
}

//=============================================================================
// �I��
//=============================================================================
void C_CPU_AI::Uninit(void)
{
	for (int i = 0; i < m_nYBlockNum; ++i)
	{
		if (m_apTile[i])
		{
			delete[] m_apTile[i];
			m_apTile[i] = NULL;
		}
	}
	delete[] m_apTile;
}

//=============================================================================
//	�X�V
//=============================================================================
void C_CPU_AI::Update(void)
{
	m_AIInputOld = m_AIInput;

	// ���͂̏�����
	InitAIInput();
	
	switch (m_AIState)
	{
	case AI_STATE_THINK:
		ThinkAI();
		break;

	case AI_STATE_MOVE_TO_TREASURE:
		MoveToTreasure();
		break;

	case AI_STATE_ATTACK_WHO:
		AttackWho();
		break;

	case AI_STATE_ATTACK_HAVE_TREASURE_PLAYER:
		AttackHaveTreasurPlayer();
		break;

	case AI_STATE_MOVE_GOAL:
		MoveGoal();
		break;

	case AI_STATE_SAVE_TREASURE:
		SaveTreasure();
		break;

	case AI_STATE_MAKE_FOOT_STEP:
		MakeFootStep();
		break;

	case AI_STATE_MOVE_FREE:
		MoveFree();
		break;

	default:
		break;
	}

#ifdef _DEBUG
	Printstate();
#endif
}

//=============================================================================
// �N���G�C�g
//=============================================================================
C_CPU_AI* C_CPU_AI::Create(CNaviTile* pNaviTile, CPlayer* pPlayer)
{
	C_CPU_AI* p = new C_CPU_AI;

	p->Init(pNaviTile, pPlayer);

	return p;
}

//=============================================================================
// �I�[�v�����X�g������
//-----------------------------------------------------------------------------
void C_CPU_AI::InitOpenList(void)
{
	// �f�t�H���g�̃^�C�����R�s�[
	m_pDefaultNaviTile->CopyTile(m_apTile);
}

//=============================================================================
// �o�H�T��
//-----------------------------------------------------------------------------
bool C_CPU_AI::SarchPath(TileIndex* start, TileIndex* goal)
{
	bool end_flag = false;
	bool sarched_flag = false;
	bool dead_end_flag = true;
	TileIndex current_sarch_center = *goal;	// ���݂ǂ��𒆐S�ɒ��ׂĂ��邩
	int current_sarch_score = 1;			// ���ݒ��ׂ�R�X�g
	m_GoalTile = *goal;
	m_nSarchTimer = 0;
	m_vDestPos = m_apTile[goal->y_block][goal->x_block].centerPos;


	// �I�[�v�����X�g������
	InitOpenList();

	// �S�[���̂������Ȃ��ꏊ�Ȃ���Ȃ�
	if (!m_apTile[goal->y_block][goal->x_block].canMove)
	{
		return false;
	}

	// �S�[���̐ݒ�
	m_apTile[goal->y_block][goal->x_block].cost = 0;
	m_apTile[goal->y_block][goal->x_block].heuristic = sqrtf((float)(start->x_block - goal->x_block) * (start->x_block - goal->x_block)
														+ (start->y_block - goal->y_block) * (start->y_block - goal->y_block));
	m_apTile[goal->y_block][goal->x_block].parent_tile = *goal;
	m_apTile[goal->y_block][goal->x_block].score = (float)(m_apTile[goal->y_block][goal->x_block].cost
														+ m_apTile[goal->y_block][goal->x_block].heuristic);

	// �N���[�Y��
	SetClose(goal, start, goal);

	// �o�H�T��
	while (end_flag == false)
	{
		sarched_flag = false;
		dead_end_flag = true;
		for (int y_block = 0; y_block < m_nYBlockNum; ++y_block)
		{
			for (int x_block = 0; x_block < m_nXBlockNum; ++x_block)
			{
				// �J���Ă�Ȃ�
				if (m_apTile[y_block][x_block].search == TILE_MODE_OPEN)
				{
					// �܏��H�łȂ�
					dead_end_flag = false;
					
					// �T���ΏۃX�R�A�Ȃ�
					if ((int)m_apTile[y_block][x_block].score == current_sarch_score)
					{
						current_sarch_score = 0;
						current_sarch_center.y_block = y_block;
						current_sarch_center.x_block = x_block;
						SetClose(&current_sarch_center, start, goal);
						sarched_flag = true;
					}
				}
			}
		}

		// �܏��H�Ȃ�
		if (dead_end_flag == true)
		{
			// ���ꎅ�o��
			ChangeAIState(AI_STATE_MAKE_FOOT_STEP);

			return false;
		}

		if (sarched_flag == false)
		{
			// �T���ΏۃX�R�A���₷
			current_sarch_score++;
		}

		// �X�^�[�g�n�_�܂ŒT��������
		if (m_apTile[start->y_block][start->x_block].search != TILE_MODE_NULL)
		{
			end_flag = true;
			TileIndex tile = m_apTile[start->y_block][start->x_block].parent_tile;
			
			// �G���[�`�F�b�N
			if (tile.x_block < 0 || tile.y_block < 0)
			{
				return false;
			}
			m_vDestPos = m_apTile[tile.y_block][tile.x_block].centerPos;
			m_DestTile = tile;
		}
	}

	return true;
}

//=============================================================================
// ����̃^�C���T��
//-----------------------------------------------------------------------------
void C_CPU_AI::SarchAroundTile(TileIndex* sarch_base, TileIndex* start, TileIndex* goal)
{
	for (int sarch_z = -1; sarch_z < 2; ++sarch_z)
	{
		if (sarch_z + sarch_base->y_block < 0 || sarch_z + sarch_base->y_block >= m_nYBlockNum)
		{
			continue;
		}

		for (int sarch_x = -1; sarch_x < 2; ++sarch_x)
		{
			if (sarch_x + sarch_base->x_block < 0 || sarch_x + sarch_base->x_block >= m_nXBlockNum)
			{
				continue;
			}

			// ���ׂĂȂ����
			if (m_apTile[sarch_z + sarch_base->y_block][sarch_x + sarch_base->x_block].search == TILE_MODE_NULL)
			{
				TileIndex tile;
				tile.x_block = sarch_x + sarch_base->x_block;
				tile.y_block = sarch_z + sarch_base->y_block;

				// �I�[�v��
				SetOpen(&tile, start, goal, sarch_base);

				// �ȂȂߔ���
				SlantingSarch(sarch_base, sarch_x, sarch_z);
			}
		}
	}
}

//=============================================================================
// �I�[�v����
//-----------------------------------------------------------------------------
void C_CPU_AI::SetOpen(TileIndex* tile, TileIndex* start, TileIndex* goal, TileIndex* sarch_base)
{
	float length = 0;
	D3DXVECTOR2 pos_gorl, pos_now;

	// �I�[�v����
	m_apTile[tile->y_block][tile->x_block].search = TILE_MODE_OPEN;

	// �R�X�g
	pos_now = *GetOpenListPos(tile);
	pos_gorl = *GetOpenListPos(goal);

	length = (float)((goal->x_block - tile->x_block) * (goal->x_block - tile->x_block)
					+ (goal->y_block - tile->y_block) * (goal->y_block - tile->y_block));
	m_apTile[tile->y_block][tile->x_block].cost = length;

	// �q���[���X�e�B�b�N
	pos_now = *GetOpenListPos(tile);
	pos_gorl = *GetOpenListPos(start);

	length = (float)((start->x_block - tile->x_block) * (start->x_block - tile->x_block)
					+ (start->y_block - tile->y_block) * (start->y_block - tile->y_block));
	m_apTile[tile->y_block][tile->x_block].heuristic = length;

	// ���v�R�X�g
	m_apTile[tile->y_block][tile->x_block].score = m_apTile[tile->y_block][tile->x_block].cost + m_apTile[tile->y_block][tile->x_block].heuristic;

	// �e�^�C��
	m_apTile[tile->y_block][tile->x_block].parent_tile.x_block = sarch_base->x_block;
	m_apTile[tile->y_block][tile->x_block].parent_tile.y_block = sarch_base->y_block;
}

//=============================================================================
// �N���[�Y��
//-----------------------------------------------------------------------------
void C_CPU_AI::SetClose(TileIndex* sarch_base, TileIndex* start, TileIndex* goal)
{
	if (!m_apTile[sarch_base->y_block][sarch_base->x_block].canMove)
	{
		// �i���֎~��
		m_apTile[sarch_base->y_block][sarch_base->x_block].search = TILE_MODE_STOP;
		return;
	}
	// �T���ς݂�
	m_apTile[sarch_base->y_block][sarch_base->x_block].search = TILE_MODE_CLOSE;

	// ����T��
	SarchAroundTile(sarch_base, start, goal);
}

//=============================================================================
// ���W�ϊ� �^�C���ɕϊ�
//-----------------------------------------------------------------------------
TileIndex C_CPU_AI::GetOpenListIndex(D3DXVECTOR3* pos)
{
	TileIndex ans;
	int blockSizeX = SCREEN_WIDTH / m_nXBlockNum;
	int blockSizeY = SCREEN_HEIGHT / m_nYBlockNum;

	// �t�B�[���h�̍���ƉE�[
	D3DXVECTOR3 left_top(0.f, 0.f, 0.f);
	D3DXVECTOR3 right_down((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.f);

	if (pos->x < left_top.x || pos->y < left_top.y || pos->x > right_down.x || pos->y > right_down.y)
	{
		ans.x_block = 0;
		ans.y_block = 0;
		return ans;
	}
	
	ans.x_block = (int)(pos->x / blockSizeX);
	ans.y_block = (int)(pos->y / blockSizeY);

	return ans;
}

//=============================================================================
// �ȂȂߔ���
//-----------------------------------------------------------------------------
void C_CPU_AI::SlantingSarch(TileIndex* sarch_base, int sarch_index_x, int sarch_index_z)
{
	// �΂߂͂����Ȃ��̂ł���ȏ���

	// �΂߂̔���
	if (sarch_index_z == -1)
	{
		if (sarch_index_x == -1)
		{
			m_apTile[sarch_index_z + sarch_base->y_block][sarch_index_x + sarch_base->x_block].search = TILE_MODE_NULL;
		}

		if (sarch_index_x == 1)
		{
			m_apTile[sarch_index_z + sarch_base->y_block][sarch_index_x + sarch_base->x_block].search = TILE_MODE_NULL;
		}
	}

	if (sarch_index_z == 1)
	{
		if (sarch_index_x == -1)
		{
			m_apTile[sarch_index_z + sarch_base->y_block][sarch_index_x + sarch_base->x_block].search = TILE_MODE_NULL;
		}

		if (sarch_index_x == 1)
		{
			m_apTile[sarch_index_z + sarch_base->y_block][sarch_index_x + sarch_base->x_block].search = TILE_MODE_NULL;
		}
	}
}

//=============================================================================
// ������ꏊ������֐�
//-----------------------------------------------------------------------------
bool C_CPU_AI::JudgeCanMove(TileIndex* sarch_tile)
{
	if (sarch_tile->x_block < 0 || sarch_tile->y_block < 0)
	{
		return false;
	}

	// �s���Ȃ��ꏊ�Ȃ�
	if (!m_apTile[sarch_tile->y_block][sarch_tile->x_block].canMove)
	{
		return false;
	}

	return true;
}

//=============================================================================
// �s�����l����
//=============================================================================
void C_CPU_AI::ThinkAI(void)
{
	TileIndex start, goal;
	// ��������������ĂȂ��Ȃ�
	if (m_pOwner->GetTreasure() == NULL)
	{
		// �}�l�[�W���[�擾
		CPlayerManager* pPlayerManager = m_pOwner->GetPlayerManager();
		CPlayer* otherPlayer = NULL;
		for (int playerID = 0; playerID < MAXIMUM_NUMBER_OF_PLAYER; ++playerID)
		{
			// �����͂��Ȃ�
			if (playerID == m_pOwner->GetPlayerNum())
			{
				continue;
			}

			// ���̃v���C���[�擾
			otherPlayer = pPlayerManager->GetPlayer(playerID);
			if (!otherPlayer)
			{
				continue;
			}

			// ���̃v���C���[���󎝂��ĂȂ��Ȃ�
			if (!otherPlayer->GetTreasure())
			{
				continue;
			}

			// �����Ă���_��
			ChangeAIState(AI_STATE_ATTACK_HAVE_TREASURE_PLAYER);
			start = GetOpenListIndex(&m_pOwner->GetPos());
			goal = GetOpenListIndex(&otherPlayer->GetPos());
			SarchPath(&start, &goal);
			m_pAimPlayer = otherPlayer;
			return;

		}

		int ans = mersenne_twister_int(0, MAX_PERCENT);
		// 80%�ŕ�Ɍ�����
		if (ans < (int)(MAX_PERCENT * 0.8f))
		{
			ChangeAIState(AI_STATE_MOVE_TO_TREASURE);
			start = GetOpenListIndex(&m_pOwner->GetPos());
			goal = GetOpenListIndex(&m_pTreasure->GetPos());
			SarchPath(&start, &goal);
			return;
		}
		// ���ꂩ�K���ɍU������
		else
		{
			bool loop = true;
			int limitCounter = 50;
			int playerID = mersenne_twister_int(0, MAXIMUM_NUMBER_OF_PLAYER);
			while (loop)
			{
				// �������[�v�h�~
				limitCounter--;
				if (limitCounter < 0)
				{
					return;
				}

				// �����͂��Ȃ�
				if (playerID == m_pOwner->GetPlayerNum())
				{
					continue;
				}

				// ���̃v���C���[�擾
				otherPlayer = pPlayerManager->GetPlayer(playerID);
				if (!otherPlayer)
				{
					continue;
				}
				loop = false;
			}
			ChangeAIState(AI_STATE_ATTACK_WHO);
			start = GetOpenListIndex(&m_pOwner->GetPos());
			goal = GetOpenListIndex(&otherPlayer->GetPos());
			SarchPath(&start, &goal);
			m_pAimPlayer = otherPlayer;
			return;
		}
	}
	// �󎝂��Ă���S�[���֌�����
	else
	{
		ChangeAIState(AI_STATE_MOVE_GOAL);
		return;
	}
}

//=============================================================================
// ��Ɍ�����
//=============================================================================
void C_CPU_AI::MoveToTreasure(void)
{
	m_nTimer++;
	m_nSarchTimer++;

	if (m_nSarchTimer > RETRY_SERCH_PATH_TIME)
	{
		// 1/10�Ŗ������Ď��R�ړ�
		int ans = mersenne_twister_int(0, MAX_PERCENT);
		if ((float)ans < 0.1f)
		{
			ChangeAIState(AI_STATE_MOVE_FREE);
			return;
		}
		TileIndex start, goal;
		start = GetOpenListIndex(&m_pOwner->GetPos());
		goal = GetOpenListIndex(&m_pTreasure->GetPos());
		
		// �����Ȃ����
		if (!SarchPath(&start, &goal))
		{
			ans = mersenne_twister_int(0, MAX_PERCENT);
			if (ans < HALF_PERCENT)
			{
				ChangeAIState(AI_STATE_MOVE_FREE);
				return;
			}
			else
			{
				ChangeAIState(AI_STATE_MAKE_FOOT_STEP);
				return;
			}
		}
	}

	// �ڕW�n�_�Ɍ�����
	Move();

	if (m_pOwner->GetTreasure())
	{
		ChangeAIState(AI_STATE_MOVE_GOAL);
	}
}

//=============================================================================
// �N�����U������
//=============================================================================
void C_CPU_AI::AttackWho(void)
{
	if (!m_pAimPlayer)
	{
		m_nTimer = 0;
		ChangeAIState(AI_STATE_THINK);
		return;
	}

	m_nTimer++;
	m_nSarchTimer++;

	// �o�H�Č���
	if (m_nSarchTimer > RETRY_SERCH_PATH_TIME)
	{
		TileIndex start, goal;
		start = GetOpenListIndex(&m_pOwner->GetPos());
		goal = GetOpenListIndex(&m_pAimPlayer->GetPos());
		SarchPath(&start, &goal);
		m_nSarchTimer = 0;
		return;
	}

	// �ڕW�n�_�Ɍ�����
	Move();

	// ���Ԋu�ŕϐg����
	int judgeCount = (int)(MAX_MOVE_TIME_FOR_ATTACK * 0.5);
	if (judgeCount % m_nTimer == 0)
	{
		int ans = mersenne_twister_int(0, MAX_PERCENT);
		if (ans < HALF_PERCENT)
		{
			m_AIInput.changeAtk = true;
		}
		else
		{
			m_AIInput.changeSpeed = true;
		}
	}

	if (!m_AIInputOld.pushAtk)
	{
		D3DXVECTOR3 vec = m_pOwner->GetPos() - m_pAimPlayer->GetPos();
		float judgeLength = m_pOwner->GetWidth() * 2.f + m_pOwner->GetWidth() * 2.f;
		float length = MagnitudeVector(vec);
		// �������߂�������
		if (length <= judgeLength)
		{
			int ans = mersenne_twister_int(0, MAX_PERCENT);
			if (ans < HALF_PERCENT)
			{
				m_AIInput.pushAtk = true;
				return;
			}
		}
	}

	// ���Ԓ�������
	if (m_nTimer > MAX_MOVE_TIME_FOR_ATTACK)
	{
		m_nTimer = 0;
		ChangeAIState(AI_STATE_THINK);
		return;
	}
}

//=============================================================================
// �󎝂��Ă����U������
//=============================================================================
void C_CPU_AI::AttackHaveTreasurPlayer(void)
{
	if (!m_pAimPlayer)
	{
		m_nTimer = 0;
		ChangeAIState(AI_STATE_THINK);
		return;
	}

	if (m_pTreasure->GetTreasureState() == TREASURE_STATE_OWNER_NONE)
	{
		m_nTimer = 0;
		ChangeAIState(AI_STATE_THINK);
		return;
	}

	m_nTimer++;
	m_nSarchTimer++;

	// �o�H�Č���
	if (m_nSarchTimer > RETRY_SERCH_PATH_TIME)
	{
		TileIndex start, goal;
		start = GetOpenListIndex(&m_pOwner->GetPos());
		goal = GetOpenListIndex(&m_pAimPlayer->GetPos());
		SarchPath(&start, &goal);
		m_nSarchTimer = 0;
		return;
	}

	// �ڕW�n�_�Ɍ�����
	Move();

	// ���Ԋu�ŕϐg����
	int judgeCount = (int)(MAX_MOVE_TIME_FOR_ATTACK * 0.5);
	if (judgeCount % m_nTimer == 0)
	{
		// �U���`�ԂłȂ����
		if (m_pOwner->GetPlayerMode() != PLAYER_MODE_ATTACK)
		{
			int ans = mersenne_twister_int(0, MAX_PERCENT);
			if (ans < HALF_PERCENT)
			{
				m_AIInput.changeAtk = true;
				return;
			}
		}
	}

	D3DXVECTOR3 vec = m_pOwner->GetPos() - m_pAimPlayer->GetPos();
	float judgeLength = m_pOwner->GetWidth() * 2.f + m_pOwner->GetWidth() * 2.f;
	float length = MagnitudeVector(vec);
	// �������߂�������
	if (length <= judgeLength)
	{
		int ans = mersenne_twister_int(0, MAX_PERCENT);
		if (ans < HALF_PERCENT)
		{
			if (!m_AIInputOld.pushAtk)
			{
				m_AIInput.pushAtk = true;
			}
			return;
		}
		else
		{
			if (!m_AIInputOld.pushThread)
			{
				m_AIInput.pushThread = true;
			}
			return;
		}
	}

	// ���Ԓ�������
	if (m_nTimer > MAX_MOVE_TIME_FOR_ATTACK)
	{
		m_nTimer = 0;
		ChangeAIState(AI_STATE_THINK);
		return;
	}
}

//=============================================================================
// �S�[���Ɍ�����
//=============================================================================
void C_CPU_AI::MoveGoal(void)
{
	m_nTimer++;
	m_nSarchTimer++;

	// �󎝂��ĂȂ��Ȃ�
	if (!m_pOwner->GetTreasure())
	{
		ChangeAIState(AI_STATE_THINK);
		return;
	}

	// ���Ԋu�ŕϐg����
	if (m_nTimer % THINK_CHANGE_MODE_TIME == 0)
	{
		// �W���}�[�`�ԂłȂ����
		if (m_pOwner->GetPlayerMode() != PLAYER_MODE_TRAP)
		{
			int ans = mersenne_twister_int(0, MAX_PERCENT);
			if (ans < HALF_PERCENT)
			{
				m_AIInput.changeJammer = true;
				//return;
			}
		}
		else
		{
			int ans = mersenne_twister_int(0, MAX_PERCENT);
			if ((float)ans < 0.1f)
			{
				if (!m_AIInputOld.pushThread)
				{
					m_AIInput.pushThread = true;
				}
				//return;
			}
		}
	}

	if (m_nSarchTimer > RETRY_SERCH_PATH_TIME)
	{
		TileIndex start, goal;
		start = GetOpenListIndex(&m_pOwner->GetPos());
		goal = GetOpenListIndex(&m_pGoal->GetPos());
		
		// �����Ȃ����
		if (!SarchPath(&start, &goal))
		{
			ChangeAIState(AI_STATE_MAKE_FOOT_STEP);
		}
	}

	// �ڕW�n�_�Ɍ�����
	if (Move())
	{
		D3DXVECTOR3 playerPos = m_pOwner->GetPos();
		float playerHeight = m_pOwner->GetHeight();
		float playerWidth = m_pOwner->GetWidth();
		// �S�[���Ɣ��f���鋗��
		float judgeSize = playerHeight * 0.25f + playerWidth * 0.5f;// +m_pGoal->GetWidth() * 0.5f + m_pGoal->GetHeight() * 0.5f;
		// ����𑫌���
		playerPos.y += playerHeight * 0.25f;

		float length = MagnitudeVector(m_pOwner->GetPos() - m_pGoal->GetPos());
		float judgeLength = judgeSize;
		judgeLength *= judgeLength;
		if (length < judgeLength)
		{
			// �S�[���ɂ��Ă�������
			ChangeAIState(AI_STATE_SAVE_TREASURE);
		}
	}
}

//=============================================================================
// ������
//=============================================================================
void C_CPU_AI::SaveTreasure(void)
{
	// �󎝂��ĂȂ��Ȃ�
	if (!m_pOwner->GetTreasure())
	{
		ChangeAIState(AI_STATE_THINK);
		return;
	}
		
	m_nTimer++;

	// ��ԋ߂��̃v���C���[��T��
	CPlayerManager* pPlayerManager = m_pOwner->GetPlayerManager();
	CPlayer* otherPlayer = NULL;
	float length = -1.f;
	float culc = -1.f;
	for (int playerID = 0; playerID < MAXIMUM_NUMBER_OF_PLAYER; ++playerID)
	{
		// �����͂��Ȃ�
		if (playerID == m_pOwner->GetPlayerNum())
		{
			continue;
		}

		// ���̃v���C���[�擾
		otherPlayer = pPlayerManager->GetPlayer(playerID);
		if (!otherPlayer)
		{
			continue;
		}

		culc = MagnitudeVector(otherPlayer->GetPos() - m_pOwner->GetPos());
		if (culc < length || length < 0.f)
		{
			length = culc;
		}
	}

	float judgeLength = m_pOwner->GetWidth() * 2.f + m_pOwner->GetWidth() * 2.f;
	if (length < judgeLength || m_pOwner->GetPlayerMode() == PLAYER_MODE_ATTACK)
	{
		if (!m_AIInputOld.pushAtk)
		{
			int ans = mersenne_twister_int(0, MAX_PERCENT);
			if (ans < HALF_PERCENT)
			{
				m_AIInput.pushAtk = true;
				return;
			}
		}
		if (!m_AIInputOld.pushThread)
		{
			int ans = mersenne_twister_int(0, MAX_PERCENT);
			if (ans < HALF_PERCENT)
			{
				m_AIInput.pushThread = true;
				return;
			}
		}
	}

	// ���Ԋu�ŕϐg����
	judgeLength = m_pOwner->GetWidth() * 4.f + m_pOwner->GetWidth() * 4.f;
	if (THINK_CHANGE_MODE_TIME % m_nTimer == 0)
	{
		// �U���`�ԂłȂ����
		if (m_pOwner->GetPlayerMode() != PLAYER_MODE_ATTACK)
		{
			m_AIInput.changeAtk = true;
		}
	}
}

//=============================================================================
// ������
//=============================================================================
void C_CPU_AI::MakeFootStep(void)
{
	m_nTimer++;

	// �Œ������
	if (m_nTimer < BEFORE_MOVE_MAKE_FOOT_STEP_TIME)
	{
		D3DXVECTOR3 pos, goalPos;
		pos = m_pOwner->GetPos();
		goalPos = D3DXVECTOR3(m_apTile[m_GoalTile.y_block][m_GoalTile.x_block].centerPos.x
							, m_apTile[m_GoalTile.y_block][m_GoalTile.x_block].centerPos.y
							, 0.f);

		if (m_nTimer % 2 == 0)
		{
			float culc = pos.x - goalPos.x;
			if (culc > JUDGE_ZERO_FOR_MOVE)
			{
				m_AIInput.moveLeft = true;
			}
			else if (culc < -JUDGE_ZERO_FOR_MOVE)
			{
				m_AIInput.moveRight = true;
			}
		}

		else
		{
			float culc = pos.y - goalPos.y;
			if (culc > JUDGE_ZERO_FOR_MOVE)
			{
				m_AIInput.moveUp = true;
			}
			else if (culc < -JUDGE_ZERO_FOR_MOVE)
			{
				m_AIInput.moveDown = true;
			}
		}
	}
	// �ό`
	else if (m_nTimer < BEFORE_MOVE_MAKE_FOOT_STEP_TIME + 5)
	{
		m_AIInput.changeSpeed = true;
	}
	// ������
	else
	{
		m_AIInput.pushThread = true;
		ChangeAIState(m_AIStateOld);
		return;
	}
}

//=============================================================================
// �ڕW�n�_�Ɍ�����
// �����ł͕����������߂Ď��ۂ̈ړ��̓v���C���[cpp���ł��
//=============================================================================
bool C_CPU_AI::Move(void)
{
	D3DXVECTOR3 playerPos = m_pOwner->GetPos();
	float playerHeight = m_pOwner->GetHeight();
	float playerWidth = m_pOwner->GetWidth();

	// �����v�Z
	float culc = m_vDestPos.x - playerPos.x;

	// �ۂ߂�l�v�Z
	float judgeLine = playerWidth * 0.01f;
	if (fabs(culc) < judgeLine)
	{
		culc = 0.f;
	}

//	if (m_nTimer % 2 == 0)
	{
		// �ړI�n��X�������f
		if (culc > 0.f)
		{
			m_AIInput.moveRight = true;
		}
		else if (culc < 0.f)
		{
			m_AIInput.moveLeft = true;
		}
	}

//	else
	{
		// �����v�Z
		culc = m_vDestPos.y - playerPos.y;

		// �ۂ߂�l�v�Z
		judgeLine = playerHeight * 0.01f;
		if (fabs(culc) < judgeLine)
		{
			culc = 0.f;
		}

		// �ړI�n��Y�������f
		if (culc > 0.f)
		{
			m_AIInput.moveDown = true;
		}
		if (culc < 0.f)
		{
			m_AIInput.moveUp = true;
		}
	}

	// ����𑫌���
	playerPos.y += playerHeight * 0.5f;

	// �ڕW�n�_�Ƃ̋����𑪂�
	float length = (playerPos.x - m_vDestPos.x) * (playerPos.x - m_vDestPos.x)
					+ (playerPos.y - m_vDestPos.y) * (playerPos.y - m_vDestPos.y);

	// �S�[���Ɣ��f���鋗��
	float judgeSize = playerHeight * 0.25f * playerHeight * 0.25f
					+ playerWidth * 0.5f * playerWidth * 0.5f;

	// �ړI�n�͈͓̔��Ȃ�
	if (length < judgeSize)
	{
		// �S�[�����Ă���
		if (m_DestTile.x_block == m_GoalTile.x_block && m_DestTile.y_block == m_GoalTile.y_block)
		{
			return true;
		}

		// �ڕW�^�C���̐ݒ�
		m_DestTile = m_apTile[m_DestTile.y_block][m_DestTile.x_block].parent_tile;

		// �ڕW�n�_�̐ݒ�
		m_vDestPos = m_apTile[m_DestTile.y_block][m_DestTile.x_block].centerPos;
	}

	return false;
}

//=============================================================================
// AI�̓��͂̏�����
//=============================================================================
void C_CPU_AI::InitAIInput(void)
{
	m_AIInput.moveRight = false;
	m_AIInput.moveLeft = false;
	m_AIInput.moveUp = false;
	m_AIInput.moveDown = false;
	m_AIInput.pushThread = false;
	m_AIInput.pushAtk = false;
	m_AIInput.changeAtk = false;
	m_AIInput.changeSpeed = false;
	m_AIInput.changeJammer = false;
}

//=============================================================================
// �K���ړ�
//=============================================================================
void C_CPU_AI::MoveFree(void)
{
	if (m_nTimer > FREE_MOVE_TIME)
	{
		ChangeAIState(AI_STATE_THINK);
		return;
	}

	if (m_nTimer % FREE_MOVE_THINK == 0)
	{
		int ans = mersenne_twister_int(0, 3);
		if (ans == 0)
		{
			m_AIInput.moveLeft = true;
		}
		else if (ans == 1)
		{
			m_AIInput.moveRight = true;
		}
		else if (ans == 2)
		{
			m_AIInput.moveUp = true;
		}
		else if (ans == 3)
		{
			m_AIInput.moveDown = true;
		}
	}
	else
	{
		m_AIInput = m_AIInputOld;
	}
	m_nTimer++;
}

//=============================================================================
// AI�̏�ԕύX
//=============================================================================
void C_CPU_AI::ChangeAIState(AI_STATE state)
{
	if (state != m_AIState)
	{
		m_AIStateOld = m_AIState;
	}
	m_AIState = state;
	m_nTimer = 0;
}

#ifdef _DEBUG
void C_CPU_AI::Printstate(void)
{
	switch (m_AIState)
	{
	case AI_STATE_THINK:
		CDebugProc::Print("AI_STATE:THINK\n");
		break;

	case AI_STATE_MOVE_TO_TREASURE:
		CDebugProc::Print("AI_STATE:MOVE_TREASURE\n");
		break;

	case AI_STATE_ATTACK_WHO:
		CDebugProc::Print("AI_STATE:ATTACK_WHO\n");
		break;

	case AI_STATE_ATTACK_HAVE_TREASURE_PLAYER:
		CDebugProc::Print("AI_STATE:ATTACK_TREASURE_PLAYER\n");
		break;

	case AI_STATE_MOVE_GOAL:
		CDebugProc::Print("AI_STATE:MOVE_GOAL\n");
		break;

	case AI_STATE_SAVE_TREASURE:
		CDebugProc::Print("AI_STATE:SAVE_TREASURE\n");
		break;

	case AI_STATE_MAKE_FOOT_STEP:
		CDebugProc::Print("AI_STATE:MAKE_FOOT_STEP\n");
		break;

	case AI_STATE_MOVE_FREE:
		CDebugProc::Print("AI_STATE:MOVE_FREE\n");
		break;

	default:
		break;
	}
}
#endif
//----EOF-----