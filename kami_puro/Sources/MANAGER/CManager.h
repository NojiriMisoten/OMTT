//=============================================================================
//
// CManagerクラス [CManager.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CMANAGER_H_
#define _CMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "../RENDERER/CRenderer.h"
#include "../PHASE/CPhase.h"
#include "../SOUND/CSound.h"
#include "../INPUT/CInputGamePad.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;
class CNowLoading;
class CInputKeyboard;
class CInputGamePad;
class CCameraManager;
class CLightManager;
class CJudgeManager;
class CPlayerManager;
class CEffectManager;
class CControllerManager;
class CDirectorManager;
class CUiManager;

#ifdef _DEBUG
class CDebugProc;
#endif

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CManager
{
public:
	// コンストラクタ
	CManager(void);

	// デストラクタ
	~CManager(void);

	//=======================================================
	// 初期化
	// 引数: インスタンス、ウインドウハンドル、ウインドウモードで表示するか
	// 戻り値: 成否
	//=======================================================
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	//=======================================================
	// レンダラーゲット
	// 戻り値: レンダラーのアドレス
	//=======================================================
	static CRenderer* GetRenderer(void){return m_pRenderer;};

	//=======================================================
	// 音再生
	// 引数: サウンドの種類
	//=======================================================
	static void PlaySound(SOUND_LABEL label){m_pSound->PlaySound(label);};

	//=======================================================
	// 指定した音停止
	// 引数: サウンドの種類
	//=======================================================
	static void StopSound(SOUND_LABEL label){m_pSound->StopSound(label);};
	
	// 音全て停止
	static void StopSound(void){m_pSound->StopSound();};

	// フェーズ切り替え
	void ChangePhase(void);

	// フェーズやり直し
	void RetryPhase(void);

	// 現在のフェーズ取得
	MODE_PHASE GetCurPhase(void){return m_CurPhase;};

	// 次のフェーズセット
	void SetNextPhase(MODE_PHASE phase){m_NextPhase = phase;};

	// カメラマネージャーゲット
	CCameraManager* GetCameraManager(void);

	// ライトマネージャーゲット
	CLightManager* GetLightManager(void){ return m_pLightManager; };

	// ジャッジマネージャーゲット
	CJudgeManager* GetJudgeManager(void){ return m_pJudgeManager; };

	//プレイヤーマネージャーゲット
	CPlayerManager* GetPlayerManager(void){ return m_pPlayerManager; };

	// ディレクターマネージャーゲット
	CDirectorManager* GetDirectorManager( void ) { return m_pDirectorManager; };

	// ＵＩマネージャーゲット
	CUiManager* GetUiManager( void ) { return m_pUiManager; };

private:
	// NOW LOADINGのスレッドに渡す情報構造体
	typedef struct
	{
		CManager* pMyAddr;		// CManagerのアドレス
		HINSTANCE hInstance;	// インスタンス
		HWND hWnd;				// ウインドウハンドル
		bool bWindow;			// ウインドウモードで表示するか
	}SEND_LOAD_THRED_PARAM;

	//===============================================
	// ロードスレッド
	// 引数: NOW LOADINGのスレッドに渡す情報構造体ポインタ
	//===============================================
	static unsigned __stdcall LoadThred(LPVOID Param);

	static CRenderer		*m_pRenderer;				// レンダラー
	static CPhase			*m_pPhase;					// フェイズ
	static MODE_PHASE		m_CurPhase;					// 現在のフェーズ
	static MODE_PHASE		m_NextPhase;				// 次のフェーズ
	static volatile bool	m_bEndload;					// ロード終了フラグ
	static CSound			*m_pSound;					// サウンド
	CCameraManager			*m_pCameraManager;			// カメラマネージャー
	CLightManager			*m_pLightManager;			// ライトマネージャー
	CJudgeManager			*m_pJudgeManager;			// ジャッジマネージャー
	CPlayerManager			*m_pPlayerManager;			// プレイヤーマネージャー
	CEffectManager			*m_pEffectManager;			// エフェクトマネージャー
	CDirectorManager		*m_pDirectorManager;		// ディレクターマネージャー
	CInputKeyboard			*m_pInputKeyboard;			// キーボード
	CInputGamePad			*m_pInputGamePad;			// ゲームパッド
	CControllerManager		*m_pControllerManager;		// コントローラーマネージャー
	CUiManager				*m_pUiManager;				// UIマネージャー
	SEND_LOAD_THRED_PARAM	m_sendParam;				// ロードスレッドに渡す情報
	HANDLE					m_ThreadHandle;				// スレッドのハンドル
	CNowLoading				*m_pNowLoading;				// NowLoading
	bool					m_isRetry;
	#ifdef _DEBUG
	CDebugProc				*m_pDebugProc;				// デバッグプロック
	#endif
};
#endif
//----EOF----