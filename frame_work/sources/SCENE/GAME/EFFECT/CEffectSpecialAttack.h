//=============================================================================
//
//	攻撃特化状態の攻撃エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CEFFECT_SPECIAL_ATTACK_H__
#define __CEFFECT_SPECIAL_ATTACK_H__

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
class CEffectSpecialAttack : public CEffectBase
{
	// 公開メンバ
public:
	CEffectSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectSpecialAttack(void);

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);

	// クリエイト関数
	static CEffectSpecialAttack *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// 非公開メンバ
private:

};

#endif // __EXPLOSION_H__