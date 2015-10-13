//=============================================================================
//
// CEffectクラス [CEffect.h]
// Author : 塚本　俊彦
//
//=============================================================================
#ifndef _CEFFECT_METAMORPHOSE_H_
#define _CEFFECT_METAMORPHOSE_H_

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
class CEffectMetamorphose : public CEffectBase
{
	// 公開メンバ
public:

	CEffectMetamorphose(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectMetamorphose(void);

	void Init(D3DXVECTOR3 pos, short playerNum);
	void Uninit(void);
	void Update(void);

	// クリエイト関数
	static CEffectMetamorphose *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		short playerNum);

	// 非公開メンバ
private:
	TEXTURE_TYPE m_Texture;
};

#endif
//----EOF----