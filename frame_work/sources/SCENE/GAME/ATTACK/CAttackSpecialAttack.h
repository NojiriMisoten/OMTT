//=============================================================================
//
// CAttackSpecialAttack�N���X [CAttackSpecialAttack.h]
// Author : �����@�Ȉ�
//
// �U���`�Ԃ̍U��
//
//=============================================================================
#ifndef _CATTACKSPECIALATTACK_H_
#define _CATTACKSPECIALATTACK_H_

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
static const short ATTACK_ATTACK_END_TIME = 30;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAttackSpecialAttack : public CAttackBase
{
	// ���J�����o
public:

	CAttackSpecialAttack(LPDIRECT3DDEVICE9 *pDevice);
	~CAttackSpecialAttack(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A�v���C���ԍ��A���W
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CAttackSpecialAttack *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:

};

#endif
//----EOF----