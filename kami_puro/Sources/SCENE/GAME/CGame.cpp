//=============================================================================
//
// CGame�N���X [CGame.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CGame.h"
#include "../../MANAGER/CManager.h"
#include "../../TEXTURE/CTexture.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../LIGHT/CLightManager.h"
#include "PLAYER/CPlayerManager.h"

#include "UI\CUiManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGame ::CGame(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CGame ::~CGame(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CGame::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(TEXTURE_NULL);

	// �J�����쐬
	CCameraManager* pCameraManager = m_pManager->GetCameraManager();
	D3DXVECTOR3	cameraPos(0.0f, 100.0f, -250.0f);
	D3DXVECTOR3	cameraPosR(0.f, 0.f, 0.f);
	pCameraManager->CreateCamera(cameraPos, cameraPosR);

	CPlayerManager::CreatePlayer(pDevice, D3DXVECTOR3(0, 0, 0), SKIN_MESH_TYPE_TEST);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// UI�쐬
	m_UiManager = CUiManager::Create(pDevice);

	// ���Đ�
	//CManager::PlaySoundA(SOUND_LABEL_BGM001);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CGame::Uninit(void)
{
	CManager::StopSound();
	CPhase::Uninit();
	
	m_UiManager->Uninit();
	SAFE_DELETE(m_UiManager);
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CGame::Update(void)
{
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �Q�[���w
		m_pManager->SetNextPhase(MODE_PHASE_RESULT);

	}

	m_UiManager->Update();

	// �J�n�A�j���[�V�����̊J�n
	// ����͂܂��ł��Ă��Ȃ�����R�����g�A�E�g
//	if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE))
//	{
//		m_UiManager->StartAnimation();
//	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CGame* CGame::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CGame* pGame = NULL;

	// �쐬
	pGame = new CGame;

	// ������
	pGame->Init(mode, pDevice);

	return pGame;
}
//----EOF-------