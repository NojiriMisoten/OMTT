//=============================================================================
//
// エフェクト本体の格納処理 [CEffectHolder.h]
// Author : 坂本 友希
//
//=============================================================================
#ifndef _CEFFECT_HOLDER_H_
#define _CEFFECT_HOLDER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "CEffectManager.h"

//*****************************************************************************
// 列挙体
//*****************************************************************************
// テクスチャの種類
typedef enum
{
	//足跡(煙)
	EFFECT_FOOTSTEP_SMOKE = 0,
	//足跡(波)
	EFFECT_FOOTSTEP_WAVE,
	//オーラ(開始)
	EFFECT_AURA_START,
	//オーラ(ループ)
	EFFECT_AURA_LOOP,
	//オーラ(終了)
	EFFECT_AURA_END,
	//バウンド時
	EFFECT_BOUND,
	//弱攻撃
	EFFECT_DAGEKI_JAKU,
	//中攻撃
	EFFECT_DAGEKI_TYU,
	//強攻撃
	EFFECT_DAGEKI_KYO,
	//防御時
	EFFECT_DEFENSE,
	//煙(入場時)
	EFFECT_ENTER_SMOKE,
	//ジャンプ時
	EFFECT_JUMP,
	//リングバシコーン
	EFFECT_RING,
	//テスト用,好きに使って!
	EFFECT_TEST1,
	//衝撃波
	EFFECT_SHOCK_WAVE,
	//煙
	EFFECT_SMOKE,
	//テストその2
	EFFECT_TEST2,
	//投げ
	EFFECT_THROW,
	//勝った時のやつ
	EFFECT_WIN,
	// ホーリーランス
	EFFECT_HOLYLANCE,
	// フィールド破壊
	EFFECT_BROKEN_FIELD,
	// チャージエフェクト
	EFFECT_CHARGE,

	EFFECT_MAX					// エフェクト総数
}EFFECT_TYPE;

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CEffectHolder
{
public:
	// コンストラクタ
	CEffectHolder(void){};

	// デストラクタ
	~CEffectHolder(void){};

	// 終了
	static void Uninit(void);

	//============================================================
	// クリエイト
	// 引数:デバイス
	// ※必ずエフェクトマネージャーを生成した後に呼ぶこと!
	//============================================================
	static void CreateEffect(void);

	//============================================================
	// テクスチャゲッター
	// 引数:テクスチャの種類
	// 戻り値:テクスチャのポインタ
	//============================================================
	static ::Effekseer::Effect* GetEffect(const EFFECT_TYPE type);

private:
	//============================================================
	// 初期化
	// 引数:デバイス
	// 戻り値:成否
	//============================================================
	static	HRESULT Init(void);

	//エフェクト本体へのポインタ
	static	::Effekseer::Effect* m_pEffect[EFFECT_MAX];
	// すでに作ったかのフラグ
	static	bool m_bDoneCreateFlag;
};

#endif
//----EOF----