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

typedef enum
{
	GAME_INTRO = 0,
	GAME_BATTLE,
	GAME_FINISH,
	GAME_MAX
}GAME_MODE;

typedef enum
{
	BATTLE_MOVE = 0,
	BATTLE_FIGHT,
	BATTLE_MAX
}BATTLE_MODE;
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
	void GameIntro(void);
	void GameBattle(void);
	void GameFinish(void);

	// ゲーム内のUIを生成、管理するクラス
	CUiManager *m_pUiManager;
	// フィールド置くものを管理するクラス
	CFieldManager *m_pFieldManager;

	GAME_MODE m_Mode;
	GAME_MODE m_PrevMode;
	BATTLE_MODE m_BattleMode;

	int m_BattleTimer;
};

#endif
//----EOF----