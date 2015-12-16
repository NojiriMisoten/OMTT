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
class CManager;
class CGame;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCountTime
{
public:

	CCountTime(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame* pGame);
	~CCountTime(void);

	void Update();
	void Uninit();

	// 作成
	// カウントの中心座標
	static CCountTime* Create(
		D3DXVECTOR2 &pos, int time,
		LPDIRECT3DDEVICE9 *pDevice, CManager *pManager, CGame* pGame);

	// 開始アニメーションをする　引数↓
	// 終了するまでのカウント(何フレームアニメーションするか)
	void StartAnimation(int endCount);

	// 鎖のアニメ―ションうごかすやつ
	void ChainAnimeStart();
	void ChainAnimeStop();

	void SetImvisible(void);
	void SetVisible(void);
private:
	// 初期化
	void Init(D3DXVECTOR2 &pos, int time);
	// 時間をカウントする更新
	void UpdateTime();
	// 開始アニメーションをする更新
	void UpdateAnime();
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

	// 開始アニメをするためのカウント
	int m_AnimeCount;
	// 開始アニメの終わり　フレーム数
	int m_AnimeCountMax;
	// 開始アニメしているフラグ
	bool m_isAnime;
	// 開始アニメ1フレームで変更するアルファ値
	float m_AnimeOneFrameAlpha;
	D3DXCOLOR m_Anime2DColor;

	// 背景
	CScene2D *m_pBack;

	//-----------------------------------グレー系
	// 止めているときに出すグレー
	CScene2D *m_pGray;
	//-----------------------------------拡縮系
	// 拡縮アニメーションの更新
	void UpdateScale();
	// 拡縮アニメ―ション 開く
	void GrayScaleOpen();
	// 拡縮アニメ―ション 閉じる
	void GrayScaleClose();
	// 拡縮の大きさ
	float m_GrayWidth;
	float m_GrayWidthDest;
	float m_GrayHeight;
	float m_GrayHeightDest;
	// グレーの座標
	D3DXVECTOR3 m_GrayPos;
	// 拡縮のタイマ
	float m_GrayTime;
	// 拡縮するよ
	bool m_isScale;

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
	// マネージャー
	CManager *m_pManager;
	// ゲーム
	CGame *m_pGame;

};

#endif
//----EOF----