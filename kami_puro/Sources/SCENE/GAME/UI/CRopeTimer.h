//=============================================================================
//
// CRopeTimerクラス [CRopeTimer.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CROPETIMER_H_
#define _CROPETIMER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRopeTimer
{
public:

	CRopeTimer(LPDIRECT3DDEVICE9 *pDevice);
	~CRopeTimer(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 作成
	// 高さ、Y座標、左端の座標、右端の座標、デバイス
	static CRopeTimer *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// ロープタイマーを起動する関数
	// 第１引数 バーを拡縮アニメーションで表示するまでのフレーム数
	// 第２引数 バーを減らし始めてから0になるまでのフレーム数
	void Start(int appearFrame, int reduceFrame);

private:
	// 初期化
	void Init();
	// バーを動かす更新
	void UpdateBarMove();
	// バーを出現するアニメーション更新
	void UpdateAppearAnime();

	// 出現アニメーション用
	struct AnimeData
	{
		float width;
		float height;
		AnimeData(){};
		AnimeData(float w, float h)
		{
			width = w;
			height = h;
		}
	};

	// バーの出現アニメーション
	bool m_isAppearAnime;
	// バーを動かす更新
	bool m_isMoveBar;
	// バー
	CScene2D *m_pBar;
	// 枠
	CScene2D *m_pFrame;
	// 端っこの座標
	float m_PosLeft;
	float m_PosRight;
	// アニメーション用座標 バー
	AnimeData m_AnimeBarSize;
	AnimeData m_AnimeBarSizeDest;
	// アニメーション用座標
	AnimeData m_AnimeFrameSize;
	AnimeData m_AnimeFrameSizeDest;
	// 補間用のタイム
	float m_Time;
	// カウント
	int m_Count;
	int m_CountMax;
	// 1フレーム当たりに減らす幅
	float m_WidthOneFrame;
	// 1フレーム当たりに増やすタイム
	float m_TimeOneFrame;

//	//-------------------------------------
//	// アニメーション更新
//	void UpdateAnime();
//	// 開始アニメをするためのカウント
//	int m_AnimeCount;
//	// 開始アニメの終わり　フレーム数
//	int m_AnimeCountMax;
//	// 開始アニメしているフラグ
//	bool m_isAnime;
//	// 開始アニメーション用の保管タイム
//	float m_AnimeEasingOneFrame;
//	float m_AnimeEasingTimer;
//	// 開始アニメ1フレームで変更するアルファ値
//	float m_AnimeOneFrameAlpha;
//	D3DXCOLOR m_Anime2DColor;
	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----