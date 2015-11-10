//=============================================================================
//
// CGameクラス [CGame.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CGAME_H_
#define _CGAME_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CPlayerManager;
class CUiManager;
class CFieldManager;
class CCrowdManager;
class CCameraManager;
class CJudgeManager;

typedef enum
{
	GAME_INTRO = 0,
	GAME_BATTLE,
	GAME_FINISH,
	GAME_MAX
}GAME_MODE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CPhase
{
	// 公開メンバ
public:
	// コンストラクタ
	CGame(void);

	// デストラクタ
	~CGame(void);

	//======================================================
	// 初期化
	// 引数: フェーズモード、デバイス
	//======================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//======================================================
	// クリエイト
	// 引数: フェーズモード、デバイス
	// 戻り値: 作ったやつのポインタ
	//======================================================
	static CGame* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	CManager* GetManager(void){ return m_pManager; };

	int GetBattleTimer(void) { return m_BattleTimer; };
private:
	//　クラス化した方がよさげ？
	void GameIntro( void );
	void GameBattle( void );
	void GameFinish( void );

	void InitGameIntro( void );
	void InitGameBattle( void );
	void InitGameFinish( void );

	void UninitGameIntro( void );
	void UninitGameBattle( void );
	void UninitGameFinish( void );

	// ゲーム内のUIを生成、管理するクラス
	CUiManager *m_pUiManager;

	// カメラマネージャー
	CCameraManager *m_pCameraManager;

	// ジャッジマネージャー
	CJudgeManager *m_pJudgeManager;
	
	// フィールド全体の管理
	CFieldManager *m_pFieldManager;
	CCrowdManager *m_pCrowdManager;

	GAME_MODE m_Mode;
	GAME_MODE m_PrevMode;

	int m_BattleTimer;
};

#endif
//----EOF----