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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const float INITIATE_BATTLE_MODE_DISTANCE = 50.0f;		// バトルモードに移行する距離

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
	m_BattleMode = BATTLE_MOVE;
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
	// 移動モードであれば
	if( m_BattleMode == BATTLE_MOVE )
	{
		// プレイヤー座標取得
		D3DXVECTOR3 playerPos[PLAYER_MAX];
		playerPos[PLAYER_1] = m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_1 );
		playerPos[PLAYER_2] = m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_2 );
		D3DXVECTOR3 playerCenter = ( playerPos[PLAYER_1] + playerPos[PLAYER_2] ) / 2;			// プレイヤー同士の中心点

		// プレイヤー同士の距離を取得
		float playerDist[PLAYER_MAX];
		playerDist[PLAYER_1] = MagnitudeVector( playerPos[PLAYER_1] );
		playerDist[PLAYER_2] = MagnitudeVector( playerPos[PLAYER_2] );
		float playerDistance = MagnitudeVector( playerPos[PLAYER_1] - playerPos[PLAYER_2] );	// プレイヤー同士の距離

		// カメラ移動
		m_pManager->GetCameraManager()->CameraSetToCoord(
			D3DXVECTOR3( playerDist[0] - playerDist[1], 200.0f, -200.0f ),
			playerCenter );

		// プレイヤーが近づけば
		if( playerDistance < INITIATE_BATTLE_MODE_DISTANCE )
		{
			// 戦闘モードに移行
			m_BattleMode = BATTLE_FIGHT;

			// カメラ移動、たぶんCDirectorに移動する
			m_pManager->GetCameraManager()->CameraMoveToCoord(
				m_pManager->GetCameraManager()->GetCameraPos(),
				D3DXVECTOR3( playerCenter.x, 90.0f, -100.0f ),
				m_pManager->GetCameraManager()->GetPosRCamera(),
				D3DXVECTOR3( playerCenter.x, 70.0f, 0.0f ),
				30 );
		}
	}


	// 戦闘モードであれば
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
// 作成
//=============================================================================
CJudge* CJudge::Create( CManager* pManager )
{
	CJudge* p = new CJudge;
	p->Init( pManager );
	return p;
}



//----EOF----