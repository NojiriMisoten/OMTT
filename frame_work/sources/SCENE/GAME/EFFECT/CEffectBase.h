//=============================================================================
//
// CEffect�N���X [CEffect.h]
// Author : �˖{�@�r�F
//
//=============================================================================
#ifndef _CEFFECTBASE_H_
#define _CEFFECTBASE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../MAIN/main.h"
#include "../../CSCENE/CScene2D.h"

//=============================================================================
// �萔
//=============================================================================

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffectBase : public CScene2D
{
	// ���J�����o
public:

	CEffectBase(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT);
	virtual ~CEffectBase(void);

	void Init(void);
	void Uninit(void);
	void Update(void);


	// ����J�����o
protected:
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vVelocity;
	int m_nCount;
	int m_nCountMax;

};

#endif
//----EOF----