//=============================================================================
//
// CEffect�N���X [CEffect.h]
// Author : �˖{�@�r�F
//
//=============================================================================
#ifndef _CEFFECT_FLAG_HOLD_H_
#define _CEFFECT_FLAG_HOLD_H_

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
class CEffectFlagHold : public CEffectBase
{
	// ���J�����o
public:

	CEffectFlagHold(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectFlagHold(void);

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectFlagHold *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// ����J�����o
private:

};

#endif
//----EOF----