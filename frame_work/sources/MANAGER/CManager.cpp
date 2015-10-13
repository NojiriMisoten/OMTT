//=============================================================================
//
// CManagerクラス [CManager.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CManager.h"
#include "../SCENE/CSCENE/CScene.h"
#include "../SCENE/PAUSE/CPause.h"
#include "../SCENE/GAME/CNowLoading.h"
#include "../INPUT/CInputKeyboard.h"
#include "../CONTROLLER/CControllerManager.h"
#include <process.h>

//*****************************************************************************
// マクロ
//*****************************************************************************
static const MODE_PHASE INIT_MODE = MODE_PHASE_TITLE;	// 最初のフェイズ

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
CRenderer*		CManager::m_pRenderer = NULL;
CPhase*			CManager::m_pPhase = NULL;
CSound*			CManager::m_pSound = NULL;
MODE_PHASE		CManager::m_CurPhase = MODE_PHASE_NULL;
MODE_PHASE		CManager::m_NextPhase = MODE_PHASE_NULL;
volatile bool	CManager::m_bEndload = false;
int				CManager::m_nWinPlayerNum = -1;
short			CManager::m_nPlayerNumManual;
short			CManager::m_nPlayerNumCpu;
short			CManager::m_nJoinNum;
int				CManager::m_nStageNum;

//=============================================================================
// コンストラクタ
//=============================================================================
CManager ::CManager(void)
{
	m_pInputKeyboard = NULL;
	m_pInputGamePad = NULL;
	m_pSound = NULL;
	m_pControllerManager = NULL;

	#ifdef _DEBUG
	m_pDebugProc = NULL;
	#endif
	m_pNowLoading = NULL;
	m_bEndload = false;
	m_nWinPlayerNum = 1;
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager ::~CManager(void)
{
}

//=============================================================================
// 初期化関数
//=============================================================================
HRESULT CManager ::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	m_sendParam.pMyAddr = this;
	m_sendParam.hInstance = hInstance;
	m_sendParam.hWnd = hWnd;
	m_sendParam.bWindow = bWindow;
	m_nWinPlayerNum = 0;
	m_nPlayerNumManual = 0;
	m_nPlayerNumCpu = 0;
	m_nJoinNum = 0;

	m_pRenderer = new CRenderer;
	if(m_pRenderer->Init(hWnd, bWindow) == E_FAIL)
	{
		return E_FAIL;
	}

	// ロードスレッド開始
	m_ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, LoadThred, &m_sendParam, 0, NULL);

	// NowLoading作成
	m_pNowLoading = new CNowLoading;
	m_pNowLoading->Init(m_pRenderer->GetDevice());

	// キーボードの初期化
	m_pInputKeyboard = new CInputKeyboard;
	if(m_pInputKeyboard->Init(hInstance, hWnd) == E_FAIL)
	{
		return E_FAIL;
	}

	// ゲームパッドの初期化
	m_pInputGamePad = new CInputGamePad;
	if(m_pInputGamePad->Init(hInstance, hWnd) == E_FAIL)
	{
		return E_FAIL;
	}

	m_pControllerManager = CControllerManager::Create();

	#ifdef _DEBUG
	// デバッグプロック作成
	m_pDebugProc = new CDebugProc;
	m_pDebugProc->Init();
	#endif

	// 音の初期化
	m_pSound = new CSound;
	m_pSound->InitSound(hWnd);

	return S_OK;
}

//=============================================================================
// 終了関数
//=============================================================================
void CManager ::Uninit(void)
{
	if(m_ThreadHandle != NULL)
	{
		// 別スレッド終了まで待つ
		WaitForSingleObject(m_ThreadHandle,INFINITE);
		if(CloseHandle(m_ThreadHandle))
		{
			m_ThreadHandle = NULL;
		}
	}
#ifdef _DEBUG
	if(m_pDebugProc)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
#endif

	// キーボードの終了
	if(m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// コントローラーマネージャーの終了
	if (m_pControllerManager)
	{
		m_pControllerManager->Uninit();
		delete m_pControllerManager;
		m_pControllerManager = NULL;
	}

	// ゲームパッドの終了
	if(m_pInputGamePad)
	{
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}

	// テクスチャの終了
	CTexture::Uninit();

	// すべて開放
	CScene::ReleaseAll();
	if(m_pRenderer)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// フェイズの終了
	if(m_pPhase)
	{
		m_pPhase->Uninit();
		delete m_pPhase;
		m_pPhase = NULL;
	}

	// サウンド終了
	if(m_pSound)
	{
		m_pSound->StopSound();
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = NULL;
	}

	// NowLoading終了
	if(m_pNowLoading)
	{
		m_pNowLoading->Uninit();
		delete m_pNowLoading;
		m_pNowLoading = NULL;
	}
}

//=============================================================================
// 更新関数
//=============================================================================
void CManager ::Update(void)
{
	// 別スレッドで読み込み終了してないなら
	if(!m_bEndload)
	{
		// NowLoadingの更新
		m_pNowLoading->Update();
		return;
	}

	// 別スレッドが残ってるなら
	DWORD ThreadStatus;
	if(m_ThreadHandle != NULL)
	{
		// スレッド状態取得
		GetExitCodeThread(m_ThreadHandle, &ThreadStatus);

		// 動いてないなら
		if(ThreadStatus != STILL_ACTIVE)
		{
			// ハンドル閉じる
			CloseHandle(m_ThreadHandle);
			m_ThreadHandle = NULL;

			// NowLoading終了
			if(m_pNowLoading)
			{
				m_pNowLoading->Uninit();
				delete m_pNowLoading;
				m_pNowLoading = NULL;
			}
		}
	}

	// キーボード更新
	m_pInputKeyboard->Update();

	// ゲームパッド更新
	m_pInputGamePad->Update();

	// フェードインが終わってるなら
	if(CFade::GetMode() == MODE_FADE_IN_END)
	{
		// フェーズ更新
		m_pPhase->Update();

		// ポーズ中じゃなければ
		if(CPause::GetPauseFlag() == false)
		{
			// ポーズ以外更新
			m_pRenderer->Update();
		}

		// ポーズ中なら
		else
		{
			// ポーズのみ更新
			m_pRenderer->Update(TYPE_PRIORITY_PAUSE);
		}
	}

	// フェード中なら
	else
	{
		// フェードのみ更新
		m_pRenderer->Update(TYPE_PRIORITY_FADE);
	}

	// リトライフラグオンなら
	if(CPause::GetRetryFlag() == true)
	{
		// フェードアウトが終わってるなら
		if(CFade::GetMode() == MODE_FADE_OUT_END)
		{
			// リトライへ
			CManager::RetryPhase();
		}
	}
}

//=============================================================================
// 描画関数
//=============================================================================
void CManager ::Draw(void)
{
	if(!m_bEndload)
	{
		m_pNowLoading->Draw();
		return;
	}

	// フェーズ切り替え
	if (m_CurPhase != m_NextPhase)
	{
		ChangePhase();
	}

	// ポーズ中じゃなければ
	if(CPause::GetPauseFlag() == false)
	{
		// ポーズ以外描画
		m_pRenderer->Draw();
	}
	else
	{
		// ポーズとフェードのみ描画
		m_pRenderer->Draw(TYPE_PRIORITY_PAUSE, TYPE_PRIORITY_FADE);
	}
}

//=============================================================================
// フェーズ切り替え関数
//=============================================================================
void CManager ::ChangePhase(void)
{
	// フェードアウトが終わってるなら
	if(CFade::GetMode() == MODE_FADE_OUT_END)
	{
		// サウンド全て停止
		m_pSound->StopSound();

		// すべて開放
		CScene::ReleaseAll();

		// 現在のフェーズ破棄
		if(m_pPhase)
		{
			m_pPhase->Uninit();
			delete m_pPhase;
			m_pPhase = NULL;
		}

		// 次のフェーズ生成
		m_pPhase = m_pPhase->Create(m_NextPhase, m_pRenderer->GetDevice(), this);

		// 現在のフェーズを更新
		m_CurPhase = m_NextPhase;
	}
}

//=============================================================================
// フェーズリトライ関数
//=============================================================================
void CManager ::RetryPhase(void)
{
	// すべて開放
	CScene::ReleaseAll();

	// 現在のフェーズ破棄
	m_pPhase->Uninit();
	delete m_pPhase;

	// 次のフェーズ生成
	m_pPhase = m_pPhase->Create(m_NextPhase, m_pRenderer->GetDevice(), this);
	m_CurPhase = m_NextPhase;
}

//=============================================================================
// 初期化関数
//=============================================================================
unsigned __stdcall CManager :: LoadThred(LPVOID Param)
{
	SEND_LOAD_THRED_PARAM* p = (SEND_LOAD_THRED_PARAM*)Param;

	// テクスチャの作成
	CTexture::CreateTexture(p->pMyAddr->m_pRenderer->GetDevice());

	// フェイズの作成
	p->pMyAddr->m_pPhase = p->pMyAddr->m_pPhase->Create(INIT_MODE, p->pMyAddr->m_pRenderer->GetDevice(), p->pMyAddr);
	p->pMyAddr->m_CurPhase = INIT_MODE;
	p->pMyAddr->m_NextPhase = INIT_MODE;

	p->pMyAddr->m_bEndload = true;

	return 0;
}

//----EOF----