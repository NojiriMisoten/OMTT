//=============================================================================
//
// CManager�N���X [CManager.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CMANAGER_H_
#define _CMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "../RENDERER/CRenderer.h"
#include "../PHASE/CPhase.h"
#include "../SOUND/CSound.h"
#include "../INPUT/CInputGamePad.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CNowLoading;
class CInputKeyboard;
class CInputGamePad;
class CCameraManager;
class CLightManager;
class CJudgeManager;
class CPlayerManager;
class CEffectManager;
class CControllerManager;
class CDirectorManager;
class CUiManager;

#ifdef _DEBUG
class CDebugProc;
#endif

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CManager
{
public:
	// �R���X�g���N�^
	CManager(void);

	// �f�X�g���N�^
	~CManager(void);

	//=======================================================
	// ������
	// ����: �C���X�^���X�A�E�C���h�E�n���h���A�E�C���h�E���[�h�ŕ\�����邩
	// �߂�l: ����
	//=======================================================
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	// �`��
	void Draw(void);

	//=======================================================
	// �����_���[�Q�b�g
	// �߂�l: �����_���[�̃A�h���X
	//=======================================================
	static CRenderer* GetRenderer(void){return m_pRenderer;};

	//=======================================================
	// ���Đ�
	// ����: �T�E���h�̎��
	//=======================================================
	static void PlaySound(SOUND_LABEL label){m_pSound->PlaySound(label);};

	//=======================================================
	// �w�肵������~
	// ����: �T�E���h�̎��
	//=======================================================
	static void StopSound(SOUND_LABEL label){m_pSound->StopSound(label);};
	
	// ���S�Ē�~
	static void StopSound(void){m_pSound->StopSound();};

	// �t�F�[�Y�؂�ւ�
	void ChangePhase(void);

	// �t�F�[�Y��蒼��
	void RetryPhase(void);

	// ���݂̃t�F�[�Y�擾
	MODE_PHASE GetCurPhase(void){return m_CurPhase;};

	// ���̃t�F�[�Y�Z�b�g
	void SetNextPhase(MODE_PHASE phase){m_NextPhase = phase;};

	// �J�����}�l�[�W���[�Q�b�g
	CCameraManager* GetCameraManager(void);

	// ���C�g�}�l�[�W���[�Q�b�g
	CLightManager* GetLightManager(void){ return m_pLightManager; };

	// �W���b�W�}�l�[�W���[�Q�b�g
	CJudgeManager* GetJudgeManager(void){ return m_pJudgeManager; };

	//�v���C���[�}�l�[�W���[�Q�b�g
	CPlayerManager* GetPlayerManager(void){ return m_pPlayerManager; };

	// �f�B���N�^�[�}�l�[�W���[�Q�b�g
	CDirectorManager* GetDirectorManager( void ) { return m_pDirectorManager; };

	// �t�h�}�l�[�W���[�Q�b�g
	CUiManager* GetUiManager( void ) { return m_pUiManager; };

private:
	// NOW LOADING�̃X���b�h�ɓn�����\����
	typedef struct
	{
		CManager* pMyAddr;		// CManager�̃A�h���X
		HINSTANCE hInstance;	// �C���X�^���X
		HWND hWnd;				// �E�C���h�E�n���h��
		bool bWindow;			// �E�C���h�E���[�h�ŕ\�����邩
	}SEND_LOAD_THRED_PARAM;

	//===============================================
	// ���[�h�X���b�h
	// ����: NOW LOADING�̃X���b�h�ɓn�����\���̃|�C���^
	//===============================================
	static unsigned __stdcall LoadThred(LPVOID Param);

	static CRenderer		*m_pRenderer;				// �����_���[
	static CPhase			*m_pPhase;					// �t�F�C�Y
	static MODE_PHASE		m_CurPhase;					// ���݂̃t�F�[�Y
	static MODE_PHASE		m_NextPhase;				// ���̃t�F�[�Y
	static volatile bool	m_bEndload;					// ���[�h�I���t���O
	static CSound			*m_pSound;					// �T�E���h
	CCameraManager			*m_pCameraManager;			// �J�����}�l�[�W���[
	CLightManager			*m_pLightManager;			// ���C�g�}�l�[�W���[
	CJudgeManager			*m_pJudgeManager;			// �W���b�W�}�l�[�W���[
	CPlayerManager			*m_pPlayerManager;			// �v���C���[�}�l�[�W���[
	CEffectManager			*m_pEffectManager;			// �G�t�F�N�g�}�l�[�W���[
	CDirectorManager		*m_pDirectorManager;		// �f�B���N�^�[�}�l�[�W���[
	CInputKeyboard			*m_pInputKeyboard;			// �L�[�{�[�h
	CInputGamePad			*m_pInputGamePad;			// �Q�[���p�b�h
	CControllerManager		*m_pControllerManager;		// �R���g���[���[�}�l�[�W���[
	CUiManager				*m_pUiManager;				// UI�}�l�[�W���[
	SEND_LOAD_THRED_PARAM	m_sendParam;				// ���[�h�X���b�h�ɓn�����
	HANDLE					m_ThreadHandle;				// �X���b�h�̃n���h��
	CNowLoading				*m_pNowLoading;				// NowLoading
	bool					m_isRetry;
	#ifdef _DEBUG
	CDebugProc				*m_pDebugProc;				// �f�o�b�O�v���b�N
	#endif
};
#endif
//----EOF----