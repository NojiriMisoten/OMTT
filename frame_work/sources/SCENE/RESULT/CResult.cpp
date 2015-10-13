//=============================================================================
//
// CResultクラス [CResult.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CResult.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneAnime.h"
#include "../GAME/PLAYER/CPlayer.h"
#include "../GAME/PLAYER/AI/mersenne_twister.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// 勝者 敗者
enum PlayerResult{
	WIN,
	LOSE,
	DRAW,
	PLAYER_RESULT_MAX
};
// リモアニメーションの大きさ
static const D3DXVECTOR2 RESULT_RIMO_SIZE[PLAYER_RESULT_MAX] = {
	// 勝者のサイズ
	D3DXVECTOR2(150, 300),
	// 敗者のサイズ
	D3DXVECTOR2(100, 200),
	// 引き分けのサイズ
	D3DXVECTOR2(75, 150),
};
// リモアニメーションの座標
static const D3DXVECTOR3 RESULT_RIMO_POS[PLAYER_MAX] = {
	// 勝者の位置
	D3DXVECTOR3(250, 400, 0),
	D3DXVECTOR3(500, 400, 0),
	D3DXVECTOR3(750, 400, 0),
	D3DXVECTOR3(1000, 400, 0),
};
// メニューの座標
static const D3DXVECTOR3 RESULT_MENU_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);
// メニューの大きさ
static const D3DXVECTOR2 RESULT_MENU_SIZE = D3DXVECTOR2(500, 600);
// PUSH2Dの大きさ
static const D3DXVECTOR2 RESULT_PUSH_SIZE = D3DXVECTOR2(180, 80);
// PUSH2Dの調整位置
static const D3DXVECTOR3 RESULT_PUSH_ADDPOS = D3DXVECTOR3(0, 250, 0);
// プレイヤのアニメーションのスピード
static const int RESULT_RIMO_ANIME_SPEED = 10;
// プレイヤの負けアニメーションのインデックス
static const int RESULT_RIMO_LOSE_INDEX_MAX = PLAYER_TEXTURE_THREAD_FRONT_MAX;
static const int RESULT_RIMO_LOSE_INDEX_MIN = PLAYER_TEXTURE_THREAD_FRONT_MAX;
// プレイヤの勝ちアニメーションの最小インデックス
static const int RESULT_RIMO_WIN_INDEX_MAX = 9;
static const int RESULT_RIMO_WIN_INDEX_MIN = 6;
// 自動でＰＵＳＨが出るまでのカウント
static const int RESULT_PUSH_INTERVAL = 100;
// PUSHの点滅間隔
static const int RESULT_PUSH_FLASH_INTERVAL = 5;
// メニューのテクスチャ　カーソル番号ごと
static const TEXTURE_TYPE RESULT_MENU_TEXTURE[] = {
	TEXTURE_RESULT_MENU_BACK_0, TEXTURE_RESULT_MENU_BACK_1, TEXTURE_RESULT_MENU_BACK_2
};
static const TEXTURE_TYPE RESULT_MENU_MOJI_TEXTURE[] = {
	TEXTURE_RESULT_MENU_MOJI_0, TEXTURE_RESULT_MENU_MOJI_1, TEXTURE_RESULT_MENU_MOJI_2
};
// メニューの文字アニメーションの拡大スピード
static const D3DXVECTOR2 RESULT_MENU_MOJI_ADDSIZE = D3DXVECTOR2(2.5f, 2.5f);
static const float RESULT_MENU_MOJI_SIZE_MAX = 550;
static const float RESULT_MENU_MOJI_SIZE_MIN = 480;
// メニュー文字の座標
static const D3DXVECTOR3 RESULT_MENU_MOJI_POS[RESULT_MENU_MAX] = {
	D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 180),	// リトライ
	D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 360),	// キャラセレクト
	D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 550),	// ステージセレクト
};

// ロゴの大きさ
static const D3DXVECTOR2 RESULT_LOGO_SIZE = D3DXVECTOR2(800, 150);
// ロゴの座標
static const D3DXVECTOR3 RESULT_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 75, 0);
// 勝者に当てるライトの大きさ
static const D3DXVECTOR2 RESULT_LIGHT_SIZE = D3DXVECTOR2(320, 640);
// 勝者に当てるライトの座標
static const D3DXVECTOR3 RESULT_LIGHT_POS[PLAYER_MAX] = {
	D3DXVECTOR3(250, 350, 0),
	D3DXVECTOR3(500, 350, 0),
	D3DXVECTOR3(750, 350, 0),
	D3DXVECTOR3(1000, 350, 0),
};
// くるくるライトの大きさ
static const float RESULT_CIRCLE_LIGHT_SIZE_MIN = 25;
static const float RESULT_CIRCLE_LIGHT_SIZE_MAX = 50;
// くるくるライトの速さ
static const float RESULT_CIRCLE_LIGHT_VELO_MIN = -3;
static const float RESULT_CIRCLE_LIGHT_VELO_MAX = 3;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CResult::CResult(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CResult ::~CResult(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CResult::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// 変数の初期化
	m_nWinPlayerNum = 0;
	m_nCount = 0;
	m_pMenuBack = NULL;
	m_nMenuCursol = 0;
	m_vMojiSize = RESULT_MENU_SIZE;
	m_bMojiScale = true;
	for (int i = 0; i < PLAYER_MAX; i++){
		m_nTextureIndex[i] = 0;
		m_nTextureCount[i] = 0;
		m_pPlayerAnime[i] = NULL;
		m_pPush2D[i] = NULL;
		m_bPush[i] = false;
		m_nPushFlashCount[i] = 0;
		m_bPushFlash[i] = true;
	}
	for (int i = 0; i < RESULT_MENU_MAX; i++){
		m_pMenuMoji[i] = NULL;
	}

	// 勝ったプレイヤ番号取得
	m_nWinPlayerNum = CManager::GetWinPlayerNum();

	// CPU人数を取得
	m_nCpuNum = PLAYER_MAX - CManager::GetPlayerManualNum();

	// 背景
	InitializeBG();

	// リモの勝者アニメーション初期化
	InitializeWinAnimation();

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ＢＧＭ再生
	CManager::PlaySoundA(SOUND_LABEL_ZINGLE_RESULT);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CResult::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CResult::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	// アニメの更新
	UpdateWinAnimation();

	// プッシュがでるまでカウント
	if (m_nCount < RESULT_PUSH_INTERVAL){
		m_nCount++;
	}
	// プッシュがでる
	else if (m_nCount == RESULT_PUSH_INTERVAL){
		// ここを二度ととおらない
		m_nCount++;
		// PUSH２Ｄ初期化
		for (int i = 0; i < PLAYER_MAX; i++){
			m_pPush2D[i] = CScene2D::Create(m_pD3DDevice,
				RESULT_RIMO_POS[i] + RESULT_PUSH_ADDPOS,
				RESULT_PUSH_SIZE.x, RESULT_PUSH_SIZE.y,
				TEXTURE_RESULT_PUSH, TYPE_PRIORITY_THREAD_OF_FOOTHOLD);
			// プレイヤの色に合わせる
			m_pPush2D[i]->SetColorPolygon(PLAYER_COLOR[i]);
		}
		// CPUがいた時はPUSHを押す
		if (m_nCpuNum > 0){
			for (int i = 0; i < m_nCpuNum; i++){
				m_bPush[PLAYER_MAX - 1 - i] = true;
			}
		}
	}
	// プッシュがでてる
	else{
		// 点滅アニメーション
		for (int i = 0; i < PLAYER_MAX; i++){
			m_nPushFlashCount[i]++;
			if (m_nPushFlashCount[i] > RESULT_PUSH_FLASH_INTERVAL){
				m_nPushFlashCount[i] = 0;
				m_bPushFlash[i] = !m_bPushFlash[i];
				m_pPush2D[i]->SetDrawFlag(m_bPushFlash[i]);
				if (m_bPush[i])	m_pPush2D[i]->SetDrawFlag(true);
			}
		}

		// 入力受け受け
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0)){
			m_bPush[0] = true;
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 1)){
			m_bPush[1] = true;
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 2)){
			m_bPush[2] = true;
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 3)){
			m_bPush[3] = true;
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}

		// すべてＰＵＳＨされた + メニューが出ていない状態（一回だけ通るように）
		if ((m_bPush[0] && m_bPush[1] && m_bPush[2] && m_bPush[3]) && !m_pMenuBack){
			m_pMenuBack = CScene2D::Create(m_pD3DDevice,
				RESULT_MENU_POS,
				RESULT_MENU_SIZE.x, RESULT_MENU_SIZE.y,
				RESULT_MENU_TEXTURE[m_nMenuCursol], TYPE_PRIORITY_UI);
			// メニューの文字
			for (int n = 0; n < RESULT_MENU_MAX; n++){
				m_pMenuMoji[n] = CScene2D::Create(m_pD3DDevice,
					RESULT_MENU_MOJI_POS[n],
					RESULT_MENU_SIZE.x, RESULT_MENU_SIZE.y,
					RESULT_MENU_MOJI_TEXTURE[n], TYPE_PRIORITY_PAUSE);
			}

			// つぎの選択メニューが出ている状態の分岐に連続フレームで通らないように脱出
			return;
		}
	}

	// 選択メニューが出ている状態
	if (m_pMenuBack){
		// 選択中の文字を拡大アニメーション
		if (m_bMojiScale){
			m_vMojiSize += RESULT_MENU_MOJI_ADDSIZE;
		}
		else{
			m_vMojiSize -= RESULT_MENU_MOJI_ADDSIZE;
		}
		if (m_vMojiSize.x > RESULT_MENU_MOJI_SIZE_MAX){
			m_bMojiScale = false;
		}
		else if (m_vMojiSize.x < RESULT_MENU_MOJI_SIZE_MIN){
			m_bMojiScale = true;
		}
		m_pMenuMoji[m_nMenuCursol]->SetWidth(m_vMojiSize.x);
		m_pMenuMoji[m_nMenuCursol]->SetHeight(m_vMojiSize.y);

		// 矢印キー
		if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_UP, 0)){
			m_nMenuCursol--;
			if (m_nMenuCursol < 0){
				m_nMenuCursol = RESULT_MENU_MAX - 1;
			}
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_DOWN, 0)){
			m_nMenuCursol++;
			if (m_nMenuCursol >= RESULT_MENU_MAX){
				m_nMenuCursol = 0;
			}
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}
		// 矢印の入力があったら
		if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
			CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_UP, 0) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_DOWN, 0)){
			// メニューのテクスチャを変更
			m_pMenuBack->ChangeTexture(RESULT_MENU_TEXTURE[m_nMenuCursol]);
			// メニュー文字の大きさリセット
			m_pMenuMoji[m_nMenuCursol]->SetHeight(RESULT_MENU_SIZE.y);
			m_pMenuMoji[m_nMenuCursol]->SetWidth(RESULT_MENU_SIZE.x);
			m_bMojiScale = true;
			m_vMojiSize = RESULT_MENU_SIZE;
		}
		// 決定キー
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0)){
			// フェードアウト開始
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
			// それぞれのシーンへ飛ぶ
			if (m_nMenuCursol == RESULT_MENU_RETRY){
				m_pManager->SetNextPhase(MODE_PHASE_GAME);
			}
			else if (m_nMenuCursol == RESULT_MENU_CHARA_SELECT){
				m_pManager->SetNextPhase(MODE_PHASE_CHARA_SELECT);
			}
			else
				m_pManager->SetNextPhase(MODE_PHASE_STAGE_SELECT);
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}
	}
	// くるくるライトのうごき
	for (int i = 0; i < RESULT_CIRCLE_LIGHT_MAX; i++){
		// 移動
		D3DXVECTOR3 pos = m_pCircleLight[i]->GetPos();
		pos.x += m_vCircleVelo[i].x;
		pos.y += m_vCircleVelo[i].y;
		m_pCircleLight[i]->SetPos(pos);
		// 移動量反転
		if ((pos.y - m_vCircleSizeHalf[i] < 0) ||
			(pos.y + m_vCircleSizeHalf[i] > SCREEN_HEIGHT)){
			m_vCircleVelo[i].y *= -1.0f;
		}
		if ((pos.x - m_vCircleSizeHalf[i] < 0) ||
			(pos.x + m_vCircleSizeHalf[i] > SCREEN_WIDTH)){
			m_vCircleVelo[i].x *= -1.0f;
		}
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CResult* CResult::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CResult* pTitle = NULL;

	// 作成
	pTitle = new CResult;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//-----------------------------------------------------------------------------
// リモの勝者アニメーション初期化
//-----------------------------------------------------------------------------
void CResult::InitializeWinAnimation(void){

	// 引き分けのアニメ
	if (m_nWinPlayerNum == -1){
		for (int i = 0; i < PLAYER_MAX; i++){
			m_pPlayerAnime[i] = CSceneAnime::Create(m_pD3DDevice,
				RESULT_RIMO_POS[i],
				RESULT_RIMO_SIZE[DRAW].x, RESULT_RIMO_SIZE[DRAW].y,
				PLAYER_TEXTURE[i], PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
				RESULT_RIMO_ANIME_SPEED, -1, TYPE_PRIORITY_PLAYER);
		}

	}
	// 誰かが勝ったアニメ
	else{
		for (int i = 0; i < PLAYER_MAX; i++){
			// 勝った人のアニメ
			if (i == m_nWinPlayerNum){
				m_pPlayerAnime[i] = CSceneAnime::Create(m_pD3DDevice,
					RESULT_RIMO_POS[i],
					RESULT_RIMO_SIZE[WIN].x, RESULT_RIMO_SIZE[WIN].y,
					PLAYER_TEXTURE[i], PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
					RESULT_RIMO_ANIME_SPEED, -1, TYPE_PRIORITY_PLAYER);
			}
			// 負けた人のアニメ
			else{
				m_pPlayerAnime[i] = CSceneAnime::Create(m_pD3DDevice,
					RESULT_RIMO_POS[i],
					RESULT_RIMO_SIZE[LOSE].x, RESULT_RIMO_SIZE[LOSE].y,
					PLAYER_TEXTURE[i], PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
					RESULT_RIMO_ANIME_SPEED, -1, TYPE_PRIORITY_PLAYER);
			}
		}
	}
	// 自動で更新しない
	for (int i = 0; i < PLAYER_MAX; i++){
		m_pPlayerAnime[i]->SetAutoUpdate(false);
		// 勝った人のインデックス
		if (i == m_nWinPlayerNum){
			m_nTextureIndex[i] = RESULT_RIMO_WIN_INDEX_MIN;
		}
		// 負けた人のインデックス
		else{
			m_nTextureIndex[i] = RESULT_RIMO_LOSE_INDEX_MIN;
		}
		m_pPlayerAnime[i]->SetIndex(m_nTextureIndex[i]);
	}

}

//-----------------------------------------------------------------------------
// リモの勝者アニメーション更新
//-----------------------------------------------------------------------------
void CResult::UpdateWinAnimation(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		m_nTextureCount[i]++;
		if (m_nTextureCount[i] > RESULT_RIMO_ANIME_SPEED){
			m_nTextureCount[i] = 0;

			m_nTextureIndex[i]++;
			// 勝った人のインデックス
			if (i == m_nWinPlayerNum){
				if (m_nTextureIndex[i] > RESULT_RIMO_WIN_INDEX_MAX){
					m_nTextureIndex[i] = RESULT_RIMO_WIN_INDEX_MIN;
				}
			}
			// 負けた人or引き分けのインデックス
			else{
				if (m_nTextureIndex[i] > RESULT_RIMO_LOSE_INDEX_MAX){
					m_nTextureIndex[i] = RESULT_RIMO_LOSE_INDEX_MIN;
				}
			}
		}
		m_pPlayerAnime[i]->SetIndex(m_nTextureIndex[i]);
	}

}

//-----------------------------------------------------------------------------
// 背景初期化
//-----------------------------------------------------------------------------
void CResult::InitializeBG(void){
	// 背景
	m_pBG = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_RESULT, TYPE_PRIORITY_BG);

	// キャラ選択っていうロゴ表示
	m_pLogo = CScene2D::Create(m_pD3DDevice,
		RESULT_LOGO_POS,
		RESULT_LOGO_SIZE.x, RESULT_LOGO_SIZE.y,
		TEXTURE_RESULT_LOGO, TYPE_PRIORITY_GOAL);

	// くるくるライトの作成
	for (int i = 0; i < RESULT_CIRCLE_LIGHT_MAX; i++){
		// サイズ決定
		float size = mersenne_twister_float(RESULT_CIRCLE_LIGHT_SIZE_MIN, RESULT_CIRCLE_LIGHT_SIZE_MAX);
		m_pCircleLight[i] = CScene2D::Create(m_pD3DDevice,
			D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
			size, size,
			TEXTURE_RESULT_CIRCLE_LIGHT, TYPE_PRIORITY_EFFECT);
		// 当たり判定用に半分の大きさを保存
		m_vCircleSizeHalf[i] = size * 0.5f;
		// 移動量決定
		m_vCircleVelo[i].x = mersenne_twister_float(RESULT_CIRCLE_LIGHT_VELO_MIN, RESULT_CIRCLE_LIGHT_VELO_MAX);
		m_vCircleVelo[i].y = mersenne_twister_float(RESULT_CIRCLE_LIGHT_VELO_MIN, RESULT_CIRCLE_LIGHT_VELO_MAX);
		// 遅すぎる奴をなくす
		(m_vCircleVelo[i].x < 0) ? m_vCircleVelo[i].x -= 1.0f : m_vCircleVelo[i].x += 1.0f;
		(m_vCircleVelo[i].y < 0) ? m_vCircleVelo[i].y -= 1.0f : m_vCircleVelo[i].y += 1.0f;
		// 色を付ける
		m_pCircleLight[i]->SetColorPolygon(
			PLAYER_COLOR[m_nWinPlayerNum] + D3DXCOLOR(
			mersenne_twister_float(0.2f, 0.7f),
			mersenne_twister_float(0.2f, 0.7f),
			mersenne_twister_float(0.2f, 0.7f),
			mersenne_twister_float(0.2f, 0.7f)));
	}

	// このさき引き分けならいらない
	if (m_nWinPlayerNum == -1){
		return;
	}

	// 勝者に当てるライト
	m_pLight = CScene2D::Create(m_pD3DDevice,
		RESULT_LIGHT_POS[m_nWinPlayerNum],
		RESULT_LIGHT_SIZE.x, RESULT_LIGHT_SIZE.y,
		TEXTURE_RESULT_LIGHT, TYPE_PRIORITY_FIELD);

}

//----EOF----