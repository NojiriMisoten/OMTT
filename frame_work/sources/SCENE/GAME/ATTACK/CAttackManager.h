//=============================================================================
//
// CAttacjManager�N���X [CAttacjManager.h]
// Author : �˖{�@�r�F
//
// �U���}�l�[�W���[
//
//=============================================================================
#ifndef _CATTACKMANAGER_H_
#define _CATTACKMANAGER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../../RENDERER/CRenderer.h"

//=============================================================================
// �萔
//=============================================================================
enum AttackType{
	ATTACK_TYPE_NORMAL,
	ATTACK_TYPE_ATTACK,			// �U���͓����^
	ATTACK_TYPE_ATTACK_THREAD,	// �U���͓����^(���U��)
	ATTACK_TYPE_SPEED,			// �X�s�[�h�����^
	ATTACK_TYPE_TRAP,			// 㩍쐬�^
	ATTACK_TYPE_MAX
};

//=============================================================================
// �O���錾
//=============================================================================
class CAttack;
class CPlayerManager;
class CAttackNormal;
class CAttackSpecialAttack;
class CAttackSpecialSpeed;
class CAttackSpecialTrap;
class CGame;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CAttackManager
{
	// ���J�����o
public:
	CAttackManager(LPDIRECT3DDEVICE9 *pDevice);//, CPlayerManager* pPlayerManager);
	CAttackManager(LPDIRECT3DDEVICE9 *pDevice , CGame* pGame);//, CPlayerManager* pPlayerManager);
	~CAttackManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// �U�������֐�(�U���^�C�v,�v���C���ԍ�,���W,�ړ��ʁi�����x�N�g���j)
	void CreateAttack(
		AttackType type,
		short sPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// ����J�����o
private:
	LPDIRECT3DDEVICE9*			m_pDevice;
	CGame*						m_pGame;
	//CPlayerManager*				m_pPlayerManager;		// �v���C���[�}�l�[�W���̃|�C���^
	CAttackNormal*			m_apAttackNormal[20];	// �ʏ�U���̃|�C���^�����Ă����z��
	//CAttackSpecialAttack*		m_apAttackAttack[12];	// �U�������U���̃|�C���^�����Ă����z��
	//CAttackSpecialSpeed*		m_apAttackSpeed[4];		// �ړ������̍U���̃|�C���^�����Ă����z��
	//CAttackSpecialTrap*		m_apAttackTrap[12];		// �W�Q�����̍U���̃|�C���^�����Ă����z��
};

#endif
//----EOF----