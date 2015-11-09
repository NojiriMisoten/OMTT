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
#include "../GAME/UI/CUiManager.h"
#include "PLAYER/CPlayerManager.h"
#include "../../EFECT/CEffect.h"
#include "../../BASE_OBJECT/CSceneX.h"
#include "../../BASE_OBJECT/CScene3D.h"
#include "COMMANDCHART/CCommandChartManager.h"
#include "FIELD/CFieldManager.h"
#include "FIELD/CROWD/CCrowdManager.h"
//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int DEFAULT_BATTLE_TIMER = 99 * 60;		// ���� * FPS
static const int INTORO_ANIMATION_FRAME = 60 * 3;
static const D3DXVECTOR3 DEFAULT_LIGHT_POS(0.0f, 250.0f, -150.0f);

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGame ::CGame(void)
{
	m_pUiManager = NULL;
	m_pFieldManager = NULL;
	m_pCrowdManager = NULL;
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
	m_pFade->Init(TEXTURE_DEFAULT);

	// �J�����쐬
	CCameraManager* pCameraManager = m_pManager->GetCameraManager();
	D3DXVECTOR3	cameraPos(0.0f, 50.0f, -150.0f);
	D3DXVECTOR3	cameraPosR(0.f, 0.f, 0.f);
	pCameraManager->CreateCamera(cameraPos, cameraPosR);

	// �t�B�[���h
	CSceneX* pX = CSceneX::Create(pDevice, D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_RING, m_pManager);
	pX->SetScl(3.0f, 2.0f, 3.0f);

	m_pCrowdManager = m_pCrowdManager->Create(m_pD3DDevice, m_pManager);

	// �v���C���[�쐬
	m_pManager->GetPlayerManager()->CreatePlayer( pDevice, D3DXVECTOR3( -50, 0, 0 ), SKIN_MESH_TYPE_TEST );

	// UI��
	m_pUiManager = CUiManager::Create(pDevice, m_pManager, this);

	m_pFieldManager = CFieldManager::Create(pDevice, m_pManager);

	// �Q�[�����[�h
	m_Mode = GAME_INTRO;
	m_BattleMode = (BATTLE_MODE)-1;

	// ���C�g�̈ʒu����(�e�͂���)
	D3DXVECTOR3 lighPos = DEFAULT_LIGHT_POS;
	m_pManager->GetCameraManager()->SetLightCamera(m_pD3DDevice, lighPos);

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
	CManager::StopSound();
	CPhase::Uninit();

	m_pUiManager->Uninit();
	SAFE_DELETE(m_pUiManager);

	m_pFieldManager->Uninit();
	SAFE_DELETE(m_pFieldManager);

	m_pCrowdManager->Uninit();
	SAFE_DELETE(m_pCrowdManager);
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CGame::Update(void)
{
	m_pManager->GetCameraManager()->Update();
	m_pFieldManager->Update();
	m_pManager->GetLightManager()->Update();
	m_pCrowdManager->Update();
	// �����[�h�̎��s
	switch (m_Mode)
	{
	case GAME_INTRO:
		GameIntro();
		break;

	case GAME_BATTLE:
		GameBattle();
		break;

	case GAME_FINISH:
		GameFinish();
		break;
	}

	// test
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_START_TEST))
	{
		m_pUiManager->StartAnimation(INTORO_ANIMATION_FRAME);
	}

	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_DECIDE))
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �Q�[���w
		m_pManager->SetNextPhase(MODE_PHASE_RESULT);
	}
}

//*****************************************************************************
// �o�g���O
//*****************************************************************************
void CGame::GameIntro(void)
{
	// �Ƃ肠����GAME_BATTLE�Ɉȍ~
	m_Mode = GAME_BATTLE;

	// �J�n�A�j���[�V�����̊J�n
	m_pUiManager->StartAnimation(INTORO_ANIMATION_FRAME);

}

//*****************************************************************************
// �o�g����
//*****************************************************************************
void CGame::GameBattle(void)
{
	// GameBattle������
	if (m_BattleMode == -1)
	{
		m_BattleTimer = DEFAULT_BATTLE_TIMER;
		m_BattleMode = BATTLE_MOVE;
	}

	// BattleMode�`�F�b�N�i�ړ����U�����j
	switch (m_BattleMode)
	{
		// �ړ����[�h
	case BATTLE_MOVE:
		// �v���C���[���m���߂Â�����
		if (0)
		{
			// �퓬���[�h�ڍs
			m_BattleMode = BATTLE_FIGHT;
		}
		break;


		// �퓬���[�h
	case BATTLE_FIGHT:
		// �v���C���[���m�����ꂽ��
		if (0)
		{
			// �ړ����[�h�ڍs
			m_BattleMode = BATTLE_MOVE;
		}
		break;
	}

	// UI�̍X�V
	m_pUiManager->Update();

#ifdef _DEBUG
	CDebugProc::PrintL("[GAME]\n");
	CDebugProc::PrintL("Timer:%d\n", (int)(m_BattleTimer / TARGET_FPS));
	CDebugProc::PrintL("\n");
#endif

	// ���Ԍ���
	m_BattleTimer--;

	// �o�g���I������
	// ���Ԑ���
	if (m_BattleTimer <= 0)
	{
	//	m_Mode = GAME_FINISH;
	}

	// �̗�0
	if (0)
	{
		m_Mode = GAME_FINISH;
	}
}

//*****************************************************************************
// �o�g���I��
//*****************************************************************************
void CGame::GameFinish(void)
{
	// �Ƃ肠�����I��
	// �t�F�[�h�A�E�g�J�n
	m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

	// ���U���g��
	m_pManager->SetNextPhase(MODE_PHASE_RESULT);
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