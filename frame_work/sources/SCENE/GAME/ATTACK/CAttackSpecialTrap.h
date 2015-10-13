//=============================================================================
//
// CAttackSpecialTrap�N���X [CAttackSpecialTrap.h]
// Author : �����@�Ȉ�
//
// 㩓����^
//
//=============================================================================
#ifndef _CATTACKSPECIALTRAP_H_
#define _CATTACKSPECIALTRAP_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CAttackBase.h"

//=============================================================================
//	�O���錾
//=============================================================================
class CPlayer;

// ����
static const short ATTACK_TRAP_END_TIME = 30;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAttackSpecialTrap : public CAttackBase
{
	// ���J�����o
public:

	CAttackSpecialTrap(LPDIRECT3DDEVICE9 *pDevice);
	~CAttackSpecialTrap(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A�v���C���ԍ��A���W
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CAttackSpecialTrap *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:
	D3DXVECTOR3 m_vDirection;
};

#endif
//----EOF----