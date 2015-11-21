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
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CUiManager
{
public:

	CUiManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CUiManager(void);

	void Update();
	void Uninit();

	// 初期化
	void Init(CGame *pGame);

	// ゲーム開始のアニメーションをする関数
	// 終了するカウントを入れる
	void StartAnimation(int interval);

	// 作成
	//static CUiManager *Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame *pGame);


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

private:
	// 制限時間
	CCountTime *m_pTimer;
	// 観客ゲージ
	CCrowdBar *m_pCrowdBar;
	// HPバ－
	CHpBar *m_pHpBar;
	// スタミナバー 今つかってない
	CStaminaBar *m_pStaminaBarL;
	CStaminaBar *m_pStaminaBarR;
	// カットイン
	CCutIn *m_pCutIn;

	// デバイスの保存
	LPDIRECT3DDEVICE9 *m_pDevice;
	// マネージャー
	CManager *m_pManager;
	CGame	*m_pGame;
	CCommandChartManager* m_pCommandChartManager;
};

#endif
//----EOF----