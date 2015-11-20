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
#include "../BASE_OBJECT/CScene.h"
#include "../MODEL/CModel.h"
#include "../LOADING/CNowLoading.h"
#include "../INPUT/CInputKeyboard.h"
#include "../CAMERA/CameraManager.h"
#include <process.h>
#include "../SHADER/CShader.h"
#include "../LIGHT/CLightManager.h"
#include "../SCENE/GAME/JUDGE/CJudgeManager.h"
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../EFECT/CEffectManager.h"
#include "../CONTROLLER/CControllerManager.h"
#include "../EFECT/CEffectHolder.h"
#include "../STAGE_DIRECTOR/CDirectorManager.h"
#include "../SCENE/GAME/UI/CUiManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const MODE_PHASE INIT_MODE = MODE_PHASE_GAME;	// �ŏ��̃t�F�C�Y

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
CRenderer*		CManager::m_pRenderer = NULL;
CPhase*			CManager::m_pPhase = NULL;
CSound*			CManager::m_pSound = NULL;
MODE_PHASE		CManager::m_CurPhase = MODE_PHASE_NULL;
MODE_PHASE		CManager::m_NextPhase = MODE_PHASE_NULL;
volatile bool	CManager::m_bEndload = false;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager ::CManager(void)
{
	m_pInputKeyboard = NULL;
	m_pInputGamePad = NULL;
	m_pSound = NULL;

	#ifdef _DEBUG
	m_pDebugProc = NULL;
	#endif
	m_pNowLoading = NULL;
	m_bEndload = false;
	m_pCameraManager = NULL;
	m_pLightManager = NULL;
	m_pControllerManager = NULL;
	m_pJudgeManager = NULL;
	m_pDirectorManager = NULL;
	m_pUiManager = NULL;
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

	m_pRenderer = new CRenderer;
	if(m_pRenderer->Init(hWnd, bWindow) == E_FAIL)
	{
		return E_FAIL;
	}

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

	// �R���g���[���[�̏�����
	m_pControllerManager = CControllerManager::Create();

	// ���[�h�X���b�h�J�n
	m_ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, LoadThred, &m_sendParam, 0, NULL);

	// NowLoading�쐬
	m_pNowLoading = new CNowLoading;
	m_pNowLoading->Init(m_pRenderer->GetDevice());

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

	// �Q�[���p�b�h�̏I��
	if(m_pInputGamePad)
	{
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}

	// �R���g���[���[�̏I��
	if (m_pControllerManager)
	{
		m_pControllerManager->Uninit();
		delete m_pControllerManager;
		m_pControllerManager = NULL;
	}

	// �e�N�X�`���̏I��
	CTexture::Uninit();

	//�G�t�F�N�g�̏I��
	CEffectHolder::Uninit();

	// ���f���쐬
	CModel::Uninit();

	// �V�F�[�_�[�̏I��
	CShader::Uninit();

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

	// �J�����}�l�[�W���[�̏I��
	if(m_pCameraManager)
	{
		m_pCameraManager->Uninit();
		delete m_pCameraManager;
		m_pCameraManager = NULL;
	}

	// ���C�g�}�l�[�W���[�̏I��
	if (m_pLightManager)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}

	// �W���b�W�}�l�[�W���[�̏I��
	if (m_pJudgeManager)
	{
		m_pJudgeManager->Uninit();
		delete m_pJudgeManager;
		m_pJudgeManager = NULL;
	}

	// �v���C���[�}�l�[�W���[�̏I��
	if (m_pPlayerManager)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = NULL;
	}

	// �G�t�F�N�g�}�l�[�W���[�̏I��
	if (m_pEffectManager)
	{
		m_pEffectManager->Uninit();
		delete m_pEffectManager;
		m_pEffectManager = NULL;
	}

	// �f�B���N�^�[�}�l�[�W���[�̏I��
	if( m_pDirectorManager )
	{
		m_pDirectorManager->Uninit();
		delete m_pDirectorManager;
		m_pDirectorManager = NULL;
	}

	// �t�h�}�l�[�W���[�̏I��
	if( m_pUiManager )
	{
		delete m_pUiManager;
		m_pUiManager = NULL;
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
		m_pPhase->Update();
		m_pRenderer->Update();
	}

	// �t�F�[�h���Ȃ�
	else
	{
		// �t�F�[�h�̂ݍX�V
		m_pRenderer->Update(CRenderer::TYPE_RENDER_FADE);
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

	// �J�����Z�b�g
	m_pCameraManager->SetCamera(m_pRenderer->GetDevice());

	// �|�[�Y�ȊO�`��
	m_pRenderer->Draw();
	
	// �t�F�[�Y�؂�ւ�
	if(m_CurPhase != m_NextPhase)
	{
		ChangePhase();
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
// �J�����}�l�[�W���[�Q�b�g
//=============================================================================
CCameraManager* CManager::GetCameraManager(void)
{
	return m_pCameraManager;
}

//=============================================================================
// �������֐�
//=============================================================================
unsigned __stdcall CManager :: LoadThred(LPVOID Param)
{
	SEND_LOAD_THRED_PARAM* p = (SEND_LOAD_THRED_PARAM*)Param;

	//�G�t�F�N�g�}�l�[�W���[����
	p->pMyAddr->m_pEffectManager = new CEffectManager(m_pRenderer->GetDevice());
	p->pMyAddr->m_pEffectManager->Init();

	// �J�����}�l�[�W���[�̍쐬
	p->pMyAddr->m_pCameraManager = new CCameraManager(p->pMyAddr->m_pEffectManager);

	// ���C�g�}�l�[�W���[�̍쐬
	p->pMyAddr->m_pLightManager = new CLightManager(p->pMyAddr);

	// �W���b�W�}�l�[�W���[�̍쐬
	p->pMyAddr->m_pJudgeManager = new CJudgeManager;

	// �v���C���[�}�l�[�W���[�̍쐬
	p->pMyAddr->m_pPlayerManager = new CPlayerManager(p->pMyAddr);
	
	// �f�B���N�^�[�}�l�[�W���[�̍쐬
	p->pMyAddr->m_pDirectorManager = new CDirectorManager( p->pMyAddr );

	// UI�}�l�[�W���[�̍쐬
	p->pMyAddr->m_pUiManager = new CUiManager( m_pRenderer->GetDevice(), p->pMyAddr );


#ifdef _DEBUG
	// �f�o�b�O�v���b�N�쐬
	p->pMyAddr->m_pDebugProc = new CDebugProc;
	p->pMyAddr->m_pDebugProc->Init();
#endif

	// �V�F�[�_�[�̍쐬
	CShader::Create(m_pRenderer->GetDevice());

	// �e�N�X�`���̍쐬
	CTexture::CreateTexture(p->pMyAddr->m_pRenderer->GetDevice());

	//�G�t�F�N�g�̍쐬
	CEffectHolder::CreateEffect();

	// ���f���쐬
	CModel::CreateModel(p->pMyAddr->m_pRenderer->GetDevice());

	// �t�F�C�Y�̍쐬
	p->pMyAddr->m_pPhase = p->pMyAddr->m_pPhase->Create(INIT_MODE, p->pMyAddr->m_pRenderer->GetDevice(), p->pMyAddr);
	p->pMyAddr->m_CurPhase = INIT_MODE;
	p->pMyAddr->m_NextPhase = INIT_MODE;

	p->pMyAddr->m_bEndload = true;

	return 0;
}

//----EOF----