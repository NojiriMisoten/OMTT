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
static const int WAIT_INPUT_FRAMES = 30;

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
	m_BattleMode = BATTLE_MAX;
	m_BattleModeOld = BATTLE_MAX;

	m_pCommandChartManager = m_pManager->GetUiManager()->GetCommandChartManager();
	m_pDirectorManager = m_pManager->GetDirectorManager();
	m_pPlayerManager = m_pManager->GetPlayerManager();

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
	m_Pos[PLAYER_1] = m_pPlayerManager->GetPlayerPos( PLAYER_1 );
	m_Pos[PLAYER_2] = m_pPlayerManager->GetPlayerPos( PLAYER_2 );
	m_PosCenter = ( m_Pos[PLAYER_1] + m_Pos[PLAYER_2] ) / 2;			// �v���C���[���m�̒��S�_

	// �v���C���[���m�̋������擾
	m_Dist[PLAYER_1] = MagnitudeVector( m_Pos[PLAYER_1] );
	m_Dist[PLAYER_2] = MagnitudeVector( m_Pos[PLAYER_2] );
	m_DistTotal = MagnitudeVector( m_Pos[PLAYER_1] - m_Pos[PLAYER_2] );	// �v���C���[���m�̋���

	// ���[�h�X�V����
	if( m_BattleMode == m_BattleModeOld )
	{
		switch( m_BattleMode )
		{
		case BATTLE_MOVE:	// �ړ����[�h�ł����
			BattleMoveUpdate();
			break;

		case BATTLE_FIGHT:	// �퓬���[�h�ł����
			BattleFightUpdate();
			break;
		}
	}

	// ���[�h�ؑ֎��̏���
	if( m_BattleMode != m_BattleModeOld )
	{
		switch( m_BattleModeOld )
		{
			// �ړ����[�h�I����
		case BATTLE_MOVE:
			break;
			
			// �퓬���[�h�I����
		case BATTLE_FIGHT:
			break;
		}


		switch( m_BattleMode )
		{
			// �ړ����[�h�J�n��
		case BATTLE_MOVE:
			// �R�}���h�`���[�g����
			m_pCommandChartManager->SetCommandChartMode( PLAYER_1, CCommandChart::MODE_VANISH );
			m_pCommandChartManager->SetCommandChartMode( PLAYER_2, CCommandChart::MODE_VANISH );
			m_pCommandChartManager->SetInputCommandChart( false );
			break;

			// �퓬���[�h�J�n��
		case BATTLE_FIGHT:
			// �R�}���h�`���[�g�\��
			m_Pos[PLAYER_1].y = 0.0f;
			m_Pos[PLAYER_2].y = 0.0f;
			m_pPlayerManager->SetPos( PLAYER_1, m_Pos[PLAYER_1] );
			m_pPlayerManager->SetPos( PLAYER_2, m_Pos[PLAYER_2] );
			m_pCommandChartManager->SetCommandChartMode( PLAYER_1, CCommandChart::MODE_APPEAR );
			m_pCommandChartManager->SetCommandChartMode( PLAYER_2, CCommandChart::MODE_APPEAR );
			m_pCommandChartManager->SetInputCommandChart( true );

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
	// �v���C���[���߂Â���
	if( m_DistTotal <= INITIATE_BATTLE_MODE_DISTANCE )
	{
		// �퓬���[�h�Ɉڍs
		m_BattleMode = BATTLE_FIGHT;
	}

	// �J�����ړ�
	m_pManager->GetCameraManager()->CameraSetToCoord(
		D3DXVECTOR3( m_Dist[0] - m_Dist[1], 200.0f, -200.0f ),
		m_PosCenter );
}


void CJudge::BattleFightUpdate( void )
{
	// ���o���I��������
	if( m_IsDirectingOld != m_pDirectorManager->GetIsDirecting() )
	{
		switch( m_pDirectorManager->GetIsDirecting() )
		{
		case -1:	// ���o�I����
			CCommandChart::MODE_COMMAND_CHART mode[PLAYER_MAX];
			mode[0] = m_pCommandChartManager->GetCommandChartMode( 0 );
			mode[1] = m_pCommandChartManager->GetCommandChartMode( 1 );
			
			// �R�}���h�`���[�g�\��
			for( int i = 0; i < PLAYER_MAX; i++ )
			{
				// ���łɕ\������Ă����
				if( mode[i] == CCommandChart::MODE_COMPLETE_COMMAND )
				{
					m_pCommandChartManager->SetCommandChartMode( i, CCommandChart::MODE_RESET );
				}
				else
				{
					m_pCommandChartManager->SetCommandChartMode( i, CCommandChart::MODE_APPEAR );
				}

				m_pCommandChartManager->SetInputCommandChart( true );
			}

			switch( m_IsDirectingOld )
			{
			case DIR_SMALL_LARIAT:
			case DIR_BIG_SHOULDER:
			case DIR_BIG_DROPKICK:
			case DIR_THROW_BACKDROP:
			case DIR_THROW_STUNNER:
				m_pPlayerManager->SetPos( PLAYER_1, DEFAULT_PLAYER_1_POS );
				m_pPlayerManager->SetRot( PLAYER_1, DEFAULT_PLAYER_1_ROT );
				m_pPlayerManager->SetPos( PLAYER_2, DEFAULT_PLAYER_2_POS );
				m_pPlayerManager->SetRot( PLAYER_2, DEFAULT_PLAYER_2_ROT );
				
				m_IsDirectingOld = -1;
				m_BattleMode = BATTLE_MOVE;

				return;
			}

			break;
		
		default:	// ���o�J�n��
			// �R�}���h�`���[�g����
//			m_pCommandChartManager->SetCommandChartMode( PLAYER_1, CCommandChart::MODE_VANISH );
//			m_pCommandChartManager->SetCommandChartMode( PLAYER_2, CCommandChart::MODE_VANISH );
			break;
		}
	}

	// �R�}���h���̓`�F�b�N�i����������T���Ȃ��j
	for( int i = 0; i < PLAYER_MAX; i++ )
	{
		if( m_InputWaitFrameCount[i] == 0 )
		{
			m_Command[i] = m_pCommandChartManager->GetCommandChartTechnic( i );
		}
	}

	// �R�}���h���͂��������Ă���΃t���[���J�E���g�A���͕s��
	for( int i = 0; i < PLAYER_MAX; i++ )
	{
		if( m_Command[i] != COMMAND_TYPE_NONE )
		{
			// �t���[���J�E���g�A�b�v
			m_InputWaitFrameCount[i]++;
			m_pCommandChartManager->SetInputCommandChart( (PLAYER_ID)i, false );
		}
	}

	// ���͊����`�F�b�N
	if( ( ( m_InputWaitFrameCount[PLAYER_1] > 0 ) && ( m_InputWaitFrameCount[PLAYER_2] > 0 ) )		// ���ғ��͂��������Ă����
		|| ( m_InputWaitFrameCount[PLAYER_1] > WAIT_INPUT_FRAMES )									// player1�����͊�������ҋ@�t���[���o��
		|| ( m_InputWaitFrameCount[PLAYER_2] > WAIT_INPUT_FRAMES ) )												// player2�̓��͊�������ҋ@�t���[���o��
	{
		// �W����������
		TYPE_RPS genre[PLAYER_MAX] = { RPS_NONE, RPS_NONE };
		for( int i = 0; i < PLAYER_MAX; i++ )
		{
			switch( m_Command[i] )
			{
			case -1:
				genre[i] = RPS_NONE;		// ��
				break;

			case COMMAND_TYPE_CHOP:
			case COMMAND_TYPE_ELBOW:
			case COMMAND_TYPE_LARIAT:
				genre[i] = RPS_SCISSOR;		// ��
				break;

			case COMMAND_TYPE_ROLLING:
			case COMMAND_TYPE_SHOULDER:
			case COMMAND_TYPE_DROPKICK:
				genre[i] = RPS_ROCK;		// ��
				break;

			case COMMAND_TYPE_SLAP:
			case COMMAND_TYPE_BACKDROP:
			case COMMAND_TYPE_STUNNER:
				genre[i] = RPS_PAPER;		// ��
				break;

			case COMMAND_TYPE_ROPE:			// ��R��苭��
				genre[i] = RPS_ROPE;
				break;

			case COMMAND_TYPE_FINISHER:
				genre[i] = RPS_FINISHER;	// ��S����苭��
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
			if( ( ( genre[PLAYER_1] == RPS_SCISSOR ) && ( genre[PLAYER_2] == RPS_PAPER ) )		// �`���L vs �p�[
			|| ( ( genre[PLAYER_1] == RPS_ROCK ) && ( genre[PLAYER_2] == RPS_SCISSOR ) )		// �O�[ vs �`���L
			|| ( ( genre[PLAYER_1] == RPS_PAPER ) && ( genre[PLAYER_2] == RPS_ROCK ) )			// �p�[ vs �O�[
			|| ( ( genre[PLAYER_1] == RPS_ROPE ) && ( genre[PLAYER_2] < RPS_ROPE ) )			// ���[�v vs ���[�v�ȉ�
			|| ( ( genre[PLAYER_1] == RPS_FINISHER ) && ( genre[PLAYER_2] < RPS_FINISHER ) )	// �t�B�j�b�V���[ vs �t�B�j�b�V���[�ȉ�
			)
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

		// ���������̃R�}���h�`���[�g�������@�i���[�v�ȊO�j
		if( m_Command[winnerID] != COMMAND_TYPE_ROPE )
		{
			m_pCommandChartManager->SetCommandChartMode( ( winnerID == PLAYER_1 ? PLAYER_2 : PLAYER_1 ), CCommandChart::MODE_VANISH );
		}
		
		// �R�}���h�[�`���[�g���͂𖳌�
		//m_pCommandChartManager->SetCommandChartMode( ( winnerID == PLAYER_1 ? PLAYER_2 : PLAYER_1 ), CCommandChart::MODE_INPUT );
		m_pCommandChartManager->SetInputCommandChart( false );

		switch( m_Command[winnerID] )
		{
		case COMMAND_TYPE_CHOP:
			m_pDirectorManager->Direct( DIR_SMALL_CHOP, winnerID );
			//m_pDirectorManager->Direct( DIR_FINISHER, winnerID );
			//m_pManager->GetDirectorManager()->Direct(DIR_BATTLE_RESULT, winnerID);
			break;

		case COMMAND_TYPE_ELBOW:
			m_pDirectorManager->Direct( DIR_SMALL_ELBOW, winnerID );
			break;

		case COMMAND_TYPE_LARIAT:
			m_pDirectorManager->Direct( DIR_SMALL_LARIAT, winnerID );
			break;

		case COMMAND_TYPE_ROLLING:
			m_pDirectorManager->Direct( DIR_BIG_ROLLING, winnerID );
			break;

		case COMMAND_TYPE_SHOULDER:
			m_pDirectorManager->Direct( DIR_BIG_SHOULDER, winnerID );
			break;

		case COMMAND_TYPE_DROPKICK:
			m_pDirectorManager->Direct( DIR_BIG_DROPKICK, winnerID );
			break;

		case COMMAND_TYPE_SLAP:
			m_pDirectorManager->Direct( DIR_THROW_SLAP, winnerID );
			break;

		case COMMAND_TYPE_BACKDROP:
			m_pDirectorManager->Direct( DIR_THROW_BACKDROP, winnerID );
			break;

		case COMMAND_TYPE_STUNNER:
			m_pDirectorManager->Direct( DIR_THROW_STUNNER, winnerID );
			break;

		case COMMAND_TYPE_FINISHER:
			m_pDirectorManager->Direct( DIR_FINISHER, winnerID );
			break;

		case COMMAND_TYPE_ROPE:
			m_pDirectorManager->Direct( DIR_ROPE, winnerID );
			break;
		}
	}



	
/*	// �ȈՃR�}���h�A�e�X�g�p
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_LEFT_UP ) )
	{
		m_pDirectorManager->Direct( DIR_SMALL_CHOP, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_LEFT_DOWN ) )
	{
		m_pDirectorManager->Direct( DIR_SMALL_ELBOW, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_Z ) )
	{
		m_pDirectorManager->Direct( DIR_SMALL_LARIAT, PLAYER_1 );
	}

	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_RIGHT_UP ) )
	{
		m_pDirectorManager->Direct( DIR_BIG_ROLLING, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_RIGHT_DOWN ) )
	{
		m_pDirectorManager->Direct( DIR_BIG_SHOULDER, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_X ) )
	{
		m_pDirectorManager->Direct( DIR_BIG_DROPKICK, PLAYER_1 );
	}

	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_E ) )
	{
		m_pDirectorManager->Direct( DIR_THROW_SLAP, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_D ) )
	{
		m_pDirectorManager->Direct( DIR_THROW_BACKDROP, PLAYER_1 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_1_C ) )
	{
		m_pDirectorManager->Direct( DIR_THROW_STUNNER, PLAYER_1 );
	}


	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_LEFT_UP ) )
	{
		m_pDirectorManager->Direct( DIR_SMALL_CHOP, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_LEFT_DOWN ) )
	{
		m_pDirectorManager->Direct( DIR_SMALL_ELBOW, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_COMMA ) )
	{
		m_pDirectorManager->Direct( DIR_SMALL_LARIAT, PLAYER_2 );
	}

	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_RIGHT_UP ) )
	{
		m_pDirectorManager->Direct( DIR_BIG_ROLLING, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_RIGHT_DOWN ) )
	{
		m_pDirectorManager->Direct( DIR_BIG_SHOULDER, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_PERIOD ) )
	{
		m_pDirectorManager->Direct( DIR_BIG_DROPKICK, PLAYER_2 );
	}

	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_P ) )
	{
		m_pDirectorManager->Direct( DIR_THROW_SLAP, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_SEMICOLON ) )
	{
		m_pDirectorManager->Direct( DIR_THROW_BACKDROP, PLAYER_2 );
	}
	if( CInputKeyboard::GetKeyboardTrigger( KEYBOARD_CODE_PLAYER_2_SLASH ) )
	{
		m_pDirectorManager->Direct( DIR_THROW_STUNNER, PLAYER_2 );
	}
*/
	m_IsDirectingOld = m_pDirectorManager->GetIsDirecting();
}

//----EOF----