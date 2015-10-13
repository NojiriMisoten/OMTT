//=============================================================================
//
//	�ړ�������Ԃ̍U���G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CEFFECT_SPECIAL_SPEED_ATTACK_H__
#define __CEFFECT_SPECIAL_SPEED_ATTACK_H__

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../GAME/EFFECT/CEffectBase.h"

//=============================================================================
// �萔
//=============================================================================

//=============================================================================
//	�O���錾
//=============================================================================
class CSceneAnime;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffectSpecialSpeedAttack : public CEffectBase
{
	// ���J�����o
public:
	CEffectSpecialSpeedAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectSpecialSpeedAttack(void);

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectSpecialSpeedAttack *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:
	CSceneAnime *m_pAnim;
};

#endif // __EXPLOSION_H__