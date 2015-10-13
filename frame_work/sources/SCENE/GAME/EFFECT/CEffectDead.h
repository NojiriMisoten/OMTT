//=============================================================================
//
//	�����G�t�F�N�g
//
//	Author : �����@�Ȉ�
//
//=============================================================================
#ifndef __CEFFECT_DEAD_H__
#define __CEFFECT_DEAD_H__

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
class CEffectDead : public CEffectBase
{
	// ���J�����o
public:

	CEffectDead(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectDead(void);

	void Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectDead *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos);

	// ����J�����o
private:

};

#endif // __EXPLOSION_H__