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
#include "JUDGE/CJudgeManager.h"
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
	m_pCameraManager = m_pManager->GetCameraManager();
	D3DXVECTOR3	cameraPos(0.0f, 50.0f, -150.0f);
	D3DXVECTOR3	cameraPosR(0.f, 0.f, 0.f);
	m_pCameraManager->CreateCamera(cameraPos, cameraPosR);

	// �t�B�[���h
	CSceneX* pX = CSceneX::Create(pDevice, D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_RING, m_pManager);
	pX->SetScl(3.0f, 2.0f, 3.0f);

	m_pCrowdManager = m_pCrowdManager->Create(m_pD3DDevice, m_pManager);

	// �v���C���[�쐬
	m_pManager->GetPlayerManager()->CreatePlayer( pDevice, D3DXVECTOR3( -50, 0, 0 ), SKIN_MESH_TYPE_TEST );

	// UI��
	m_pUiManager = CUiManager::Create(pDevice, m_pManager, this);

	m_pFieldManager = CFieldManager::Create(pDevice, m_pManager);

	// �W���b�W�̍쐬&������
	m_pJudgeManager = m_pManager->GetJudgeManager();
	m_pJudgeManager->Init( m_pManager );
	m_pJudgeManager->SetBattleMode( BATTLE_MOVE );

	// �Q�[�����[�h
	m_Mode = GAME_INTRO;
	m_PrevMode = GAME_MAX;

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

	// �����[�h�̏���������&�O���[�h�̏I������
	if( m_PrevMode != m_Mode )
	{
		// �O���[�h�̏I������
		switch( m_PrevMode )
		{
		case GAME_INTRO:
			UninitGameIntro();
			break;

		case GAME_BATTLE:
			UninitGameBattle();
			break;

		case GAME_FINISH:
			UninitGameFinish();
			break;
		}

		// �����[�h�̏���������
		switch( m_Mode )
		{
		case GAME_INTRO:
			InitGameIntro();
			break;

		case GAME_BATTLE:
			InitGameBattle();
			break;

		case GAME_FINISH:
			InitGameFinish();
			break;
		}
	}
	m_PrevMode = m_Mode;

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
	// �W���b�W�̍X�V����
	m_pJudgeManager->Update();

	// UI�̍X�V
	m_pUiManager->Update();

#ifdef _DEBUG
	CDebugProc::PrintR( "[GAME]\n" );
	CDebugProc::PrintR( "Timer:%d\n", (int)( m_BattleTimer / TARGET_FPS ) );
	CDebugProc::PrintR( "GameMode:" );
	switch( m_Mode )
	{
	case GAME_INTRO:
		CDebugProc::PrintR( "GAME_INTRO" );
		break;

	case GAME_BATTLE:
		CDebugProc::PrintR( "GAME_BATTLE" );
		break;

	case GAME_FINISH:
		CDebugProc::PrintR( "GAME_FINISH" );
		break;
	}
	CDebugProc::PrintR( "\n" );

	CDebugProc::PrintR( "BattleMode:" );
	switch( m_pJudgeManager->GetBattleMode() )
	{
	case BATTLE_MOVE:
		CDebugProc::PrintR( "BATTLE_MOVE" );
		break;

	case BATTLE_FIGHT:
		CDebugProc::PrintR( "BATTLE_FIGHT" );
		break;
	}
	CDebugProc::PrintR( "\n\n" );
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
// �o�g���O������
//*****************************************************************************
void CGame::InitGameIntro( void )
{

}

//*****************************************************************************
// �o�g����������
//*****************************************************************************
void CGame::InitGameBattle( void )
{
	m_BattleTimer = DEFAULT_BATTLE_TIMER;

	m_pCameraManager->CameraSetToCoord(
		D3DXVECTOR3( 0.0f, 150.0f, -250.0f ),
		D3DXVECTOR3( 0.0f, 25.0f, 0.0f ) );
}

//*****************************************************************************
// �o�g���I��������
//*****************************************************************************
void CGame::InitGameFinish( void )
{

}

//*****************************************************************************
// �o�g���O�I������
//*****************************************************************************
void CGame::UninitGameIntro( void )
{

}

//*****************************************************************************
// �o�g�����I������
//*****************************************************************************
void CGame::UninitGameBattle( void )
{

}

//*****************************************************************************
// �o�g���I���I������
//*****************************************************************************
void CGame::UninitGameFinish( void )
{

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