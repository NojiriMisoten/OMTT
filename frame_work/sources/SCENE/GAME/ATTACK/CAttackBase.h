//=============================================================================
//
// CAttackBase�N���X [CAttackBase.h]
// Author : �˖{�@�r�F
//
//=============================================================================
#ifndef _CATTACKBASE_H_
#define _CATTACKBASE_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../CSCENE/CScene2D.h"
#include "../PLAYER/CPlayerManager.h"
#include "CAttackManager.h"
#include "../../../MANAGER/CManager.h"

//=============================================================================
//	�O���錾
//=============================================================================
class CPlayer;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAttackBase : public CScene2D
{
	// ���J�����o
public:

	CAttackBase(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_ATTACK, OBJTYPE objType = OBJTYPE_ATTACK);
	virtual ~CAttackBase(void);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void){};
	short GetPlayerNumber(void){ return m_nPlayerNum; };
	bool GetHitFlag(int playerID);

	// �U�����q�b�g�������ɌĂяo���֐�
	// �����@���̍U���ɓ��������v���C���[�̃v���C���[�ԍ�
	virtual void HitPlayer(CPlayer* pPlayer);

	// �p�������o
protected:
	// �������J�E���g����
	short m_nCount;
	// ����
	short m_nEndTime;
	// �����蔻������鎞��
	short m_nHitStartTime;
	short m_nHitEndTime;
	// ���̍U���͒N�̂��v���C���[�ԍ�
	short m_nPlayerNum;
	// �����蔻�������t���O
	bool m_bHitFlg;
	// ���������v���C���[�Ǘ��z��
	bool m_bHitPlayer[MAXIMUM_NUMBER_OF_PLAYER];
	// �U���̎��
	AttackType m_AttackType;

	// ����J�����o
private:
	
};

#endif
//----EOF----