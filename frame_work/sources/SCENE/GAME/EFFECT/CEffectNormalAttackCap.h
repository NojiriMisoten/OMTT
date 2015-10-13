//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CEFFECT_NORMAL_ATTACK_CAP_H__
#define __CEFFECT_NORMAL_ATTACK_CAP_H__

//=============================================================================
// インクルード
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../GAME/EFFECT/CEffectBase.h"

//=============================================================================
// 定数
//=============================================================================

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffectNormalAttackCap : public CEffectBase
{
	// 公開メンバ
public:

	CEffectNormalAttackCap(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectNormalAttackCap(void);

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);

	// クリエイト関数
	static CEffectNormalAttackCap *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// 非公開メンバ
private:

};

#endif // __EXPLOSION_H__