//=============================================================================
//
// CAttackSpecialSpeed�N���X [CAttackSpecialSpeed.h]
// Author : �����@�Ȉ�
//
// �X�s�[�h�����^
//
//=============================================================================
#ifndef _CATTACKSPECIALSPEED_H_
#define _CATTACKSPECIALSPEED_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CAttackBase.h"

//=============================================================================
// �O���錾
//=============================================================================
class CPlayer;

// ����
static const short ATTACK_SPEED_END_TIME = 10;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAttackSpecialSpeed : public CAttackBase
{
	// ���J�����o
public:

	CAttackSpecialSpeed(LPDIRECT3DDEVICE9 *pDevice);
	~CAttackSpecialSpeed(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A�v���C���ԍ��A���W
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CAttackSpecialSpeed *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:
	D3DXVECTOR3 m_vVelocity;
};

#endif
//----EOF----