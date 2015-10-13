//=============================================================================
//
//	�U��������Ԃ̍U���G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CEFFECT_SPECIAL_ATTACK_H__
#define __CEFFECT_SPECIAL_ATTACK_H__

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
class CEffectSpecialAttack : public CEffectBase
{
	// ���J�����o
public:
	CEffectSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectSpecialAttack(void);

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectSpecialAttack *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:

};

#endif // __EXPLOSION_H__