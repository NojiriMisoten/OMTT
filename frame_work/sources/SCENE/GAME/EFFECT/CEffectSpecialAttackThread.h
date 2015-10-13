//=============================================================================
//
//	�U��������Ԃ̎��U���G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CEFFECT_SPECIAL_ATTACK_THREAD_H__
#define __CEFFECT_SPECIAL_ATTACK_THREAD_H__

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
class CEffectSpecialAttackThread : public CEffectBase
{
	// ���J�����o
public:
	CEffectSpecialAttackThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectSpecialAttackThread(void);

	void Init(D3DXVECTOR3 pos , D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectSpecialAttackThread *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:
	CSceneAnime *m_pAnim[4];
};

#endif // __EXPLOSION_H__