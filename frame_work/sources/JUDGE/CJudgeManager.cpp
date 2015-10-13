//=============================================================================
//
// CJudgeManager�N���X [CJudgeManager.cpp]
// Author : ��K�@����
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CJudgeManager.h"
#include "CJudge.h"
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"

//=========================================================================
// �R���X�g���N�^
//=========================================================================
CJudgeManager::CJudgeManager(void)
{
	m_pJudge = NULL;
	m_pPlayerManager = NULL;
}

//=========================================================================
// �f�X�g���N�^
//=========================================================================
CJudgeManager::~CJudgeManager(void)
{
	m_pJudge = NULL;
	m_pPlayerManager = NULL;
}

//=========================================================================
// �N���G�C�g
//=========================================================================
CJudgeManager* CJudgeManager::Create(CPlayerManager* playerManager)
{
	CJudgeManager* p = new CJudgeManager;
	
	p->Init(playerManager);

	return p;
}

//=========================================================================
// �I��
//=========================================================================
void CJudgeManager::Uninit(void)
{
	if (m_pJudge)
	{
		delete m_pJudge;
		m_pJudge = NULL;
	}
}

//=========================================================================
// �X�V
//=========================================================================
void CJudgeManager::Update(void)
{
	// �v���C���ƃt�B�[���h�̓����蔻��
	m_pJudge->ColiFieldxPlayer();

	// �v���C���Ƃ���̓����蔻��
	m_pJudge->ColiTreasurexPlayer();

	// �v���C���ƍU���̓����蔻��
	m_pJudge->ColiAttackxPlayer();

	// �v���C���Ǝ��̓����蔻��
	m_pJudge->ColiThreadxPlayer();

	// ���Ǝ��Ƃ̂����蔻��
	m_pJudge->ColiFieldxThreadOfFoothold();

	// ���ƃt�B�[���h�̓����蔻��
	m_pJudge->ColiFieldxTreasure();

	m_pJudge->ColiGoalxPlayer();
}

//=========================================================================
// ������
//=========================================================================
void CJudgeManager::Init(CPlayerManager* playerManager)
{
	m_pJudge = new CJudge(this);
	m_pPlayerManager = playerManager;
}


//----EOF----