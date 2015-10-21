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
//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int DEFAULT_BATTLE_TIMER = 99 * 60;		// ���� * FPS

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

	// �Q�[�����[�h
	m_Mode = GAME_INTRO;
	m_BattleMode = (BATTLE_MODE)-1;

	CPlayerManager::CreatePlayer(pDevice, D3DXVECTOR3(0, 0, 0), SKIN_MESH_TYPE_TEST);

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
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CGame::Update(void)
{
	m_pManager->GetCameraManager()->Update();


	// �����[�h�̎��s
	switch( m_Mode )
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

#ifdef _DEBUG
	CDebugProc::Print( "GameMode:%d\n", m_Mode );
	CDebugProc::Print( "BattleMode:%d\n", m_Mode );
#endif

	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// ���U���g��
		m_pManager->SetNextPhase(MODE_PHASE_RESULT);
	}
}

void CGame::GameIntro( void )
{
	// �Ƃ肠����GAME_BATTLE�Ɉȍ~
	m_Mode = GAME_BATTLE;
}

void CGame::GameBattle( void )
{
	// GameBattle������
	if( m_BattleMode == -1 )
	{
		m_BattleTimer = DEFAULT_BATTLE_TIMER;
		m_BattleMode = BATTLE_MOVE;
	}

	// BattleMode�`�F�b�N�i�ړ����U�����j
	switch( m_BattleMode )
	{
	// �ړ����[�h
	case BATTLE_MOVE:
		// �v���C���[���m���߂Â�����
		if( 0 )
		{
			// �퓬���[�h�ڍs
			m_BattleMode = BATTLE_FIGHT;
		}
		break;


	// �퓬���[�h
	case BATTLE_FIGHT:
		// �v���C���[���m�����ꂽ��
		if( 0 )
		{
			// �ړ����[�h�ڍs
			m_BattleMode = BATTLE_MOVE;
		}
		break;
	}

#ifdef _DEBUG
	CDebugProc::Print( "Timer:%d\n", (int)( m_BattleTimer / 60 ) );
#endif

	// ���Ԍ���
	m_BattleTimer--;

	// �o�g���I������
	// ���Ԑ���
	if( m_BattleTimer <= 0 )
	{
		m_Mode = GAME_FINISH;
	}
	
	// �̗�0
	if( 0 )
	{
		m_Mode = GAME_FINISH;
	}
}

void CGame::GameFinish( void )
{
	// �Ƃ肠�����I��
	// �t�F�[�h�A�E�g�J�n
	m_pFade->Start( MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME );

	// ���U���g��
	m_pManager->SetNextPhase( MODE_PHASE_RESULT );
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