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
#include "../UI/CUiManager.h"
#include "../COMMANDCHART/CCommandChartManager.h"

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
	m_BattleModeOld = BATTLE_MOVE;

	for( int i = 0; i < PLAYER_MAX; i++ )
	{
		m_InputWaitFrameCount[i] = 0;
	}
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
	// �v���C���[���W�擾
	m_Pos[PLAYER_1] = m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_1 );
	m_Pos[PLAYER_2] = m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_2 );
	m_PosCenter = ( m_Pos[PLAYER_1] + m_Pos[PLAYER_2] ) / 2;			// �v���C���[���m�̒��S�_

	// �v���C���[���m�̋������擾
	m_Dist[PLAYER_1] = MagnitudeVector( m_Pos[PLAYER_1] );
	m_Dist[PLAYER_2] = MagnitudeVector( m_Pos[PLAYER_2] );
	m_DistTotal = MagnitudeVector( m_Pos[PLAYER_1] - m_Pos[PLAYER_2] );	// �v���C���[���m�̋���

	// ���͎�t�����i���o���͎󂯕t���Ȃ��j
	if( m_pManager->GetDirectorManager()->GetIsDirecting() >= 0 )
	{
		m_pManager->GetUiManager()->GetCommandChartManager()->SetInputCommandChart( false );
	}
	else
	{
		m_pManager->GetUiManager()->GetCommandChartManager()->SetInputCommandChart( true );
	}

	// �v���C���[���߂Â���
	if( m_DistTotal <= INITIATE_BATTLE_MODE_DISTANCE )
	{
		// �퓬���[�h�Ɉڍs
		m_BattleMode = BATTLE_FIGHT;
	}


	// ���[�h�ؑ֎��̏���
	if( m_BattleMode != m_BattleModeOld )
	switch( m_BattleMode )
	{
	case BATTLE_MOVE:
		break;

	case BATTLE_FIGHT:
		// �J�����ړ��A���Ԃ�CDirector�Ɉړ�����
		m_pManager->GetCameraManager()->CameraMoveToCoord(
			m_pManager->GetCameraManager()->GetCameraPos(),
			D3DXVECTOR3( m_PosCenter.x, 90.0f, -100.0f ),
			m_pManager->GetCameraManager()->GetPosRCamera(),
			D3DXVECTOR3( m_PosCenter.x, 70.0f, 0.0f ),
			30 );

		for( int i = 0; i < PLAYER_MAX; i++ )
		{
			m_SavePos[i] = m_Pos[i];
		}
		break;
	}


	// ���[�h�X�V����
	if( m_BattleMode == m_BattleModeOld )
	{
		switch( m_BattleMode )
		{
		// �ړ����[�h�ł����
		case BATTLE_MOVE:
			BattleMoveUpdate();


		// �퓬���[�h�ł����
		case BATTLE_FIGHT:
			BattleFightUpdate();
		}
	}

	m_BattleModeOld = m_BattleMode;
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

void CJudge::BattleMoveUpdate( void )
{
	// �J�����ړ�
	m_pManager->GetCameraManager()->CameraSetToCoord(
		D3DXVECTOR3( m_Dist[0] - m_Dist[1], 200.0f, -200.0f ),
		m_PosCenter );

	m_pManager->GetUiManager()->GetCommandChartManager()->SetInputCommandChart( false );
}


void CJudge::BattleFightUpdate( void )
{
	// ���o���I��������
	if( m_IsDirectingOld != m_pManager->GetDirectorManager()->GetIsDirecting() )
	{
		switch( m_pManager->GetDirectorManager()->GetIsDirecting() )
		{
		case -1:	// ���o�I����
			m_pManager->GetUiManager()->GetCommandChartManager()->SetCommandChartMode( PLAYER_1, CCommandChart::MODE_APPEAR );
			m_pManager->GetUiManager()->GetCommandChartManager()->SetCommandChartMode( PLAYER_2, CCommandChart::MODE_APPEAR );
			break;
		
		default:	// ���o�J�n��
			m_pManager->GetUiManager()->GetCommandChartManager()->SetCommandChartMode( PLAYER_1, CCommandChart::MODE_VANISH );
			m_pManager->GetUiManager()->GetCommandChartManager()->SetCommandChartMode( PLAYER_2, CCommandChart::MODE_VANISH );
			break;
		}
	}

	// �R�}���h���̓`�F�b�N�i����������T���Ȃ��j
	for( int i = 0; i < PLAYER_MAX; i++ )
	{
		if( m_InputWaitFrameCount[i] == 0 )
		{
			m_Command[i] = m_pManager->GetUiManager()->GetCommandChartManager()->GetCommandChartTechnic( i );
		}
	}

	// �R�}���h���͂��������Ă���΃t���[���J�E���g
	for( int i = 0; i < PLAYER_MAX; i++ )
	{
		if( m_Command[i] != -1 )
		{
			// �t���[���J�E���g�A�b�v
			m_InputWaitFrameCount[i]++;
		}
	}

	// ���͊����`�F�b�N
	if( ( ( m_InputWaitFrameCount[PLAYER_1] > 0 ) && ( m_InputWaitFrameCount[PLAYER_2] > 0 ) )		// ���ғ��͂��������Ă����
		|| ( m_InputWaitFrameCount[PLAYER_1] > 10 )													// player1�����͊�������ҋ@�t���[���o��
		|| ( m_InputWaitFrameCount[PLAYER_2] > 10 ) )												// player2�̓��͊�������ҋ@�t���[���o��
	{
		// �W����������
		TYPE_RPS genre[PLAYER_MAX] = { RPS_NONE, RPS_NONE };
		for( int i = 0; i < PLAYER_MAX; i++ )
		{
			switch( m_Command[i] )
			{
			case -1:
				genre[i] = RPS_NONE;	// ��
				break;

			case 0:
			case 1:
			case 2:
				genre[i] = RPS_SCISSOR;	// ��
				break;

			case 3:
			case 4:
			case 5:
				genre[i] = RPS_ROCK;	// ��
				break;

			case 6:
			case 7:
			case 8:
				genre[i] = RPS_PAPER;	// ��
				break;
			}
		}
		
		// ���㔻��
		PLAYER_ID winnerID = PLAYER_MAX;	// ���҂h�c
		bool isWon = false;					// ���Ҍ���t���O

		// �Е����U�����s���Ă����
		if( !isWon )
		{
			if( genre[PLAYER_2] == RPS_NONE )
			{
				winnerID = PLAYER_1;
				isWon = true;
			}
			if( genre[PLAYER_1] == RPS_NONE )
			{
				winnerID = PLAYER_2;
				isWon = true;
			}
		}

		// ���폟������
		if( !isWon )
		{
			if( genre[PLAYER_1] == genre[PLAYER_2] )
			{
				// �傫���ق�������
				if( m_Command[PLAYER_1] > m_Command[PLAYER_2] )
				{
					winnerID = PLAYER_1;
					isWon = true;
				}
				if( m_Command[PLAYER_2] > m_Command[PLAYER_1] )
				{
					winnerID = PLAYER_2;
					isWon = true;
				}
			}
		}

		// ����񂯂񏟂��𔻒�i����񂯂�j
		if( !isWon ){
			if( ( ( genre[PLAYER_1] == RPS_SCISSOR ) && ( genre[PLAYER_2] == RPS_PAPER ) )	// �`���L vs �p�[
			|| ( ( genre[PLAYER_1] == RPS_ROCK ) && ( genre[PLAYER_2] == RPS_SCISSOR ) )	// �O�[ vs �`���L
			|| ( ( genre[PLAYER_1] == RPS_PAPER ) && ( genre[PLAYER_2] == RPS_ROCK ) ) )	// �p�[ vs �O�[
			{
				winnerID = PLAYER_1;
			}
			else
			{
				winnerID = PLAYER_2;
			}
		}

		// ��s��������
		if( !isWon )
		{
			if( m_Command[PLAYER_1] == m_Command[PLAYER_2] )
			{
				// ��ɓ��͂��������Ă����������
				if( m_InputWaitFrameCount[PLAYER_1] >= m_InputWaitFrameCount[PLAYER_2] )
				{
					winnerID = PLAYER_1;
					isWon = true;
				}
				else
				{
					winnerID = PLAYER_2;
					isWon = true;
				}
			}
		}


		// �t���[���J�E���g���Z�b�g
		for( int i = 0; i < PLAYER_MAX; i++ )
		{
			m_InputWaitFrameCount[i] = 0;
		}


		switch( m_Command[winnerID] )
		{
		case COMMAND_TYPE_CHOP:
			m_pManager->GetDirectorManager()->Direct( DIR_SMALL_CHOP, winnerID );
			break;

		case COMMAND_TYPE_ELBOW:
			m_pManager->GetDirectorManager()->Direct( DIR_SMALL_ELBOW, winnerID );
			break;

		case COMMAND_TYPE_LARIAT:
			m_pManager->GetDirectorManager()->Direct( DIR_SMALL_LARIAT, winnerID );
			break;

		case COMMAND_TYPE_ROLLING_ELBOW:
			m_pManager->GetDirectorManager()->Direct( DIR_BIG_ROLLING, winnerID );
			break;

		case COMMAND_TYPE_FLYING_ELBOW:
			m_pManager->GetDirectorManager()->Direct( DIR_BIG_SHOULDER, winnerID );
			break;

		case COMMAND_TYPE_DROP_KICK:
			m_pManager->GetDirectorManager()->Direct( DIR_BIG_DROPKICK, winnerID );
			break;

		case COMMAND_TYPE_FACE_SLAPPING:
			m_pManager->GetDirectorManager()->Direct( DIR_THROW_SLAP, winnerID );
			break;

		case COMMAND_TYPE_BACKDROP:
			m_pManager->GetDirectorManager()->Direct( DIR_THROW_BACKDROP, winnerID );
			break;

		case COMMAND_TYPE_STANER:
			m_pManager->GetDirectorManager()->Direct( DIR_THROW_STUNNER, winnerID );
			break;
		}
	}



	
/*	// �ȈՃR�}���h�A�e�X�g�p
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_LEFT_UP ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_SMALL_CHOP, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_LEFT_DOWN ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_SMALL_ELBOW, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_Z ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_SMALL_LARIAT, PLAYER_1 );
	}

	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_RIGHT_UP ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_BIG_ROLLING, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_RIGHT_DOWN ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_BIG_SHOULDER, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_X ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_BIG_DROPKICK, PLAYER_1 );
	}

	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_E ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_THROW_SLAP, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_D ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_THROW_BACKDROP, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_C ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_THROW_STUNNER, PLAYER_1 );
	}


	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_LEFT_UP ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_SMALL_CHOP, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_LEFT_DOWN ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_SMALL_ELBOW, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_COMMA ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_SMALL_LARIAT, PLAYER_2 );
	}

	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_RIGHT_UP ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_BIG_ROLLING, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_RIGHT_DOWN ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_BIG_SHOULDER, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_PERIOD ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_BIG_DROPKICK, PLAYER_2 );
	}

	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_P ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_THROW_SLAP, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_SEMICOLON ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_THROW_BACKDROP, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_SLASH ) )
	{
		m_pManager->GetDirectorManager()->Direct( DIR_THROW_STUNNER, PLAYER_2 );
	}
*/
	m_IsDirectingOld = m_pManager->GetDirectorManager()->GetIsDirecting();
}

//----EOF----