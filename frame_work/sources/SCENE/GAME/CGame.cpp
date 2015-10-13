//=============================================================================
//
// CGame�N���X [CGame.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneScore.h"
#include "CTimeManager.h"
#include "PLAYER\CPlayerManager.h"
#include "TREASURE/CTreasureManager.h"
#include "GOAL/CGoalManager.h"
#include "ATTACK\CAttackManager.h"
#include "THREAD\CThreadManager.h"
#include "CGame.h"
#include "../../JUDGE/CJudgeManager.h"
#include "FIELD/CFieldManager.h"
#include "UI\CCountDown.h"
#include "../../BACKGROUND/CBackGroundManager.h"
#include "EFFECT\CEffectManager.h"
#include "UI\CWinDraw.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

// �Q�[���̐�������
static const short GAME_TIME = 200;

// �󕨂̏ꏊ
static const D3DXVECTOR3 TREASURE_POS = D3DXVECTOR3(647, 315, 0);

// �S�[��
static const D3DXVECTOR3 GOAL_POS[GOAL_MAX] = {
	D3DXVECTOR3(50, 50, 0),
	D3DXVECTOR3(1230, 50, 0),
	D3DXVECTOR3(50, 670, 0),
	D3DXVECTOR3(1230, 670, 0),
};
static const short GOAL_PLAYER_NUMBER[GOAL_MAX] = {
	0, 1, 2, 3
};

// �w�i�̃X�N���[���̑���
static const float BG_SPEED = 2.0f;

// ���U���g���S��\�����Ă���t�F�[�h����܂ł̃J�E���g��
static const short RESULT_LOGO_TO_FADE_INTERVAL = 180;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGame::CGame(void)
{
	m_pPause = NULL;
	m_pTimeManager = NULL;
	m_pPlayerManager = NULL;
	m_pAttackManager = NULL;
	m_pThreadManager = NULL;
	m_pGoalManager = NULL;
	m_pJudgeManager = NULL;
	m_pFieldManager = NULL;
	m_pCountDown = NULL;
	// ���BGM��炵����
	m_bPlaySoundOnece = false;

	// �ȈՃ��U���g���玟�̃V�[���J�ڂ܂ł̃J�E���^�[
	m_nResultCount = 0;

	// �v���C������\�t���O
	m_bPlayerControl = false;
	// �Q�[���I���t���O
	m_bGameOver = false;
	// �������v���C���[�ԍ�
	m_nWinPlayerNum = -1;
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
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �|�[�Y�쐬
	m_pPause = new CPause(pDevice);
	m_pPause->Init(DEFFAULT_PAUSE_POS, (float)SCREEN_WIDTH, DEFFAULT_PAUSE_HEIGHT, TEXTURE_NULL);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// �Q�[���̎��ԊǗ��Ǝc�莞�ԕ\��
	m_pTimeManager = new CTimeManager(pDevice, GAME_TIME);
	m_pTimeManager->Init();

	// �U���}�l�[�W������
	m_pAttackManager = new CAttackManager(pDevice);
	m_pAttackManager->Init();

	// ���}�l�[�W������
	m_pThreadManager = new CThreadManager(pDevice);
	m_pThreadManager->Init();

	// �󕨐���
	m_pTreasureManager = new CTreasureManager(pDevice);
	m_pTreasureManager->Init();
	m_pTreasureManager->CreateTreasure(TREASURE_POS);

	// �S�[������
	m_pGoalManager = new CGoalManager(pDevice, this);
	m_pGoalManager->Init();
	m_pGoalManager->CreateGoal(
		const_cast<D3DXVECTOR3*>(GOAL_POS),
		const_cast<short*>(GOAL_PLAYER_NUMBER), this);

	// ��������t�B�[���h�ԍ��擾
	m_nStageNum = CManager::GetStageNum();

	// �t�B�[���h�쐬
	m_pFieldManager = new CFieldManager;
	m_pFieldManager->LoadField(m_pD3DDevice, (CFieldManager::FIELD_TYPE)m_nStageNum);

	// ��������v���C���̐����擾
	m_nPlayerNumManual = CManager::GetPlayerManualNum();
	m_nPlayerNumCpu = PLAYER_MAX - m_nPlayerNumManual;

	// �v���C������
	m_pPlayerManager = new CPlayerManager(m_pAttackManager, m_pThreadManager, m_pEffectManager);
	m_pPlayerManager->Init(m_nPlayerNumCpu, m_nPlayerNumManual, &m_bPlayerControl);

	// �w�i�쐬
	m_pBackGroundManager = new CBackGroundManager(pDevice);
	m_pBackGroundManager->Init();
	m_pBackGroundManager->CreateBG(TEXTURE_BG_0);
	m_pBackGroundManager->CreateBG(TEXTURE_BG_1, BG_SPEED);

	// �W���b�W�쐬
	m_pJudgeManager = CJudgeManager::Create(m_pPlayerManager);

	// �J�E���g�_�E��
	m_pCountDown = new CCountDown(m_pD3DDevice, &m_bPlayerControl);
	m_pCountDown->Init();

	// �G�t�F�N�g�}�l�[�W���[
	m_pEffectManager = new CEffectManager(m_pD3DDevice);
	m_pEffectManager->Init();

	// ��������������\������}�l�[�W��
	m_pWinDrawLogo = new CWinDraw(m_pD3DDevice);
	m_pWinDrawLogo->Init();
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CGame::Uninit(void)
{
	if (m_pWinDrawLogo){
		m_pWinDrawLogo->Uninit();
		SAFE_DELETE(m_pWinDrawLogo);
	}
	if (m_pEffectManager){
		m_pEffectManager->Uninit();
		SAFE_DELETE(m_pEffectManager);
	}
	if (m_pCountDown){
		m_pCountDown->Uninit();
		SAFE_DELETE(m_pCountDown);
	}
	if (m_pBackGroundManager){
		m_pBackGroundManager->Uninit();
		SAFE_DELETE(m_pBackGroundManager);
	}
	if (m_pJudgeManager){
		m_pJudgeManager->Uninit();
		SAFE_DELETE(m_pJudgeManager);
	}
	if (m_pFieldManager){
		m_pFieldManager->Uninit();
		SAFE_DELETE(m_pFieldManager);
	}
	if (m_pThreadManager){
		m_pThreadManager->Uninit();
		SAFE_DELETE(m_pThreadManager);
	}
	if (m_pAttackManager){
		m_pAttackManager->Uninit();
		SAFE_DELETE(m_pAttackManager);
	}
	if (m_pPlayerManager){
		m_pPlayerManager->Uninit();
		SAFE_DELETE(m_pPlayerManager);
	}
	if (m_pGoalManager){
		m_pGoalManager->Uninit();
		SAFE_DELETE(m_pGoalManager);
	}
	if (m_pTreasureManager){
		m_pTreasureManager->Uninit();
		SAFE_DELETE(m_pTreasureManager);
	}
	if (m_pTimeManager){
		m_pTimeManager->Uninit();
		SAFE_DELETE(m_pTimeManager);
	}

	CManager::StopSound();
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CGame::Update(void)
{
	// �J�E���g�_�E���̍X�V
	if (m_pCountDown->Update() && !m_bPlaySoundOnece){
		// �J�E���g�_�E�����I�������̂�BGM�𗬂�
		CManager::PlaySoundA(SOUND_LABEL_BGM000);
		// ��񂵂��ʂ�Ȃ�
		m_bPlaySoundOnece = true;
	}

	// �w�i�̍X�V
	m_pBackGroundManager->Update();

	if (!m_pPause->GetPauseFlag())
	{
		CPhase::Update();

		// �v���C���̑��삪�ł���Ȃ�
		if (m_bPlayerControl){
			// �}�l�[�W���[�X�V
			m_pPlayerManager->Update();
			m_pTimeManager->Update();
			m_pTreasureManager->Update();
			m_pJudgeManager->Update();
			m_pFieldManager->Update();
		}
		// �c�莞�Ԃ�0�ɂȂ�����Draw
		if (m_pTimeManager->GetRemaining() == 0){
			SetDraw();
			CManager::PlaySoundA(SOUND_LABEL_SE_END);
		}

		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			// �t�F�[�h�A�E�g�J�n
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

			// ���U���g��
			m_pManager->SetNextPhase(MODE_PHASE_RESULT);
		}

		// �Q�[���N���A�t���O
		if (m_bGameOver)
		{
			Result();
		}
	}
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

//*****************************************************************************
// �������v���C���ԍ��̃Z�b�g ��񂾂��Ă΂��
//*****************************************************************************
void CGame::SetWinPlayer(short num){

	// ���łɃQ�[�����I����Ă����牽�����Ȃ�
	if (m_bGameOver){
		return;
	}
	m_nWinPlayerNum = num;

	// �Q�[�����ȈՃ��U���g�̍X�V�t���O��ON
	m_bGameOver = true;

	m_pWinDrawLogo->CreateWinLogo();

	CManager::PlaySoundA(SOUND_LABEL_SE_END);
}
//*****************************************************************************
// ���������ɂ��� ��񂾂��Ă΂��
//*****************************************************************************
void CGame::SetDraw(){

	// ���łɃQ�[�����I����Ă����牽�����Ȃ�
	if (m_bGameOver){
		return;
	}

	// �Q�[�����ȈՃ��U���g�̍X�V�t���O��ON
	m_bGameOver = true;

	// ���S�̕\��
	m_pWinDrawLogo->CreateDrawLogo();
}

//*****************************************************************************
// �Q�[�����̃��U���g�V�[���̍X�V ���t���[���Ă΂��
//*****************************************************************************
void CGame::Result(){

	// ��������
	if (m_nWinPlayerNum == -1){
		// �c�q�`�v���S�̍X�V�i�A�j���[�V�����j
	}
	m_nResultCount++;

	if (m_nResultCount > RESULT_LOGO_TO_FADE_INTERVAL){
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		// ���U���g��
		m_pManager->SetNextPhase(MODE_PHASE_RESULT);
		// �������v���C���ԍ������U���g�ɑ��邽��Manager�ɑ���
		m_pManager->SetWinPlayerNum(m_nWinPlayerNum);
	}

	//	if (m_pLogoDraw)
	//		m_pLogoDraw->Update();

}
//----EOF-------