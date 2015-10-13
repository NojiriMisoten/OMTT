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
#include "../../TEXTURE/CTexture.h"
#include "../PAUSE/CPause.h"
#include "CTimeManager.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CTimeManager;
class CTreasureManager;
class CGoalManager;
class CPlayerManager;
class CAttackManager;
class CThreadManager;
class CJudgeManager;
class CFieldManager;
class CBackGroundManager;
class CCountDown;
class CEffectManager;
class CWinDraw;

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

	// ゲーム終了セット
	void SetGameOver(){
		m_bGameOver = true;
	}
	
	//======================================================
	// こいつらはゲーム中のリザルトシーンに
	// 移るときに一回のみ呼ばれる Init的な。
	//======================================================
	// 勝ったプレイヤ番号をセット
	void SetWinPlayer(short num);
	// 引き分けにする
	void SetDraw();

	// 非公開メンバ
private:
	// ゲーム中のリザルトシーンの更新 毎フレーム呼ばれる
	void Result();

	// ポーズ
	CPause *m_pPause;
	// ゲームのタイム管理者
	CTimeManager *m_pTimeManager;
	// 宝物管理者
	CTreasureManager *m_pTreasureManager;
	// ゴール管理者
	CGoalManager *m_pGoalManager;
	// プレイヤマネージャ
	CPlayerManager *m_pPlayerManager;
	// 攻撃マネージャ
	CAttackManager * m_pAttackManager;
	// 糸マネージャ
	CThreadManager * m_pThreadManager;
	// ジャッジマネージャー
	CJudgeManager* m_pJudgeManager;
	// フィールドマネージャー
	CFieldManager* m_pFieldManager;
	// 背景マネージャー
	CBackGroundManager *m_pBackGroundManager;
	// エフェクトマネージャー
	CEffectManager *m_pEffectManager;
	// カウントダウン
	CCountDown* m_pCountDown;
	// 勝ち引き分けロゴ
	CWinDraw *m_pWinDrawLogo;

	// ゲーム開始フラグ(プレイヤを操作できるフラグ)
	bool m_bPlayerControl;
	// ゲームクリアフラグ
	bool m_bGameOver;
	// 勝ったプレイヤー番号
	short m_nWinPlayerNum;
	// 一回だけBGMを鳴らすためのフラグ 一回通ったらtrue
	bool m_bPlaySoundOnece;

	// ゲーム上の簡易リザルトを出してからシーン遷移するまでのカウント
	int m_nResultCount;

	// 生成するプレイヤ数
	short m_nPlayerNumManual;
	short m_nPlayerNumCpu;

	// 生成するステージ番号
	int m_nStageNum;
};

#endif
//----EOF----