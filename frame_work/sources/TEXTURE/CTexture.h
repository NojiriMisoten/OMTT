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
	TEXTURE_NULL = 0,			// なし
	TEXTURE_BG_0,
	TEXTURE_BG_1,
	TEXTURE_FIRE_0,
	TEXTURE_FIRE_1,
	TEXTURE_THREAD,
	TEXTURE_PLAYER_YELLOW,
	TEXTURE_PLAYER_RED,
	TEXTURE_PLAYER_BLUE,
	TEXTURE_PLAYER_GREEN,
	TEXTURE_NUMBER_BLACK,
	TEXTURE_NUMBER_WHITE,
	TEXTURE_NUMBER_GREEN,
	TEXTURE_TREASURE,
	TEXTURE_TREASURE_ICON,
	TEXTURE_BG_TITLE,
	TEXTURE_BG_TUTORIAL,
	TEXTURE_BG_RESULT,
	TEXTURE_BG_CHARA_SELECT,
	TEXTURE_BG_STAGE_SELECT,
	TEXTURE_STAGE_1,
	TEXTURE_STAGE_2,
	TEXTURE_STAGE_3,
	TEXTURE_STAGE_4,
	TEXTURE_STAGE_5,
	TEXTURE_STAGE_6,
	TEXTURE_STAGE_7,
	TEXTURE_STAGE_8,
	TEXTURE_WIN_LOGO,
	TEXTURE_DRAW_LOGO,
	TEXTURE_FIELD_01,	// 浮島
	TEXTURE_FIELD_02,	// 宝おいてるやつ
	TEXTURE_FIELD_03,	// スタート兼ゴール
	TEXTURE_SPAWN_YELLOW,
	TEXTURE_SPAWN_RED,
	TEXTURE_SPAWN_BLUE,
	TEXTURE_SPAWN_GREEN,
	TEXTURE_ATTACK_THUNDERBOLT,	// 電撃
	TEXTURE_HIT_SMALL,
	TEXTURE_HIT_BIG,
	TEXTURE_ATTACK_AROUND,
	TEXTURE_ATTACK_TACKLE,
	TEXTURE_SLOW,
	TEXTURE_PLAYER_ETAMORPHOSE_YELLOW,
	TEXTURE_PLAYER_ETAMORPHOSE_RED,
	TEXTURE_PLAYER_ETAMORPHOSE_BLUE,
	TEXTURE_PLAYER_ETAMORPHOSE_GREEN,
	TEXTURE_TREASURE_KIRAKIRA,
	TEXTURE_TREASURE_LIGHT,
	TEXTURE_TITLE_LOGO,
	TEXTURE_TITLE_PUSHSTART,
	TEXTURE_TITLE_MENU,
	TEXTURE_TITLE_CURSOL,
	TEXTURE_OPTION_MENU,			// オプションメニュー
	TEXTURE_OPTION_BG,				// オプションBG
	TEXTURE_CURSOL,					// カーソル
	TEXTURE_RETURN,					// 戻る
	TEXTURE_PLAYER_NUM,				// プレイヤー番号
	TEXTURE_RESULT_CIRCLE_LIGHT,	// RESULTのくるくるライト
	TEXTURE_RESULT_LIGHT,			// RESULTの勝者にあてるライト
	TEXTURE_RESULT_PUSH,			// RESULT_PUSH
	TEXTURE_RESULT_MENU_BACK_0,		// RESULT_MENU 背景
	TEXTURE_RESULT_MENU_BACK_1,		// RESULT_MENU 背景
	TEXTURE_RESULT_MENU_BACK_2,		// RESULT_MENU 背景
	TEXTURE_RESULT_MENU_MOJI_0,		// RESULT_MENU もう一度
	TEXTURE_RESULT_MENU_MOJI_1,		// RESULT_MENU キャラ選択へ
	TEXTURE_RESULT_MENU_MOJI_2,		// RESULT_MENU ステージ選択へ
	TEXTURE_CHARA_SELECT_PLAYER_BG,	// キャラクタ選択のプレイヤ背景
	TEXTURE_CHARA_SELECT_JOIN,		// キャラクタ選択の参戦2D
	TEXTURE_CHARA_SELECT_START,		// キャラクタ選択のスタートボタン
	TEXTURE_CHARA_SELECT_CONFIG,	// キャラクタ選択のコンフィグボタン
	TEXTURE_CHARA_SELECT_CURSOL,	// キャラクタ選択のカーソル
	TEXTURE_CHARA_SELECT_READY,		// キャラクタ選択の準備完了
	TEXTURE_CHARA_SELECT_LOGO,		// キャラクタ選択のロゴ
	TEXTURE_STAGE_SELECT_LOGO,		// ステージ選択のロゴ
	TEXTURE_RESULT_LOGO,			// リザルトのロゴ
	TEXTURE_TUTORIAL_0,				// チュートリアルの絵
	TEXTURE_TUTORIAL_1,				// チュートリアルの絵
	TEXTURE_TUTORIAL_2,				// チュートリアルの絵
	TEXTURE_TUTORIAL_3,				// チュートリアルの絵
	TEXTURE_TUTORIAL_ARROW,			// チュートリアルの矢印
	TEXTURE_TUTORIAL_PAGE_1_4,		// チュートリアルのページテクスチャ
	TEXTURE_TUTORIAL_PAGE_2_4,		// チュートリアルのページテクスチャ
	TEXTURE_TUTORIAL_PAGE_3_4,		// チュートリアルのページテクスチャ
	TEXTURE_TUTORIAL_PAGE_4_4,		// チュートリアルのページテクスチャ
	TEXTURE_GAME_START,				// ゲームスタートのカウントテクスチャ（4,1）
	TEXTURE_THREAD_POISON,			// 妨害形態の攻撃(緑の糸)
	TEXTURE_THREAD_DROP,			// 通常形態の糸攻撃
	TEXTURE_TRAP,					// 罠のテクスチャ
	TEXTURE_MAX				// テクスチャタイプ総数
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
	static void Uninit(void);

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

	//============================================================
	// テクスチャタイプゲッター
	// 引数:テクスチャのファイルパス
	// 戻り値:テクスチャタイプ
	//============================================================
	static TEXTURE_TYPE GetTextureTye(char* texFilePath);

private:
	//============================================================
	// 初期化
	// 引数:デバイス
	// 戻り値:成否
	//============================================================
	static	HRESULT Init(LPDIRECT3DDEVICE9 *pDevice);

	static	LPDIRECT3DTEXTURE9 m_pD3DTex[TEXTURE_MAX];				// テクスチャポインタ
	static	bool m_bDoneCreateFlag;									// すでに作ったかのフラグ
};

#endif
//----EOF----