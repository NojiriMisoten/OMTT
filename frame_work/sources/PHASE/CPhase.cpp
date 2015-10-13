//=============================================================================
//
// CPhase�N���X [CPhase.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CPhase.h"
#include "../SCENE/TITLE/CTitle.h"
#include "../SCENE/GAME/CGame.h"
#include "../SCENE/CSCENE/CScene.h"
#include "../SCENE/RESULT/CResult.h"
#include "../SCENE/OPTION/COption.h"
#include "../SCENE/CHARA_SELECT/CCharaSelect.h"
#include "../SCENE/STAGE_SELECT/CStageSelect.h"
#include "../SCENE/TUTORIAL/CTutorial.h"
#include "../MANAGER/CManager.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CManager*		CPhase::m_pManager = NULL;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPhase ::CPhase(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPhase ::~CPhase(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CPhase ::Init(LPDIRECT3DDEVICE9* pD3DDevice, MODE_PHASE mode)
{
	m_pD3DDevice = pD3DDevice;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CPhase ::Uninit(void)
{

}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CPhase ::Update(void)
{
}


//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CPhase* CPhase::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice, CManager* pManager)
{
	m_pManager = pManager;
	switch(mode)
	{
		// �^�C�g��
		case MODE_PHASE_TITLE:
		{
			CTitle* pTitle;
			pTitle = CTitle::Create(mode, pDevice);
			return pTitle;

			break;
		}
		// �`���[�g���A��
		case MODE_PHASE_TUTORIAL:
		{
			CTutorial* pTutorial;
			pTutorial = CTutorial::Create(mode, pDevice);
			return pTutorial;

			break;
		}
		
		// �L�����I��
		case MODE_PHASE_CHARA_SELECT:
		{
			CCharaSelect* pCharaSelect;
			pCharaSelect = CCharaSelect::Create(mode, pDevice);
			return pCharaSelect;

			break;
		}
		
		// �X�e�[�W�I��
		case MODE_PHASE_STAGE_SELECT:
		{
			CStageSelect* pStageSelect;
			pStageSelect = CStageSelect::Create(mode, pDevice);
			return pStageSelect;

			break;
		}

		// �Q�[��
		case MODE_PHASE_GAME:
		{
			CGame* pGame;
			pGame = CGame::Create(mode, pDevice);
			return pGame;

			break;
		}

		// ���U���g
		case MODE_PHASE_RESULT:
		{
			CResult* pResult;
			pResult = CResult::Create(mode, pDevice);
			return pResult;

			break;
		}

		// �I�v�V����
		case MODE_PHASE_OPTION:
		{
			COption* pOption;
			pOption = COption::Create(mode, pDevice);
			return pOption;

			break;
		}

	}
	return NULL;
}
//----EOF----