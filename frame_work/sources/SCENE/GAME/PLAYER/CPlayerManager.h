//=============================================================================
//
// CPlayerManagerクラス [CPlayerManager.h]
// Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CPLAYERMANAGER_H__
#define __CPLAYERMANAGER_H__

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CPlayer;
class CAttackManager;
class CThreadManager;
class CEffectManager;

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
// プレイヤーの最大数
static const short MAXIMUM_NUMBER_OF_PLAYER = 4;

//-----------------------------------------------------------------------------
// プレイヤーマネージャクラス定義
//-----------------------------------------------------------------------------
class CPlayerManager
{
public:
	// コンストラクタ
	CPlayerManager(
		CAttackManager *pAttackManager, 
		CThreadManager *pThreadManager,
		CEffectManager *pEffectManager);

	// デストラクタ
	~CPlayerManager();

	// 初期化
	// CPU人数、プレイヤー人数
	void Init(int nNumPlayer, int nManualPlayer, bool *bPlayerControl);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// プレイヤーポインタのゲッター
	//	引数　　呼び出すプレイヤーをセットしている配列番号
	//	戻り値　プレイヤーのポインタ
	static CPlayer* GetPlayer(int nNumber){ return m_apPlayer[nNumber]; }

	// プレイヤーポインタのセッター
	//	引数　　プレイヤーポインタ、セットする先の配列番号
	void SetPlayer(CPlayer* pPlayer, int nNumber){ m_apPlayer[nNumber] = pPlayer; }

private:
	// プレイヤーのポインタ格納用配列
	static CPlayer* m_apPlayer[MAXIMUM_NUMBER_OF_PLAYER];
	// テスト用のタイマー
	int	m_nTimer;

	// 攻撃時に必要なマネージャー
	CAttackManager *m_pAttackManager;
	// 糸出す時に必要なマネージャー
	CThreadManager *m_pThreadManager;
	// エフェクトに必要なマネージャー
	CEffectManager *m_pEffectManager;

};

#endif //__CPLAYERMANAGER_H__

// EOF