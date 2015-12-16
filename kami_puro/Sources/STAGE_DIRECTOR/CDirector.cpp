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
#include "../MANAGER/CManager.h"
#include "../CAMERA/CameraManager.h"
#include "../EFECT/CEffectManager.h"
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "CDirectList.h"
#include "Direct\CDirect.h"

//=================================================
// �R���X�g���N�^
//=================================================
CDirector::CDirector( CManager *pManager, CGame *pGame )
{
	m_pManager = pManager;
	m_pGame = pGame;
	m_CueDirect = DIRECT_PLAYER{ (DIRECT_ID)-1, (PLAYER_ID)-1 };
	m_CurDirect = DIRECT_PLAYER{ (DIRECT_ID)-1, (PLAYER_ID)-1 };
}

//=================================================
// �f�X�g���N�^
//=================================================
CDirector::~CDirector( void )
{
	m_pManager = NULL;
}

//=================================================
// ������
//=================================================
void CDirector::Init( void )
{
	// ���o���X�g���쐬
	m_pDirectList = new CDirectList( m_pManager, m_pGame );
	m_pDirectList->Init();
}

//=================================================
// �I��
//=================================================
void CDirector::Uninit( void )
{
	m_pDirectList->Uninit();
	SAFE_DELETE( m_pDirectList );
}

//=================================================
// �X�V
//=================================================
void CDirector::Update( void )
{
	// �U�����o�����̏ꍇ
	if( m_CurDirect.directingID < 0 )
	{
		// �\�񂪓����Ă����
		if( m_CueDirect.directingID != (DIRECT_ID)-1 )
		{
			// ���
			m_CurDirect = m_CueDirect;
			m_CueDirect = DIRECT_PLAYER{ (DIRECT_ID)-1, (PLAYER_ID)-1 };

			// ���o������
			m_pDirectList->GetDirectList( m_CurDirect.directingID )->Init( m_CurDirect.playerID );
		}
	}

	// �U�����o�L��̏ꍇ
	else
	{
		// ���o���X�V
		m_pDirectList->GetDirectList( m_CurDirect.directingID )->Update();
	}
}


//=================================================
// �f�B���N�^�[�ɍĐ����鉉�o�𑗂�
//=================================================
void CDirector::SendDirector( DIRECT_ID directingID, PLAYER_ID playerID )
{
	m_CurDirect = DIRECT_PLAYER{ directingID, playerID };
}

int CDirector::Direct( DIRECT_ID cueDirectingID, PLAYER_ID cuePlayerID )
{
	// �Đ�������Ȃ���Η\��
	if( GetIsDirecting().directingID < (DIRECT_ID)0 )
	{
		m_CueDirect = DIRECT_PLAYER{ cueDirectingID, cuePlayerID };
		return 1;
	}

	return 0;
}

void CDirector::SetEndDirecting( void )
{
	m_CurDirect.directingID = (DIRECT_ID)-1;
	SetDefaultCamera();
}

void CDirector::SetDefaultCamera( void )
{
	D3DXVECTOR3 playerCenter = (
		m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_1 )
		+ m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_2 ) )
		/ 2;

	m_pManager->GetCameraManager()->CameraSetToCoord(
		D3DXVECTOR3( playerCenter.x, 90.0f, -100.0f ),
		D3DXVECTOR3( playerCenter.x, 70.0f, 0.0f ) );
}

//----EOF----