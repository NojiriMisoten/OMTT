//=============================================================================
//
// CJudgeクラス [CJudge.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CJUDGE_H_
#define _CJUDGE_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../PLAYER/CPlayerManager.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	BATTLE_MOVE = 0,
	BATTLE_FIGHT,
	BATTLE_MAX
}BATTLE_MODE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CJudge
{
public:
	// コンストラクタ
	CJudge( void );

	// デストラクタ
	~CJudge( void );

	// 更新
	void Update( void );

	// 作成
	static CJudge* Create( CManager* pManager );

	// 終了
	void Uninit( void );

	// 戦闘モードのゲット
	void SetBattleMode( BATTLE_MODE battleMode ) { m_BattleMode = battleMode; };

	// 戦闘モードのセット
	BATTLE_MODE GetBattleMode( void ) { return m_BattleMode; };

	// 保存用プレイヤー座標ゲット
	D3DXVECTOR3 GetSavePos( PLAYER_ID playerID ) { return m_SavePos[playerID]; };

private:
	void Init( CManager* pManager );

	CManager *m_pManager;			// マネージャー

	// 戦闘モード
	BATTLE_MODE m_BattleMode;
	BATTLE_MODE m_BattleModeOld;

	// プレイヤー座標保存用
	D3DXVECTOR3 m_SavePos[PLAYER_MAX];
};

#endif
//----EOF----
