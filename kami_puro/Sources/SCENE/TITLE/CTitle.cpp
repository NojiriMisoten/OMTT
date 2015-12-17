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
#include "TitleConfig.h"
#include "../../MANAGER/CManager.h"
#include "../../TEXTURE/CTexture.h"
#include "../../CAMERA/CameraManager.h"
#include "../../LIGHT/CLightManager.h"
#include "../../BASE_OBJECT/CScene2D.h"
#include "TextureAnimator\CTextureAnimator.h"
#include <time.h>
//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int PLAY_TIME_OF_TITLE_BGM_FIRST = 12;	// �^�C�g���̍ŏ���BGM�̉��t����

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTitle ::CTitle(void)
{
	//�g���I�u�W�F�N�g��0����
	memset(&m_BG, 0, sizeof(POLYGON_PARAMETER));
	memset(&m_ScrollBG, 0, sizeof(POLYGON_PARAMETER));
	memset(&m_Fire, 0, sizeof(POLYGON_PARAMETER));
	memset(&m_Logo, 0, sizeof(POLYGON_PARAMETER));
	memset(&m_AnyCongaLogo, 0, sizeof(POLYGON_PARAMETER));
	memset(&m_BG_Jiji, 0, sizeof(POLYGON_PARAMETER));
	m_FireAnimatior = NULL;

	isObject = false;
	m_isPlayBGM = false;
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

	//�I�u�W�F�N�g�̃Z�b�g
	ObjectSet(pDevice);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	time(&m_time);

	// BGM�Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM_TITLE_FIRST);
	m_isPlayBGM = false;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CTitle::Uninit(void)
{
	// ����~
	CManager::StopSound();

	SAFE_DELETE(m_FireAnimatior);

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CTitle::Update(void)
{
	if (!m_isPlayBGM)
	{
		time_t cur;
		time(&cur);

		int time = (int)(cur - m_time);
		if (time >= PLAY_TIME_OF_TITLE_BGM_FIRST)
		{
			// BGM�Đ�
			CManager::PlaySoundA(SOUND_LABEL_BGM_TITLE_LOOP);
			m_isPlayBGM = true;
		}
	}
	// �t�F�C�Y�̍X�V(�t�F�C�Y�̋��ʂ̍X�V,���͉������Ȃ�����?)
	CPhase::Update();

	//�I�u�W�F�N�g�̍X�V
	ObjectUpdate();

	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_DECIDE)
		|| CInputGamePad::GetGamePadTrigger(CInputGamePad::CONTROLLER_DECIDE, 0)
		|| CInputGamePad::GetGamePadTrigger(CInputGamePad::CONTROLLER_DECIDE, 1)
		)
	{
		CManager::PlaySoundA(SOUND_LABEL_SE_DECIDE);

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

//*****************************************************************************
//�I�u�W�F�N�g��������
//*****************************************************************************
void CTitle::ObjectSet(LPDIRECT3DDEVICE9* pDevice)
{
	// �e2D�摜�̐���

	//�w�i�|���S���̐���
	m_BG.Color     = BASE_COLOR;
	CScene2D* m_BG = CScene2D::Create(pDevice, BG_FIRST_POS, BG_FIRST_WIDTH, BG_FIRST_HEIGHT, TEXTURE_BG_TITLE);
	m_BG->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//�X�N���[������w�i
	m_ScrollBG.V3Offset = SCROLLBG_FIRST_POS;
	m_ScrollBG.Color    = BASE_COLOR;
	m_ScrollBG.UV       = BASE_UV;
	m_ScrollBG.Scene2D  = CScene2D::Create(pDevice, SCROLLBG_FIRST_POS, SCROLLBG_FIRST_WIDTH, SCROLLBG_FIRST_HEIGHT, TEXTURE_BG_SCROLL_TITLE);
	m_ScrollBG.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//���|���S��
	m_Fire.V3Offset = FIRE_FIRST_POS;
	m_Fire.Color    = BASE_COLOR;
	m_Fire.UV       = BASE_UV;
	m_Fire.Scene2D  = CScene2D::Create(pDevice, FIRE_FIRST_POS, FIRE_FIRST_WIDTH, FIRE_FIRST_HEIGHT, TEXTURE_FIRE_TITLE);
	m_Fire.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FireAnimatior = CTextureAnimator::Create(SCROLLBG_ANIM_MAX_X, SCROLLBG_ANIM_MAX_Y, m_Fire.Scene2D, SCROLLBG_ANIM_SPD);

	// ��
	m_BG_Jiji.Color = BASE_COLOR;
	CScene2D* m_BG_Jiji = CScene2D::Create(pDevice, FIRE_FIRST_POS, BG_FIRST_WIDTH, BG_FIRST_HEIGHT, TEXTURE_JIJI_TITLE);
	m_BG_Jiji->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//HitAnyConga
	m_AnyCongaLogo.V3Offset = HITANYCONG_FIRST_POS;
	m_AnyCongaLogo.Color    = BASE_COLOR;
	m_AnyCongaLogo.Scene2D  = CScene2D::Create(pDevice, HITANYCONG_FIRST_POS, HITANYCONG_FIRST_WIDTH, HITANYCONG_FIRST_HEIGHT, TEXTURE_LOGOANY_TITLE);
	m_AnyCongaLogo.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//�^�C�g�����S�@
	m_Logo.V3Offset = LOGO_FIRST_POS;
	m_Logo.Scene2D  = CScene2D::Create(pDevice, LOGO_FIRST_POS, LOGO_FIRST_WIDTH, LOGO_FIRST_HEIGHT, TEXTURE_LOGO_TITLE);
	m_Logo.Color    = LOGO_COLOR;
	m_Logo.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//�I�u�W�F�N�g���܂�����`�̒l
	isObject = true;

}

//*****************************************************************************
//�I�u�W�F�N�g�̈ړ�����
//*****************************************************************************
void CTitle::ObjectMove()
{
	//m_Fire.Scene2D->SetVertexPolygonX(sinf(D3DX_PI * m_fCount)*m_Fire.V3Offset.x*0.5f + m_Fire.V3Offset.x);
	//m_Fire.Scene2D->SetVertexPolygonY(cosf(D3DX_PI * m_fCount)*m_Fire.V3Offset.y*0.5f + m_Fire.V3Offset.y);
}

//*****************************************************************************
//�I�u�W�F�N�g��UV�X�V����
//*****************************************************************************
void CTitle::ObjectAnimation()
{
	//�X�N���[������BG�̃A�j���[�V��������
	m_ScrollBG.UV.top    += SCROLL_SPD;
	m_ScrollBG.UV.bottom += SCROLL_SPD;
	m_ScrollBG.Scene2D->SetUV(m_ScrollBG.UV.top, m_ScrollBG.UV.bottom);

	m_FireAnimatior->Update();
}

//*****************************************************************************
//�I�u�W�F�N�g�̐F�̍X�V����
//*****************************************************************************
void CTitle::ObjectColorSet()
{
	static float s_fBlinkingCount = 0;

	//�q�b�g�R���K�̃��S����������o������
	m_AnyCongaLogo.Color.a = abs(cosf(D3DX_PI * s_fBlinkingCount));
	m_AnyCongaLogo.Scene2D->SetColorPolygon(m_AnyCongaLogo.Color);
	s_fBlinkingCount += BLINKING_SPD;

}

//*****************************************************************************
//�I�u�W�F�N�g�̍X�V����
//*****************************************************************************
void CTitle::ObjectUpdate()
{
	//�I�u�W�F�N�g����������Ă���Ɗm��ł��Ă��鎞�ɃI�u�W�F�N�g�̍X�V���s��
	if (isObject)
	{
		//�I�u�W�F�N�g�̈ړ�����
		ObjectMove();
		//�I�u�W�F�N�g��UV�X�V����
		ObjectAnimation();
		//�I�u�W�F�N�g�̐F�̍X�V����
		ObjectColorSet();
	}
}

//----EOF----