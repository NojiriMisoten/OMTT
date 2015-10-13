//=============================================================================
//
// CEffect�N���X [CEffect.h]
// Author : �˖{�@�r�F
//
//=============================================================================
#ifndef _CEFFECT_TRAP_FOUNDATION_H_
#define _CEFFECT_TRAP_FOUNDATION_H_

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
class CEffectTrapFoundation : public CEffectBase
{
	// ���J�����o
public:

	CEffectTrapFoundation(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectTrapFoundation(void);

	void Init(D3DXVECTOR3 pos , short playerNum);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectTrapFoundation *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		short playerNum);

	// ����J�����o
private:
	CSceneAnime* m_pAnim;
};

#endif
//----EOF----