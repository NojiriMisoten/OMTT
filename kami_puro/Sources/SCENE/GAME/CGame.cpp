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

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int DEFAULT_BATTLE_TIMER = 99 * 60;		// ���� * FPS
static const int INTORO_ANIMATION_FRAME = 60 * 3;


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGame ::CGame(void)
{
	m_pUiManager = NULL;
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
	D3DXVECTOR3	cameraPos(0.0f, 50.0f, -150.0f);
	D3DXVECTOR3	cameraPosR(0.f, 0.f, 0.f);
	pCameraManager->CreateCamera(cameraPos, cameraPosR);

	// �v���C���[�쐬
	m_pManager->GetPlayerManager()->CreatePlayer(pDevice, D3DXVECTOR3(0, 0, 0), SKIN_MESH_TYPE_TEST);

	// ******TEST*****
	CSceneX::Create(pDevice, D3DXVECTOR3(0, -20, 0), MODEL_TEST, m_pManager);
	CScene3D* p3D = CScene3D::Create(pDevice, D3DXVECTOR3(-30, 0, 0), 20, 20, TEXTURE_PLAYER, m_pManager);
	p3D->SetRot(-D3DX_PI * 0.5f, 0, 0);
	//****************

	// UI�쐬
	m_pUiManager = CUiManager::Create(pDevice);

	// �R�}���h�`���[�g�}�l�[�W���[�̍쐬
	CCommandChartManager::Create(pDevice);

	// �Q�[�����[�h
	m_Mode = GAME_INTRO;
	m_BattleMode = (BATTLE_MODE)-1;

	CEffect *pEffect;
	pEffect = CEffect::Create(30, (char*)L"../data/EFECT/shock_weve001test.efk", true);
	pEffect->Play(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(30, 30, 30));

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

	// �R�}���h�`���[�g�}�[�W���̏I������
	CCommandChartManager::Uninit();

	m_pUiManager->Uninit();
	SAFE_DELETE(m_pUiManager);
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CGame::Update(void)
{
	m_pManager->GetCameraManager()->Update();
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

	// �R�}���h�`���[�g�̍X�V
	CCommandChartManager::Update();

	// UI�̍X�V
	m_pUiManager->Update();

#ifdef _DEBUG
	CDebugProc::Print("Timer:%d\n", (int)(m_BattleTimer / 60));
#endif

	// ���Ԍ���
	m_BattleTimer--;

	// �o�g���I������
	// ���Ԑ���
	if (m_BattleTimer <= 0)
	{
		m_Mode = GAME_FINISH;
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