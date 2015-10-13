//=============================================================================
//
// CEffect�N���X [CEffect.h]
// Author : �˖{�@�r�F
//
//=============================================================================
#ifndef _CEFFECT_METAMORPHOSE_H_
#define _CEFFECT_METAMORPHOSE_H_

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
class CEffectMetamorphose : public CEffectBase
{
	// ���J�����o
public:

	CEffectMetamorphose(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	~CEffectMetamorphose(void);

	void Init(D3DXVECTOR3 pos, short playerNum);
	void Uninit(void);
	void Update(void);

	// �N���G�C�g�֐�
	static CEffectMetamorphose *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos,
		short playerNum);

	// ����J�����o
private:
	TEXTURE_TYPE m_Texture;
};

#endif
//----EOF----