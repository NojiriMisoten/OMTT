//=============================================================================
//
// CEffect�N���X [CEffect.h]
// Author : �˖{�@�r�F
//
//=============================================================================
#ifndef _CEFFECTATTACKHIT_H_
#define _CEFFECTATTACKHIT_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../GAME/EFFECT/CEffectBase.h"

//=============================================================================
// �萔
//=============================================================================

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffectAttackHit : public CEffectBase
{
	// ���J�����o
public:

	CEffectAttackHit(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectAttackHit(void);

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectAttackHit *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// ����J�����o
private:

};

#endif
//----EOF----