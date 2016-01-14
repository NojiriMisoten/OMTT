//=============================================================================
//
// CDirectRopeクラス [CDirectRope.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectRope.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"
#include "../../SCENE/GAME/CGame.h"
#include "../../SCENE/GAME/FIELD/CFieldManager.h"
#include "../../SCENE/GAME/FIELD/ROPE/CRopeManager.h"
#include "../../SOUND/CSound.h"
#include "../../SCENE/GAME/COMMANDCHART/CCommandChartManager.h"

const D3DXVECTOR3 ROPE_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 ROPE_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 ROPE_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 ROPE_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int ROPE_DAMAGE = 10;

//=================================================
// コンストラクタ
//=================================================
CDirectRope::CDirectRope(CManager *pManager, CGame *pGame) : CDirect( pManager, pGame )
{
	
}

//=================================================
// デストラクタ
//=================================================
CDirectRope::~CDirectRope( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectRope::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 190;		// 技ごとに別

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();
	m_pUIManager = m_pManager->GetUiManager();

	CDirect::SetPlayerID( playerID );
}

//=================================================
// 終了
//=================================================
void CDirectRope::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirectRope::Update( void )
{
	D3DXVECTOR3 pos[2] = {
		m_pPlayerManager->GetPlayerPos( PLAYER_1 ),
		m_pPlayerManager->GetPlayerPos( PLAYER_2 )
	};

	D3DXVECTOR3 rot[2] = {
		m_pPlayerManager->GetPlayerRot( PLAYER_1 ),
		m_pPlayerManager->GetPlayerRot( PLAYER_2 )
	};

	/* ここから個別 */
	switch( m_FrameCount )
	{
		// フレーム別の処理
	case 0:
		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_ROPE );
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.6f );
		m_pPlayerManager->SetAnimType(m_Enemy, CPlayer::PLAYER_APPEAL05);
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.6f );
		//CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, ROPE_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)ROPE_EFFECT_AURA_SCALE );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 160.0f, 150.0f, -20.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 160.0f, 150.0f, 20.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			150 );
		m_pUIManager->GetCommandChartManager()->SetCommandChartMode(m_Player, CCommandChart::MODE_VANISH);
		m_pUIManager->GetCommandChartManager()->SetCommandChartMode(m_Enemy, CCommandChart::MODE_VANISH);

		m_pManager->PlaySoundA(SOUND_LABEL_SE_SLOW);
		break;

	case 40:
		m_pUIManager->StartRopeTimer( 30, 120 );
		m_pUIManager->GetCommandChartManager()->SetInputCommandChart( true );
		m_pUIManager->GetCommandChartManager()->SetInputCommandChart( true );
		m_pUIManager->GetCommandChartManager()->StartOpenAnimeForRope(PLAYER_1);
		m_pUIManager->GetCommandChartManager()->StartOpenAnimeForRope(PLAYER_2);
		m_pUIManager->GetCommandChartManager()->SetCommandChartMode( m_Player, CCommandChart::MODE_APPEAR );
		m_pUIManager->GetCommandChartManager()->SetCommandChartMode( m_Enemy, CCommandChart::MODE_APPEAR );
		break;

	case 50:
		if (m_Player == PLAYER_1)
		{
			m_pGame->GetFieldManager()->GetRopeManager()->Pull(CRopeManager::RopeNumLeft, 3.0f, 80);
		}
		else
		{
			m_pGame->GetFieldManager()->GetRopeManager()->Pull(CRopeManager::RopeNumRight, 3.0f, 80);
		}
		break;

	case 90:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.05f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.6f );
		
		break;

	case 179:
		//m_pPlayerManager->SetAnimType(m_Enemy, CPlayer::PLAYER_WAIT);

		// 技判定
		m_CommandPlayer[0] = m_pUIManager->GetCommandChartManager()->GetCommandChartTechnic(0);
		m_CommandPlayer[1] = m_pUIManager->GetCommandChartManager()->GetCommandChartTechnic(1);

		if (m_CommandPlayer[0] == m_CommandPlayer[1])
		{
			// どちらも攻撃してない
			// そのまま抜ける
			m_WinnerID = PLAYER_MAX;
		}
		else if (((m_CommandPlayer[0] == COMMAND_TYPE_LARIAT) && (m_CommandPlayer[1] == COMMAND_TYPE_STUNNER))
			|| ((m_CommandPlayer[0] == COMMAND_TYPE_DROPKICK) && (m_CommandPlayer[1] == COMMAND_TYPE_LARIAT))
			|| ((m_CommandPlayer[0] == COMMAND_TYPE_STUNNER) && (m_CommandPlayer[1] == COMMAND_TYPE_DROPKICK))
			|| ((m_CommandPlayer[0] != COMMAND_TYPE_NONE) && (m_CommandPlayer[1] == COMMAND_TYPE_NONE)))
		{
			// プレイヤー１勝利
			m_WinnerID = PLAYER_1;
		}
		else if (((m_CommandPlayer[1] == COMMAND_TYPE_LARIAT) && (m_CommandPlayer[0] == COMMAND_TYPE_STUNNER))
			|| ((m_CommandPlayer[1] == COMMAND_TYPE_DROPKICK) && (m_CommandPlayer[0] == COMMAND_TYPE_LARIAT))
			|| ((m_CommandPlayer[1] == COMMAND_TYPE_STUNNER) && (m_CommandPlayer[0] == COMMAND_TYPE_DROPKICK))
			|| ((m_CommandPlayer[1] != COMMAND_TYPE_NONE) && (m_CommandPlayer[0] == COMMAND_TYPE_NONE)))
		{
			// プレイヤー2勝利
			m_WinnerID = PLAYER_2;
		}

		if (m_WinnerID != PLAYER_MAX)
		{
			m_LoserID = (m_WinnerID == PLAYER_1 ? PLAYER_2 : PLAYER_1);
			m_pUIManager->GetCommandChartManager()->SetCommandChartMode(PLAYER_1, CCommandChart::MODE_VANISH);
			m_pUIManager->GetCommandChartManager()->SetCommandChartMode(PLAYER_2, CCommandChart::MODE_VANISH);
			//m_pUIManager->GetCommandChartManager()->SetCommandChartMode( ( m_WinnerID == PLAYER_1 ? PLAYER_2 : PLAYER_1 ), CCommandChart::MODE_VANISH );
		}
		break;

		// ここで何も入力がなかった時用にリセットする必要がある
		m_pUIManager->GetCommandChartManager()->SetCommandChartMode(PLAYER_1, CCommandChart::MODE_VANISH);
		m_pUIManager->GetCommandChartManager()->SetCommandChartMode(PLAYER_2, CCommandChart::MODE_VANISH);
	case 180:
		if (m_WinnerID != PLAYER_MAX)
		{
			m_pManager->GetDirectorManager()->SetEndDirecting();
			switch (m_CommandPlayer[m_WinnerID])
			{
			case COMMAND_TYPE_LARIAT:
				m_pManager->GetDirectorManager()->Direct(DIR_SMALL_LARIAT, m_WinnerID);
				break;

			case COMMAND_TYPE_DROPKICK:
				m_pManager->GetDirectorManager()->Direct(DIR_BIG_DROPKICK, m_WinnerID);
				break;

			case COMMAND_TYPE_STUNNER:
				m_pManager->GetDirectorManager()->Direct(DIR_THROW_STUNNER, m_WinnerID);
				break;
			}
		}
		else
		{
			m_pUIManager->GetCommandChartManager()->SetInputCommandChart(true);
			m_pUIManager->GetCommandChartManager()->SetCommandChartMode(m_Player, CCommandChart::MODE_INPUT);
			m_pUIManager->GetCommandChartManager()->SetCommandChartMode(m_Enemy, CCommandChart::MODE_INPUT);
		}
		break;

	case 190:
		// リセット続き
		m_pUIManager->GetCommandChartManager()->SetCommandChartMode(PLAYER_1, CCommandChart::MODE_APPEAR);
		m_pUIManager->GetCommandChartManager()->SetCommandChartMode(PLAYER_2, CCommandChart::MODE_APPEAR);
		m_pUIManager->GetCommandChartManager()->ResetCommandList(PLAYER_1);
		m_pUIManager->GetCommandChartManager()->ResetCommandList(PLAYER_2);
		break;
	}


	/* ここまで個別 */






	/* ここまで個別 */



	CDirect::CheckEnd();
}

//----EOF----