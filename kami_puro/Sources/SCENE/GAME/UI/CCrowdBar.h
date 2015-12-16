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
#include "../../../BASE_OBJECT/CScene2D.h"

// TODO 仮の量
static const float CROWD_MAX = 500;

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
	void Add( int value );

	void SetAmount( float value ) { m_Value = value; };

	float GetAmount( void ) { return m_Value; };

	// 開始アニメーションをする　引数↓
	// 終了するまでのカウント(何フレームアニメーションするか)
	void StartAnimation(int endCount);

	void SetImvisible(void);
	void SetVisible(void);
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

	//-------------------------------------
	// 二つのバー
	CScene2D *m_pBarLeft;
	CScene2D *m_pBarRight;
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

	//-------------------------------------
	// 枠
	CScene2D *m_pFrame;
	// 観客たちの絵
	CScene2D *m_pCrowd;
	UV_INDEX m_CrowdUV;
	// 観客たちのアニメーション
	void UpdateCrowdAnimation();
	// テクスチャを変更するカウント
	int m_CrowdAnimeCount;
	// ＵＶを変更する（右にずれてループする）
	void CrowdChangeUV();

	//-------------------------------------
	// バチバチ
	CScene2D *m_pSpark;
	CScene2D *m_pSpark2;
	// 現在の角度
	float m_SparkRot;
	// 角度を変更するためのカウント
	int m_SparkCountRot;
	// アニメーション用フラグ
	bool m_isSparkAdd;
	// 座標
	D3DXVECTOR3 m_SparkPos;
	// アニメーション更新
	void UpdateSparkAnimation();
	// テクスチャ座標
	UV_INDEX m_SparkUV;
	// テクスチャ座標を変更するためのカウント
	int m_SparkCountUV;

	//-------------------------------------
	// 定期的に震える系
	// 補間用の目的座標
	float m_ShakePosStart;
	float m_ShakePosEnd;
	// 震える座標を決定するカウント
	int m_ShakeCount;
	// 震える座標の補間用のタイマ
	float m_ShakeTime;

	//-------------------------------------
	// アニメーション更新
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
	// 開始アニメ1フレームで変更するアルファ値
	float m_AnimeOneFrameAlpha;
	D3DXCOLOR m_Anime2DColor;
	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----