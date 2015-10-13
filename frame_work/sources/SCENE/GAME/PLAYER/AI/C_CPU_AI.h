//=============================================================================
//
// C_CPU_AIクラス [C_CPU_AI.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _C_CPU_AI_H_
#define _C_CPU_AI_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../../MAIN/main.h"
#include "../../FIELD/CNaviTile.h"

class CPlayer;
class CTreasure;
class CGoal;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class C_CPU_AI
{
public:
	// コンストラクタ
	C_CPU_AI();

	// デストラクタ
	~C_CPU_AI();

	// 終了
	void	Uninit(void);

	//	更新
	void	Update(void);

	// クリエイト
	static C_CPU_AI* Create(CNaviTile* pNaviTile, CPlayer* pPlayer);
	
	// AIの入力ゲッター
	bool GetAIInputRight(void){ return m_AIInput.moveRight; };
	bool GetAIInputLeft(void){ return m_AIInput.moveLeft; };
	bool GetAIInputUp(void){ return m_AIInput.moveUp; };
	bool GetAIInputDown(void){ return m_AIInput.moveDown; };
	bool GetAIInputThread(void){ return m_AIInput.pushThread; };
	bool GetAIInputAtk(void){ return m_AIInput.pushAtk; };
	bool GetAIInputChangeAtk(void){ return m_AIInput.changeAtk; };
	bool GetAIInputChangeSpeed(void){ return m_AIInput.changeSpeed; };
	bool GetAIInputChangeJammer(void){ return m_AIInput.changeJammer; };
private:
	// AIの思考
	typedef enum
	{
		AI_STATE_THINK = 0,						// 行動を考える
		AI_STATE_MOVE_TO_TREASURE,				// 宝に向かう
		AI_STATE_ATTACK_WHO,					// 誰かを攻撃する
		AI_STATE_ATTACK_HAVE_TREASURE_PLAYER,	// 宝を持ってるやつを攻撃する
		AI_STATE_MOVE_GOAL,						// ゴールに向かう
		AI_STATE_SAVE_TREASURE,					// ゴール地点で宝を守る
		AI_STATE_MAKE_FOOT_STEP,				// 足場作る
		AI_STATE_MOVE_FREE,						// 自由移動
		AI_STATE_MAX
	}AI_STATE;

	// AIの押したボタン
	typedef struct
	{
		bool moveRight;
		bool moveLeft;
		bool moveUp;
		bool moveDown;
		bool pushThread;
		bool pushAtk;
		bool changeAtk;
		bool changeSpeed;
		bool changeJammer;
	}AI_INPUT;

	// 初期化
	void Init(CNaviTile* pNaviTile, CPlayer* pPlayer);

	// 行動を考える
	void ThinkAI(void);

	// 宝に向かう
	void MoveToTreasure(void);

	// 誰かを攻撃する
	void AttackWho(void);

	// 他から持ってるやつを攻撃する
	void AttackHaveTreasurPlayer(void);

	// ゴールに向かう
	void MoveGoal(void);

	// 宝を守る
	void SaveTreasure(void);

	// 足場作る
	void MakeFootStep(void);
	
	// 適当移動
	void MoveFree(void);

	// 目標地点に向かう
	bool Move(void);

	// オープンリスト初期化
	void InitOpenList(void);

	// 経路探索
	bool SarchPath(TileIndex* start, TileIndex* goal);

	// 周りのタイル探索
	void SarchAroundTile(TileIndex* sarch_base, TileIndex* start, TileIndex* goal);

	// オープン化
	void SetOpen(TileIndex* tile, TileIndex* start, TileIndex* goal, TileIndex* sarch_base);

	// クローズ化
	void SetClose(TileIndex* sarch_base, TileIndex* start, TileIndex* goal);

	// ななめ判定
	void SlantingSarch(TileIndex* sarch_base, int sarch_index_x, int sarch_index_y);

	// タイルから座標ゲット
	D3DXVECTOR2* GetOpenListPos(TileIndex* index){ return &m_apTile[index->y_block][index->x_block].centerPos; };

	// 座標からタイルインデックスゲット
	TileIndex GetOpenListIndex(D3DXVECTOR3* position);

	// いける場所か判定関数
	bool JudgeCanMove(TileIndex* sarch_tile);

	// AIの入力の初期化
	void InitAIInput(void);

	// AIの状態変更
	void ChangeAIState(AI_STATE state);

	CNaviTile*	m_pDefaultNaviTile;
	NAVI_TILE**	m_apTile;		// 経路保存用
	int			m_nXBlockNum;	// m_apTile[m_nYBlockNum][m_nXBlockNum]
	int			m_nYBlockNum;
	CPlayer*	m_pOwner;		// プレイヤーアドレス
	D3DXVECTOR2 m_vDestPos;		// 目標地点
	TileIndex	m_DestTile;		// 目標タイル（現在の目標）
	TileIndex	m_GoalTile;		// ゴールタイル（最終目標）
	AI_STATE	m_AIState;		// AIの状態
	AI_STATE	m_AIStateOld;	// AIの状態
	AI_INPUT	m_AIInput;		// AIの入力
	AI_INPUT	m_AIInputOld;	// AIの入力
	int			m_nTimer;		// 行動管理用タイマー
	int			m_nSarchTimer;	// 経路探索してからの時間
	CTreasure*	m_pTreasure;	// 宝物
	CGoal*		m_pGoal;		// ゴール
	CPlayer*	m_pAimPlayer;	// 狙っているプレイヤー

#ifdef _DEBUG
	void Printstate(void);
#endif
};


#endif
//----EOF-----