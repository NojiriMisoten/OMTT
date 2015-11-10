//=============================================================================
//
// CJudge�N���X [CJudge.cpp]
// Author : �r���@���
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CJudge.h"
#include "../../../MANAGER/CManager.h"
#include "../PLAYER/CPlayerManager.h"
#include "../../../CAMERA/CameraManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float INITIATE_BATTLE_MODE_DISTANCE = 50.0f;		// �o�g�����[�h�Ɉڍs���鋗��

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CJudge::CJudge( void )
{
	m_pManager = NULL;
	m_BattleMode = (BATTLE_MODE)-1;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CJudge::~CJudge( void )
{

}

//=============================================================================
//=============================================================================
void CJudge::Init( CManager* pManager )
{
	m_pManager = pManager;
	m_BattleMode = BATTLE_MOVE;
}

//=============================================================================
// �I��
//=============================================================================
void CJudge::Uninit( void )
{

}

//=============================================================================
// �X�V
//=============================================================================
void CJudge::Update( void )
{
	// �ړ����[�h�ł����
	if( m_BattleMode == BATTLE_MOVE )
	{
		// �v���C���[���W�擾
		D3DXVECTOR3 playerPos[2];
		playerPos[0] = m_pManager->GetPlayerManager()->GetPlayerPos( 0 );
		playerPos[1] = m_pManager->GetPlayerManager()->GetPlayerPos( 1 );
		D3DXVECTOR3 playerCenter = ( playerPos[0] + playerPos[1] ) / 2;			// �v���C���[���m�̒��S�_

		// �v���C���[���m�̋������擾
		float playerDist[2];
		playerDist[0] = MagnitudeVector( playerPos[0] );
		playerDist[1] = MagnitudeVector( playerPos[1] );
		float playerDistance = MagnitudeVector( playerPos[0] - playerPos[1] );	// �v���C���[���m�̋���

		// �J�����ړ�
		m_pManager->GetCameraManager()->CameraSetToCoord(
			D3DXVECTOR3( playerDist[0] - playerDist[1], 200.0f, -200.0f ),
			playerCenter );



		// �v���C���[���߂Â���
		if( playerDistance < INITIATE_BATTLE_MODE_DISTANCE )
		{
			// �퓬���[�h�Ɉڍs
			m_BattleMode = BATTLE_FIGHT;

			// �J�����ړ��A���Ԃ�CDirector�Ɉړ�����
			m_pManager->GetCameraManager()->CameraMoveToCoord(
				m_pManager->GetCameraManager()->GetCameraPos(),
				D3DXVECTOR3( playerCenter.x, 90.0f, -100.0f ),
				m_pManager->GetCameraManager()->GetPosRCamera(),
				D3DXVECTOR3( playerCenter.x, 70.0f, 0.0f ),
				30 );
		}
	}


	// �퓬���[�h�ł����
	if( m_BattleMode == BATTLE_FIGHT )
	{

	}
}

//=============================================================================
// �쐬
//=============================================================================
CJudge* CJudge::Create( CManager* pManager )
{
	CJudge* p = new CJudge;
	p->Init( pManager );
	return p;
}



//----EOF----