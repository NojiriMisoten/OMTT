//=============================================================================
//
// CEffectManagerクラス [CEffectManager.h]
// Author : 塚本　俊彦
//
// 宝物マネージャー　こいつを介して宝物を生成する
//
//=============================================================================
#ifndef _CEFFECTMANAGER_H_
#define _CEFFECTMANAGER_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"

//=============================================================================
// 定数
//=============================================================================
enum EFFECT_TYPE{
	EFFECT_ATTACK_HIT = 0,
	EFFECT_EXPLOSION,				// 爆発
	EFFECT_SPECIAL_ATTACK_ATTACK,	// 攻撃特化攻撃○
	EFFECT_SPECIAL_ATTACK_SPEED,	// 移動特化攻撃○
	EFFECT_SPECIAL_THREAD_ATTACK,	// 攻撃特化糸攻撃○
	EFFECT_CAUGHT_TRAP,				// 罠にかかったエフェクト○
	EFFECT_CREATE_ROAD,				// 道作成時のエフェクト○
	EFFECT_FLAG_GET,				// フラグ取得エフェクト○
	EFFECT_FLAG_HOLD,				// フラグ所持中エフェクト○
	EFFECT_PLAYER_DEAD,				// プレイヤー体力０エフェクト
	EFFECT_MP_ATTACK,				// MPダメージエフェクト○
	EFFECT_NORMAL_ATTACK_CAP,		// 帽子攻撃エフェクト○
	EFFECT_NORMAL_ATTACK_THREAD,	// 通常糸発射攻撃○
	EFFECT_PLAYER_REVIAVE,			// プレイヤー復活エフェクト
	EFFECT_TRAP_FOUNDATION,			// 罠設置エフェクト○
	EFFECT_METAMORPHORSE,			// 変形エフェクト

	EFFECT_MAX,
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffectManager
{
	// 公開メンバ
public:
	CEffectManager(LPDIRECT3DDEVICE9 *pDevice);
	~CEffectManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// エフェクト生成関数
	// 座標とタイプを設定
	static void CreateEffect(D3DXVECTOR3 pos, EFFECT_TYPE type, D3DXVECTOR3 velocity);

	// エフェクト生成関数
	// 座標とプレイヤーナンバーを設定
	static void CreateEffectMeta(D3DXVECTOR3 pos, short playerNum);

	// エフェクト生成関数(罠)
	// 座標とプレイヤーナンバーをセット
	static void CreateEffectTrap(D3DXVECTOR3 pos, short playerNum);

	// 非公開メンバ
private:

	// デバイス
	static LPDIRECT3DDEVICE9 *m_pDevice;
};

#endif
//----EOF----