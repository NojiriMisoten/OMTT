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
#include "../BASE_OBJECT/CScene.h"
#include "../MODEL/CModel.h"
#include "../LOADING/CNowLoading.h"
#include "../INPUT/CInputKeyboard.h"
#include "../CAMERA/CameraManager.h"
#include <process.h>
#include "../SHADER/CShader.h"
#include "../LIGHT/CLightManager.h"
#include "../SCENE/GAME/JUDGE/CJudgeManager.h"
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../EFECT/CEffectManager.h"
#include "../CONTROLLER/CControllerManager.h"
#include "../EFECT/CEffectHolder.h"
#include "../STAGE_DIRECTOR/CDirectorManager.h"
#include "../SCENE/GAME/UI/CUiManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const MODE_PHASE INIT_MODE = MODE_PHASE_GAME;	// 最初のフェイズ

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
CRenderer*		CManager::m_pRenderer = NULL;
CPhase*			CManager::m_pPhase = NULL;
CSound*			CManager::m_pSound = NULL;
MODE_PHASE		CManager::m_CurPhase = MODE_PHASE_NULL;
MODE_PHASE		CManager::m_NextPhase = MODE_PHASE_NULL;
volatile bool	CManager::m_bEndload = false;
//=============================================================================
// コンストラクタ
//=============================================================================
CManager ::CManager(void)
{
	m_pInputKeyboard = NULL;
	m_pInputGamePad = NULL;
	m_pSound = NULL;

	#ifdef _DEBUG
	m_pDebugProc = NULL;
	#endif
	m_pNowLoading = NULL;
	m_bEndload = false;
	m_pCameraManager = NULL;
	m_pLightManager = NULL;
	m_pControllerManager = NULL;
	m_pJudgeManager = NULL;
	m_pDirectorManager = NULL;
	m_pUiManager = NULL;
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

	m_pRenderer = new CRenderer;
	if(m_pRenderer->Init(hWnd, bWindow) == E_FAIL)
	{
		return E_FAIL;
	}

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

	// コントローラーの初期化
	m_pControllerManager = CControllerManager::Create();

	// ロードスレッド開始
	m_ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, LoadThred, &m_sendParam, 0, NULL);

	// NowLoading作成
	m_pNowLoading = new CNowLoading;
	m_pNowLoading->Init(m_pRenderer->GetDevice());

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

	// ゲームパッドの終了
	if(m_pInputGamePad)
	{
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}

	// コントローラーの終了
	if (m_pControllerManager)
	{
		m_pControllerManager->Uninit();
		delete m_pControllerManager;
		m_pControllerManager = NULL;
	}

	// テクスチャの終了
	CTexture::Uninit();

	//エフェクトの終了
	CEffectHolder::Uninit();

	// モデル作成
	CModel::Uninit();

	// シェーダーの終了
	CShader::Uninit();

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

	// カメラマネージャーの終了
	if(m_pCameraManager)
	{
		m_pCameraManager->Uninit();
		delete m_pCameraManager;
		m_pCameraManager = NULL;
	}

	// ライトマネージャーの終了
	if (m_pLightManager)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}

	// ジャッジマネージャーの終了
	if (m_pJudgeManager)
	{
		m_pJudgeManager->Uninit();
		delete m_pJudgeManager;
		m_pJudgeManager = NULL;
	}

	// プレイヤーマネージャーの終了
	if (m_pPlayerManager)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = NULL;
	}

	// エフェクトマネージャーの終了
	if (m_pEffectManager)
	{
		m_pEffectManager->Uninit();
		delete m_pEffectManager;
		m_pEffectManager = NULL;
	}

	// ディレクターマネージャーの終了
	if( m_pDirectorManager )
	{
		m_pDirectorManager->Uninit();
		delete m_pDirectorManager;
		m_pDirectorManager = NULL;
	}

	// ＵＩマネージャーの終了
	if( m_pUiManager )
	{
		delete m_pUiManager;
		m_pUiManager = NULL;
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
		m_pPhase->Update();
		m_pRenderer->Update();
	}

	// フェード中なら
	else
	{
		// フェードのみ更新
		m_pRenderer->Update(CRenderer::TYPE_RENDER_FADE);
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

	// カメラセット
	m_pCameraManager->SetCamera(m_pRenderer->GetDevice());

	// ポーズ以外描画
	m_pRenderer->Draw();
	
	// フェーズ切り替え
	if(m_CurPhase != m_NextPhase)
	{
		ChangePhase();
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
// カメラマネージャーゲット
//=============================================================================
CCameraManager* CManager::GetCameraManager(void)
{
	return m_pCameraManager;
}

//=============================================================================
// 初期化関数
//=============================================================================
unsigned __stdcall CManager :: LoadThred(LPVOID Param)
{
	SEND_LOAD_THRED_PARAM* p = (SEND_LOAD_THRED_PARAM*)Param;

	//エフェクトマネージャー生成
	p->pMyAddr->m_pEffectManager = new CEffectManager(m_pRenderer->GetDevice());
	p->pMyAddr->m_pEffectManager->Init();

	// カメラマネージャーの作成
	p->pMyAddr->m_pCameraManager = new CCameraManager(p->pMyAddr->m_pEffectManager);

	// ライトマネージャーの作成
	p->pMyAddr->m_pLightManager = new CLightManager(p->pMyAddr);

	// ジャッジマネージャーの作成
	p->pMyAddr->m_pJudgeManager = new CJudgeManager;

	// プレイヤーマネージャーの作成
	p->pMyAddr->m_pPlayerManager = new CPlayerManager(p->pMyAddr);
	
	// ディレクターマネージャーの作成
	p->pMyAddr->m_pDirectorManager = new CDirectorManager( p->pMyAddr );

	// UIマネージャーの作成
	p->pMyAddr->m_pUiManager = new CUiManager( m_pRenderer->GetDevice(), p->pMyAddr );


#ifdef _DEBUG
	// デバッグプロック作成
	p->pMyAddr->m_pDebugProc = new CDebugProc;
	p->pMyAddr->m_pDebugProc->Init();
#endif

	// シェーダーの作成
	CShader::Create(m_pRenderer->GetDevice());

	// テクスチャの作成
	CTexture::CreateTexture(p->pMyAddr->m_pRenderer->GetDevice());

	//エフェクトの作成
	CEffectHolder::CreateEffect();

	// モデル作成
	CModel::CreateModel(p->pMyAddr->m_pRenderer->GetDevice());

	// フェイズの作成
	p->pMyAddr->m_pPhase = p->pMyAddr->m_pPhase->Create(INIT_MODE, p->pMyAddr->m_pRenderer->GetDevice(), p->pMyAddr);
	p->pMyAddr->m_CurPhase = INIT_MODE;
	p->pMyAddr->m_NextPhase = INIT_MODE;

	p->pMyAddr->m_bEndload = true;

	return 0;
}

//----EOF----