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
// 定数
//*****************************************************************************
//プレイする人数
static const int PLAYER_MAX = 4;

// プレイヤ番号に応じて糸トラップの色を変える色
static const D3DXCOLOR PLAYER_COLOR[] = {
	D3DCOLOR(0xbbfbec35),
	D3DCOLOR(0xbbf92500),
	D3DCOLOR(0xbb1987e5),
	D3DCOLOR(0xbb00ff41),
};
// プレイヤ番号に応じてテクスチャ
static TEXTURE_TYPE PLAYER_TEXTURE[] = {
	TEXTURE_PLAYER_YELLOW,
	TEXTURE_PLAYER_RED,
	TEXTURE_PLAYER_BLUE,
	TEXTURE_PLAYER_GREEN,
};

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;
class CNowLoading;
class CInputKeyboard;
class CInputGamePad;
class CControllerManager;
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

	// 勝ったプレイヤアクセサ
	static void SetWinPlayerNum(int num){ m_nWinPlayerNum = num; }
	static int GetWinPlayerNum(){ return m_nWinPlayerNum; }

	// プレイヤ人数のセッタゲッタ
	static void SetPlayerNum(short manual){
		m_nPlayerNumManual = manual;
		m_nPlayerNumCpu = PLAYER_MAX - m_nPlayerNumManual;
	}
	static short GetPlayerManualNum(){
		return m_nPlayerNumManual;
	}

	// 選択したステージ選択番号
	static void SetStageNum(int num){
		m_nStageNum = num;
	}
	static int GetStageNum(){
		return m_nStageNum;
	}
	// キャラ選択で参加した人数
	static void SetJoinNum(short num){
		m_nJoinNum = num;
	}
	static short GetJoinNum(){
		return m_nJoinNum;
	}

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
	CInputKeyboard			*m_pInputKeyboard;			// キーボード
	CInputGamePad			*m_pInputGamePad;			// ゲームパッド
	CControllerManager		*m_pControllerManager;		// コントローラーマネージャー
	SEND_LOAD_THRED_PARAM	m_sendParam;				// ロードスレッドに渡す情報
	HANDLE					m_ThreadHandle;				// スレッドのハンドル
	CNowLoading				*m_pNowLoading;				// NowLoading
	static int				m_nWinPlayerNum;			// 勝ったプレイヤb番号
	static short			m_nPlayerNumManual;			// 操作するプレイヤ数
	static short			m_nPlayerNumCpu;			// ＣＰＵプレイヤ数
	static int				m_nStageNum;				// 選択したステージ番号
	static short			m_nJoinNum;					// キャラ選択で参戦した人数
#ifdef _DEBUG
	CDebugProc				*m_pDebugProc;				// デバッグプロック
	#endif
};
#endif
//----EOF----