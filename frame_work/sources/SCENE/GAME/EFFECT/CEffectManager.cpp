//=============================================================================
//
// CEffectManager�N���X [CEffectManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "CEffectManager.h"
#include "CEffectAttackHit.h"				// �U���q�b�g
#include "CEffectExplosion.h"				// ����
#include "CEffectSpecialAttack.h"			// �U�������U��
#include "CEffectSpecialSpeedAttack.h"		// �ړ������U��
#include "CEffectSpecialAttackThread.h"		// �U���������U��
#include "CEffectCaughtTrap.h"				// 㩂ɂ���������
#include "CEffectCreateRoad.h"				// ���쐬��
#include "CEffectFlagGet.h"					// �t���O�擾��
#include "CEffectFlagHold.h"				// �t���O������
#include "CEffectDead.h"					// �v���C���[�̗͂O
#include "CEffectMPAttack.h"				// MP�_���[�W��
#include "CEffectNormalAttackCap.h"			// �X�q�U��
#include "CEffectNormalAttackThread.h"		// �ʏ펅����
#include "CEffectPlayerRevive.h"			// �v���C���[������
#include "CEffectTrapFoundation.h"			// 㩐ݒu��
#include "CEffectMetamorphose.h"			// �ϐg

//*****************************************************************************
// �}�N��
//*****************************************************************************


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DDEVICE9* CEffectManager::m_pDevice;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectManager::CEffectManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectManager ::~CEffectManager(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectManager::Init()
{
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectManager::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectManager::Update(void)
{
}

//*****************************************************************************
// �G�t�F�N�g����
//*****************************************************************************
void CEffectManager::CreateEffect(D3DXVECTOR3 pos, EFFECT_TYPE type, D3DXVECTOR3 velocity)
{
	switch (type)
	{
		// �U�����q�b�g�������̃G�t�F�N�g
	case EFFECT_ATTACK_HIT:
		CEffectAttackHit::Create(m_pDevice, pos);
		break;
		// �����G�t�F�N�g
	case EFFECT_EXPLOSION:
		CEffectExplosion::Create(m_pDevice, pos);
		break;
		// �U���`�Ԏ��̍U��
	case EFFECT_SPECIAL_ATTACK_ATTACK:
		CEffectSpecialAttack::Create(m_pDevice, pos, velocity);
		break;
		// �ړ��`�Ԏ��̍U��
	case EFFECT_SPECIAL_ATTACK_SPEED:
		CEffectSpecialSpeedAttack::Create(m_pDevice, pos, velocity);
		break;
		// �U���`�Ԏ��̎��U��
	case EFFECT_SPECIAL_THREAD_ATTACK:
		CEffectSpecialAttackThread::Create(m_pDevice, pos, velocity);
		break;
		// 㩂ɂ����������̃G�t�F�N�g
	case EFFECT_CAUGHT_TRAP:
		CEffectCaughtTrap::Create(m_pDevice, pos);
		break;
		// ���쐬���̃G�t�F�N�g
	case EFFECT_CREATE_ROAD:
		CEffectCreateRoad::Create(m_pDevice, pos);
		break;
		// �t���O�擾��
	case EFFECT_FLAG_GET:
		CEffectFlagGet::Create(m_pDevice, pos);
		break;
		// �t���O������
	case EFFECT_FLAG_HOLD:
		CEffectFlagHold::Create(m_pDevice, pos);
		break;
		// �v���C���[�̗̑͂O���̃G�t�F�N�g
	case EFFECT_PLAYER_DEAD:
		CEffectDead::Create(m_pDevice, pos);
		break;
		// MP�U�����̃G�t�F�N�g
	case EFFECT_MP_ATTACK:
		CEffectMPAttack::Create(m_pDevice, pos, velocity);
		break;
		// �ʏ�`�Ԃ̖X�q�U���G�t�F�N�g
	case EFFECT_NORMAL_ATTACK_CAP:
		CEffectNormalAttackCap::Create(m_pDevice, pos);
		break;
		// �ʏ�`�Ԃ̎����˃G�t�F�N�g
	case EFFECT_NORMAL_ATTACK_THREAD:
		CEffectNormalAttackThread::Create(m_pDevice, pos, velocity);
		break;
		// �v���C���[�����G�t�F�N�g
	case EFFECT_PLAYER_REVIAVE:
		CEffectPlayerRevive::Create(m_pDevice, pos);
		break;
	default:
		break;
	}
}

//*****************************************************************************
// �G�t�F�N�g����
//*****************************************************************************
void CEffectManager::CreateEffectMeta(D3DXVECTOR3 pos, short playerNum)
{
	// �ϐg�G�t�F�N�g
	CEffectMetamorphose::Create(m_pDevice,pos,playerNum);
}

//*****************************************************************************
// �G�t�F�N�g����
//*****************************************************************************
void CEffectManager::CreateEffectTrap(D3DXVECTOR3 pos, short playerNum)
{
	// �g���b�v�G�t�F�N�g
	CEffectTrapFoundation::Create(m_pDevice, pos, playerNum);
}
//----EOF-------