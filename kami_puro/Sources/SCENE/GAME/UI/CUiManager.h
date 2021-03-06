//=============================================================================
//
// CUiManagerクラス [CUiManager.h]
// Author : 塚本俊彦
//
// ゲームシーン内でのUIをまとめた管理関数
//
//
//=============================================================================
#ifndef _CUIMANAGER_H_
#define _CUIMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "CCutIn.h"
#include "COverLay.h"
#include "CRopeTimer.h"
#include "CCountTime.h"
//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CStaminaBar;
class CCountTime;
class CCrowdBar;
class CHpBar;
class CManager;
class CGame;
class CCommandChartManager;
class CCutIn;
class CBattleFade;
class COverLay;
class CForcusLine;
class CCamera;
class CCameraManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CUiManager
{
public:

	CUiManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CCameraManager* pCamera);
	~CUiManager(void);

	void Update();
	void Uninit();

	// 初期化
	void Init(CGame *pGame);

	// ゲーム開始のアニメーションをする関数
	// 終了するカウントを入れる
	void StartAnimation(int interval);

	// カットイン開始
	void StartCutIn(int ID, CutInType type) { m_pCutIn->Start(ID, type); };

	// 作成
	//static CUiManager *Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame *pGame);

	CCommandChartManager *GetCommandChartManager(void) { return m_pCommandChartManager; };

	// HPバー
	CHpBar *GetHpBar()
	{
		return m_pHpBar;
	}
	// 観客バー
	CCrowdBar *GetCrowdBar()
	{
		return m_pCrowdBar;
	}

	CGame* GetGame(void)
	{
		return m_pGame;
	}

	void StartRopeTimer( int openFrameCount, int timerFrameCount ) { m_pRopeTimer->Start( openFrameCount, timerFrameCount ); };

	// バトルフェードスタート
	void StartBattleFade(void);

	// オーバーレイスタート
	void StartOverLay(COverLay::Data *data);

	// 集中線を出す
	// 第1引数：表示するフレーム数
	// 第2引数：trueなら派手なテクスチャになる falseなら白黒
	void StartForcusLine(int timerFrameCount, bool isFinish);

	void SetImvisible(void);
	void SetVisible(void);

	// 鎖のアニメ―ションうごかすやつ
	void ChainAnimeStart(void) { m_pTimer->ChainAnimeStart(); };
	void ChainAnimeStop(void) { m_pTimer->ChainAnimeStop(); };

	// プレイヤがジャンプしたときに出すエフェクト
	void CreateJumpEffect(D3DXVECTOR3& pos, TEXTURE_TYPE texture, int playerNum);

	// ジャンプしたときのエフェクト用ゲッタ
	// 歓声ゲージの最小から最大の2D座標
	float GetPosCrowdCenter(int playerNum);
	// HPゲージの最小から最大の2D座標
	float GetPosHpCenter(int playerNum);


private:
	// じじいを燃やす管理の更新
	void UpdateFireOldMan();

	// 制限時間
	CCountTime *m_pTimer;
	// 観客ゲージ
	CCrowdBar *m_pCrowdBar;
	// HPバ−
	CHpBar *m_pHpBar;
	// スタミナバー 今つかってない
	CStaminaBar *m_pStaminaBarL;
	CStaminaBar *m_pStaminaBarR;
	// カットイン
	CCutIn *m_pCutIn;
	// バトルフェード
	CBattleFade *m_pBattleFade;
	// READYとか表示するよう
	COverLay *m_pOverLay;
	// ロープタイマー
	CRopeTimer *m_pRopeTimer;
	// 集中線
	CForcusLine *m_pForcusLine;

	// デバイスの保存
	LPDIRECT3DDEVICE9 *m_pDevice;
	// マネージャー
	CManager *m_pManager;
	CGame	*m_pGame;
	CCommandChartManager* m_pCommandChartManager;
	CCameraManager *m_pCamera;
};

#endif
//----EOF----