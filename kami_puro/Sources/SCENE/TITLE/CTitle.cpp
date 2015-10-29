//=============================================================================
//
// CTitle�N���X [CTitle.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CTitle.h"
#include "../../MANAGER/CManager.h"
#include "../../TEXTURE/CTexture.h"
#include "../../CAMERA/CameraManager.h"
#include "../../LIGHT/CLightManager.h"
#include "../../BASE_OBJECT/CScene2D.h"
//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float			MOV_POW = D3DX_PI / 120.f;											// �ω���

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTitle ::CTitle(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTitle ::~CTitle(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CTitle::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(TEXTURE_DEFAULT);

	D3DXVECTOR3 pos((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.f);
	CScene2D* p = CScene2D::Create(pDevice, pos, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, TEXTURE_TITLE);
	p->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// BGM�Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CTitle::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CTitle::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_DECIDE))
	{
		CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �Q�[���w
		m_pManager->SetNextPhase(MODE_PHASE_GAME);
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CTitle* CTitle::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CTitle* pTitle = NULL;

	// �쐬
	pTitle = new CTitle;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//----EOF----