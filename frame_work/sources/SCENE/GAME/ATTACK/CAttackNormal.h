//=============================================================================
//
// CAttackNormal�N���X [CAttackNoaml.h]
// Author : �˖{�@�r�F
//
// ���ʍU��
//
//=============================================================================
#ifndef _CATTACKNORMAL_H_
#define _CATTACKNORMAL_H_

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
static const short ATTACK_NORMAL_END_TIME = 30;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAttackNormal : public CAttackBase
{
	// ���J�����o
public:

	CAttackNormal(LPDIRECT3DDEVICE9 *pDevice);
	~CAttackNormal(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// �U�����q�b�g�������ɌĂяo���֐�
	// �����@���̍U���ɓ��������v���C���[�̃v���C���[�ԍ�
	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A�v���C���ԍ��A���W
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CAttackNormal *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:

};

#endif
//----EOF----