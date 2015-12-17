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
//*****************************************************************************
// マクロ
//*****************************************************************************
static const int PLAY_TIME_OF_TITLE_BGM_FIRST = 12;	// タイトルの最初のBGMの演奏時間

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

	//オブジェクトのセット
	ObjectSet(pDevice);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	time(&m_time);

	// BGM再生
	CManager::PlaySoundA(SOUND_LABEL_BGM_TITLE_FIRST);
	m_isPlayBGM = false;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CTitle::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	SAFE_DELETE(m_FireAnimatior);

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
			CManager::PlaySoundA(SOUND_LABEL_BGM_TITLE_LOOP);
			m_isPlayBGM = true;
		}
	}
	// フェイズの更新(フェイズの共通の更新,今は何もしないかな?)
	CPhase::Update();

	//オブジェクトの更新
	ObjectUpdate();

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
	CScene2D* m_BG = CScene2D::Create(pDevice, BG_FIRST_POS, BG_FIRST_WIDTH, BG_FIRST_HEIGHT, TEXTURE_BG_TITLE);
	m_BG->AddLinkList(CRenderer::TYPE_RENDER_UI);

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
	CScene2D* m_BG_Jiji = CScene2D::Create(pDevice, FIRE_FIRST_POS, BG_FIRST_WIDTH, BG_FIRST_HEIGHT, TEXTURE_JIJI_TITLE);
	m_BG_Jiji->AddLinkList(CRenderer::TYPE_RENDER_UI);

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

//----EOF----