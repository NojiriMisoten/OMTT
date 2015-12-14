//=============================================================================
//
// テクスチャー処理 [CTexture.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// 列挙体
//*****************************************************************************
// テクスチャの種類
typedef enum
{
	TEXTURE_MONO = 0,				// トゥーン用
	TEXTURE_DEFAULT,			// モデルのデフォルトテクスチャ
	TEXTURE_NUMBER,				// 数字
	TEXTURE_TITLE,
	TEXTURE_RESULT,
	TEXTURE_HP_GAGE_G,			// 体力ゲージみどり
	TEXTURE_HP_GAGE_R,			// 体力ゲージ減った時の赤
	TEXTURE_HP_GAGE_FRAME,		// 体力ゲージの枠
	TEXTURE_HP_GAGE_FRAME_TOP,	// 体力ゲージの枠 上半分
	TEXTURE_CROWD_GAGE_FRAME,	// 観客ゲージの枠
	TEXTURE_CROWD_GAGE_HUMAN,	// 観客ゲージ上の人間
	TEXTURE_CROWD_SPARK,		// 観客ゲージ中央のバチバチ
	TEXTURE_JIJII,				// ゲーム画面のじじいのテクスチャ
	TEXTURE_JIJII_BACK,			// ゲーム画面のじじいの背景
	TEXTURE_BLUE,				// リングテクスチャ
	TEXTURE_RED,				// リングテクスチャ
	TEXTURE_METALICGRAY,		// リングテクスチャ
	TEXTURE_WINDOWS,			// リングテクスチャ
	TEXTURE_BUTTON,				// ボタンのテクスチャ
	TEXTURE_ROPE,				// ロープテクスチャ
	TEXTURE_UI_FADE_LINE,		// フェードのライン
	TEXTURE_UI_FADE_BALL_LARGE,	// フェードのボール
	TEXTURE_UI_FADE_BALL_SMALL,	// フェードのボール
	TEXTURE_UI_CUT_IN_0,		// カットイン0
	TEXTURE_UI_CUT_IN_1,		// カットイン1
	TEXTURE_UI_CUT_IN_BACK_1,	// カットイン背景
	TEXTURE_UI_CUT_IN_BACK_2,	// カットイン背景
	TEXTURE_SKILL_NAME_CHOP,	// 技名 チョップ
	TEXTURE_SKILL_NAME_ELBOW,	// 技名 エルボー
	TEXTURE_SKILL_NAME_LARIAT,	// 技名 ラリアット
	TEXTURE_SKILL_NAME_SLAP,	// 技名 ビンタ
	TEXTURE_SKILL_NAME_BACKDROP,// 技名 バックドロップ
	TEXTURE_SKILL_NAME_STUNNER,	// 技名 スタナー
	TEXTURE_SKILL_NAME_ROLLING,	// 技名 ローリングエルボー
	TEXTURE_SKILL_NAME_SHOULDER,// 技名 フラインショルダー
	TEXTURE_SKILL_NAME_DROPKICK,// 技名 ドロップキック
	TEXTURE_SKILL_NAME_SMALL_ATTACK,// 技名 小攻撃
	TEXTURE_SKILL_NAME_BIG_ATTACK,	// 技名 大攻撃
	TEXTURE_SKILL_NAME_THROW,		// 技名 投げ
	TEXTURE_SKILL_NAME_ROPE,	// 技名 ロープ
	TEXTURE_SKILL_NAME_FINISH,	// 技名 FINISH
	TEXTURE_UI_TIME_BACK,		// 制限時間の背景
	TEXTURE_COMMAND_CHART_BACK,	// コマンドチャートの背景
	TEXTURE_COMMAND_BACK,		// コマンドの一つ一つの背景
	TEXTURE_BG_TITLE,			// タイトル黒BG
	TEXTURE_LOGO_TITLE,			// タイトルロゴ
	TEXTURE_LOGOANY_TITLE,		// タイトルコンガを押してロゴ
	TEXTURE_BG_SCROLL_TITLE,	// タイトルのスクロール用BG
	TEXTURE_FIRE_TITLE,			// タイトル炎
	TEXTURE_CHAIN,				// くさり
	TEXTURE_READY,				// READY
	TEXTURE_FIGHT,				// FIGHT
	TEXTURE_KO,					// K.O
	TEXTURE_HUMAN,				// 観客のテクスチャ
	TEXTURE_MAX					// テクスチャタイプ総数
}TEXTURE_TYPE;

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CTexture
{
public:
	// コンストラクタ
	CTexture(void){};

	// デストラクタ
	~CTexture(void){};

	// 終了
	static void Uninit( void );

	//============================================================
	// クリエイト
	// 引数:デバイス
	//============================================================
	static void CreateTexture(LPDIRECT3DDEVICE9 *pDevice);

	//============================================================
	// テクスチャゲッター
	// 引数:テクスチャの種類
	// 戻り値:テクスチャのポインタ
	//============================================================
	static LPDIRECT3DTEXTURE9 GetTexture(const TEXTURE_TYPE type);

private:
	//============================================================
	// 初期化
	// 引数:デバイス
	// 戻り値:成否
	//============================================================
	static	HRESULT Init( LPDIRECT3DDEVICE9 *pDevice );

	static	LPDIRECT3DTEXTURE9 m_pD3DTex[TEXTURE_MAX];				// テクスチャポインタ
	static	bool m_bDoneCreateFlag;									// すでに作ったかのフラグ
};

#endif
//----EOF----