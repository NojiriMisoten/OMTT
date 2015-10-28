//=============================================================================
//
// CDirector�N���X [CDirector.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirector.h"

//=================================================
// �R���X�g���N�^
//=================================================
CDirector::CDirector( void )
{
	m_IsDirecting = false;
	m_CurAttackID = (ATTACK_LIST)-1;
	m_CurPlayerID = -1;
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirector::~CDirector( void )
{

}

//=================================================
// ������
//=================================================
void CDirector::Init( void )
{

}

//=================================================
// �I��
//=================================================
void CDirector::Uninit( void )
{

}

//=================================================
// �X�V
//=================================================
void CDirector::Update( void )
{
	// �U�����o�����̏ꍇ
	if( !m_IsDirecting )
	{
		// �J�����Ƃ��̐ݒ�

	}

	// �U�����o�L��̏ꍇ
	else
	{
		// �Đ�����AttackID
		switch( m_CurAttackID )
		{
		case ATTACK_SMALL_CHOP:
			break;

		case ATTACK_SMALL_ELBOW:
			break;

		case ATTACK_SMALL_LARIAT:
			break;

		case ATTACK_BIG_ROLLING:
			break;

		case ATTACK_BIG_SHOLDER:
			break;

		case ATTACK_BIG_DROPKICK:
			break;

		case ATTACK_THROW_SLAP:
			break;

		case ATTACK_THROW_BACKDROP:
			break;

		case ATTACK_THROW_STUNNER:
			break;

		case ATTACK_FINISH_BODYPRESS:
			break;

		case ATTACK_FINSH_ATOMICBUSTER:
			break;
		}
	}
}

//=================================================
// �f�B���N�^�[�ɍĐ����鉉�o�𑗂�
//=================================================
void CDirector::SendDirector( ATTACK_LIST attackID, int playerID )
{
	m_IsDirecting = true;
	m_CurAttackID = attackID;
	m_CurPlayerID = playerID;
}

//----EOF----