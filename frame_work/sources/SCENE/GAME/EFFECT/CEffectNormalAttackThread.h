//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CEFFECT_NORMAL_ATTACK_THREAD_H__
#define __CEFFECT_NORMAL_ATTACK_THREAD_H__

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
class CEffectNormalAttackThread : public CEffectBase
{
	// 公開メンバ
public:

	CEffectNormalAttackThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectNormalAttackThread(void);

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);

	// クリエイト関数
	static CEffectNormalAttackThread *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// 非公開メンバ
private:
	CSceneAnime* m_pAnim;
};

#endif // __EXPLOSION_H__