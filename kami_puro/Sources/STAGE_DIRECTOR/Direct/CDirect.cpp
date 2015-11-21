//=============================================================================
//
// CDirect�N���X [CDirect.cpp]
// Author : �r���@���
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CDirect.h"
#include "../../MANAGER/CManager.h"
#include "../CDirectorManager.h"

//=================================================
// �R���X�g���N�^
//=================================================
CDirect::CDirect( CManager *pManager )
{
	m_pManager = pManager;
	m_FrameCount = 0;
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirect::~CDirect( void )
{

}


//=================================================
// ���o���I���������`�F�b�N
//=================================================
void CDirect::CheckEnd( void )
{
	m_FrameCount++;
	if( m_FrameCount > m_TotalFrame )
	{
		m_pManager->GetDirectorManager()->SetEndDirecting();
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_1, CPlayer::PLAYER_WAIT );
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_2, CPlayer::PLAYER_WAIT );
	}
}

void CDirect::SetPlayerID( PLAYER_ID playerID )
{
	// �v���C���[ID�����蓖��
	m_Player = playerID;
	if( playerID == PLAYER_1 )
	{
		m_Enemy = PLAYER_2;
	}
	else
	{
		m_Enemy = PLAYER_1;
	}
}

//----EOF----