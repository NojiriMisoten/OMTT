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
#include "COMMANDCHART\CCommandChartManager.h"
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

	// �R�}���h�`���[�g�̐���
	CCommandChartManger::Create(pDevice);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ���Đ�
	//CManager::PlaySoundA(SOUND_LABEL_BGM001);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CGame::Uninit(void)
{
	// �R�}���h�`���[�g�̏I��
	CCommandChartManger::Uninit();
	CManager::StopSound();
	CPhase::Uninit();
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
	// �R�}���h�`���[�g�̍X�V�ƕ`��
	CCommandChartManger::Update();
	CCommandChartManger::Draw();
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