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
#include "../CSCENE/CScene2D.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// ゲームロゴ
static const float TITLE_LOGO_WIDTH = 1000;
static const float TITLE_LOGO_HEIGHT = 370;
static const D3DXVECTOR3 TITLE_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 20, 180, 0);
static const short TITLE_YURE = 10;
// PUSH START
static const float TITLE_PUSHSTART_WIDTH = 500;
static const float TITLE_PUSHSTART_HEIGHT = 200;
static const D3DXVECTOR3 TITLE_PUSHSTART_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500, 0);
// MENU
static const float TITLE_MENU_WIDTH = 200;
static const float TITLE_MENU_HEIGHT = 200;
static const D3DXVECTOR3 TITLE_MENU_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 550, 0);
// MENUのカーソル 基本メニューの定数を変えれば変わるようにした
static const int TITLE_MENU_MAX = 2;
static const float TITLE_CURSOL_WIDTH = TITLE_MENU_WIDTH;
static const float TITLE_CURSOL_HEIGHT = TITLE_MENU_HEIGHT / TITLE_MENU_MAX;
static const D3DXVECTOR3 TITLE_CURSOL_POS[TITLE_MENU_MAX] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_MENU_POS.y - TITLE_CURSOL_HEIGHT / TITLE_MENU_MAX, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, TITLE_MENU_POS.y + TITLE_CURSOL_HEIGHT / TITLE_MENU_MAX, 0),
};
// タイトルシーン開始からPUSH STARTが出るまでのカウント
static const int TITLE_TO_PUSHSTART_INTERVAL = 60;
// PUSHSTART点滅の間隔
static const int TITLE_PUSHSTART_FLASH_INTERVAL = 20;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitle::CTitle(void)
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

	// 変数初期化
	m_nCursol = 0;
	m_nCount = 0;
	m_bPushed = false;
	m_bPushStartFlg = true;
	m_nPushStartCount = 0;
	m_nPuruCount = 0;

	m_pPushStart = NULL;
	m_pBackGround = NULL;
	m_pLogo = NULL;
	m_pPushStart = NULL;
	m_pMenuBack = NULL;
	m_pCursol = NULL;

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// 背景
	m_pBackGround = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_BG);
	m_pBackGround->Init(
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_0);
	// Logo
	m_pLogo = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pLogo->Init(
		TITLE_LOGO_POS, TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT,
		TEXTURE_TITLE_LOGO);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ＢＧＭ再生
	CManager::PlaySoundA(SOUND_LABEL_ZINGLE_TITLE);
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

	if (m_pLogo){
		static int count = 0;
		count++;
		if (count > 60)
			count = 0;
		else if (count < 10)
			m_pLogo->SetPos(TITLE_LOGO_POS.x + rand() % TITLE_YURE, TITLE_LOGO_POS.y + rand() % TITLE_YURE, 0);
	}

	// PUSH STARTを出すタイミング
	if (m_nCount == TITLE_TO_PUSHSTART_INTERVAL){
		// 二度とここを通らない
		m_nCount++;
		// PushStart
		m_pPushStart = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
		m_pPushStart->Init(
			TITLE_PUSHSTART_POS, TITLE_PUSHSTART_WIDTH, TITLE_PUSHSTART_HEIGHT,
			TEXTURE_TITLE_PUSHSTART);
	}
	else{
		m_nCount++;
	}

	if (m_pPushStart){
		// PUSHSTARTのアニメーション
		m_nPushStartCount++;
		if (m_nPushStartCount > TITLE_PUSHSTART_FLASH_INTERVAL){
			m_nPushStartCount = 0;
			m_bPushStartFlg = !m_bPushStartFlg;
			m_pPushStart->SetDrawFlag(m_bPushStartFlg);
		}

		// まだ押していない
		if (!m_bPushed){
			// 入力されたらPUSH START実行
			if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_A, 0) ||
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_B, 0) ||
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_X, 0) ||
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_Y, 0)){

				m_bPushed = true;
				PushStart();
				SAFE_RELEASE(m_pPushStart);
				CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
			}
		}
	}
	else{
		// 入力されたらPUSH START実行
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CControllerManager::GetTriggerKey(CInputGamePad::KEY_A, 0) ||
			CControllerManager::GetTriggerKey(CInputGamePad::KEY_B, 0) ||
			CControllerManager::GetTriggerKey(CInputGamePad::KEY_X, 0) ||
			CControllerManager::GetTriggerKey(CInputGamePad::KEY_Y, 0)){

			// まだ押していない
			if (!m_bPushed){
				m_nCount = TITLE_TO_PUSHSTART_INTERVAL;
			}
			// メニューが出ている状態
			else {
				CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
				// げーむ
				if (m_nCursol == 0){
					// フェードアウトしてキャラ選択ヘ
					m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
					m_pManager->SetNextPhase(MODE_PHASE_CHARA_SELECT);
				}
				// あそびかた
				else if (m_nCursol == 1){
					// フェードアウトしてオプション画面ヘ
					m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
					m_pManager->SetNextPhase(MODE_PHASE_TUTORIAL);
				}
			}
		}

	}

	// メニューが出ている状態
	if (m_bPushed){
		if (CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
			CControllerManager::GetReleaseKey(CInputGamePad::LEFT_STICK_DOWN, 0)){
			m_nCursol++;
			if (m_nCursol == TITLE_MENU_MAX){
				m_nCursol = 0;
			}
			m_pCursol->SetPos(TITLE_CURSOL_POS[m_nCursol]);
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
			CControllerManager::GetReleaseKey(CInputGamePad::LEFT_STICK_UP, 0)){
			m_nCursol--;
			if (m_nCursol < 0){
				m_nCursol = TITLE_MENU_MAX - 1;
			}
			m_pCursol->SetPos(TITLE_CURSOL_POS[m_nCursol]);
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}
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
// PUSH STARTしたときに呼ばれる
//*****************************************************************************
void CTitle::PushStart(){

	// メニュー
	m_pMenuBack = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pMenuBack->Init(
		TITLE_MENU_POS, TITLE_MENU_WIDTH, TITLE_MENU_HEIGHT,
		TEXTURE_TITLE_MENU);

	// カーソル
	m_nCursol = 0;
	m_pCursol = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pCursol->Init(
		TITLE_CURSOL_POS[m_nCursol], TITLE_CURSOL_WIDTH, TITLE_CURSOL_HEIGHT,
		TEXTURE_TITLE_CURSOL);
}

//----EOF----