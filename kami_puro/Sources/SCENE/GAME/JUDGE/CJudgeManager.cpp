//=============================================================================
//
// CJudgeManagerManager�N���X [CJudgeManager.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CJudgeManager.h"
#include "CJudge.h"
#include "../../../CONST/const.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CJudgeManager::CJudgeManager(void)
{
	m_pJudge = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CJudgeManager::~CJudgeManager(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CJudgeManager::Init(CManager* pManager)
{
	m_pJudge = CJudge::Create(pManager);
}

//=============================================================================
// �I��
//=============================================================================
void CJudgeManager::Uninit(void)
{
	if (m_pJudge)
	{
		m_pJudge->Uninit();
		delete m_pJudge;
		m_pJudge = NULL;
	}
}

//----EOF----