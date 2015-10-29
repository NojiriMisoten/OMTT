//=============================================================================
//
// CResult�N���X [CResult.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CResult.h"
#include "../../MANAGER/CManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CResult ::CResult(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CResult ::~CResult(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CResult::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(TEXTURE_DEFAULT);

	// 
	D3DXVECTOR3 pos((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.f);
	CScene2D* p = CScene2D::Create(pDevice, pos, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, TEXTURE_RESULT);
	p->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);


	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// BGM�Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CResult::Uninit(void)
{
	CManager::StopSound();
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CResult::Update(void)
{

	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_DECIDE))
	{
		CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �^�C�g����
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CResult* CResult::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CResult* pResult = NULL;

	// �쐬
	pResult = new CResult;

	// ������
	pResult->Init(mode, pDevice);

	return pResult;
}

//----EOF----