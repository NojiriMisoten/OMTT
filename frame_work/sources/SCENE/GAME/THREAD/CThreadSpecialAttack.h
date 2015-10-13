//=============================================================================
//
// CThreadSpecialAttack�N���X [CThreadSpecialAttack.h]
// Author : �����@�Ȉ�
//
// �U�������`�Ԃ̎�
//
//=============================================================================
#ifndef _CTHREAD_SPECIAL_ATTACK_H_
#define _CTHREAD_SPECIAL_ATTACK_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "../ATTACK/CAttackBase.h"

//*****************************************************************************
//	�O���錾
//*****************************************************************************
class CPlayer;

// ����
static const short THREAD_ATTACK_END_TIME = 120;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CThreadSpecialAttack : public CAttackBase
{
	// ���J�����o
public:
	// �v���C�I���e�B�ƃI�u�W�F�^�C�v�̓e�X�g
	CThreadSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int priority = TYPE_PRIORITY_THREAD_OF_FOOTHOLD, OBJTYPE type = OBJTYPE_ATTACK);
	~CThreadSpecialAttack(void);

	// ������
	//  �����@�@���W�A�v���C���[�ԍ�
	HRESULT Init(D3DXVECTOR3 pos, short playerNumber, D3DXVECTOR3 velocity);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �U�����q�b�g�������ɌĂяo���֐�
	// �����@���̍U���ɓ��������v���C���[�̃v���C���[�ԍ�
	void HitPlayer(CPlayer* pPlayer);

	//=======================================================================
	// �N���G�C�g�֐�
	// ����:�f�o�C�X�A�v���C���ԍ��A���W
	// �߂�l:�쐬�����A�h���X
	//=======================================================================
	static CThreadSpecialAttack *Create(
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