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
class CFace;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CUiManager
{
public:

	CUiManager(LPDIRECT3DDEVICE9 *pDevice);
	~CUiManager(void);

	void Update();
	void Uninit();

	// ゲーム開始のアニメーションをする関数
	void StartAnimation();

	// 作成
	static CUiManager *Create(LPDIRECT3DDEVICE9 *pDevice);

private:
	// 初期化
	void Init();

	// 制限時間
	CCountTime *m_pTimer;
	// スタミナバー
	CStaminaBar *m_pStaminaBarL;
	CStaminaBar *m_pStaminaBarR;
	// 観客ゲージ
	CCrowdBar *m_pCrowdBar;
	// HPバ－
	CHpBar *m_pHpBarL;
	CHpBar *m_pHpBarR;
	// じじいの顔
	CFace *m_pFace;
	// デバイスの保存
	LPDIRECT3DDEVICE9 *m_pDevice;

	bool m_isAnimation;
	int m_AnimationCount;
};

#endif
//----EOF----