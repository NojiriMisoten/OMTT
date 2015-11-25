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
#include "../../SCENE/GAME/JUDGE/CJudgeManager.h"

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

		// ���[�V�������Z�b�g
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_1, CPlayer::PLAYER_WAIT );
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_2, CPlayer::PLAYER_WAIT );

		// ���W���Z�b�g
		m_pManager->GetPlayerManager()->SetPos( PLAYER_1, m_pManager->GetJudgeManager()->GetSavePos( PLAYER_1 ) );
		m_pManager->GetPlayerManager()->SetPos( PLAYER_2, m_pManager->GetJudgeManager()->GetSavePos( PLAYER_2 ) );

		// ��]���Z�b�g
		m_pManager->GetPlayerManager()->SetRot( PLAYER_1, (D3DXVECTOR3)DEFAULT_PLAYER_1_ROT );
		m_pManager->GetPlayerManager()->SetRot( PLAYER_2, (D3DXVECTOR3)DEFAULT_PLAYER_2_ROT );

		//�@�A�j���[�V�������x���Z�b�g
		m_pManager->GetPlayerManager()->SetAnimSpd( PLAYER_1, DEFFAULT_ANIM_SPD );
		m_pManager->GetPlayerManager()->SetAnimSpd( PLAYER_2, DEFFAULT_ANIM_SPD );
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