//=============================================================================
//
// CManager�N���X [CManager.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CManager.h"
#include "../SCENE/CSCENE/CScene.h"
#include "../SCENE/PAUSE/CPause.h"
#include "../SCENE/GAME/CNowLoading.h"
#include "../INPUT/CInputKeyboard.h"
#include "../CONTROLLER/CControllerManager.h"
#include <process.h>

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const MODE_PHASE INIT_MODE = MODE_PHASE_TITLE;	// �ŏ��̃t�F�C�Y

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
CRenderer*		CManager::m_pRenderer = NULL;
CPhase*			CManager::m_pPhase = NULL;
CSound*			CManager::m_pSound = NULL;
MODE_PHASE		CManager::m_CurPhase = MODE_PHASE_NULL;
MODE_PHASE		CManager::m_NextPhase = MODE_PHASE_NULL;
volatile bool	CManager::m_bEndload = false;
int				CManager::m_nWinPlayerNum = -1;
short			CManager::m_nPlayerNumManual;
short			CManager::m_nPlayerNumCpu;
short			CManager::m_nJoinNum;
int				CManager::m_nStageNum;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager ::CManager(void)
{
	m_pInputKeyboard = NULL;
	m_pInputGamePad = NULL;
	m_pSound = NULL;
	m_pControllerManager = NULL;

	#ifdef _DEBUG
	m_pDebugProc = NULL;
	#endif
	m_pNowLoading = NULL;
	m_bEndload = false;
	m_nWinPlayerNum = 1;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager ::~CManager(void)
{
}

//=============================================================================
// �������֐�
//=============================================================================
HRESULT CManager ::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	m_sendParam.pMyAddr = this;
	m_sendParam.hInstance = hInstance;
	m_sendParam.hWnd = hWnd;
	m_sendParam.bWindow = bWindow;
	m_nWinPlayerNum = 0;
	m_nPlayerNumManual = 0;
	m_nPlayerNumCpu = 0;
	m_nJoinNum = 0;

	m_pRenderer = new CRenderer;
	if(m_pRenderer->Init(hWnd, bWindow) == E_FAIL)
	{
		return E_FAIL;
	}

	// ���[�h�X���b�h�J�n
	m_ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, LoadThred, &m_sendParam, 0, NULL);

	// NowLoading�쐬
	m_pNowLoading = new CNowLoading;
	m_pNowLoading->Init(m_pRenderer->GetDevice());

	// �L�[�{�[�h�̏�����
	m_pInputKeyboard = new CInputKeyboard;
	if(m_pInputKeyboard->Init(hInstance, hWnd) == E_FAIL)
	{
		return E_FAIL;
	}

	// �Q�[���p�b�h�̏�����
	m_pInputGamePad = new CInputGamePad;
	if(m_pInputGamePad->Init(hInstance, hWnd) == E_FAIL)
	{
		return E_FAIL;
	}

	m_pControllerManager = CControllerManager::Create();

	#ifdef _DEBUG
	// �f�o�b�O�v���b�N�쐬
	m_pDebugProc = new CDebugProc;
	m_pDebugProc->Init();
	#endif

	// ���̏�����
	m_pSound = new CSound;
	m_pSound->InitSound(hWnd);

	return S_OK;
}

//=============================================================================
// �I���֐�
//=============================================================================
void CManager ::Uninit(void)
{
	if(m_ThreadHandle != NULL)
	{
		// �ʃX���b�h�I���܂ő҂�
		WaitForSingleObject(m_ThreadHandle,INFINITE);
		if(CloseHandle(m_ThreadHandle))
		{
			m_ThreadHandle = NULL;
		}
	}
#ifdef _DEBUG
	if(m_pDebugProc)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
#endif

	// �L�[�{�[�h�̏I��
	if(m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �R���g���[���[�}�l�[�W���[�̏I��
	if (m_pControllerManager)
	{
		m_pControllerManager->Uninit();
		delete m_pControllerManager;
		m_pControllerManager = NULL;
	}

	// �Q�[���p�b�h�̏I��
	if(m_pInputGamePad)
	{
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}

	// �e�N�X�`���̏I��
	CTexture::Uninit();

	// ���ׂĊJ��
	CScene::ReleaseAll();
	if(m_pRenderer)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// �t�F�C�Y�̏I��
	if(m_pPhase)
	{
		m_pPhase->Uninit();
		delete m_pPhase;
		m_pPhase = NULL;
	}

	// �T�E���h�I��
	if(m_pSound)
	{
		m_pSound->StopSound();
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = NULL;
	}

	// NowLoading�I��
	if(m_pNowLoading)
	{
		m_pNowLoading->Uninit();
		delete m_pNowLoading;
		m_pNowLoading = NULL;
	}
}

//=============================================================================
// �X�V�֐�
//=============================================================================
void CManager ::Update(void)
{
	// �ʃX���b�h�œǂݍ��ݏI�����ĂȂ��Ȃ�
	if(!m_bEndload)
	{
		// NowLoading�̍X�V
		m_pNowLoading->Update();
		return;
	}

	// �ʃX���b�h���c���Ă�Ȃ�
	DWORD ThreadStatus;
	if(m_ThreadHandle != NULL)
	{
		// �X���b�h��Ԏ擾
		GetExitCodeThread(m_ThreadHandle, &ThreadStatus);

		// �����ĂȂ��Ȃ�
		if(ThreadStatus != STILL_ACTIVE)
		{
			// �n���h������
			CloseHandle(m_ThreadHandle);
			m_ThreadHandle = NULL;

			// NowLoading�I��
			if(m_pNowLoading)
			{
				m_pNowLoading->Uninit();
				delete m_pNowLoading;
				m_pNowLoading = NULL;
			}
		}
	}

	// �L�[�{�[�h�X�V
	m_pInputKeyboard->Update();

	// �Q�[���p�b�h�X�V
	m_pInputGamePad->Update();

	// �t�F�[�h�C�����I����Ă�Ȃ�
	if(CFade::GetMode() == MODE_FADE_IN_END)
	{
		// �t�F�[�Y�X�V
		m_pPhase->Update();

		// �|�[�Y������Ȃ����
		if(CPause::GetPauseFlag() == false)
		{
			// �|�[�Y�ȊO�X�V
			m_pRenderer->Update();
		}

		// �|�[�Y���Ȃ�
		else
		{
			// �|�[�Y�̂ݍX�V
			m_pRenderer->Update(TYPE_PRIORITY_PAUSE);
		}
	}

	// �t�F�[�h���Ȃ�
	else
	{
		// �t�F�[�h�̂ݍX�V
		m_pRenderer->Update(TYPE_PRIORITY_FADE);
	}

	// ���g���C�t���O�I���Ȃ�
	if(CPause::GetRetryFlag() == true)
	{
		// �t�F�[�h�A�E�g���I����Ă�Ȃ�
		if(CFade::GetMode() == MODE_FADE_OUT_END)
		{
			// ���g���C��
			CManager::RetryPhase();
		}
	}
}

//=============================================================================
// �`��֐�
//=============================================================================
void CManager ::Draw(void)
{
	if(!m_bEndload)
	{
		m_pNowLoading->Draw();
		return;
	}

	// �t�F�[�Y�؂�ւ�
	if (m_CurPhase != m_NextPhase)
	{
		ChangePhase();
	}

	// �|�[�Y������Ȃ����
	if(CPause::GetPauseFlag() == false)
	{
		// �|�[�Y�ȊO�`��
		m_pRenderer->Draw();
	}
	else
	{
		// �|�[�Y�ƃt�F�[�h�̂ݕ`��
		m_pRenderer->Draw(TYPE_PRIORITY_PAUSE, TYPE_PRIORITY_FADE);
	}
}

//=============================================================================
// �t�F�[�Y�؂�ւ��֐�
//=============================================================================
void CManager ::ChangePhase(void)
{
	// �t�F�[�h�A�E�g���I����Ă�Ȃ�
	if(CFade::GetMode() == MODE_FADE_OUT_END)
	{
		// �T�E���h�S�Ē�~
		m_pSound->StopSound();

		// ���ׂĊJ��
		CScene::ReleaseAll();

		// ���݂̃t�F�[�Y�j��
		if(m_pPhase)
		{
			m_pPhase->Uninit();
			delete m_pPhase;
			m_pPhase = NULL;
		}

		// ���̃t�F�[�Y����
		m_pPhase = m_pPhase->Create(m_NextPhase, m_pRenderer->GetDevice(), this);

		// ���݂̃t�F�[�Y���X�V
		m_CurPhase = m_NextPhase;
	}
}

//=============================================================================
// �t�F�[�Y���g���C�֐�
//=============================================================================
void CManager ::RetryPhase(void)
{
	// ���ׂĊJ��
	CScene::ReleaseAll();

	// ���݂̃t�F�[�Y�j��
	m_pPhase->Uninit();
	delete m_pPhase;

	// ���̃t�F�[�Y����
	m_pPhase = m_pPhase->Create(m_NextPhase, m_pRenderer->GetDevice(), this);
	m_CurPhase = m_NextPhase;
}

//=============================================================================
// �������֐�
//=============================================================================
unsigned __stdcall CManager :: LoadThred(LPVOID Param)
{
	SEND_LOAD_THRED_PARAM* p = (SEND_LOAD_THRED_PARAM*)Param;

	// �e�N�X�`���̍쐬
	CTexture::CreateTexture(p->pMyAddr->m_pRenderer->GetDevice());

	// �t�F�C�Y�̍쐬
	p->pMyAddr->m_pPhase = p->pMyAddr->m_pPhase->Create(INIT_MODE, p->pMyAddr->m_pRenderer->GetDevice(), p->pMyAddr);
	p->pMyAddr->m_CurPhase = INIT_MODE;
	p->pMyAddr->m_NextPhase = INIT_MODE;

	p->pMyAddr->m_bEndload = true;

	return 0;
}

//----EOF----