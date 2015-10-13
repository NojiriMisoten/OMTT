//=============================================================================
//
// CJudge�N���X [CJudge.cpp]
// Author : ��K�@����
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CJudge.h"
#include "CJudgeManager.h"
#include "../MATH/matrix.h"
#include "../MATH/vector.h"
#include "../MATH/math.h"
#include "../SCENE/CSCENE/CScene.h"
#include "../SCENE/CSCENE/CScene2D.h"
#include "../SCENE/GAME/PLAYER/CPlayer.h"
#include "../SCENE/CSCENE/CScene2D.h"
#include "../SCENE/GAME/TREASURE/CTreasure.h"
#include "../SCENE/GAME/ATTACK/CAttackBase.h"
#include "../SCENE/GAME/THREAD/CThreadBase.h"
#include "../SCENE/GAME/FIELD/CField.h"
#include "../SCENE/GAME/THREAD/CThreadSpecialSpeed.h"
#include "../SCENE/GAME/GOAL/CGoal.h"
#include "../SCENE/GAME/GOAL/CGoalManager.h"
#include "../CONTROLLER/CControllerManager.h"
#include "../SCENE/GAME/EFFECT/CEffectManager.h"

//=========================================================================
// �R���X�g���N�^
//=========================================================================
CJudge::CJudge(CJudgeManager* pJudgeManager)
{
	m_pJudgeManager = pJudgeManager;

	for (int idx = 0; idx < MAXIMUM_NUMBER_OF_PLAYER; ++idx)
	{
		m_LastFieldColiPlayer[idx] = NULL;
	}
}

//=========================================================================
// �f�X�g���N�^
//=========================================================================
CJudge::~CJudge()
{
	m_pJudgeManager = NULL;

	for (int idx = 0; idx < MAXIMUM_NUMBER_OF_PLAYER; ++idx)
	{
		m_LastFieldColiPlayer[idx] = NULL;
	}
}

//=========================================================================
// �t�B�[���h�ƃv���C���[�̂����蔻��
//=========================================================================
void CJudge::ColiFieldxPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// �v���C���[�̍ő�l�����p��
	CScene2D *pField;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// �v���C���[�������
	CPlayerManager* playerManager = m_pJudgeManager->GetPlayerManager();

	for (int playerCount = 0; playerCount < MAXIMUM_NUMBER_OF_PLAYER; ++playerCount)
	{
		pPlayer[playerCount] = playerManager->GetPlayer(playerCount);
		if (!pPlayer[playerCount])
		{
			continue;
		}
		D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
		pos.y += pPlayer[playerCount]->GetHeight() * 0.25f;
		float rot = pPlayer[playerCount]->GetRot().z;
		float width = pPlayer[playerCount]->GetWidth() * 0.25f;
		float height = pPlayer[playerCount]->GetHeight() * 0.15f;

		// OBB���쐬
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// �t�B�[���h�Ƃ̓����蔻�胋�[�v
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// �擪���w��
		pScene = CScene::GetTopAddress(priority);

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_FIELD)
			{
				// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
				pScene = pSceneNext;
				continue;
			}

			// �t�B�[���h�������
			pField = (CScene2D*)pScene;
			D3DXVECTOR2 pos(pField->GetJudgePos().x, pField->GetJudgePos().y);
			float rot = pField->GetRot().z;
			float width = pField->GetJudgeWidth();
			float height = pField->GetJudgeHeight();
			CJudge::OBB_INFO fieldOBB;
			// OBB���쐬
			CreateOBBInfo(&fieldOBB, &pos, &rot, &width, &height);

			if (priority == TYPE_PRIORITY_FIELD)
			{
				// �v���C���[����Ă��񏉊���
				CField* p = (CField*)pField;
				p->Ride(false);
			}
			else if (priority == TYPE_PRIORITY_THREAD_OF_FOOTHOLD)
			{
				// �v���C���[����Ă��񏉊���
				CThreadSpecialSpeed* p = (CThreadSpecialSpeed*)pField;
				p->Ride(false);
			}

			// �����蔻��
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// ���łɂ������Ă�Ȃ画�肵�Ȃ�
				if (coli[idx])
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], fieldOBB))
				{
					// �q�b�g�t���O�I��
					coli[idx] = true;

					// �Ō�ɓ��������ꏊ�X�V
					m_LastFieldColiPlayer[idx] = pField;

					if (priority == TYPE_PRIORITY_FIELD)
					{
						// �v���C���[����Ă��񏉊���
						CField* p = (CField*)pField;
						p->Ride(true);
					}
					else if (priority == TYPE_PRIORITY_THREAD_OF_FOOTHOLD)
					{
						// �v���C���[����Ă��񏉊���
						CThreadSpecialSpeed* p = (CThreadSpecialSpeed*)pField;
						p->Ride(true);
					}
				}
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}

	// �t�B�[���h�O�ɋ����̍��W�ύX
	for (int idx = 0; idx < playerNum; ++idx)
	{
		// �������Ă��͏���Ă�̂Ŗ���
		if (coli[idx])
		{
			continue;
		}

		// NULL�`�F�b�N
		if (!pPlayer[idx])
		{
			continue;
		}
		if (!m_LastFieldColiPlayer[idx])
		{
			continue;
		}

		D3DXVECTOR2 playerPos, fieldPos, vertexPosA, vertexPosB, vertexPosC, vertexPosD, hitPos;
		playerPos = (D3DXVECTOR2)pPlayer[idx]->GetPos();
		playerPos.y += pPlayer[idx]->GetHeight() * 0.25f;
		if (m_LastFieldColiPlayer[idx])
		{
			fieldPos = (D3DXVECTOR2)m_LastFieldColiPlayer[idx]->GetJudgePos();
		}
		else
		{
			fieldPos = playerPos;
		}

		// �R�̎��Ȃ̂ŏ��Ԓ���
		vertexPosA = *m_LastFieldColiPlayer[idx]->GetVertexJudgePos(0);
		vertexPosB = *m_LastFieldColiPlayer[idx]->GetVertexJudgePos(1);
		vertexPosC = *m_LastFieldColiPlayer[idx]->GetVertexJudgePos(3);
		vertexPosD = *m_LastFieldColiPlayer[idx]->GetVertexJudgePos(2);

		Segment playerSegment, vertexSegment;
		playerSegment.s = fieldPos;
		playerSegment.v = playerPos - fieldPos;
		vertexSegment.s = vertexPosA;
		vertexSegment.v = vertexPosB - vertexPosA;

		if (!ColiRayxRay(playerSegment, vertexSegment, &hitPos))
		{
			vertexSegment.s = vertexPosB;
			vertexSegment.v = vertexPosC - vertexPosB;
			if (!ColiRayxRay(playerSegment, vertexSegment, &hitPos))
			{
				vertexSegment.s = vertexPosC;
				vertexSegment.v = vertexPosD - vertexPosC;
				if (!ColiRayxRay(playerSegment, vertexSegment, &hitPos))
				{
					vertexSegment.s = vertexPosD;
					vertexSegment.v = vertexPosA - vertexPosD;
					if (!ColiRayxRay(playerSegment, vertexSegment, &hitPos))
					{
						return;
					}
				}
			}
		}

		D3DXVECTOR3 oldPlayerPos = pPlayer[idx]->GetOldPos();
		D3DXVECTOR3 setPlayerPos = oldPlayerPos;
		D3DXVECTOR3 currentPlayerPos = pPlayer[idx]->GetPos();
		D3DXVECTOR2 playerVec = (D3DXVECTOR2)(currentPlayerPos - oldPlayerPos);
		D3DXVec2Normalize(&playerVec, &playerVec);
		playerVec.x = fabs(playerVec.x);
		playerVec.y = fabs(playerVec.y);
		D3DXVECTOR2 vertexVec = vertexSegment.v;
		D3DXVec2Normalize(&vertexVec, &vertexVec);
		vertexVec.x = fabs(vertexVec.x);
		vertexVec.y = fabs(vertexVec.y);

		if (D3DXVec2Dot(&playerVec, &vertexVec) >= 0.5f)
		{
			playerSegment.s = (D3DXVECTOR2)oldPlayerPos;
			playerSegment.v = (D3DXVECTOR2)(currentPlayerPos - oldPlayerPos);
			ColiRayxRay(playerSegment, vertexSegment, &hitPos);
			setPlayerPos.x = hitPos.x;
			setPlayerPos.y = hitPos.y - pPlayer[idx]->GetHeight() * 0.25f;
			pPlayer[idx]->SetPos(setPlayerPos);
		}

		pPlayer[idx]->SetPos(setPlayerPos);
	}
}

//=========================================================================
// �U���ƃv���C���[�̂����蔻��
//=========================================================================
void CJudge::ColiAttackxPlayer(void){

	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// �v���C���[�̍ő�l�����p��
	CScene2D *pAttack;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;

	// �v���C���[�������
	CPlayerManager* playerManager = m_pJudgeManager->GetPlayerManager();

	for (int playerCount = 0; playerCount < MAXIMUM_NUMBER_OF_PLAYER; ++playerCount)
	{
		pPlayer[playerCount] = playerManager->GetPlayer(playerCount);
		if (!pPlayer[playerCount])
		{
			continue;
		}
		D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
		pos.y += pPlayer[playerCount]->GetHeight() * 0.25f;
		float rot = pPlayer[playerCount]->GetRot().z;
		float width = pPlayer[playerCount]->GetWidth();
		float height = pPlayer[playerCount]->GetHeight() * 0.75f;

		// OBB���쐬
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// �U���Ƃ̓����蔻�胋�[�v
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// �擪���w��
		pScene = CScene::GetTopAddress(priority);

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_ATTACK)
			{
				// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
				pScene = pSceneNext;
				continue;
			}

			// �U���������
			pAttack = (CScene2D*)pScene;
			CAttackBase* pAttackBase = (CAttackBase*)pScene;
			D3DXVECTOR2 pos(pAttack->GetPos().x, pAttack->GetPos().y);
			float rot = pAttack->GetRot().z;
			float width = pAttack->GetWidth();
			float height = pAttack->GetHeight();
			CJudge::OBB_INFO attackOBB;
			// OBB���쐬
			CreateOBBInfo(&attackOBB, &pos, &rot, &width, &height);

			// �����蔻��
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// ���ɂ��̃v���C���[�ɂ������Ă�Ȃ���Ȃ�
				bool hit = pAttackBase->GetHitFlag(idx);
				if (hit == true)
				{
					continue;
				}

				// �����̍U����
				if (idx == (int)pAttackBase->GetPlayerNumber())
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], attackOBB))
				{
					// �����������̏���
					pAttackBase->HitPlayer(pPlayer[idx]);
				}
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}
}

//=========================================================================
// ���ƃv���C���[�̂����蔻��
//=========================================================================
void CJudge::ColiThreadxPlayer(void){

	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// �v���C���[�̍ő�l�����p��
	CScene2D *pThread;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// �v���C���[�������
	CPlayerManager* playerManager = m_pJudgeManager->GetPlayerManager();

	for (int playerCount = 0; playerCount < MAXIMUM_NUMBER_OF_PLAYER; ++playerCount)
	{
		pPlayer[playerCount] = playerManager->GetPlayer(playerCount);
		if (!pPlayer[playerCount])
		{
			continue;
		}
		D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
		pos.y += pPlayer[playerCount]->GetHeight() * 0.25f;
		float rot = pPlayer[playerCount]->GetRot().z;
		float width = pPlayer[playerCount]->GetWidth();
		float height = pPlayer[playerCount]->GetHeight() * 0.75f;

		// OBB���쐬
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// �U���Ƃ̓����蔻�胋�[�v
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// �擪���w��
		pScene = CScene::GetTopAddress(priority);

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_THREAD)
			{
				// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
				pScene = pSceneNext;
				continue;
			}

			// ���������
			pThread = (CScene2D*)pScene;
			CThreadBase* pThreadBase = (CThreadBase*)pScene;
			D3DXVECTOR2 pos(pThread->GetPos().x, pThread->GetPos().y);
			float rot = pThread->GetRot().z;
			float width = pThread->GetWidth();
			float height = pThread->GetHeight();
			CJudge::OBB_INFO attackOBB;
			// OBB���쐬
			CreateOBBInfo(&attackOBB, &pos, &rot, &width, &height);

			// �����蔻��
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// ���łɂ������Ă�Ȃ画�肵�Ȃ�
				if (coli[idx])
				{
					continue;
				}

				// �����̍U����
				if (idx == (int)pThreadBase->GetPlayerNum())
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], attackOBB))
				{
					// �q�b�g�t���O�I��
					coli[idx] = true;

					// �v���C���ɓ�������
					pThreadBase->HitPlayer(pPlayer[idx]);
				}
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}
}

//=========================================================================
// �t�B�[���h�Ƒ���ɂȂ鎅�̂����蔻��
//=========================================================================
void CJudge::ColiFieldxThreadOfFoothold(void)
{
	CScene *pSceneThread = CScene::GetTopAddress(TYPE_PRIORITY_THREAD_OF_FOOTHOLD);
	CScene *pSceneNextThread;
	CScene *pSceneField;
	CScene *pSceneNextField;
	CScene2D *pThread;
	CScene2D *pField;
	CScene2D* lastCheckField = NULL;
	CJudge::OBB_INFO threadOBB, fieldOBB, lastCheckOBB;
	bool hit = false;


	// ���肷�ׂ����̂��Ȃ��Ȃ�
	if (!pSceneThread)
	{
		return;
	}

	// �����[�v
	while (pSceneThread)
	{
		// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
		pSceneNextThread = pSceneThread->GetNextAddress();

		// ����ɂȂ��ĂȂ���Ȃ�
		if (pSceneThread->GetObjType() != CScene::OBJTYPE_FIELD)
		{
			// �����擾
			pThread = (CScene2D*)pSceneThread;
			D3DXVECTOR2 pos(pThread->GetPos().x, pThread->GetPos().y);
			float rot = pThread->GetRot().z;
			float width = pThread->GetWidth() * 0.8f;
			float height = pThread->GetHeight() * 0.8f;
			CThreadBase* threadBase = (CThreadBase*)pSceneThread;
			int playerNum = threadBase->GetPlayerNum();
			lastCheckField = m_LastFieldColiPlayer[playerNum];

			// OBB���쐬
			CreateOBBInfo(&threadOBB, &pos, &rot, &width, &height);

			for (int prioryty = TYPE_PRIORITY_0; prioryty < TYPE_PRIORITY_MAX; ++prioryty)
			{
				pSceneField = CScene::GetTopAddress(prioryty);

				// �t�B�[���h�Ƃ̓����蔻�胋�[�v
				while (pSceneField)
				{
					// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
					pSceneNextField = pSceneField->GetNextAddress();

					if (pSceneField->GetObjType() == CScene::OBJTYPE_FIELD)
					{
						// �t�B�[���h���擾
						pField = (CScene2D*)pSceneField;
						D3DXVECTOR2 pos2(pField->GetJudgePos().x, pField->GetJudgePos().y);
						float rot2 = pField->GetRot().z;
						float width2 = pField->GetJudgeWidth();
						float height2 = pField->GetJudgeHeight();

						// ����������Ă鏰�Ȃ���Ȃ�
						CScene2D* lastField = m_LastFieldColiPlayer[playerNum];
						if (lastField)
						{
							if (lastField == pField)
							{
								// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
								pSceneField = pSceneNextField;
								continue;
							}
						}

						// �t�B�[���h��OBB���쐬
						CreateOBBInfo(&fieldOBB, &pos2, &rot2, &width2, &height2);

						// �����蔻��
						if (IsOBB(threadOBB, fieldOBB))
						{
							if (lastCheckField)
							{
								D3DXVECTOR2 lastCheckPos(lastCheckField->GetJudgePos().x, lastCheckField->GetJudgePos().y);
								float lastCheckRot = lastCheckField->GetRot().z;
								float lastCheckWidth = lastCheckField->GetJudgeWidth() * 1.5f;
								float lastCheckHeight = lastCheckField->GetJudgeHeight() * 1.5f;
								// �Ō�ɒ��ׂ��t�B�[���h��OBB���쐬
								CreateOBBInfo(&lastCheckOBB, &lastCheckPos, &lastCheckRot, &lastCheckWidth, &lastCheckHeight);

								// �Ō�ɒ��ׂ����ƍ��̏����������Ă邩
								if (IsOBB(lastCheckOBB, fieldOBB))
								{
									// �Ō�ɒ��ׂ����X�V
									lastCheckField = pField;

									// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
									pSceneField = pSceneNextField;
									continue;
								}
							}

							// �t�B�[���h�Ƀ^�C�v�ύX
							pThread->SetObjType(CScene::OBJTYPE_FIELD);

							// ���쐬�G�t�F�N�g����
							CEffectManager::CreateEffect(pThread->GetPos(), EFFECT_CREATE_ROAD, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

							hit = true;
							break;
						}

					}

					// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
					pSceneField = pSceneNextField;
				}

				// ���ɒ��ׂĂ��鎅���������Ă��玟�̎���
				if (hit)
				{
					hit = false;
					break;
				}
			}
		}

		// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
		pSceneThread = pSceneNextThread;
	}
}

//=========================================================================
// �󔠂ƃv���C���[�̂����蔻��
//=========================================================================
void CJudge::ColiTreasurexPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// �v���C���[�̍ő�l�����p��
	CTreasure *pTreasure;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// �v���C���[�������
	CPlayerManager* playerManager = m_pJudgeManager->GetPlayerManager();

	for (int playerCount = 0; playerCount < MAXIMUM_NUMBER_OF_PLAYER; ++playerCount)
	{
		pPlayer[playerCount] = playerManager->GetPlayer(playerCount);
		if (!pPlayer[playerCount])
		{
			continue;
		}
		D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
		pos.y += pPlayer[playerCount]->GetHeight() * 0.25f;
		float rot = pPlayer[playerCount]->GetRot().z;
		float width = pPlayer[playerCount]->GetWidth() * 0.5f;
		float height = pPlayer[playerCount]->GetHeight() * 0.25f;

		// OBB���쐬
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// ��Ƃ̓����蔻�胋�[�v
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// �擪���w��
		pScene = CScene::GetTopAddress(priority);

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_TREASURE)
			{
				// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
				pScene = pSceneNext;
				continue;
			}

			// ��̏������
			pTreasure = (CTreasure*)pScene;
			D3DXVECTOR2 pos(pTreasure->GetPos().x, pTreasure->GetPos().y);
			float rot = pTreasure->GetRot().z;
			float width = pTreasure->GetWidth();
			float height = pTreasure->GetHeight();
			CJudge::OBB_INFO treasureOBB;

			// �����ʊO�ɂ����Ȃ����� by �˖{
			if (pTreasure->GetTreasureState() != TREASURE_STATE_JUMPING){
				pos.x = max(pos.x, width * 0.5f);
				pos.x = min(pos.x, SCREEN_WIDTH - width * 0.5f);
				pos.y = max(pos.y, height * 0.5f);
				pos.y = min(pos.y, SCREEN_HEIGHT - height * 0.5f);
				pTreasure->SetPos(D3DXVECTOR3(pos));
			}

			// OBB���쐬
			pos.y += pTreasure->GetHeight() * 0.25f;
			height *= 0.5f;
			CreateOBBInfo(&treasureOBB, &pos, &rot, &width, &height);

			// �����蔻��
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// ���łɂ������Ă�Ȃ画�肵�Ȃ�
				if (coli[idx])
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], treasureOBB))
				{
					// �q�b�g�t���O�I��
					coli[idx] = true;
					break;
				}
			}
			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			//			pScene = pSceneNext;

			// ����͈�����甲����
			break;
		}
	}

	// �����蔻��Ȃ�ƂȂ����₷�����ȂƎv���Ă����ɕ�����
	if (pTreasure->GetTreasureState() != TREASURE_STATE_OWNED &&
		pTreasure->GetTreasureState() != TREASURE_STATE_JUMPING){
		for (int idx = 0; idx < playerNum; ++idx){
			if (coli[idx]){
				// �v���C���ɂ����n��
				pPlayer[idx]->SetTreasure(pTreasure);
				pTreasure->SetTreasureState(TREASURE_STATE_OWNED);
				CEffectManager::CreateEffect(pPlayer[idx]->GetPos(), EFFECT_FLAG_GET, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				break;
			}
		}
	}
}

//=========================================================================
// �v���C���[�ƃS�[���̂����蔻��
//=========================================================================
void CJudge::ColiGoalxPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// �v���C���[�̍ő�l�����p��
	CGoal *pGoal = NULL;
	CGoal *pGoalHit = NULL;
	int nHitGoalPlayerNum;
	CGoal *pGoalAll[GOAL_MAX] = { NULL };
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// �v���C���[�������
	CPlayerManager* playerManager = m_pJudgeManager->GetPlayerManager();

	for (int playerCount = 0; playerCount < MAXIMUM_NUMBER_OF_PLAYER; ++playerCount)
	{
		pPlayer[playerCount] = playerManager->GetPlayer(playerCount);
		if (!pPlayer[playerCount])
		{
			continue;
		}
		D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
		pos.y += pPlayer[playerCount]->GetHeight() * 0.25f;
		float rot = pPlayer[playerCount]->GetRot().z;
		float width = pPlayer[playerCount]->GetWidth() * 0.5f;
		float height = pPlayer[playerCount]->GetHeight() * 0.25f;

		// OBB���쐬
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// �����蔻�胋�[�v
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// �擪���w��
		pScene = CScene::GetTopAddress(priority);

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_GOAL)
			{
				// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
				pScene = pSceneNext;
				continue;
			}

			// �t�B�[���h�������
			pGoal = (CGoal*)pScene;
			D3DXVECTOR2 pos(pGoal->GetPos().x, pGoal->GetPos().y);

			float rot = pGoal->GetRot().z;
			float width = pGoal->GetWidth();
			float height = pGoal->GetHeight();
			CJudge::OBB_INFO goalOBB;
			// OBB���쐬
			CreateOBBInfo(&goalOBB, &pos, &rot, &width, &height);

			// �v���C���ԍ���z��ԍ��Ƃ��ăS�[���̃|�C���^���i�[
			pGoalAll[pGoal->GetPlayerNum()] = pGoal;

			// �����蔻��
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// �󕨂������Ă��Ȃ������玟��
				if (pPlayer[idx]->GetTreasure() == NULL){
					continue;
				}

				// ���łɂ������Ă�Ȃ画�肵�Ȃ�
				if (coli[idx])
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], goalOBB))
				{
					// ���������S�[���̃C���X�^���X��ۑ�
					pGoalHit = pGoal;
					nHitGoalPlayerNum = pGoal->GetPlayerNum();

					// �q�b�g�t���O�I��
					coli[idx] = true;
				}
			}
			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}

	// �����蔻��`�F�b�N
	for (int idx = 0; idx < playerNum; ++idx){
		if (coli[idx]){

			// �v���C���ɑΉ������S�[������Ȃ���΂���Ȃ�
			if (nHitGoalPlayerNum != pPlayer[idx]->GetPlayerNum()){
				break;
			}
			pGoalHit->SetTrans();
		}
		else{
			for (int i = 0; i < GOAL_MAX; i++){
				if (i != nHitGoalPlayerNum){
					pGoalAll[i]->SetNoTrans();
				}
			}
		}
	}

}

//=========================================================================
// �t�B�[���h�ƕ󔠂̂����蔻��
//=========================================================================
void CJudge::ColiFieldxTreasure(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CTreasure *pTreasure = NULL;
	CScene2D *pField = NULL;
	CJudge::OBB_INFO treasureOBB;
	bool coli = false;

	// �t�B�[���h�Ƃ̓����蔻�胋�[�v
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// �擪���w��
		pScene = CScene::GetTopAddress(priority);

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() == CScene::OBJTYPE_TREASURE){
				// ��OBB���쐬
				pTreasure = (CTreasure*)pScene;
				D3DXVECTOR2 pos(pTreasure->GetPos().x, pTreasure->GetPos().y);
				float rot = pTreasure->GetRot().z;
				float width = pTreasure->GetWidth();
				float height = pTreasure->GetHeight();
				CreateOBBInfo(&treasureOBB, &pos, &rot, &width, &height);
				// �󕨂��������甲����
				break;
			}
			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
		// ���̃v���C�I���e�B�ŕ�������Ă����玟�ɂ�����
		if (pTreasure){
			break;
		}
	}
	// �����ŕ󕨂̃C���X�^���X���擾���s������OUT
	if (!pTreasure){
		return;
	}
	// �󂪒n�ʂɗ����Ă���Ƃ��̂ݔ��������
	if (pTreasure->GetTreasureState() != TREASURE_STATE_OWNER_NONE){
		return;
	}
	// �t�B�[���h�Ƃ̓����蔻�胋�[�v
	for (int priority = 0; priority < TYPE_PRIORITY_THREAD_OF_FOOTHOLD+1; priority++)
	{
		// �擪���w��
		pScene = CScene::GetTopAddress(priority);

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->GetNextAddress();

			CJudge::OBB_INFO fieldOBB;

			if (pScene->GetObjType() == CScene::OBJTYPE_FIELD){
				// �t�B�[���hOBB���쐬
				pField = (CScene2D*)pScene;
				D3DXVECTOR2 pos(pField->GetJudgePos().x, pField->GetJudgePos().y);
				float rot = pField->GetRot().z;
				float width = pField->GetJudgeWidth();
				float height = pField->GetJudgeHeight();
				CreateOBBInfo(&fieldOBB, &pos, &rot, &width, &height);
			}
			else{
				// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
				pScene = pSceneNext;
				continue;
			}

			// ���łɂ������Ă��A�󕨂������Ă��Ȃ������画�肵�Ȃ�
			if (coli)
			{
				// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
				pScene = pSceneNext;
				continue;
			}

			if (IsOBB(treasureOBB, fieldOBB))
			{
				// �q�b�g�t���O�I��
				coli = true;
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}

	// �S�t�B�[���h�Ɠ����蔻������ē�����Ȃ����������͂˂�����
	if (!coli){
		pTreasure->SetFall();
	}

	// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
	pScene = pSceneNext;
}

//=========================================================================
// ��`�Ƌ�`�̂����蔻��
//=========================================================================
bool CJudge::IsOBB(CJudge::OBB_INFO& rectA, CJudge::OBB_INFO& rectB)
{
	// �v�Z�p
	float A = 0.f, B = 0.f, C = 0.f;
	D3DXVECTOR2 culcVecX, culcVecY, baseAxis, V;

	V = rectB.pos - rectA.pos;

	culcVecX = rectB.axisX * rectB.lengthX;
	culcVecY = rectB.axisY * rectB.lengthY;

	// 1����
	baseAxis = rectA.axisX;

	A = rectA.lengthX;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 2����
	baseAxis = rectA.axisY;

	A = rectA.lengthY;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 3����
	culcVecX = rectA.axisX * rectA.lengthX;
	culcVecY = rectA.axisY * rectA.lengthY;

	baseAxis = rectB.axisX;

	//	V *= -1.f;

	A = rectB.lengthX;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 4����
	baseAxis = rectB.axisY;

	A = rectB.lengthY;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	return true;
}

//=========================================================================
// OBB���쐬�֐�
//=========================================================================
void CJudge::CreateOBBInfo(CJudge::OBB_INFO* outOBB, D3DXVECTOR2* pos, float* rot, float* width, float* height)
{
	MATRIX3 trasrationMatrix, rotationMatrix, wMatrix;
	VECTOR2 positon(pos->x, pos->y);

	Matrix3Identity(&wMatrix);
	Matrix3Rotation(&rotationMatrix, *rot);
	Matrix3Translation(&trasrationMatrix, &positon);

	Matrix3Multiply(&wMatrix, &wMatrix, &rotationMatrix);
	Matrix3Multiply(&wMatrix, &wMatrix, &trasrationMatrix);

	outOBB->pos = *pos;
	outOBB->lengthX = *width * 0.5f;
	outOBB->lengthY = *height * 0.5f;

	outOBB->axisX.x = wMatrix.m11;
	outOBB->axisX.y = wMatrix.m12;

	outOBB->axisY.x = wMatrix.m21;
	outOBB->axisY.y = wMatrix.m22;
}

//=========================================================================
// �����Ɛ����̂����蔻��
//=========================================================================
bool CJudge::ColiRayxRay(Segment &seg1, Segment &seg2, D3DXVECTOR2* outPos)
{
	D3DXVECTOR2 v = seg2.s - seg1.s;
	float Crs_v1_v2 = D3DXVec2Cross(&seg1.v, &seg2.v);
	if (Crs_v1_v2 == 0.0f)
	{
		// ���s���
		return false;
	}

	float Crs_v_v1 = D3DXVec2Cross(&v, &seg1.v);
	float Crs_v_v2 = D3DXVec2Cross(&v, &seg2.v);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	// �e�F�덷
	const float eps = 0.00001f;
	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1)
	{
		// �������Ă��Ȃ�
		return false;
	}

	if (outPos)
	{
		*outPos = seg1.s + seg1.v * t1;
#ifdef _DEBUG
		CDebugProc::Print("X:%f\nY:%f\n", outPos->x, outPos->y);
#endif
	}

	return true;
}


//----EOF----