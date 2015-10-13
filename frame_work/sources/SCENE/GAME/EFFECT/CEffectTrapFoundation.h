//=============================================================================
//
// CEffectクラス [CEffect.h]
// Author : 塚本　俊彦
//
//=============================================================================
#ifndef _CEFFECT_TRAP_FOUNDATION_H_
#define _CEFFECT_TRAP_FOUNDATION_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../GAME/EFFECT/CEffectBase.h"

//=============================================================================
// 定数
//=============================================================================

class CSceneAnime;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffectTrapFoundation : public CEffectBase
{
	// 公開メンバ
public:

	CEffectTrapFoundation(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectTrapFoundation(void);

	void Init(D3DXVECTOR3 pos , short playerNum);
	void Uninit(void);
	void Update(void);

	// クリエイト関数
	static CEffectTrapFoundation *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		short playerNum);

	// 非公開メンバ
private:
	CSceneAnime* m_pAnim;
};

#endif
//----EOF----