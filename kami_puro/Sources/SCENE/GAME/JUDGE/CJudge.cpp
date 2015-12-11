//=============================================================================
//
// CJudgeクラス [CJudge.cpp]
// Author : 池島　大樹
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CJudge.h"
#include "../../../MANAGER/CManager.h"
#include "../PLAYER/CPlayerManager.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../STAGE_DIRECTOR/CDirectorManager.h"
#include "../UI/CUiManager.h"
#include "../COMMANDCHART/CCommandChartManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const float INITIATE_BATTLE_MODE_DISTANCE = 50.0f;		// バトルモードに移行する距離
static const int WAIT_INPUT_FRAMES = 30;

//=============================================================================
// コンストラクタ
//=============================================================================
CJudge::CJudge( void )
{
	m_pManager = NULL;
	m_BattleMode = (BATTLE_MODE)-1;
}

//=============================================================================
// デストラクタ
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
// 終了
//=============================================================================
void CJudge::Uninit( void )
{

}

//=============================================================================
// 更新
//=============================================================================
void CJudge::Update( void )
{
	// プレイヤー座標取得
	m_Pos[PLAYER_1] = m_pPlayerManager->GetPlayerPos( PLAYER_1 );
	m_Pos[PLAYER_2] = m_pPlayerManager->GetPlayerPos( PLAYER_2 );
	m_PosCenter = ( m_Pos[PLAYER_1] + m_Pos[PLAYER_2] ) / 2;			// プレイヤー同士の中心点

	// プレイヤー同士の距離を取得
	m_Dist[PLAYER_1] = MagnitudeVector( m_Pos[PLAYER_1] );
	m_Dist[PLAYER_2] = MagnitudeVector( m_Pos[PLAYER_2] );
	m_DistTotal = MagnitudeVector( m_Pos[PLAYER_1] - m_Pos[PLAYER_2] );	// プレイヤー同士の距離

	// モード更新処理
	if( m_BattleMode == m_BattleModeOld )
	{
		switch( m_BattleMode )
		{
		case BATTLE_MOVE:	// 移動モードであれば
			BattleMoveUpdate();
			break;

		case BATTLE_FIGHT:	// 戦闘モードであれば
			BattleFightUpdate();
			break;
		}
	}

	// モード切替時の処理
	if( m_BattleMode != m_BattleModeOld )
	{
		switch( m_BattleModeOld )
		{
			// 移動モード終了時
		case BATTLE_MOVE:
			break;
			
			// 戦闘モード終了時
		case BATTLE_FIGHT:
			break;
		}


		switch( m_BattleMode )
		{
			// 移動モード開始時
		case BATTLE_MOVE:
			// コマンドチャート消滅
			m_pCommandChartManager->SetCommandChartMode( PLAYER_1, CCommandChart::MODE_VANISH );
			m_pCommandChartManager->SetCommandChartMode( PLAYER_2, CCommandChart::MODE_VANISH );
			m_pCommandChartManager->SetInputCommandChart( false );
			break;

			// 戦闘モード開始時
		case BATTLE_FIGHT:
			// コマンドチャート表示
			m_Pos[PLAYER_1].y = 0.0f;
			m_Pos[PLAYER_2].y = 0.0f;
			m_pPlayerManager->SetPos( PLAYER_1, m_Pos[PLAYER_1] );
			m_pPlayerManager->SetPos( PLAYER_2, m_Pos[PLAYER_2] );
			m_pCommandChartManager->SetCommandChartMode( PLAYER_1, CCommandChart::MODE_APPEAR );
			m_pCommandChartManager->SetCommandChartMode( PLAYER_2, CCommandChart::MODE_APPEAR );
			m_pCommandChartManager->SetInputCommandChart( true );

			// カメラ移動、たぶんCDirectorに移動する
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
// 作成
//=============================================================================
CJudge* CJudge::Create( CManager* pManager )
{
	CJudge* p = new CJudge;
	p->Init( pManager );
	return p;
}

void CJudge::BattleMoveUpdate( void )
{
	// プレイヤーが近づけば
	if( m_DistTotal <= INITIATE_BATTLE_MODE_DISTANCE )
	{
		// 戦闘モードに移行
		m_BattleMode = BATTLE_FIGHT;
	}

	// カメラ移動
	m_pManager->GetCameraManager()->CameraSetToCoord(
		D3DXVECTOR3( m_Dist[0] - m_Dist[1], 200.0f, -200.0f ),
		m_PosCenter );
}


void CJudge::BattleFightUpdate( void )
{
	// 演出が終了したら
	if( m_IsDirectingOld != m_pDirectorManager->GetIsDirecting() )
	{
		switch( m_pDirectorManager->GetIsDirecting() )
		{
		case -1:	// 演出終了時
			CCommandChart::MODE_COMMAND_CHART mode[PLAYER_MAX];
			mode[0] = m_pCommandChartManager->GetCommandChartMode( 0 );
			mode[1] = m_pCommandChartManager->GetCommandChartMode( 1 );
			
			// コマンドチャート表示
			for( int i = 0; i < PLAYER_MAX; i++ )
			{
				// すでに表示されていれば
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
		
		default:	// 演出開始時
			// コマンドチャート消滅
//			m_pCommandChartManager->SetCommandChartMode( PLAYER_1, CCommandChart::MODE_VANISH );
//			m_pCommandChartManager->SetCommandChartMode( PLAYER_2, CCommandChart::MODE_VANISH );
			break;
		}
	}

	// コマンド入力チェック（見つかったら探さない）
	for( int i = 0; i < PLAYER_MAX; i++ )
	{
		if( m_InputWaitFrameCount[i] == 0 )
		{
			m_Command[i] = m_pCommandChartManager->GetCommandChartTechnic( i );
		}
	}

	// コマンド入力が完成していればフレームカウント、入力不可に
	for( int i = 0; i < PLAYER_MAX; i++ )
	{
		if( m_Command[i] != COMMAND_TYPE_NONE )
		{
			// フレームカウントアップ
			m_InputWaitFrameCount[i]++;
			m_pCommandChartManager->SetInputCommandChart( (PLAYER_ID)i, false );
		}
	}

	// 入力完成チェック
	if( ( ( m_InputWaitFrameCount[PLAYER_1] > 0 ) && ( m_InputWaitFrameCount[PLAYER_2] > 0 ) )		// 両者入力が完成していれば
		|| ( m_InputWaitFrameCount[PLAYER_1] > WAIT_INPUT_FRAMES )									// player1が入力完成から待機フレーム経過
		|| ( m_InputWaitFrameCount[PLAYER_2] > WAIT_INPUT_FRAMES ) )												// player2の入力完成から待機フレーム経過
	{
		// ジャンル分け
		TYPE_RPS genre[PLAYER_MAX] = { RPS_NONE, RPS_NONE };
		for( int i = 0; i < PLAYER_MAX; i++ )
		{
			switch( m_Command[i] )
			{
			case -1:
				genre[i] = RPS_NONE;		// 無
				break;

			case COMMAND_TYPE_CHOP:
			case COMMAND_TYPE_ELBOW:
			case COMMAND_TYPE_LARIAT:
				genre[i] = RPS_SCISSOR;		// 弱
				break;

			case COMMAND_TYPE_ROLLING:
			case COMMAND_TYPE_SHOULDER:
			case COMMAND_TYPE_DROPKICK:
				genre[i] = RPS_ROCK;		// 強
				break;

			case COMMAND_TYPE_SLAP:
			case COMMAND_TYPE_BACKDROP:
			case COMMAND_TYPE_STUNNER:
				genre[i] = RPS_PAPER;		// 投
				break;

			case COMMAND_TYPE_ROPE:			// 上３つより強い
				genre[i] = RPS_ROPE;
				break;

			case COMMAND_TYPE_FINISHER:
				genre[i] = RPS_FINISHER;	// 上全部より強い
				break;
			}
		}
		
		// 強弱判定
		PLAYER_ID winnerID = PLAYER_MAX;	// 勝者ＩＤ
		bool isWon = false;					// 勝者決定フラグ

		// 片方が攻撃失敗していれば
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

		// 同種勝ち判定
		if( !isWon )
		{
			if( genre[PLAYER_1] == genre[PLAYER_2] )
			{
				// 大きいほうが勝ち
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

		// じゃんけん勝ちを判定（じゃんけん）
		if( !isWon ){
			if( ( ( genre[PLAYER_1] == RPS_SCISSOR ) && ( genre[PLAYER_2] == RPS_PAPER ) )		// チョキ vs パー
			|| ( ( genre[PLAYER_1] == RPS_ROCK ) && ( genre[PLAYER_2] == RPS_SCISSOR ) )		// グー vs チョキ
			|| ( ( genre[PLAYER_1] == RPS_PAPER ) && ( genre[PLAYER_2] == RPS_ROCK ) )			// パー vs グー
			|| ( ( genre[PLAYER_1] == RPS_ROPE ) && ( genre[PLAYER_2] < RPS_ROPE ) )			// ロープ vs ロープ以下
			|| ( ( genre[PLAYER_1] == RPS_FINISHER ) && ( genre[PLAYER_2] < RPS_FINISHER ) )	// フィニッシャー vs フィニッシャー以下
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

		// 先行勝ち判定
		if( !isWon )
		{
			if( m_Command[PLAYER_1] == m_Command[PLAYER_2] )
			{
				// 先に入力が完成している方が勝ち
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

		// フレームカウントリセット
		for( int i = 0; i < PLAYER_MAX; i++ )
		{
			m_InputWaitFrameCount[i] = 0;
		}

		// 負けた方のコマンドチャートを消す　（ロープ以外）
		if( m_Command[winnerID] != COMMAND_TYPE_ROPE )
		{
			m_pCommandChartManager->SetCommandChartMode( ( winnerID == PLAYER_1 ? PLAYER_2 : PLAYER_1 ), CCommandChart::MODE_VANISH );
		}
		
		// コマンドーチャート入力を無効
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



	
/*	// 簡易コマンド、テスト用
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