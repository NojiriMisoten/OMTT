//=============================================================================
//
// CStageSelectクラス [CStageSelect.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CStageSelect.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// ロゴの大きさ
static const D3DXVECTOR2 STAGE_SELECT_LOGO_SIZE = D3DXVECTOR2(800, 150);
// ロゴの座標
static const D3DXVECTOR3 STAGE_SELECT_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 75, 0);
// ステージ絵の大きさ
static const float STAGE_SELECT_WIDTH = 200;
static const float STAGE_SELECT_HEIGHT = 200;
// ステージ絵の座標（左上、右上、左下、右下）
static const float STAGE_SELECT_WIDTH_ONE = SCREEN_WIDTH / 5;
static const D3DXVECTOR3 STAGE_SELECT_POS[] = {
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 1, 300, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 2, 300, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 3, 300, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 4, 300, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 1, 550, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 2, 550, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 3, 550, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 4, 550, 0),
};
// ステージ絵のテクスチャ（左上、右上、左下、右下）
static const TEXTURE_TYPE STAGE_SELECT_TEX[] = {
	TEXTURE_STAGE_1,
	TEXTURE_STAGE_2,
	TEXTURE_STAGE_3,
	TEXTURE_STAGE_4,
	TEXTURE_STAGE_5,
	TEXTURE_STAGE_6,
	TEXTURE_STAGE_7,
	TEXTURE_STAGE_8,
};
// 選択枠の大きさ、色
static const float STAGE_SELECT_FRAME_WIDTH = STAGE_SELECT_WIDTH + 20;
static const float STAGE_SELECT_FRAME_HEIGHT = STAGE_SELECT_HEIGHT + 20;
static const D3DXCOLOR STAGE_SELECT_FRAME_COLOR = D3DXCOLOR(0.9f, 0.1f, 0.0f, 0.0f);
// 選択枠のアルファ変更スピード
static const float STAGE_SELECT_FRAME_AALPHA_SPEED = 0.05f;
// BGのアニメーション移動量
static const float STAGE_SELECT_BG_VELO = 1;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CStageSelect::CStageSelect(void)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CStageSelect::~CStageSelect(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CStageSelect::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	m_nBgDirection = 0;

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);
	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// 背景
	m_pBG = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0),
		static_cast<float>(SCREEN_WIDTH)* 2.1f, static_cast<float>(SCREEN_HEIGHT)* 2.1f,
		TEXTURE_BG_CHARA_SELECT, TYPE_PRIORITY_BG);

	// ロゴ
	m_pLogo = CScene2D::Create(m_pD3DDevice,
		STAGE_SELECT_LOGO_POS,
		STAGE_SELECT_LOGO_SIZE.x, STAGE_SELECT_LOGO_SIZE.y,
		TEXTURE_STAGE_SELECT_LOGO, TYPE_PRIORITY_FIELD);

	// 各ステージの絵
	for (int i = 0; i < STAGE_MAX; i++){
		m_pStage2D[i] = CScene2D::Create(m_pD3DDevice,
			STAGE_SELECT_POS[i],
			STAGE_SELECT_WIDTH, STAGE_SELECT_HEIGHT,
			STAGE_SELECT_TEX[i], TYPE_PRIORITY_UI);
	}

	// 選択中の枠 PRIORITY_UIよりも前に表示したいからEFFECTにした
	m_pFrame2D = CScene2D::Create(m_pD3DDevice,
		STAGE_SELECT_POS[0],
		STAGE_SELECT_FRAME_WIDTH, STAGE_SELECT_FRAME_HEIGHT,
		TEXTURE_NULL, TYPE_PRIORITY_EFFECT);

	// 選択中の枠に色設定
	m_FrameColor = STAGE_SELECT_FRAME_COLOR;
	m_pFrame2D->SetColorPolygon(m_FrameColor);

	// 使用変数初期化
	m_nSelectNum = 0;
	m_bFrameAlphaAdd = true;

	// ＢＧＭ再生
	CManager::PlaySoundA(SOUND_LABEL_BGM_STAGE_SELECT);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CStageSelect::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CStageSelect::Update(void)
{
	// フェイズの更新
	CPhase::Update();
	
	// 背景のアニメーション
	UpdateBG();

	// 選択の更新
	UpdateSelect();

	// 決定ボタンを押したら
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
		CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0)){

		CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);

		// 選択したステージ番号をセット
		CManager::SetStageNum(m_nSelectNum);

		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		// ゲームヘ
		m_pManager->SetNextPhase(MODE_PHASE_GAME);
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CStageSelect* CStageSelect::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CStageSelect* pTitle = NULL;

	// 作成
	pTitle = new CStageSelect;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
// 選択中の更新
//*****************************************************************************
void CStageSelect::UpdateSelect(){
	
	// 入力で選択を変更
	if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_LEFT, 0)){
		if (m_nSelectNum == 0)
			m_nSelectNum = 3;
		else if (m_nSelectNum == 4)
			m_nSelectNum = 7;
		else
			m_nSelectNum--;
	}
	else if (CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_RIGHT, 0)){
		if (m_nSelectNum == 3)
			m_nSelectNum = 0;
		else if (m_nSelectNum == 7)
			m_nSelectNum = 4;
		else
			m_nSelectNum++;
	}
	if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_UP, 0)){
			m_nSelectNum -= 4;
			if (m_nSelectNum < 0)
				m_nSelectNum += STAGE_MAX;
	}
	else if (CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_DOWN, 0)){
		m_nSelectNum += 4;
		if (m_nSelectNum >= STAGE_MAX)
			m_nSelectNum -= STAGE_MAX;
	}

	// 移動の入力があったら
	if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
		CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
		CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
		CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_RIGHT, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_LEFT, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_UP, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_DOWN, 0)){
		// 選択枠の移動
		m_pFrame2D->SetPos(STAGE_SELECT_POS[m_nSelectNum]);
		// なんか不自然だったから色も初期化
		m_bFrameAlphaAdd = true;
		m_FrameColor.a = 0.0f;
		CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
	}

	// 選択枠の色変更
	if (m_bFrameAlphaAdd){
		m_FrameColor.a += STAGE_SELECT_FRAME_AALPHA_SPEED;
		if (m_FrameColor.a > 1.0f){
			m_FrameColor.a = 1.0f;
			m_bFrameAlphaAdd = !m_bFrameAlphaAdd;
		}
	}
	else {
		m_FrameColor.a -= STAGE_SELECT_FRAME_AALPHA_SPEED;
		if (m_FrameColor.a < 0.0f){
			m_FrameColor.a = 0.0f;
			m_bFrameAlphaAdd = !m_bFrameAlphaAdd;
		}
	}
	m_pFrame2D->SetColorPolygon(m_FrameColor);
}

//-----------------------------------------------------------------------------
// BGの更新
//-----------------------------------------------------------------------------
void CStageSelect::UpdateBG(){

	D3DXVECTOR3 pos = m_pBG->GetPos();

	if (m_nBgDirection == 3){
		if (pos.y > SCREEN_HEIGHT){
			m_nBgDirection = 0;
			pos.y = SCREEN_HEIGHT;
		}
	}
	if (m_nBgDirection == 2){
		if (pos.x > SCREEN_WIDTH){
			m_nBgDirection = 3;
			pos.x = SCREEN_WIDTH;
		}
	}
	if (m_nBgDirection == 1){
		if (pos.y < 0){
			m_nBgDirection = 2;
			pos.y = 0;
		}
	}
	if (m_nBgDirection == 0){
		if (pos.x < 0){
			m_nBgDirection = 1;
			pos.x = 0;
		}
	}

	// 左へ
	if (m_nBgDirection == 0) m_vBgVelo = D3DXVECTOR3(-STAGE_SELECT_BG_VELO, 0, 0);
	// 上へ
	if (m_nBgDirection == 1) m_vBgVelo = D3DXVECTOR3(0, -STAGE_SELECT_BG_VELO, 0);
	// 右へ
	if (m_nBgDirection == 2) m_vBgVelo = D3DXVECTOR3(STAGE_SELECT_BG_VELO, 0, 0);
	// 下へ
	if (m_nBgDirection == 3) m_vBgVelo = D3DXVECTOR3(0, STAGE_SELECT_BG_VELO, 0);

	// 移動量加算
	m_pBG->AddPos(m_vBgVelo);
}
//----EOF----