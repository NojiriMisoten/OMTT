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
#include "../../../STAGE_DIRECTOR/CDirectorManager.h"

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
		D3DXVECTOR3 playerPos[PLAYER_MAX];
		playerPos[PLAYER_1] = m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_1 );
		playerPos[PLAYER_2] = m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_2 );
		D3DXVECTOR3 playerCenter = ( playerPos[PLAYER_1] + playerPos[PLAYER_2] ) / 2;			// �v���C���[���m�̒��S�_

		// �v���C���[���m�̋������擾
		float playerDist[PLAYER_MAX];
		playerDist[PLAYER_1] = MagnitudeVector( playerPos[PLAYER_1] );
		playerDist[PLAYER_2] = MagnitudeVector( playerPos[PLAYER_2] );
		float playerDistance = MagnitudeVector( playerPos[PLAYER_1] - playerPos[PLAYER_2] );	// �v���C���[���m�̋���

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
		if( CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_LEFT_UP) )
		{
			m_pManager->GetDirectorManager()->Direct( DIR_SMALL_CHOP, PLAYER_1 );
		}
		if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_RIGHT_UP ) )
		{
			m_pManager->GetDirectorManager()->Direct( DIR_SMALL_ELBOW, PLAYER_1 );
		}
		if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_LEFT_DOWN ) )
		{
			m_pManager->GetDirectorManager()->Direct( DIR_SMALL_LARIAT, PLAYER_1 );
		}


		if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_LEFT_UP ) )
		{
			m_pManager->GetDirectorManager()->Direct( DIR_SMALL_CHOP, PLAYER_2 );
		}
		if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_RIGHT_UP ) )
		{
			m_pManager->GetDirectorManager()->Direct( DIR_SMALL_ELBOW, PLAYER_2 );
		}
		if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_LEFT_DOWN ) )
		{
			m_pManager->GetDirectorManager()->Direct( DIR_SMALL_LARIAT, PLAYER_2 );
		}


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