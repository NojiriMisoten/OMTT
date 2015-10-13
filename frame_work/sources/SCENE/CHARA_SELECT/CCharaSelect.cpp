//=============================================================================
//
// CCharaSelectクラス [CCharaSelect.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCharaSelect.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneAnime.h"
#include "../GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// ロゴの大きさ
static const D3DXVECTOR2 CHARASELECT_LOGO_SIZE = D3DXVECTOR2(800, 150);
// ロゴの座標
static const D3DXVECTOR3 CHARASELECT_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 75, 0);
// プレイヤ背景の大きさ
static const D3DXVECTOR2 CHARASELECT_PLAYER_BG_SIZE = D3DXVECTOR2(320, 200);
static const D3DXVECTOR2 CHARASELECT_PLAYER_BG_SIZE_HALF = D3DXVECTOR2(160, 100);
// プレイヤ背景の座標
static const D3DXVECTOR3 CHARASELECT_PLAYER_BG_POS[PLAYER_MAX] = {
	D3DXVECTOR3(330, 280, 0),
	D3DXVECTOR3(950, 280, 0),
	D3DXVECTOR3(330, 510, 0),
	D3DXVECTOR3(950, 510, 0),
};
// PUSH背景大きさ 色
static const D3DXVECTOR2 CHARASELECT_PUSH_BG_SIZE = D3DXVECTOR2(205, 95);
static const D3DXCOLOR CHARASELECT_PUSH_BG_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
// PUSH大きさ
static const D3DXVECTOR2 CHARASELECT_PUSH_SIZE = D3DXVECTOR2(200, 90);
// PUSHの点滅間隔
static const int CHARASELECT_PUSH_FLASH_INTERVAL = 15;
static const D3DXCOLOR CHARASELECT_PUSH_ADDCOLOR = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.0f);
// リモの大きさ
static const D3DXVECTOR2 CHARASELECT_RIMO_SIZE = D3DXVECTOR2(100, 160);
// リモの座標
static const D3DXVECTOR3 CHARASELECT_RIMO_POS[PLAYER_MAX] = {
	D3DXVECTOR3(390, 300, 0),
	D3DXVECTOR3(1010, 300, 0),
	D3DXVECTOR3(390, 530, 0),
	D3DXVECTOR3(1010, 530, 0),
};
// 参戦2D大きさ
static const D3DXVECTOR2 CHARASELECT_JOIN_SIZE = D3DXVECTOR2(180, 80);
// 参戦2D座標
static const D3DXVECTOR3 CHARASELECT_JOIN_POS[PLAYER_MAX] = {
	D3DXVECTOR3(260, 230, 0),
	D3DXVECTOR3(880, 230, 0),
	D3DXVECTOR3(260, 460, 0),
	D3DXVECTOR3(880, 460, 0),
};
// ボタン系大きさ
static const D3DXVECTOR2 CHARASELECT_BUTTON_SIZE = D3DXVECTOR2(200, 80);
static const D3DXVECTOR2 CHARASELECT_BUTTON_SIZE_HALF = D3DXVECTOR2(100, 40);
// 準備完了ボタンの位置
static const D3DXVECTOR3 CHARASELECT_START_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400, 0);
// キーコンフィグボタンの位置
static const D3DXVECTOR3 CHARASELECT_CONFIG_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 610, 0);
// カーソルのマックス
static const int CHARASELECT_CURSOL_MAX = 3;
// カーソルの大きさ
static const D3DXVECTOR2 CHARASELECT_CURSOL_SIZE = D3DXVECTOR2(50, 50);
// プレイヤーごとのカーソルの初期位置
static const D3DXVECTOR3 CHARASELECT_CURSOLR_POS[PLAYER_MAX] = {
	D3DXVECTOR3(440, 220, 0),
	D3DXVECTOR3(1060, 220, 0),
	D3DXVECTOR3(440, 450, 0),
	D3DXVECTOR3(1060, 450, 0),
};
// カーソルの角度
static const float CHARASELECT_CURSOL_RAD_MAX = D3DX_PI / 4;
static const float CHARASELECT_CURSOL_ADDRAD = 0.07f;
// カーソルの色
static const D3DXCOLOR CHARASELECT_CURSOL_ADDCOLOR = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.1f);
// カーソルの移動量
static const float CHARASELECT_CURSOL_VELO = 4;
// カーソルの当たり判定を取るときの調整位置
static const D3DXVECTOR3 CHARASELECT_CURSOL_COLI_DIST = D3DXVECTOR3(-10, 10, 0);
// 準備完了2Dの大きさ
static const D3DXVECTOR2 CHARASELECT_READY_SIZE = D3DXVECTOR2(250, 100);
// プレイヤ背景の座標
static const D3DXVECTOR3 CHARASELECT_READY_POS[PLAYER_MAX] = {
	D3DXVECTOR3(330, 180, 0),
	D3DXVECTOR3(950, 180, 0),
	D3DXVECTOR3(330, 510, 0),
	D3DXVECTOR3(950, 510, 0),
};
// 準備完了テクスチャの色（透過度設定）
static const D3DXCOLOR CHARASELECT_READY_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
// 全員が準備完了してから次のシーンに移行するまでの時間 なんんとなく入れたよ
static const int CHARASELECT_NEXT_PHASE_INTERVAL = 60;
// BGのアニメーション移動量
static const float CHARASELECT_BG_VELO = 1;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCharaSelect::CCharaSelect(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCharaSelect ::~CCharaSelect(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CCharaSelect::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// 変数初期化
	for (int i = 0; i < PLAYER_MAX; i++){
		m_bPush[i] = false;
		m_pPush2D[i] = NULL;
		m_pPush2DBG[i] = NULL;
		m_nPushFlashCount[i] = 0;
		m_bPushFlash[i] = false;
		m_pRimo[i] = NULL;
		m_nRimoTextureIndex[i] = 0;
		m_nRimoTextureCount[i] = 0;
		m_nRimoTextureMax[i] = 0;
		m_nRimoTextureMin[i] = 0;
		m_pJoin2D[i] = NULL;
		m_nCursol[i] = 0;
		m_pCursol2D[i] = NULL;
		m_fCursolRad[i] = 0;
		m_bCursolFlg[i] = false;
		m_bReady[i] = false;
		m_pReady2D[i] = NULL;
		m_nReadyFlashCount[i] = 0;
		m_bReadyFlash[i] = false;
		m_PlayerMode[i] = PLAYER_MODE_NONE;
	}
	m_pConfig = NULL;
	m_pStart2D = NULL;
	m_bNextPhaseOnece = false;
	m_nNextPhaseCount = 0;
	m_nPlayerManualNum = 0;
	m_nPlayerReadyNum = 0;
	m_nBgDirection = 0;

	// 参戦していた数を取得する
	m_nPlayerJoinedNum = CManager::GetJoinNum();

	// 背景
	InitializeBG();

	for (int i = 0; i < PLAYER_MAX; i++){
		// プレイヤ背景
		m_pPlayerBG[i] = CScene2D::Create(m_pD3DDevice,
			CHARASELECT_PLAYER_BG_POS[i],
			CHARASELECT_PLAYER_BG_SIZE.x, CHARASELECT_PLAYER_BG_SIZE.y,
			TEXTURE_CHARA_SELECT_PLAYER_BG, TYPE_PRIORITY_FIELD);
		// プレイヤの色に合わせる
		m_pPlayerBG[i]->SetColorPolygon(PLAYER_COLOR[i] + D3DXCOLOR(0, 0, 0, 1));
		// ＰＵＳＨボタン背景
		m_pPush2DBG[i] = CScene2D::Create(m_pD3DDevice,
			CHARASELECT_PLAYER_BG_POS[i],
			CHARASELECT_PUSH_BG_SIZE.x, CHARASELECT_PUSH_BG_SIZE.y,
			TEXTURE_RESULT_PUSH, TYPE_PRIORITY_GOAL);
		// プレイヤの色に合わせる
		m_pPush2DBG[i]->SetColorPolygon(CHARASELECT_PUSH_BG_COLOR);
		// ＰＵＳＨボタン
		m_pPush2D[i] = CScene2D::Create(m_pD3DDevice,
			CHARASELECT_PLAYER_BG_POS[i],
			CHARASELECT_PUSH_SIZE.x, CHARASELECT_PUSH_SIZE.y,
			TEXTURE_RESULT_PUSH, TYPE_PRIORITY_GOAL);
		// プレイヤの色に合わせる
		m_pPush2D[i]->SetColorPolygon(PLAYER_COLOR[i] + CHARASELECT_PUSH_ADDCOLOR);
		// 準備完了2Dを先に作ってフラグで管理しよう
		m_pReady2D[i] = CScene2D::Create(m_pD3DDevice,
			CHARASELECT_PLAYER_BG_POS[i],
			CHARASELECT_READY_SIZE.x, CHARASELECT_READY_SIZE.y,
			TEXTURE_CHARA_SELECT_READY, TYPE_PRIORITY_UI);
		m_pReady2D[i]->SetColorPolygon(CHARASELECT_READY_COLOR);
		m_pReady2D[i]->SetDrawFlag(false);
	}

	// スタートボタン
	m_pStart2D = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_START_POS,
		CHARASELECT_BUTTON_SIZE.x, CHARASELECT_BUTTON_SIZE.y,
		TEXTURE_CHARA_SELECT_START, TYPE_PRIORITY_GOAL);

	// キーコンフィグボタン
	m_pConfig = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_CONFIG_POS,
		CHARASELECT_BUTTON_SIZE.x, CHARASELECT_BUTTON_SIZE.y,
		TEXTURE_CHARA_SELECT_CONFIG, TYPE_PRIORITY_GOAL);

	// PUSH2Dが上のJoinで作って消すときの1フレーム分見えるから描画しないようにしておく
	m_pPush2D[0]->SetDrawFlag(false);
	m_pPush2DBG[0]->SetDrawFlag(false);

	// 1Pと参戦していた数だけ参戦
	Join(0);
	for (int i = 1; i < m_nPlayerJoinedNum; i++){
		Join(i);
	}

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ＢＧＭ再生
	CManager::PlaySoundA(SOUND_LABEL_BGM_STAGE_SELECT);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CCharaSelect::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CCharaSelect::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	// BGのアニメーション
	UpdateBG();

	// PUSHの点滅アニメーション
	UpdatePushAnimation();

	// リモのアニメーション
	UpdateRimoAnimation();

	// なんかの入力があったらプレイヤー参戦
	UpdateInputJoin();

	// カーソルについての更新
	UpdateCursol();

	// 準備完了の点滅アニメーション これみにくい！
	//	UpdateReadyAnimation();

	// 入力でテクスチャアニメーション
	UpdateInput();

	// 全員が準備完了だったらステージセレクトに移動
	if (!m_bNextPhaseOnece){
		// 1Pが準備完了していない場合は進まない
		if (m_bReady[0]){
			// 参加したプレイヤ人数が全員準備完了を押したら
			if (m_nPlayerReadyNum == m_nPlayerManualNum){
				// 一回のみ実行
				m_bNextPhaseOnece = true;
				// プレイヤの人数を送る
				CManager::SetPlayerNum(m_nPlayerManualNum);
				CManager::PlaySoundA(SOUND_LABEL_SE_START);
			}
		}
	}
	// 一回全員準備完了したから一定カウントしたらシーン遷移
	else{
		m_nNextPhaseCount++;
		if (m_nNextPhaseCount > CHARASELECT_NEXT_PHASE_INTERVAL){
			// フェードアウト開始
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
			// コンフィグヘ
			m_pManager->SetNextPhase(MODE_PHASE_STAGE_SELECT);
		}
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CCharaSelect* CCharaSelect::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CCharaSelect* pTitle = NULL;

	// 作成
	pTitle = new CCharaSelect;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//-----------------------------------------------------------------------------
// 背景初期化
//-----------------------------------------------------------------------------
void CCharaSelect::InitializeBG(void){
	// 全体の背景
	m_pBG = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0),
		static_cast<float>(SCREEN_WIDTH)* 2.1f, static_cast<float>(SCREEN_HEIGHT)* 2.1f,
		TEXTURE_BG_CHARA_SELECT, TYPE_PRIORITY_BG);

	// キャラ選択っていうロゴ表示
	m_pLogo = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_LOGO_POS,
		CHARASELECT_LOGO_SIZE.x, CHARASELECT_LOGO_SIZE.y,
		TEXTURE_CHARA_SELECT_LOGO, TYPE_PRIORITY_FIELD);
}

//-----------------------------------------------------------------------------
// BGの更新
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateBG(){

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
	if (m_nBgDirection == 0) m_vBgVelo = D3DXVECTOR3(-CHARASELECT_BG_VELO, 0, 0);
	// 上へ
	if (m_nBgDirection == 1) m_vBgVelo = D3DXVECTOR3(0, -CHARASELECT_BG_VELO, 0);
	// 右へ
	if (m_nBgDirection == 2) m_vBgVelo = D3DXVECTOR3(CHARASELECT_BG_VELO, 0, 0);
	// 下へ
	if (m_nBgDirection == 3) m_vBgVelo = D3DXVECTOR3(0, CHARASELECT_BG_VELO, 0);

	// 移動量加算
	m_pBG->AddPos(m_vBgVelo);
}

//-----------------------------------------------------------------------------
// 指定したプレイヤを参戦
//-----------------------------------------------------------------------------
void CCharaSelect::Join(int playerNum){

	// すでに参加済みは抜ける
	if (m_bPush[playerNum]){
		return;
	}

	CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_SANSEN);

	// 押したフラグ
	m_bPush[playerNum] = true;

	// ＰＵＳＨ2D消す
	SAFE_RELEASE(m_pPush2DBG[playerNum]);
	SAFE_RELEASE(m_pPush2D[playerNum]);

	// 操作できるプレイヤ人数インクリメント
	m_nPlayerManualNum++;

	// りも2D
	m_pRimo[playerNum] = CSceneAnime::Create(m_pD3DDevice,
		CHARASELECT_RIMO_POS[playerNum],
		CHARASELECT_RIMO_SIZE.x, CHARASELECT_RIMO_SIZE.y,
		PLAYER_TEXTURE[playerNum], PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
		PLAYER_ANIME_SPEED, -1, TYPE_PRIORITY_PLAYER);

	// 自動でテクスチャアニメーションしない
	m_pRimo[playerNum]->SetAutoUpdate(false);
	// 正面の歩きモーション
	m_nRimoTextureIndex[playerNum] = PLAYER_STOP_TEXTURE_MIN;
	m_pRimo[playerNum]->SetIndex(m_nRimoTextureIndex[playerNum]);
	m_nRimoTextureMin[playerNum] = PLAYER_STOP_TEXTURE_MIN;
	m_nRimoTextureMax[playerNum] = PLAYER_STOP_TEXTURE_MAX;
	// やっぱり正面向いて静止
	m_nRimoTextureIndex[playerNum] = PLAYER_STOP_TEXTURE_MIN;
	m_pRimo[playerNum]->SetIndex(m_nRimoTextureIndex[playerNum]);
	m_nRimoTextureMin[playerNum] = PLAYER_STOP_TEXTURE_MIN;
	m_nRimoTextureMax[playerNum] = PLAYER_STOP_TEXTURE_MAX;

	// 参戦2D
	m_pJoin2D[playerNum] = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_JOIN_POS[playerNum],
		CHARASELECT_JOIN_SIZE.x, CHARASELECT_JOIN_SIZE.y,
		TEXTURE_CHARA_SELECT_JOIN, TYPE_PRIORITY_PLAYER);

	// カーソルを初期化
	m_pCursol2D[playerNum] = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_CURSOLR_POS[playerNum],
		CHARASELECT_CURSOL_SIZE.x, CHARASELECT_CURSOL_SIZE.y,
		TEXTURE_CHARA_SELECT_CURSOL, TYPE_PRIORITY_UI);
	m_pCursol2D[playerNum]->SetColorPolygon(PLAYER_COLOR[playerNum] + CHARASELECT_CURSOL_ADDCOLOR);

	// 準備状態初期化
	if (m_pReady2D[playerNum])
		m_pReady2D[playerNum]->SetDrawFlag(false);
	m_bReady[playerNum] = false;
}

//-----------------------------------------------------------------------------
// 指定したプレイヤを不参加
//-----------------------------------------------------------------------------
void CCharaSelect::UnJoin(int playerNum){

	// すでに不参加なら抜ける
	if (!m_bPush[playerNum]){
		return;
	}
	// 準備完了していたら消す
	if (m_bReady[playerNum]){
		m_bReady[playerNum] = false;
		m_pReady2D[playerNum]->SetDrawFlag(false);
		m_nReadyFlashCount[playerNum] = 0;
		m_bReadyFlash[playerNum] = false;
		m_nPlayerReadyNum--;
	}

	// 押したフラグ
	m_bPush[playerNum] = false;

	// ＰＵＳＨボタン背景
	m_pPush2DBG[playerNum] = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_PLAYER_BG_POS[playerNum],
		CHARASELECT_PUSH_BG_SIZE.x, CHARASELECT_PUSH_BG_SIZE.y,
		TEXTURE_RESULT_PUSH, TYPE_PRIORITY_GOAL);
	// プレイヤの色に合わせる
	m_pPush2DBG[playerNum]->SetColorPolygon(CHARASELECT_PUSH_BG_COLOR);
	// ＰＵＳＨボタン
	m_pPush2D[playerNum] = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_PLAYER_BG_POS[playerNum],
		CHARASELECT_PUSH_SIZE.x, CHARASELECT_PUSH_SIZE.y,
		TEXTURE_RESULT_PUSH, TYPE_PRIORITY_GOAL);
	// プレイヤの色に合わせる
	m_pPush2D[playerNum]->SetColorPolygon(PLAYER_COLOR[playerNum] + CHARASELECT_PUSH_ADDCOLOR);

	// 操作できるプレイヤ人数
	m_nPlayerManualNum--;

	SAFE_RELEASE(m_pRimo[playerNum]);
	SAFE_RELEASE(m_pJoin2D[playerNum]);
	SAFE_RELEASE(m_pCursol2D[playerNum]);
}

//-----------------------------------------------------------------------------
// リモの勝者アニメーション更新
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateRimoAnimation(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		// リモがいなければ抜ける
		if (!m_pRimo[i])	continue;

		if (m_nRimoTextureIndex[i] > m_nRimoTextureMax[i]){
			continue;
		}

		m_nRimoTextureCount[i]++;
		if (m_nRimoTextureCount[i] > PLAYER_ANIME_SPEED){
			m_nRimoTextureCount[i] = 0;

			m_nRimoTextureIndex[i]++;
			if (m_nRimoTextureIndex[i] > m_nRimoTextureMax[i]){
				m_nRimoTextureIndex[i] = PLAYER_STOP_TEXTURE_MIN;
				m_nRimoTextureMax[i] = PLAYER_STOP_TEXTURE_MAX;
			}
			m_pRimo[i]->SetIndex(m_nRimoTextureIndex[i]);
		}
	}

}

//-----------------------------------------------------------------------------
// 入力で参戦する更新
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateInputJoin(void){

	// 各コントローラーで参加させる 0はすでに参加している
	if (CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 1)){
		Join(1);
	}
	if (CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 2)){
		Join(2);
	}
	if (CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 3)){
		Join(3);
	}
	// デバッグ操作
	if (CInputKeyboard::GetKeyboardTrigger(DIK_1))	Join(1);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_2))	Join(2);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_3))	Join(3);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_4))	UnJoin(1);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_5))	UnJoin(2);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_6))	UnJoin(3);
}

//-----------------------------------------------------------------------------
// PUSHの点滅更新
//-----------------------------------------------------------------------------
void CCharaSelect::UpdatePushAnimation(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		// PUSHが無ければ抜ける
		if (!m_pPush2D[i])	continue;
		// PUSHがあればアニメーション
		m_nPushFlashCount[i]++;
		if (m_nPushFlashCount[i] > CHARASELECT_PUSH_FLASH_INTERVAL){
			m_nPushFlashCount[i] = 0;
			m_bPushFlash[i] = !m_bPushFlash[i];
			m_pPush2DBG[i]->SetDrawFlag(m_bPushFlash[i]);
			if (m_bPush[i])	m_pPush2D[i]->SetDrawFlag(true);
		}
	}
}

//-----------------------------------------------------------------------------
// READYの点滅更新
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateReadyAnimation(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		// PUSHが無ければ抜ける
		if (!m_pReady2D[i])	continue;
		// PUSHがあればアニメーション
		m_nReadyFlashCount[i]++;
		if (m_nReadyFlashCount[i] > CHARASELECT_PUSH_FLASH_INTERVAL){
			m_nReadyFlashCount[i] = 0;
			m_bReadyFlash[i] = !m_bReadyFlash[i];
			m_pReady2D[i]->SetDrawFlag(m_bReadyFlash[i]);
			if (m_bReady[i])	m_pReady2D[i]->SetDrawFlag(true);
		}
	}
}

//-----------------------------------------------------------------------------
// カーソルの更新　アニメーションや入力で移動など
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateCursol(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		if (m_pCursol2D[i]){
			// 角度更新
			if (m_bCursolFlg[i]){
				m_fCursolRad[i] += CHARASELECT_CURSOL_ADDRAD;
				if (m_fCursolRad[i] > CHARASELECT_CURSOL_RAD_MAX){
					m_fCursolRad[i] = CHARASELECT_CURSOL_RAD_MAX;
					m_bCursolFlg[i] = !m_bCursolFlg[i];
				}
			}
			else{
				m_fCursolRad[i] -= CHARASELECT_CURSOL_ADDRAD;
				if (m_fCursolRad[i] < 0){
					m_fCursolRad[i] = 0;
					m_bCursolFlg[i] = !m_bCursolFlg[i];
				}
			}
			// 角度をセット
			m_pCursol2D[i]->SetRot(D3DXVECTOR3(0, 0, m_fCursolRad[i]));

			// 矢印キー

			// こっから斜め移動
			if (CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_LEFT_UP, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x -= CHARASELECT_CURSOL_VELO * 0.75f;
				pos.y -= CHARASELECT_CURSOL_VELO * 0.75f;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_LEFT_DOWN, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x -= CHARASELECT_CURSOL_VELO * 0.75f;
				pos.y += CHARASELECT_CURSOL_VELO * 0.75f;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_RIGHT_DOWN, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x += CHARASELECT_CURSOL_VELO * 0.75f;
				pos.y += CHARASELECT_CURSOL_VELO * 0.75f;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_RIGHT_UP, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x += CHARASELECT_CURSOL_VELO * 0.75f;
				pos.y -= CHARASELECT_CURSOL_VELO * 0.75f;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_W) ||
				CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_UP, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.y -= CHARASELECT_CURSOL_VELO;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_S) ||
				CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_DOWN, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.y += CHARASELECT_CURSOL_VELO;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_A) ||
				CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_LEFT, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x -= CHARASELECT_CURSOL_VELO;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_D) ||
				CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_RIGHT, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x += CHARASELECT_CURSOL_VELO;
				m_pCursol2D[i]->SetPos(pos);
			}
		}
		// 決定ボタンを押したら
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, i)){
			
			// 参加していてカーソルがある場合
			if (m_pCursol2D[i]){
				// カーソルとボタンの当たり判定
				ButtonNumber hitButtonNum = CollisionButton(i, m_pCursol2D[i]->GetPos());

				// 準備完了ボタン
				if (hitButtonNum == BUTTON_READY){
					// 準備完了状態の切り替え（参戦しているプレイヤのみ）
					if (m_bPush[i]){
						if (m_bReady[i]){
							UnReady(i);
						}
						else{
							Ready(i);
							CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_ZYUNBIKANRYOU);
							continue;
						}
					}
				}
				// コンフィグボタン
				else if (hitButtonNum == BUTTON_CONFIG){
					// フェーズ移動していなければ
					if (!m_bNextPhaseOnece){
						// 今参戦している人数を保存
						CManager::SetJoinNum(m_nPlayerManualNum);
						// フェードアウト開始
						m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
						// コンフィグヘ
						m_pManager->SetNextPhase(MODE_PHASE_OPTION);

						CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
					}
				}
				// キャラクタを選択ボタン
				else if (hitButtonNum == BUTTON_CHARA){
					// 参戦するしないの切り替え
					if (m_bPush[i]){
						UnJoin(i);
						continue;
					}
				}
			}
			// 不参加でカーソルがない場合
			else{
				if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
					CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, i)){
					Join(i);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 入力でテクスチャアニメーション
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateInput(void){

	for (int i = 0; i < PLAYER_MAX; i++){

		// カーソルが自分の時以外は抜ける
		if (m_nCursol[i] != 0){
			continue;
		}
		// リモの絵が無ければ抜ける
		if (!m_pRimo[i]){
			continue;
		}

		// Lキー　糸
		if (CInputKeyboard::GetKeyboardTrigger(DIK_4) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_L, i)){
			// 通常
			if (m_PlayerMode[i] == PLAYER_MODE_NONE){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_THREAD_FRONT_MIN, PLAYER_TEXTURE_THREAD_FRONT_MAX);
			}
			// 攻撃
			else if (m_PlayerMode[i] == PLAYER_MODE_ATTACK){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_THREAD_FRONT_MIN, PLAYER_TEXTURE_THREAD_FRONT_MAX);
			}
			// 移動
			else if (m_PlayerMode[i] == PLAYER_MODE_SPEED){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_THREAD_FRONT_MIN, PLAYER_TEXTURE_THREAD_FRONT_MAX);
			}
			// 罠
			else if (m_PlayerMode[i] == PLAYER_MODE_TRAP){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_THREAD_FRONT_MIN, PLAYER_TEXTURE_THREAD_FRONT_MAX);
			}
			m_nRimoTextureIndex[i] = m_nRimoTextureMin[i];
			m_nRimoTextureCount[i] = 0;
		}
		// Rキー　攻撃
		if (CInputKeyboard::GetKeyboardTrigger(DIK_5) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_R, i)){
			// 通常
			if (m_PlayerMode[i] == PLAYER_MODE_NONE){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_ATTACK_FRONT_MIN, PLAYER_TEXTURE_ATTACK_FRONT_MAX);
			}
			// 攻撃
			else if (m_PlayerMode[i] == PLAYER_MODE_ATTACK){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_ATTACK_FRONT_MIN, PLAYER_TEXTURE_ATTACK_FRONT_MAX);
			}
			// 移動
			else if (m_PlayerMode[i] == PLAYER_MODE_SPEED){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_ATTACK_FRONT_MIN, PLAYER_TEXTURE_ATTACK_FRONT_MAX);
			}
			// 罠
			else if (m_PlayerMode[i] == PLAYER_MODE_TRAP){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_ATTACK_FRONT_MIN, PLAYER_TEXTURE_ATTACK_FRONT_MAX);
			}
			m_nRimoTextureIndex[i] = m_nRimoTextureMin[i];
			m_nRimoTextureCount[i] = 0;
		}
	}
}

//-----------------------------------------------------------------------------
// 引数のposと当たっているボタン番号を返す
//-----------------------------------------------------------------------------
CCharaSelect::ButtonNumber CCharaSelect::CollisionButton(int playerNum, D3DXVECTOR3 vCursolPos){
	// デフォルト
	CCharaSelect::ButtonNumber num = BUTTON_NONE;

	// 指先に当たり判定を調整
	vCursolPos += CHARASELECT_CURSOL_COLI_DIST;

	// キャラボタンとの当たり判定
	if (
		((CHARASELECT_PLAYER_BG_POS[playerNum].x - CHARASELECT_PLAYER_BG_SIZE_HALF.x) < vCursolPos.x) &&
		((CHARASELECT_PLAYER_BG_POS[playerNum].x + CHARASELECT_PLAYER_BG_SIZE_HALF.x) > vCursolPos.x) &&
		((CHARASELECT_PLAYER_BG_POS[playerNum].y - CHARASELECT_PLAYER_BG_SIZE_HALF.y) < vCursolPos.y) &&
		((CHARASELECT_PLAYER_BG_POS[playerNum].y + CHARASELECT_PLAYER_BG_SIZE_HALF.y) > vCursolPos.y)){
		num = BUTTON_CHARA;
	}
	// 準備完了ボタンとの当たり判定
	else if (
		((CHARASELECT_START_POS.x - CHARASELECT_BUTTON_SIZE_HALF.x) < vCursolPos.x) &&
		((CHARASELECT_START_POS.x + CHARASELECT_BUTTON_SIZE_HALF.x) > vCursolPos.x) &&
		((CHARASELECT_START_POS.y - CHARASELECT_BUTTON_SIZE_HALF.y) < vCursolPos.y) &&
		((CHARASELECT_START_POS.y + CHARASELECT_BUTTON_SIZE_HALF.y) > vCursolPos.y)){
		num = BUTTON_READY;
	}
	// コンフィグボタンとの当たり判定
	else if (
		(CHARASELECT_CONFIG_POS.x - CHARASELECT_BUTTON_SIZE_HALF.x < vCursolPos.x) &&
		(CHARASELECT_CONFIG_POS.x + CHARASELECT_BUTTON_SIZE_HALF.x > vCursolPos.x) &&
		(CHARASELECT_CONFIG_POS.y - CHARASELECT_BUTTON_SIZE_HALF.y < vCursolPos.y) &&
		(CHARASELECT_CONFIG_POS.y + CHARASELECT_BUTTON_SIZE_HALF.y > vCursolPos.y)){
		num = BUTTON_CONFIG;
	}

	// ボタン番号
	return num;
}

//-----------------------------------------------------------------------------
// 準備完了
//-----------------------------------------------------------------------------
void CCharaSelect::Ready(int playerNum){
	if (m_pReady2D[playerNum])
		m_pReady2D[playerNum]->SetDrawFlag(true);
	m_bReady[playerNum] = true;
	m_nPlayerReadyNum++;
}
//-----------------------------------------------------------------------------
// 準備完了じゃない
//-----------------------------------------------------------------------------
void CCharaSelect::UnReady(int playerNum){
	if (m_pReady2D[playerNum])
		m_pReady2D[playerNum]->SetDrawFlag(false);
	m_bReady[playerNum] = false;
	m_nPlayerReadyNum--;
}

//----EOF----