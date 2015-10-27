//=============================================================================
//
// CStaminaBarクラス [CStaminaBar.h]
// Author : 塚本俊彦
//
//
// 【概要】
// 二つバーを生成して、ポリゴンの頂点を左右に移動させている
// 
// 【使い方】
// Addに＋－色々値を入れると観客バーが左右に移動する
// 
// 【変更できる数値】
// ・テクスチャ
// ・バーの速度（初期速度、その加速度）
// ・バーの色（右の1P側、左の2P側）
// 
//=============================================================================
#ifndef _CCROWNDBAR_H_
#define _CCROWNDBAR_H_
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
class CCrowdBar
{
public:

	CCrowdBar(LPDIRECT3DDEVICE9 *pDevice);
	~CCrowdBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 作成
	// 高さ、Y座標、左端の座標、右端の座標、デバイス
	static CCrowdBar *Create(
		float height,
		float posCenterY,
		float posLeft,
		float posRight,
		LPDIRECT3DDEVICE9 *pDevice);

	// バーを増減させる
	void Add(float value);

	// 開始アニメーションをする　引数↓
	// 終了するまでのカウント(何フレームアニメーションするか)
	void StartAnimation(int endCount);

private:
	// 初期化
	void Init(float height,
		float posCenterY,
		float posLeft,
		float posRight,
		LPDIRECT3DDEVICE9 *pDevice);

	void Init();

	// バーを動かす更新
	void UpdateBarMove();

	// 左端と右端の座標（動かない）
	float m_PosLeftX;
	float m_PosRightX;
	// 真ん中の座標（動かない）
	float m_PosCenterX;
	// セットする座標のDest
	float m_PosDestX;
	// 現在の境目の座標
	float m_PosCurrentX;
	// セットする座標の移動量
	float m_PosVel;
	// セットする座標があるときフラグ
	bool m_isPosMove;

	// 現在のバーの値
	float m_Value;
	// 現在のバーの値最大値
	float m_ValueMax;
	// バーの値（m_Value）１当たりのピクセル数
	float m_WidthOneValue;

	// 二つのバー
	CScene2D *m_pBarLeft;
	CScene2D *m_pBarRight;

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;

	//-------------------------------------
	// アニメーション用
	void UpdateAnime();
	// 開始アニメをするためのカウント
	int m_AnimeCount;
	// 開始アニメの終わり　フレーム数
	int m_AnimeCountMax;
	// 開始アニメしているフラグ
	bool m_isAnime;
	// 開始アニメーション用の保管タイム
	float m_AnimeEasingOneFrame;
	float m_AnimeEasingTimer;

};

#endif
//----EOF----