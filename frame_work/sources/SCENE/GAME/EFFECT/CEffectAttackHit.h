//=============================================================================
//
// CEffectクラス [CEffect.h]
// Author : 塚本　俊彦
//
//=============================================================================
#ifndef _CEFFECTATTACKHIT_H_
#define _CEFFECTATTACKHIT_H_

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
class CEffectAttackHit : public CEffectBase
{
	// 公開メンバ
public:

	CEffectAttackHit(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectAttackHit(void);

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);

	// クリエイト関数
	static CEffectAttackHit *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// 非公開メンバ
private:

};

#endif
//----EOF----