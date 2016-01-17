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
#include "TITLE_PLAYER/CTItlePlayer.h"
#include "../GAME/PLAYER/CPlayer.h"
#include "LOGO/CLogo.h"
#include "../GAME/UI/CCutIn.h"
//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int PLAY_TIME_OF_TITLE_BGM_FIRST = 12;	// �^�C�g���̍ŏ���BGM�̉��t����
static const D3DXVECTOR3 INIT_CAMERA_POS(0.f, 50.0f, -250.0f);
static const D3DXVECTOR3 INIT_CAMERA_POSR(0.f, 50.0f, 0.0f);
static const D3DXVECTOR3 DEFAULT_PLAYER_1_POS = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);
static const D3DXVECTOR3 DEFAULT_PLAYER_2_POS = D3DXVECTOR3(+50.0f, 0.0f, 0.0f);
static const D3DXVECTOR3 DEFAULT_PLAYER_1_ROT = D3DXVECTOR3(0.0f, +D3DX_PI * 0.5f, 0.0f);
static const D3DXVECTOR3 DEFAULT_PLAYER_2_ROT = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);
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
	m_pPlayer = NULL;
	m_pTitleLogo = NULL;
	m_pFade = NULL;
	m_isTitlePlay = false;
	m_pCutIn1 = NULL;
	m_pCutIn2 = NULL;
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


	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	time(&m_time);

	// �J�����쐬
	m_pCameraManager = m_pManager->GetCameraManager();
	D3DXVECTOR3	cameraPos = INIT_CAMERA_POS;
	D3DXVECTOR3	cameraPosR = INIT_CAMERA_POSR;
	m_pCameraManager->CreateCamera(cameraPos, cameraPosR);

	// �v���C���[���
	m_pPlayer = CTitlePlayer::Create(pDevice, (D3DXVECTOR3)DEFAULT_PLAYER_1_POS, (D3DXVECTOR3)DEFAULT_PLAYER_1_ROT, SKIN_MESH_TYPE_PLAYER01, m_pManager, 0);
	m_pPlayer2 = CTitlePlayer::Create(pDevice, (D3DXVECTOR3)DEFAULT_PLAYER_2_POS, (D3DXVECTOR3)DEFAULT_PLAYER_2_ROT, SKIN_MESH_TYPE_PLAYER01, m_pManager, 1);
	m_pPlayer->SetAnimSpd(DEFFAULT_ANIM_SPD);
	m_pPlayer2->SetAnimSpd(DEFFAULT_ANIM_SPD);
	m_pPlayer->SetAnimType(CPlayer::PLAYER_WAIT);
	m_pPlayer2->SetAnimType(CPlayer::PLAYER_WAIT);

	// �����̃^�C�g�����S
	m_pTitleLogo = CLogo::Create(pDevice
								, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0)
								, SCREEN_WIDTH * 0.5f
								, SCREEN_HEIGHT * 0.5f
								, TEXTURE_LOGO_TITLE);
	m_pTitleLogo->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pTitleLogo->SetDrawFlag(false);

	m_pCutIn1 = CCutIn::Create(pDevice);
	m_pCutIn1->SetPos(D3DXVECTOR3((float)-SCREEN_WIDTH, 100.f, 0.f));
	m_pCutIn1->SetHeight(200.f);
	m_pCutIn2 = CCutIn::Create(pDevice);
	m_pCutIn2->SetPos(D3DXVECTOR3((float)-SCREEN_WIDTH, (float)SCREEN_HEIGHT - 100.f, 0.f));
	m_pCutIn2->SetHeight(200.f);

	//�I�u�W�F�N�g�̃Z�b�g
	ObjectSet(pDevice);

	// �t�F�[�h�p
	m_pFadeTex = CLogo::Create(pDevice
		, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0)
		, (float)SCREEN_WIDTH
		, (float)SCREEN_HEIGHT
		, TEXTURE_DEFAULT);
	m_pFadeTex->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFadeTex->SetDrawFlag(false);

	// BGM�Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM_TITLE_FIRST);
	m_PlayerAnimTimer = 0;
	m_isPlayBGM = false;
	m_isTitlePlay = false;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CTitle::Uninit(void)
{
	// ����~
	CManager::StopSound();

	SAFE_DELETE(m_FireAnimatior);

	if (m_pCameraManager)
	{
		m_pCameraManager->Uninit();
		m_pCameraManager = NULL;
	}

	SAFE_DELETE(m_pCutIn1);
	SAFE_DELETE(m_pCutIn2);

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
			//CManager::PlaySoundA(SOUND_LABEL_BGM_TITLE_LOOP);
			m_isPlayBGM = true;
		}
	}
	// �t�F�C�Y�̍X�V(�t�F�C�Y�̋��ʂ̍X�V,���͉������Ȃ�����?)
	CPhase::Update();

	//�I�u�W�F�N�g�̍X�V
	if (m_isTitlePlay)
	{
		ObjectUpdate();
	}

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
	// �^�C�g���v���C���[�̍X�V
	else
	{
		m_pCutIn1->Update();
		m_pCutIn2->Update();
		TitlePlayerAnim();
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
	m_BG.Scene2D = CScene2D::Create(pDevice, BG_FIRST_POS, BG_FIRST_WIDTH, BG_FIRST_HEIGHT, TEXTURE_BG_TITLE);
	m_BG.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

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
	m_BG_Jiji.Scene2D = CScene2D::Create(pDevice, FIRE_FIRST_POS, BG_FIRST_WIDTH, BG_FIRST_HEIGHT, TEXTURE_JIJI_TITLE);
	m_BG_Jiji.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

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
	
	m_BG.Scene2D->SetDrawFlag(false);
	m_ScrollBG.Scene2D->SetDrawFlag(false);
	m_Fire.Scene2D->SetDrawFlag(false);
	m_BG_Jiji.Scene2D->SetDrawFlag(false);
	m_AnyCongaLogo.Scene2D->SetDrawFlag(false);
	m_Logo.Scene2D->SetDrawFlag(false);
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

//*****************************************************************************
// �^�C�g���v���C���[�̃A�j���[�V����
//*****************************************************************************
void CTitle::TitlePlayerAnim()
{
	switch (m_PlayerAnimTimer)
	{
	case 120:
		m_pTitleLogo->StartFadeIN(60);
		m_pTitleLogo->SetDrawFlag(true);
		m_pPlayer->SetDrawFlag(false);
		m_pPlayer2->SetDrawFlag(false);
		break;

	case 240:
		m_pTitleLogo->SetDrawFlag(false);
		m_pPlayer->SetDrawFlag(true);
		m_pPlayer2->SetDrawFlag(true);
		m_pPlayer->SetDrawMode(CTitlePlayer::DRAW_MODE_HALF);
		m_pPlayer2->SetDrawMode(CTitlePlayer::DRAW_MODE_HALF);
		m_pPlayer->SetAnimType(CPlayer::PLAYER_LARIAT_RIGHT, 0.0);
		m_pPlayer2->SetAnimType(CPlayer::PLAYER_LARIAT_DAMAGE, 0.0);

		m_pCameraManager->CameraSetToCoord(D3DXVECTOR3(80, 80, 20)
											, D3DXVECTOR3(0, 50, 0));
		break;

	case 340:
		m_pPlayer->SetDrawFlag(false);
		m_pPlayer2->SetDrawFlag(false);

		m_pTitleLogo->StartFadeIN(60);
		m_pTitleLogo->SetDrawFlag(true);
		m_pTitleLogo->SetGrayScalColor(D3DXVECTOR3(0.298912f, 0.586611f, 0.114478f));
		break;

	case 460:
		m_pTitleLogo->SetDrawFlag(false);
		m_pPlayer->SetDrawFlag(true);
		m_pPlayer2->SetDrawFlag(true);
		m_pPlayer->SetDrawMode(CTitlePlayer::DRAW_MODE_NORMAL);
		m_pPlayer2->SetDrawMode(CTitlePlayer::DRAW_MODE_NORMAL);
		m_pPlayer->SetPos(D3DXVECTOR3(-50.f, 0.f, 0.f));
		m_pPlayer->SetRot(D3DXVECTOR3(0.f, D3DX_PI * 1.5f, 0.f));
		m_pPlayer2->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_pPlayer->SetAnimType(CPlayer::PLAYER_BACKDROP_DAMAGE, 0.0);
		m_pPlayer2->SetAnimType(CPlayer::PLAYER_BACKDROP, 0.0);

		m_pCameraManager->CameraSetToCoord(D3DXVECTOR3(0.f, 150, 50.f)
											, D3DXVECTOR3(0, 50, 0));
		break;

	case 630:
		m_pPlayer->SetDrawFlag(false);
		m_pPlayer2->SetDrawFlag(false);

		m_pTitleLogo->StartFadeIN(60);
		m_pTitleLogo->SetDrawFlag(true);
		m_pTitleLogo->SetGrayScalColor(D3DXVECTOR3(1.f, 1.f, 1.f));
		break;

	case 650:
		CManager::PlaySoundA(SOUND_LABEL_BGM_TITLE_LOOP);
		m_pCutIn1->Start(0, CutInType::CUT_IN_JIJII);
		m_pCutIn2->Start(1, CutInType::CUT_IN_JIJII);
		break;

//	case 660:
//		break;

	case 665:
		m_pFadeTex->StartFadeIN(60);
		m_pFadeTex->SetDrawFlag(true);
		m_pFadeTex->SetGrayScalColor(D3DXVECTOR3(1.f, 1.f, 1.f));
		break;

	case 725:
		m_pFadeTex->StartFadeOUT(60);
		m_pTitleLogo->SetDrawFlag(false);
		
		m_BG.Scene2D->SetDrawFlag(true);
		m_ScrollBG.Scene2D->SetDrawFlag(true);
		m_Fire.Scene2D->SetDrawFlag(true);
		m_BG_Jiji.Scene2D->SetDrawFlag(true);
		m_AnyCongaLogo.Scene2D->SetDrawFlag(true);
		m_Logo.Scene2D->SetDrawFlag(true);
		m_isTitlePlay = true;
		break;

	case 2010:
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		m_pManager->RetryPhase();
		break;

	default:
		break;
	}

	m_PlayerAnimTimer++;
}

//----EOF----