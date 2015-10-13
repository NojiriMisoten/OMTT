//=============================================================================
//
// CAttackManager�N���X [CAttackManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "CAttackManager.h"
#include "CAttackNormal.h"
#include "CAttackSpecialAttack.h"
#include "CAttackSpecialSpeed.h"
#include "CAttackSpecialTrap.h"
#include "../CGame.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAttackManager::CAttackManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
}

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAttackManager::CAttackManager(LPDIRECT3DDEVICE9 *pDevice , CGame* pGame)
{
	m_pDevice = pDevice;
	m_pGame = pGame;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CAttackManager ::~CAttackManager(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CAttackManager::Init()
{
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CAttackManager::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CAttackManager::Update(void)
{
}

//*****************************************************************************
// �U���𐶐�����
//*****************************************************************************
void CAttackManager::CreateAttack(
	AttackType type,
	short sPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	switch (type)
	{
		// �ʏ��Ԃ̍U��
	case ATTACK_TYPE_NORMAL:
		CAttackNormal::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
		// �U���`�Ԃ̍U��
	case ATTACK_TYPE_ATTACK:
		CAttackSpecialAttack::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
		// �ړ��`�Ԃ̍U��
	case ATTACK_TYPE_SPEED:
		CAttackSpecialSpeed::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
		// 㩌`�Ԃ̍U��
	case ATTACK_TYPE_TRAP:
		CAttackSpecialTrap::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
	default:
		break;
	}
}
//----EOF-------