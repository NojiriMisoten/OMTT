//=============================================================================
//
// CDirectBattleResult�N���X [CDirectBattleResult.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirectBattleResult.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"
#include "../../SCENE/GAME/UI/COverLay.h"
#include "../../TEXTURE/CTexture.h"
//=================================================
// �R���X�g���N�^
//=================================================
CDirectBattleResult::CDirectBattleResult(CManager *pManager) : CDirect(pManager)
{

}

//=================================================
// �f�X�g���N�^
//=================================================
CDirectBattleResult::~CDirectBattleResult(void)
{

}

//=================================================
// ������
//=================================================
void CDirectBattleResult::Init(PLAYER_ID playerID)
{
	m_FrameCount = 0;		// �Œ�
	m_TotalFrame = 180;		// �Z���Ƃɕ�

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();

	CDirect::SetPlayerID(playerID);
}

//=================================================
// �I��
//=================================================
void CDirectBattleResult::Uninit(void)
{

}

//=================================================
// �X�V
//=================================================
void CDirectBattleResult::Update(void)
{
	D3DXVECTOR3 pos[2] = {
		m_pPlayerManager->GetPlayerPos(PLAYER_1),
		m_pPlayerManager->GetPlayerPos(PLAYER_2)
	};

	D3DXVECTOR3 rot[2] = {
		m_pPlayerManager->GetPlayerRot(PLAYER_1),
		m_pPlayerManager->GetPlayerRot(PLAYER_2)
	};

	/* ��������� */
	switch (m_FrameCount)
	{
		// �t���[���ʂ̏���
	case 0:
	{
			  m_pManager->GetCameraManager()->CameraSetToCoord(D3DXVECTOR3(-150.f, 800.0f, .0f)
				  , D3DXVECTOR3(0.f, 0.0f, 0.0f));

			  D3DXVECTOR3 cameraPos = m_pManager->GetCameraManager()->GetCameraPos();
			  D3DXVECTOR3 cameraPosR = m_pManager->GetCameraManager()->GetPosRCamera();
			  m_pManager->GetCameraManager()->CameraMoveToCoord(cameraPos
				  , D3DXVECTOR3(-75.f, 275.0f, -125.0f)
				  , cameraPosR
				  , D3DXVECTOR3(0.f, 0.0f, 0.0f)
				  , 60);
			  break;
	}
	case 2:
	{
			  m_pManager->GetUiManager()->StartOverLay(&COverLay::Data(TEXTURE_LOGO_TITLE, 1.f / 60.f, 20, 30));
			  break;
	}
	case 60:
	{
			   D3DXVECTOR3 cameraPos = m_pManager->GetCameraManager()->GetCameraPos();
			   D3DXVECTOR3 cameraPosR = m_pManager->GetCameraManager()->GetPosRCamera();
			   m_pManager->GetCameraManager()->CameraMoveToCoord(cameraPos
				   , D3DXVECTOR3(0.0f, 150.0f, -250.0f)
				   , cameraPosR
				   , D3DXVECTOR3(0.0f, 25.0f, 0.0f)
				   , 60);
			   break;
	}
	case 120:
		// �����ŃQ�[�W�ރt�F�[�h�C���J�n

		break;

	case 170:
		// �t�@�C�g�\��
		m_pManager->GetUiManager()->StartOverLay(&COverLay::Data(TEXTURE_LOGO_TITLE, 5, 15, 10));
		break;
	}
	/* �����܂Ō� */



	CDirect::CheckEnd();
}

//----EOF----