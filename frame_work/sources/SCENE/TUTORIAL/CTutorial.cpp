//=============================================================================
//
// CTutorialクラス [CTutorial.h]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CTutorial.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneAnime.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// 絵の移動量
static const D3DXVECTOR3 TUTORIAL_PICT_VELO = D3DXVECTOR3(100, 0, 0);
// 絵の慣性調整位置
static const float TUTORIAL_PICT_RESIST = 0.98f;
// 絵の位置
static const D3DXVECTOR3 TUTORIAL_PICT_POS[TUTORIAL_PICT_MAX] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f + SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
};
// ページ数に対応したテクスチャ
static const TEXTURE_TYPE TUTORIAL_PICT_TEXTURE[TUTORIAL_PAGE_MAX] = {
	TEXTURE_TUTORIAL_0,
	TEXTURE_TUTORIAL_1,
	TEXTURE_TUTORIAL_2,
	TEXTURE_TUTORIAL_3,
};
// 矢印アニメーション
static const D3DXVECTOR3 TUTORIAL_ARROR_POS_R = D3DXVECTOR3(50, SCREEN_HEIGHT * 0.5f, 0);
static const D3DXVECTOR3 TUTORIAL_ARROR_POS_L = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT * 0.5f, 0);
static const float TUTORIAL_ARROR_WIDTH = 100;
static const float TUTORIAL_ARROR_HEIGHT = 100;
static const int TUTORIAL_ARROW_ANIME_SPEED = 10;
static const int TUTORIAL_ARROR_TEXTURE_SEP_X = 1;
static const int TUTORIAL_ARROR_TEXTURE_SEP_Y = 1;
// 矢印の拡縮値
static const float TUTORIAL_ARROW_SCALE = 1.3f;
// 矢印が拡大と縮小を切り替えるインターバル
static const int TUTORIAL_ARROW_SCALE_INTERVAL = 40;
// ページ表示の座標
static const D3DXVECTOR3 TUTORIAL_PAGE_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT -50, 0);
// ページ表示のおおきさ
static const float TUTORIAL_PAGE_WIDTH = 100;
static const float TUTORIAL_PAGE_HEIGHT = 100;
// ページ表示のテクスチャ（ページ数に対応している）
static const TEXTURE_TYPE TUTORIAL_PAGE_TEXTURE[TUTORIAL_PAGE_MAX] = {
	TEXTURE_TUTORIAL_PAGE_1_4,
	TEXTURE_TUTORIAL_PAGE_2_4,
	TEXTURE_TUTORIAL_PAGE_3_4,
	TEXTURE_TUTORIAL_PAGE_4_4,
};

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTutorial::CTutorial(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTutorial ::~CTutorial(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CTutorial::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// 変数初期化
	m_pBackGround = NULL;
	m_vVelo = D3DXVECTOR3(0, 0, 0);
	m_nPage = 0;
	m_bRight = true;
	m_nPictNumLeft = 0;
	m_nPictNumCenter = 1;
	m_nPictNumRight = 2;
	m_pArrowRight = NULL;
	m_pArrowLeft = NULL;
	m_bArrowScale = true;
	m_nArrowCount = 0;
	m_pPage2D = NULL;

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// 背景
	m_pBackGround = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_TUTORIAL, TYPE_PRIORITY_BG);

	// チュートリアルの絵
	for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
		m_p2D[n] = CScene2D::Create(m_pD3DDevice,
			TUTORIAL_PICT_POS[n],
			static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
			TEXTURE_TUTORIAL_0, TYPE_PRIORITY_FIELD);
	}

	// 矢印アニメーション
	m_pArrowLeft = CSceneAnime::Create(m_pD3DDevice,
		TUTORIAL_ARROR_POS_L,
		TUTORIAL_ARROR_WIDTH, TUTORIAL_ARROR_HEIGHT,
		TEXTURE_TUTORIAL_ARROW, TUTORIAL_ARROR_TEXTURE_SEP_X, TUTORIAL_ARROR_TEXTURE_SEP_Y,
		TUTORIAL_ARROW_ANIME_SPEED, -1);

	m_pArrowRight = CSceneAnime::Create(m_pD3DDevice,
		TUTORIAL_ARROR_POS_R,
		TUTORIAL_ARROR_WIDTH, TUTORIAL_ARROR_HEIGHT,
		TEXTURE_TUTORIAL_ARROW, TUTORIAL_ARROR_TEXTURE_SEP_X, TUTORIAL_ARROR_TEXTURE_SEP_Y,
		TUTORIAL_ARROW_ANIME_SPEED, -1);
	m_pArrowRight->SetRot(D3DXVECTOR3(0, 0, D3DX_PI));

	// ページ表示２D
	m_pPage2D = CScene2D::Create(m_pD3DDevice,
		TUTORIAL_PAGE_POS,
		TUTORIAL_PAGE_WIDTH, TUTORIAL_PAGE_HEIGHT,
		TUTORIAL_PAGE_TEXTURE[m_nPage], TYPE_PRIORITY_FIELD);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ＢＧＭ再生
	CManager::PlaySoundA(SOUND_LABEL_BGM_TUTORIAL);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CTutorial::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CTutorial::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	// 矢印のアニメーション
	UpdateArrow();

	// フェードアウトしてタイトルヘもどる
	// 入力されたらPUSH START実行
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_DECIDE, 0)){
		CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}

	// 絵が止まっているときのみ動かせる
	if (m_vVelo.x == 0){
		// 左右キーで次のページへ移動
		for (int i = 0; i < PLAYER_MAX; i++){
			if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
				CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_LEFT, i)){
				CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
				// 絵の移動量を入れる
				m_vVelo = TUTORIAL_PICT_VELO;
				// フラグ切り替え
				m_bRight = true;
				// ページ切り替え
				m_nPage--;
				if (m_nPage <= -1){
					m_nPage = TUTORIAL_PAGE_MAX - 1;
	  			}
				// テクスチャ切り替え
				m_p2D[m_nPictNumLeft]->ChangeTexture(TUTORIAL_PICT_TEXTURE[m_nPage]);
				// 一回だけ通る
				break;
			}
			else if (CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
				CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_RIGHT, i)){
				CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
				// 絵の移動量を入れる
				m_vVelo = -TUTORIAL_PICT_VELO;
				// フラグ切り替え
				m_bRight = false;
				// ページ切り替え
				m_nPage++;
				if (m_nPage > TUTORIAL_PAGE_MAX - 1){
					m_nPage = 0;
				}
				// テクスチャ切り替え
				m_p2D[m_nPictNumRight]->ChangeTexture(TUTORIAL_PICT_TEXTURE[m_nPage]);
				// 一回だけ通る
				break;
			}
		}
	}

	// 絵の動き
	for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
		m_vVelo *= TUTORIAL_PICT_RESIST;
		m_p2D[n]->AddPos(m_vVelo);
	}

	// 端っこ当たり判定
	if (m_bRight){
		if (m_p2D[m_nPictNumLeft]->GetPos().x >= SCREEN_WIDTH * 0.5f){
			// 移動量初期化
			m_vVelo = D3DXVECTOR3(0, 0, 0);

			// 対応添え字の入れ替え
			short t = m_nPictNumRight;
			m_nPictNumRight = m_nPictNumCenter;
			m_nPictNumCenter = m_nPictNumLeft;
			m_nPictNumLeft = t;

			// 全位置調整
			m_p2D[m_nPictNumLeft]->SetPos(TUTORIAL_PICT_POS[0]);
			m_p2D[m_nPictNumCenter]->SetPos(TUTORIAL_PICT_POS[1]);
			m_p2D[m_nPictNumRight]->SetPos(TUTORIAL_PICT_POS[2]);

			// ページ表示の切り替え
			m_pPage2D->ChangeTexture(TUTORIAL_PAGE_TEXTURE[m_nPage]);
		}
	}
	else{
		if (m_p2D[m_nPictNumRight]->GetPos().x <= SCREEN_WIDTH * 0.5f){
			// 移動量初期化
			m_vVelo = D3DXVECTOR3(0, 0, 0);

			// 対応添え字の入れ替え
			short t = m_nPictNumLeft;
			m_nPictNumLeft = m_nPictNumCenter;
			m_nPictNumCenter = m_nPictNumRight;
			m_nPictNumRight = t;

			// 全位置調整
			m_p2D[m_nPictNumLeft]->SetPos(TUTORIAL_PICT_POS[0]);
			m_p2D[m_nPictNumCenter]->SetPos(TUTORIAL_PICT_POS[1]);
			m_p2D[m_nPictNumRight]->SetPos(TUTORIAL_PICT_POS[2]);

			// ページ表示の切り替え
			m_pPage2D->ChangeTexture(TUTORIAL_PAGE_TEXTURE[m_nPage]);
		}
	}

}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CTutorial* CTutorial::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CTutorial* pTitle = NULL;

	// 作成
	pTitle = new CTutorial;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
// 矢印アニメーション更新
//*****************************************************************************
void CTutorial::UpdateArrow(){

	// 一定カウントで拡縮フラグ切り替え
	m_nArrowCount++;
	if (m_nArrowCount > TUTORIAL_ARROW_SCALE_INTERVAL){
		m_nArrowCount = 0;
		m_bArrowScale = !m_bArrowScale;
	}
	// 拡大する
	if (m_bArrowScale){
		m_pArrowLeft->AddHeight_BaseBottom(-TUTORIAL_ARROW_SCALE);
		m_pArrowLeft->AddHeight_BaseTop(-TUTORIAL_ARROW_SCALE);
		m_pArrowRight->AddHeight_BaseBottom(-TUTORIAL_ARROW_SCALE);
		m_pArrowRight->AddHeight_BaseTop(-TUTORIAL_ARROW_SCALE);
	}
	// 縮小する
	else{
		m_pArrowLeft->AddHeight_BaseBottom(TUTORIAL_ARROW_SCALE);
		m_pArrowLeft->AddHeight_BaseTop(TUTORIAL_ARROW_SCALE);
		m_pArrowRight->AddHeight_BaseBottom(TUTORIAL_ARROW_SCALE);
		m_pArrowRight->AddHeight_BaseTop(TUTORIAL_ARROW_SCALE);
	}
}

//----EOF----