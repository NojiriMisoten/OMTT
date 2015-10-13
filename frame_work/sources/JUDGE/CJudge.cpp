//=============================================================================
//
// CJudgeクラス [CJudge.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//=============================================================================
// インクルード
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
// コンストラクタ
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
// デストラクタ
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
// フィールドとプレイヤーのあたり判定
//=========================================================================
void CJudge::ColiFieldxPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// プレイヤーの最大人数分用意
	CScene2D *pField;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// プレイヤー情報入れる
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

		// OBB情報作成
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// フィールドとの当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_FIELD)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// フィールド情報入れる
			pField = (CScene2D*)pScene;
			D3DXVECTOR2 pos(pField->GetJudgePos().x, pField->GetJudgePos().y);
			float rot = pField->GetRot().z;
			float width = pField->GetJudgeWidth();
			float height = pField->GetJudgeHeight();
			CJudge::OBB_INFO fieldOBB;
			// OBB情報作成
			CreateOBBInfo(&fieldOBB, &pos, &rot, &width, &height);

			if (priority == TYPE_PRIORITY_FIELD)
			{
				// プレイヤー乗ってる情報初期化
				CField* p = (CField*)pField;
				p->Ride(false);
			}
			else if (priority == TYPE_PRIORITY_THREAD_OF_FOOTHOLD)
			{
				// プレイヤー乗ってる情報初期化
				CThreadSpecialSpeed* p = (CThreadSpecialSpeed*)pField;
				p->Ride(false);
			}

			// 当たり判定
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// すでにあたってるなら判定しない
				if (coli[idx])
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], fieldOBB))
				{
					// ヒットフラグオン
					coli[idx] = true;

					// 最後に当たった場所更新
					m_LastFieldColiPlayer[idx] = pField;

					if (priority == TYPE_PRIORITY_FIELD)
					{
						// プレイヤー乗ってる情報初期化
						CField* p = (CField*)pField;
						p->Ride(true);
					}
					else if (priority == TYPE_PRIORITY_THREAD_OF_FOOTHOLD)
					{
						// プレイヤー乗ってる情報初期化
						CThreadSpecialSpeed* p = (CThreadSpecialSpeed*)pField;
						p->Ride(true);
					}
				}
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}

	// フィールド外に居るやつの座標変更
	for (int idx = 0; idx < playerNum; ++idx)
	{
		// 当たってるやつは乗ってるので無視
		if (coli[idx])
		{
			continue;
		}

		// NULLチェック
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

		// コの字なので順番注意
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
// 攻撃とプレイヤーのあたり判定
//=========================================================================
void CJudge::ColiAttackxPlayer(void){

	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// プレイヤーの最大人数分用意
	CScene2D *pAttack;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;

	// プレイヤー情報入れる
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

		// OBB情報作成
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// 攻撃との当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_ATTACK)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// 攻撃情報入れる
			pAttack = (CScene2D*)pScene;
			CAttackBase* pAttackBase = (CAttackBase*)pScene;
			D3DXVECTOR2 pos(pAttack->GetPos().x, pAttack->GetPos().y);
			float rot = pAttack->GetRot().z;
			float width = pAttack->GetWidth();
			float height = pAttack->GetHeight();
			CJudge::OBB_INFO attackOBB;
			// OBB情報作成
			CreateOBBInfo(&attackOBB, &pos, &rot, &width, &height);

			// 当たり判定
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// 既にそのプレイヤーにあったてるならやらない
				bool hit = pAttackBase->GetHitFlag(idx);
				if (hit == true)
				{
					continue;
				}

				// 自分の攻撃か
				if (idx == (int)pAttackBase->GetPlayerNumber())
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], attackOBB))
				{
					// 当たった時の処理
					pAttackBase->HitPlayer(pPlayer[idx]);
				}
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}
}

//=========================================================================
// 糸とプレイヤーのあたり判定
//=========================================================================
void CJudge::ColiThreadxPlayer(void){

	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// プレイヤーの最大人数分用意
	CScene2D *pThread;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// プレイヤー情報入れる
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

		// OBB情報作成
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// 攻撃との当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_THREAD)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// 糸情報入れる
			pThread = (CScene2D*)pScene;
			CThreadBase* pThreadBase = (CThreadBase*)pScene;
			D3DXVECTOR2 pos(pThread->GetPos().x, pThread->GetPos().y);
			float rot = pThread->GetRot().z;
			float width = pThread->GetWidth();
			float height = pThread->GetHeight();
			CJudge::OBB_INFO attackOBB;
			// OBB情報作成
			CreateOBBInfo(&attackOBB, &pos, &rot, &width, &height);

			// 当たり判定
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// すでにあたってるなら判定しない
				if (coli[idx])
				{
					continue;
				}

				// 自分の攻撃か
				if (idx == (int)pThreadBase->GetPlayerNum())
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], attackOBB))
				{
					// ヒットフラグオン
					coli[idx] = true;

					// プレイヤに当たった
					pThreadBase->HitPlayer(pPlayer[idx]);
				}
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}
}

//=========================================================================
// フィールドと足場になる糸のあたり判定
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


	// 判定すべきものがないなら
	if (!pSceneThread)
	{
		return;
	}

	// 糸ループ
	while (pSceneThread)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNextThread = pSceneThread->GetNextAddress();

		// 足場になってないやつなら
		if (pSceneThread->GetObjType() != CScene::OBJTYPE_FIELD)
		{
			// 糸情報取得
			pThread = (CScene2D*)pSceneThread;
			D3DXVECTOR2 pos(pThread->GetPos().x, pThread->GetPos().y);
			float rot = pThread->GetRot().z;
			float width = pThread->GetWidth() * 0.8f;
			float height = pThread->GetHeight() * 0.8f;
			CThreadBase* threadBase = (CThreadBase*)pSceneThread;
			int playerNum = threadBase->GetPlayerNum();
			lastCheckField = m_LastFieldColiPlayer[playerNum];

			// OBB情報作成
			CreateOBBInfo(&threadOBB, &pos, &rot, &width, &height);

			for (int prioryty = TYPE_PRIORITY_0; prioryty < TYPE_PRIORITY_MAX; ++prioryty)
			{
				pSceneField = CScene::GetTopAddress(prioryty);

				// フィールドとの当たり判定ループ
				while (pSceneField)
				{
					// 現在対象としているインスタンスの次のインスタンスを保存
					pSceneNextField = pSceneField->GetNextAddress();

					if (pSceneField->GetObjType() == CScene::OBJTYPE_FIELD)
					{
						// フィールド情報取得
						pField = (CScene2D*)pSceneField;
						D3DXVECTOR2 pos2(pField->GetJudgePos().x, pField->GetJudgePos().y);
						float rot2 = pField->GetRot().z;
						float width2 = pField->GetJudgeWidth();
						float height2 = pField->GetJudgeHeight();

						// 自分が乗ってる床ならやらない
						CScene2D* lastField = m_LastFieldColiPlayer[playerNum];
						if (lastField)
						{
							if (lastField == pField)
							{
								// 次のインスタンスを対象のインスタンスにする
								pSceneField = pSceneNextField;
								continue;
							}
						}

						// フィールドのOBB情報作成
						CreateOBBInfo(&fieldOBB, &pos2, &rot2, &width2, &height2);

						// 当たり判定
						if (IsOBB(threadOBB, fieldOBB))
						{
							if (lastCheckField)
							{
								D3DXVECTOR2 lastCheckPos(lastCheckField->GetJudgePos().x, lastCheckField->GetJudgePos().y);
								float lastCheckRot = lastCheckField->GetRot().z;
								float lastCheckWidth = lastCheckField->GetJudgeWidth() * 1.5f;
								float lastCheckHeight = lastCheckField->GetJudgeHeight() * 1.5f;
								// 最後に調べたフィールドのOBB情報作成
								CreateOBBInfo(&lastCheckOBB, &lastCheckPos, &lastCheckRot, &lastCheckWidth, &lastCheckHeight);

								// 最後に調べた床と今の床が当たってるか
								if (IsOBB(lastCheckOBB, fieldOBB))
								{
									// 最後に調べた床更新
									lastCheckField = pField;

									// 次のインスタンスを対象のインスタンスにする
									pSceneField = pSceneNextField;
									continue;
								}
							}

							// フィールドにタイプ変更
							pThread->SetObjType(CScene::OBJTYPE_FIELD);

							// 道作成エフェクト生成
							CEffectManager::CreateEffect(pThread->GetPos(), EFFECT_CREATE_ROAD, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

							hit = true;
							break;
						}

					}

					// 次のインスタンスを対象のインスタンスにする
					pSceneField = pSceneNextField;
				}

				// 既に調べている糸が当たってたら次の糸へ
				if (hit)
				{
					hit = false;
					break;
				}
			}
		}

		// 次のインスタンスを対象のインスタンスにする
		pSceneThread = pSceneNextThread;
	}
}

//=========================================================================
// 宝箱とプレイヤーのあたり判定
//=========================================================================
void CJudge::ColiTreasurexPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// プレイヤーの最大人数分用意
	CTreasure *pTreasure;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// プレイヤー情報入れる
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

		// OBB情報作成
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// 宝との当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_TREASURE)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// 宝の情報入れる
			pTreasure = (CTreasure*)pScene;
			D3DXVECTOR2 pos(pTreasure->GetPos().x, pTreasure->GetPos().y);
			float rot = pTreasure->GetRot().z;
			float width = pTreasure->GetWidth();
			float height = pTreasure->GetHeight();
			CJudge::OBB_INFO treasureOBB;

			// 宝を画面外にださない処理 by 塚本
			if (pTreasure->GetTreasureState() != TREASURE_STATE_JUMPING){
				pos.x = max(pos.x, width * 0.5f);
				pos.x = min(pos.x, SCREEN_WIDTH - width * 0.5f);
				pos.y = max(pos.y, height * 0.5f);
				pos.y = min(pos.y, SCREEN_HEIGHT - height * 0.5f);
				pTreasure->SetPos(D3DXVECTOR3(pos));
			}

			// OBB情報作成
			pos.y += pTreasure->GetHeight() * 0.25f;
			height *= 0.5f;
			CreateOBBInfo(&treasureOBB, &pos, &rot, &width, &height);

			// 当たり判定
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// すでにあたってるなら判定しない
				if (coli[idx])
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], treasureOBB))
				{
					// ヒットフラグオン
					coli[idx] = true;
					break;
				}
			}
			// 次のインスタンスを対象のインスタンスにする
			//			pScene = pSceneNext;

			// お宝は一つだから抜ける
			break;
		}
	}

	// 当たり判定なんとなく見やすいかなと思ってここに分けた
	if (pTreasure->GetTreasureState() != TREASURE_STATE_OWNED &&
		pTreasure->GetTreasureState() != TREASURE_STATE_JUMPING){
		for (int idx = 0; idx < playerNum; ++idx){
			if (coli[idx]){
				// プレイヤにお宝を渡す
				pPlayer[idx]->SetTreasure(pTreasure);
				pTreasure->SetTreasureState(TREASURE_STATE_OWNED);
				CEffectManager::CreateEffect(pPlayer[idx]->GetPos(), EFFECT_FLAG_GET, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				break;
			}
		}
	}
}

//=========================================================================
// プレイヤーとゴールのあたり判定
//=========================================================================
void CJudge::ColiGoalxPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// プレイヤーの最大人数分用意
	CGoal *pGoal = NULL;
	CGoal *pGoalHit = NULL;
	int nHitGoalPlayerNum;
	CGoal *pGoalAll[GOAL_MAX] = { NULL };
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// プレイヤー情報入れる
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

		// OBB情報作成
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// 当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_GOAL)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// フィールド情報入れる
			pGoal = (CGoal*)pScene;
			D3DXVECTOR2 pos(pGoal->GetPos().x, pGoal->GetPos().y);

			float rot = pGoal->GetRot().z;
			float width = pGoal->GetWidth();
			float height = pGoal->GetHeight();
			CJudge::OBB_INFO goalOBB;
			// OBB情報作成
			CreateOBBInfo(&goalOBB, &pos, &rot, &width, &height);

			// プレイヤ番号を配列番号としてゴールのポインタを格納
			pGoalAll[pGoal->GetPlayerNum()] = pGoal;

			// 当たり判定
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// 宝物を持っていなかったら次へ
				if (pPlayer[idx]->GetTreasure() == NULL){
					continue;
				}

				// すでにあたってるなら判定しない
				if (coli[idx])
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], goalOBB))
				{
					// 当たったゴールのインスタンスを保存
					pGoalHit = pGoal;
					nHitGoalPlayerNum = pGoal->GetPlayerNum();

					// ヒットフラグオン
					coli[idx] = true;
				}
			}
			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}

	// 当たり判定チェック
	for (int idx = 0; idx < playerNum; ++idx){
		if (coli[idx]){

			// プレイヤに対応したゴールじゃなければさよなら
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
// フィールドと宝箱のあたり判定
//=========================================================================
void CJudge::ColiFieldxTreasure(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CTreasure *pTreasure = NULL;
	CScene2D *pField = NULL;
	CJudge::OBB_INFO treasureOBB;
	bool coli = false;

	// フィールドとの当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() == CScene::OBJTYPE_TREASURE){
				// 宝OBB情報作成
				pTreasure = (CTreasure*)pScene;
				D3DXVECTOR2 pos(pTreasure->GetPos().x, pTreasure->GetPos().y);
				float rot = pTreasure->GetRot().z;
				float width = pTreasure->GetWidth();
				float height = pTreasure->GetHeight();
				CreateOBBInfo(&treasureOBB, &pos, &rot, &width, &height);
				// 宝物を見つけたら抜ける
				break;
			}
			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
		// このプライオリティで宝を見つけていたら次にすすむ
		if (pTreasure){
			break;
		}
	}
	// ここで宝物のインスタンスが取得失敗したらOUT
	if (!pTreasure){
		return;
	}
	// 宝が地面に落ちているときのみ判定をする
	if (pTreasure->GetTreasureState() != TREASURE_STATE_OWNER_NONE){
		return;
	}
	// フィールドとの当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_THREAD_OF_FOOTHOLD+1; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			CJudge::OBB_INFO fieldOBB;

			if (pScene->GetObjType() == CScene::OBJTYPE_FIELD){
				// フィールドOBB情報作成
				pField = (CScene2D*)pScene;
				D3DXVECTOR2 pos(pField->GetJudgePos().x, pField->GetJudgePos().y);
				float rot = pField->GetRot().z;
				float width = pField->GetJudgeWidth();
				float height = pField->GetJudgeHeight();
				CreateOBBInfo(&fieldOBB, &pos, &rot, &width, &height);
			}
			else{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// すでにあたってか、宝物が落ちていなかったら判定しない
			if (coli)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			if (IsOBB(treasureOBB, fieldOBB))
			{
				// ヒットフラグオン
				coli = true;
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}

	// 全フィールドと当たり判定をして当たらなかったら宝をはねさせる
	if (!coli){
		pTreasure->SetFall();
	}

	// 次のインスタンスを対象のインスタンスにする
	pScene = pSceneNext;
}

//=========================================================================
// 矩形と矩形のあたり判定
//=========================================================================
bool CJudge::IsOBB(CJudge::OBB_INFO& rectA, CJudge::OBB_INFO& rectB)
{
	// 計算用
	float A = 0.f, B = 0.f, C = 0.f;
	D3DXVECTOR2 culcVecX, culcVecY, baseAxis, V;

	V = rectB.pos - rectA.pos;

	culcVecX = rectB.axisX * rectB.lengthX;
	culcVecY = rectB.axisY * rectB.lengthY;

	// 1軸目
	baseAxis = rectA.axisX;

	A = rectA.lengthX;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 2軸目
	baseAxis = rectA.axisY;

	A = rectA.lengthY;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 3軸目
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

	// 4軸目
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
// OBB情報作成関数
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
// 線分と線分のあたり判定
//=========================================================================
bool CJudge::ColiRayxRay(Segment &seg1, Segment &seg2, D3DXVECTOR2* outPos)
{
	D3DXVECTOR2 v = seg2.s - seg1.s;
	float Crs_v1_v2 = D3DXVec2Cross(&seg1.v, &seg2.v);
	if (Crs_v1_v2 == 0.0f)
	{
		// 平行状態
		return false;
	}

	float Crs_v_v1 = D3DXVec2Cross(&v, &seg1.v);
	float Crs_v_v2 = D3DXVec2Cross(&v, &seg2.v);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	// 容認誤差
	const float eps = 0.00001f;
	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1)
	{
		// 交差していない
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