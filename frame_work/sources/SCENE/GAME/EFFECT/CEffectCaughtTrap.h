//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CEFFECT_CAUGHT_TRAP_H__
#define __CEFFECT_CAUGHT_TRAP_H__

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
class CEffectCaughtTrap : public CEffectBase
{
	// ���J�����o
public:

	CEffectCaughtTrap(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectCaughtTrap(void);

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectCaughtTrap *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// ����J�����o
private:

};

#endif // __EXPLOSION_H__