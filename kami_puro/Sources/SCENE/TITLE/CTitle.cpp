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
#include "../../MANAGER/CManager.h"
#include "../../TEXTURE/CTexture.h"
#include "../../CAMERA/CameraManager.h"
#include "../../LIGHT/CLightManager.h"
#include "../../BASE_OBJECT/CScene2D.h"
//*****************************************************************************
// マクロ
//*****************************************************************************
static const float			MOV_POW = D3DX_PI / 120.f;											// 変化量

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitle ::CTitle(void)
{
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

	D3DXVECTOR3 pos((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.f);
	CScene2D* p = CScene2D::Create(pDevice, pos, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, TEXTURE_TITLE);
	p->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// BGM再生
	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CTitle::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CTitle::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_DECIDE))
	{
		CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
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

//----EOF----