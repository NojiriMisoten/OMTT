//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CEFFECT_NORMAL_ATTACK_CAP_H__
#define __CEFFECT_NORMAL_ATTACK_CAP_H__

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
class CEffectNormalAttackCap : public CEffectBase
{
	// ���J�����o
public:

	CEffectNormalAttackCap(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectNormalAttackCap(void);

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectNormalAttackCap *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// ����J�����o
private:

};

#endif // __EXPLOSION_H__