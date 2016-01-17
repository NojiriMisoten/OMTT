//=============================================================================
//
// CTitleクラス [CTitle.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
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
// マクロ
//*****************************************************************************
static const int PLAY_TIME_OF_TITLE_BGM_FIRST = 12;	// タイトルの最初のBGMの演奏時間
static const D3DXVECTOR3 INIT_CAMERA_POS(0.f, 50.0f, -250.0f);
static const D3DXVECTOR3 INIT_CAMERA_POSR(0.f, 50.0f, 0.0f);
static const D3DXVECTOR3 DEFAULT_PLAYER_1_POS = D3DXVECTOR3(-50.0f, 0.0f, 0.0f);
static const D3DXVECTOR3 DEFAULT_PLAYER_2_POS = D3DXVECTOR3(+50.0f, 0.0f, 0.0f);
static const D3DXVECTOR3 DEFAULT_PLAYER_1_ROT = D3DXVECTOR3(0.0f, +D3DX_PI * 0.5f, 0.0f);
static const D3DXVECTOR3 DEFAULT_PLAYER_2_ROT = D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f);
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitle ::CTitle(void)
{
	//使うオブジェクトの0埋め
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
// デストラクタ
//*****************************************************************************
CTitle ::~CTitle(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CTitle::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(TEXTURE_DEFAULT);


	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	time(&m_time);

	// カメラ作成
	m_pCameraManager = m_pManager->GetCameraManager();
	D3DXVECTOR3	cameraPos = INIT_CAMERA_POS;
	D3DXVECTOR3	cameraPosR = INIT_CAMERA_POSR;
	m_pCameraManager->CreateCamera(cameraPos, cameraPosR);

	// プレイヤー作る
	m_pPlayer = CTitlePlayer::Create(pDevice, (D3DXVECTOR3)DEFAULT_PLAYER_1_POS, (D3DXVECTOR3)DEFAULT_PLAYER_1_ROT, SKIN_MESH_TYPE_PLAYER01, m_pManager, 0);
	m_pPlayer2 = CTitlePlayer::Create(pDevice, (D3DXVECTOR3)DEFAULT_PLAYER_2_POS, (D3DXVECTOR3)DEFAULT_PLAYER_2_ROT, SKIN_MESH_TYPE_PLAYER01, m_pManager, 1);
	m_pPlayer->SetAnimSpd(DEFFAULT_ANIM_SPD);
	m_pPlayer2->SetAnimSpd(DEFFAULT_ANIM_SPD);
	m_pPlayer->SetAnimType(CPlayer::PLAYER_WAIT);
	m_pPlayer2->SetAnimType(CPlayer::PLAYER_WAIT);

	// 中央のタイトルロゴ
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

	//オブジェクトのセット
	ObjectSet(pDevice);

	// フェード用
	m_pFadeTex = CLogo::Create(pDevice
		, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0)
		, (float)SCREEN_WIDTH
		, (float)SCREEN_HEIGHT
		, TEXTURE_DEFAULT);
	m_pFadeTex->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFadeTex->SetDrawFlag(false);

	// BGM再生
	CManager::PlaySoundA(SOUND_LABEL_BGM_TITLE_FIRST);
	m_PlayerAnimTimer = 0;
	m_isPlayBGM = false;
	m_isTitlePlay = false;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CTitle::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	SAFE_DELETE(m_FireAnimatior);

	if (m_pCameraManager)
	{
		m_pCameraManager->Uninit();
		m_pCameraManager = NULL;
	}

	SAFE_DELETE(m_pCutIn1);
	SAFE_DELETE(m_pCutIn2);

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
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
			// BGM再生
			//CManager::PlaySoundA(SOUND_LABEL_BGM_TITLE_LOOP);
			m_isPlayBGM = true;
		}
	}
	// フェイズの更新(フェイズの共通の更新,今は何もしないかな?)
	CPhase::Update();

	//オブジェクトの更新
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

		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// ゲームヘ
		m_pManager->SetNextPhase(MODE_PHASE_GAME);
	}
	// タイトルプレイヤーの更新
	else
	{
		m_pCutIn1->Update();
		m_pCutIn2->Update();
		TitlePlayerAnim();
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CTitle* CTitle::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CTitle* pTitle = NULL;

	// 作成
	pTitle = new CTitle;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
//オブジェクト生成処理
//*****************************************************************************
void CTitle::ObjectSet(LPDIRECT3DDEVICE9* pDevice)
{
	// 各2D画像の生成

	//背景ポリゴンの生成
	m_BG.Color     = BASE_COLOR;
	m_BG.Scene2D = CScene2D::Create(pDevice, BG_FIRST_POS, BG_FIRST_WIDTH, BG_FIRST_HEIGHT, TEXTURE_BG_TITLE);
	m_BG.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//スクロールする背景
	m_ScrollBG.V3Offset = SCROLLBG_FIRST_POS;
	m_ScrollBG.Color    = BASE_COLOR;
	m_ScrollBG.UV       = BASE_UV;
	m_ScrollBG.Scene2D  = CScene2D::Create(pDevice, SCROLLBG_FIRST_POS, SCROLLBG_FIRST_WIDTH, SCROLLBG_FIRST_HEIGHT, TEXTURE_BG_SCROLL_TITLE);
	m_ScrollBG.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//炎ポリゴン
	m_Fire.V3Offset = FIRE_FIRST_POS;
	m_Fire.Color    = BASE_COLOR;
	m_Fire.UV       = BASE_UV;
	m_Fire.Scene2D  = CScene2D::Create(pDevice, FIRE_FIRST_POS, FIRE_FIRST_WIDTH, FIRE_FIRST_HEIGHT, TEXTURE_FIRE_TITLE);
	m_Fire.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FireAnimatior = CTextureAnimator::Create(SCROLLBG_ANIM_MAX_X, SCROLLBG_ANIM_MAX_Y, m_Fire.Scene2D, SCROLLBG_ANIM_SPD);

	// 爺
	m_BG_Jiji.Color = BASE_COLOR;
	m_BG_Jiji.Scene2D = CScene2D::Create(pDevice, FIRE_FIRST_POS, BG_FIRST_WIDTH, BG_FIRST_HEIGHT, TEXTURE_JIJI_TITLE);
	m_BG_Jiji.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//HitAnyConga
	m_AnyCongaLogo.V3Offset = HITANYCONG_FIRST_POS;
	m_AnyCongaLogo.Color    = BASE_COLOR;
	m_AnyCongaLogo.Scene2D  = CScene2D::Create(pDevice, HITANYCONG_FIRST_POS, HITANYCONG_FIRST_WIDTH, HITANYCONG_FIRST_HEIGHT, TEXTURE_LOGOANY_TITLE);
	m_AnyCongaLogo.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//タイトルロゴ　
	m_Logo.V3Offset = LOGO_FIRST_POS;
	m_Logo.Scene2D  = CScene2D::Create(pDevice, LOGO_FIRST_POS, LOGO_FIRST_WIDTH, LOGO_FIRST_HEIGHT, TEXTURE_LOGO_TITLE);
	m_Logo.Color    = LOGO_COLOR;
	m_Logo.Scene2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	//オブジェクト作りましたよ～の値
	isObject = true;
	
	m_BG.Scene2D->SetDrawFlag(false);
	m_ScrollBG.Scene2D->SetDrawFlag(false);
	m_Fire.Scene2D->SetDrawFlag(false);
	m_BG_Jiji.Scene2D->SetDrawFlag(false);
	m_AnyCongaLogo.Scene2D->SetDrawFlag(false);
	m_Logo.Scene2D->SetDrawFlag(false);
}

//*****************************************************************************
//オブジェクトの移動処理
//*****************************************************************************
void CTitle::ObjectMove()
{
	//m_Fire.Scene2D->SetVertexPolygonX(sinf(D3DX_PI * m_fCount)*m_Fire.V3Offset.x*0.5f + m_Fire.V3Offset.x);
	//m_Fire.Scene2D->SetVertexPolygonY(cosf(D3DX_PI * m_fCount)*m_Fire.V3Offset.y*0.5f + m_Fire.V3Offset.y);
}

//*****************************************************************************
//オブジェクトのUV更新処理
//*****************************************************************************
void CTitle::ObjectAnimation()
{
	//スクロールするBGのアニメーション処理
	m_ScrollBG.UV.top    += SCROLL_SPD;
	m_ScrollBG.UV.bottom += SCROLL_SPD;
	m_ScrollBG.Scene2D->SetUV(m_ScrollBG.UV.top, m_ScrollBG.UV.bottom);

	m_FireAnimatior->Update();
}

//*****************************************************************************
//オブジェクトの色の更新処理
//*****************************************************************************
void CTitle::ObjectColorSet()
{
	static float s_fBlinkingCount = 0;

	//ヒットコンガのロゴを消したり出したり
	m_AnyCongaLogo.Color.a = abs(cosf(D3DX_PI * s_fBlinkingCount));
	m_AnyCongaLogo.Scene2D->SetColorPolygon(m_AnyCongaLogo.Color);
	s_fBlinkingCount += BLINKING_SPD;

}

//*****************************************************************************
//オブジェクトの更新処理
//*****************************************************************************
void CTitle::ObjectUpdate()
{
	//オブジェクトが生成されていると確約できている時にオブジェクトの更新を行う
	if (isObject)
	{
		//オブジェクトの移動処理
		ObjectMove();
		//オブジェクトのUV更新処理
		ObjectAnimation();
		//オブジェクトの色の更新処理
		ObjectColorSet();
	}
}

//*****************************************************************************
// タイトルプレイヤーのアニメーション
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
		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		m_pManager->RetryPhase();
		break;

	default:
		break;
	}

	m_PlayerAnimTimer++;
}

//----EOF----