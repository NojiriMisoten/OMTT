//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CEFFECT_CAUGHT_TRAP_H__
#define __CEFFECT_CAUGHT_TRAP_H__

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
class CEffectCaughtTrap : public CEffectBase
{
	// 公開メンバ
public:

	CEffectCaughtTrap(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectCaughtTrap(void);

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);

	// クリエイト関数
	static CEffectCaughtTrap *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// 非公開メンバ
private:

};

#endif // __EXPLOSION_H__