//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CEFFECT_MP_ATTACK_H__
#define __CEFFECT_MP_ATTACK_H__

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
class CEffectMPAttack : public CEffectBase
{
	// ���J�����o
public:

	CEffectMPAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectMPAttack(void);

	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectMPAttack *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:
	CSceneAnime* m_pAnim;
};

#endif // __EXPLOSION_H__