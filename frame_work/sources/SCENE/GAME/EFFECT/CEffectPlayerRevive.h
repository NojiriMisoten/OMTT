//=============================================================================
//
// CEffect�N���X [CEffect.h]
// Author : �˖{�@�r�F
//
//=============================================================================
#ifndef _CEFFECT_PLAYER_REVIVE_H_
#define _CEFFECT_PLAYER_REVIVE_H_

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
class CEffectPlayerRevive : public CEffectBase
{
	// ���J�����o
public:

	CEffectPlayerRevive(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectPlayerRevive(void);

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectPlayerRevive *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// ����J�����o
private:

};

#endif
//----EOF----