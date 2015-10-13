//=============================================================================
//
// CThreadManager�N���X [CThreadManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "CThreadManager.h"
#include "CThreadBase.h"
#include "CThreadNormal.h"
#include "CThreadSpecialAttack.h"
#include "CThreadSpecialSpeed.h"
#include "CThreadSpecialTrap.h"

//********************************************** *******************************
// �}�N��
//*****************************************************************************


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CThreadManager::CThreadManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CThreadManager ::~CThreadManager(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CThreadManager::Init()
{
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CThreadManager::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CThreadManager::Update(void)
{
}

//*****************************************************************************
// ���𐶐�����
//*****************************************************************************
void CThreadManager::CreateThread(ThreadType type, int nPlayerNum, D3DXVECTOR3 pos, DIRECTION_PLAYER_FACING playerFacing, CEffectManager *pEffectManager)
{
	switch (type)
	{
		// �ʏ�`�Ԃ̎�
	case THREAD_TYPE_NORMAL:
		CThreadNormal::Create(m_pDevice, nPlayerNum, pos, PLAYER_DIRECTION_VECTOR[playerFacing]);
		break;
		// �U�������`�Ԃ̎�
	case THREAD_TYPE_ATTACK:
		CThreadSpecialAttack::Create(m_pDevice, nPlayerNum, pos , PLAYER_DIRECTION_VECTOR[playerFacing]);
		break;
		// �ړ������`�Ԃ̎�
	case THREAD_TYPE_SPEED:
		CThreadSpecialSpeed::Create(m_pDevice, nPlayerNum, pos, playerFacing);
		break;
		// 㩓����`�Ԃ̎�
	case THREAD_TYPE_TRAP:
		CThreadSpecialTrap::Create(m_pDevice, nPlayerNum, pos);
		break;
	default:
		break;
	}
}
//----EOF-------