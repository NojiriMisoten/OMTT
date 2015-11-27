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
#include "../COMMANDCHART/CCommandChart.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
class CPlayerManager;
class CDirectorManager;
class CCommandChartManager;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	BATTLE_MOVE = 0,
	BATTLE_FIGHT,
	BATTLE_MAX
}BATTLE_MODE;

typedef enum
{
	RPS_NONE = -1,
	RPS_ROCK = 0,
	RPS_PAPER,
	RPS_SCISSOR,
	RPS_MAX
}TYPE_RPS;

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

	void BattleMoveUpdate( void );
	
	void BattleFightUpdate( void );

private:
	void Init( CManager* pManager );

	CManager				*m_pManager;			// マネージャー
	CCommandChartManager	*m_pCommandChartManager;
	CPlayerManager			*m_pPlayerManager;
	CDirectorManager		*m_pDirectorManager;

	// 戦闘モード
	BATTLE_MODE m_BattleMode;
	BATTLE_MODE m_BattleModeOld;

	// プレイヤー座標保存用
	D3DXVECTOR3 m_SavePos[PLAYER_MAX];
	
	// 入力関連
	int m_InputWaitFrameCount[PLAYER_MAX];
	COMMAND_TYPE m_Command[PLAYER_MAX];

	// プレイや―座標関連
	D3DXVECTOR3 m_Pos[PLAYER_MAX];
	D3DXVECTOR3 m_PosCenter;
	float m_Dist[PLAYER_MAX];
	float m_DistTotal;

	// 前回演出保存用
	int m_IsDirectingOld;
};

#endif
//----EOF----
