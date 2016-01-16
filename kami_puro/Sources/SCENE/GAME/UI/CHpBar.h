//=============================================================================
//
// CHpBarクラス [CHpBar.h]
// Author : 塚本俊彦
//
// 【概要】
// 内部クラスにCBarBaseってのがあって、それで４つのバーを生成。
// 左右にの緑/赤のバーをつくっている。
// 
// 【使い方】
// 左側のバー（1PのHP）を増減するなら→AddLeft、SubLeft
// 右側のバー（2PのHP）を増減するなら→AddRight、SubRight
// を呼ぶ。引数はどっちも正の整数
// プラスとマイナスでいろいろ変わるから分けた。文句言わないで。
//
// 【変更できる数値】
// ・テクスチャ
// ・赤いバーを減らすまでの間隔
// ・バーの速度
// ・バーの色（あか、みどり）
//
//=============================================================================
#ifndef _CHPBAR_H_
#define _CHPBAR_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"
//#include "CCutIn2D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CCutIn2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CHpBar
{
public:
	CHpBar(LPDIRECT3DDEVICE9 *pDevice);
	~CHpBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 作成
	// Y座標、左側バーの左右のX座標、右側バーの左右X座標
	static CHpBar *Create(
		float height,
		float posCenterY,
		float posLeftBarLeftX,
		float posLeftBarRightX,
		float posRightBarLeftX,
		float posRightBarRightX,
		LPDIRECT3DDEVICE9 *pDevice);

	// 左サイドのHPを引数の値で加算
	void AddLeft(float value);
	// 左サイドのHPを引数の値で減算
	void SubLeft(float value);
	// 右サイドのHPを引数の値で加算
	void AddRight(float value);
	// 右サイドのHPを引数の値で減算
	void SubRight(float value);

	// 開始アニメーションをする　引数↓
	// 終了するまでのカウント(何フレームアニメーションするか)
	void StartAnimation(int endCount);

	// 表情
	enum Expression{
		// 良い表情
		EXPRESSION_GOOD,
		// ふつうな表情
		EXPRESSION_NORAML,
		// ダメな表情
		EXPRESSION_BAD,
		// 攻撃されている表情
		EXPRESSION_ATTACKED,
		// MAX
		EXPRESSION_MAX
	};

	void SetImvisible(void);
	void SetVisible(void);

	// 左側の火が
	// trueで出る　falseで消える
	void FireLeft(bool isAppear);

	// 右側の火が
	// trueで出る　falseで消える
	void FireRight(bool isAppear);

	// 現在のHPを返すJUDGE用
	float GetHPLeft(){ return m_pBar[BAR_GREEN_L].m_Value; }
	float GetHPRight(){ return m_pBar[BAR_GREEN_R].m_Value; }

	// HPゲージの最小から最大の2D座標
	float GetPosHpCenter(int playerNum);

private:
	enum BarInfo{
		BAR_RED_L,
		BAR_RED_R,
		BAR_GREEN_L,
		BAR_GREEN_R,
		BAR_MAX
	};
	// これで４つのバーを作る
	class CBarBase{
	public:
		// 現在の値
		float m_Value;
		// 左側の座標
		float m_PosLeft;
		// 右側の座標
		float m_PosRight;
		// 補間する際のstart値
		float m_PosEasingStart;
		// 補間する際のend値
		float m_PosEasingEnd;
		// 補間するときに使うタイマ（0~1）
		float m_TimerEasing;
		// バーを表示するポリゴン
		CCutIn2D *m_p2D;
		void SetImvisible(void);
		void SetVisible(void);
	};
	// じじいのテクスチャの一コマのサイズ
	static const float JIJII_TEX_U;
	static const float JIJII_TEX_V;
	struct FaceBace{
		// 座標
		D3DXVECTOR2 m_Pos;
		// 顔の2D
		CScene2D *m_pFace2D;
		CScene2D *m_pBack;
		// 表情
		Expression m_Expression;
		// テクスチャ座標
		UV_INDEX m_UV;

		// 現在の自分の表情をテクスチャにセットする
		void SetUV(){
			m_UV.left = JIJII_TEX_U * m_Expression;
			m_UV.right = JIJII_TEX_U * (m_Expression + 1);
			m_pFace2D->SetUV(m_UV.left, m_UV.right);
		}
		void SetImvisible(void){
			m_pFace2D->SetDrawFlag(false);
			m_pBack->SetDrawFlag(false);
		};
		void SetVisible(void){
			m_pFace2D->SetDrawFlag(true);
			m_pBack->SetDrawFlag(true);
		};
	};

	// 初期化
	void Init(
		float height,
		float posCenterY,
		float posLeftBarLeftX,
		float posLeftBarRightX,
		float posRightBarLeftX,
		float posRightBarRightX);

	// 各値の初期化　開始アニメションの後で呼ぶ
	void Init();

	//-------------------------------------
	// バーのインスタンス
	CBarBase m_pBar[BAR_MAX];
	// 現在のバーの値最大値
	float m_ValueMax;
	// 1value当たりのピクセル
	float m_WidthOneValue;
	// ダメージを受けてからのカウント　赤いバーに使う
	int m_RedResetCountLeft;
	int m_RedResetCountRight;
	// 赤いバーを変更するかフラグ
	bool m_isRedResetLeft;
	bool m_isRedResetRight;
	// 赤いバーの線形補間をするフラグ 更新を分けたかった
	bool m_isRedEasingLeft;
	bool m_isRedEasingRight;

	//-------------------------------------
	// 枠
	CScene2D *m_pFrameLeftBar;
	CScene2D *m_pFrameLeft;
	CScene2D *m_pFrameRightBar;
	CScene2D *m_pFrameRight;

	//-------------------------------------
	// じじいの顔
	FaceBace m_FaceLeft;
	FaceBace m_FaceRight;
	// 開始アニメ1フレームで変更するアルファ値
	float m_AnimeOneFrameAlpha;
	// 開始アニメーション用のカラー
	D3DXCOLOR m_Anime2DColor;
	// 開始アニメーション用のカラーじじいの背景用
	D3DXCOLOR m_Anime2DColorJijiiLeft;
	D3DXCOLOR m_Anime2DColorJijiiRight;
	// 現在のHPから表情を変更する。UVもセットする
	void JudgeExpressionLeft();
	void JudgeExpressionRight();

	//-------------------------------------
	// 震わす系
	void UpdateShake();
	void ShakeLeft();
	void ShakeRight();
	// 震えているか
	bool m_isShakeLeft;
	bool m_isShakeRight;
	// カウント
	int m_ShakeCountLeft;
	int m_ShakeCountRight;
	// Y座標を保存した
	float m_PosCenterY;
	// Y座標をずらす差分
	float m_ShakePosYLeft;
	float m_ShakePosYRight;
	// Y座標の動かす幅
	float m_ShakeRangeLeft;
	float m_ShakeRangeRight;

	//-------------------------------------
	void UpdateWhite();
	// 白くするなら
	bool m_isChangeWhiteLeft;
	bool m_isChangeWhiteRight;
	// 加算中なのなんなの
	bool m_isAddWhiteLeft;
	bool m_isAddWhiteRight;
	// 白くする判定
	void WhiteJudge();

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
	// 開始アニメーション時の補間のタイマ
	float m_AnimeTimerEasing;

	//-------------------------------------
	// 炎だよ
	CScene2D *m_pFireLeftBack;
	CScene2D *m_pFireRightBack;
	// フェードして出るよ
	D3DXCOLOR m_FireColorRight;
	D3DXCOLOR m_FireColorLeft;
	// 炎のテクスチャ座標
	float m_FireTextureLeft;
	float m_FireTextureRight;
	// テクスチャを更新するカウント
	int m_FireTextureCountLeft;
	int m_FireTextureCountRight;
	// アニメ更新するの？
	bool m_isFireLeft;
	bool m_isFireRight;
	// 炎を出すの？
	bool m_isFireAppearLeft;
	bool m_isFireAppearRight;
	bool m_isFireDisappearLeft;
	bool m_isFireDisappearRight;
	// 炎をフェードインする
	void FireAppearLeft();
	void FireAppearRight();
	// 炎をフェードアウトする
	void FireDisappearLeft();
	void FireDisappearRight();
	// 更新まとめ
	void UpdateFire();

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----