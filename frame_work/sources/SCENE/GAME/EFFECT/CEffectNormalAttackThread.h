//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CEFFECT_NORMAL_ATTACK_THREAD_H__
#define __CEFFECT_NORMAL_ATTACK_THREAD_H__

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../GAME/EFFECT/CEffectBase.h"

//=============================================================================
// �萔
//=============================================================================

class CSceneAnime;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffectNormalAttackThread : public CEffectBase
{
	// ���J�����o
public:

	CEffectNormalAttackThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectNormalAttackThread(void);

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectNormalAttackThread *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:
	CSceneAnime* m_pAnim;
};

#endif // __EXPLOSION_H__