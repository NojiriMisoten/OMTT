//=============================================================================
//
// CStaminaBarクラス [CStaminaBar.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CCOUNTTIME_H_
#define _CCOUNTTIME_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCountTime
{
public:

	CCountTime(LPDIRECT3DDEVICE9 *pDevice);
	~CCountTime(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 作成
	// カウントの中心座標
	static CCountTime* Create(
		D3DXVECTOR2 &pos, int time,
		LPDIRECT3DDEVICE9 *pDevice);

private:
	// 初期化
	void Init(D3DXVECTOR2 &pos, int time);

	// 時間を二つの2Dのテクスチャに反映させる
	void Set(int time);

	// タイム
	int m_Time;

	// 時間を図るためのカウント
	int m_TimeCount;

	// 一の位の桁
	CScene2D *m_pFigure1st;
	// 十の位の桁
	CScene2D *m_pFigure2nd;

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----