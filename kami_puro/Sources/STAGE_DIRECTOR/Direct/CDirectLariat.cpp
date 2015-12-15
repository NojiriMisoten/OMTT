//=============================================================================
//
// CDirectLariatクラス [CDirectLariat.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectLariat.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 LARIAT_POS_OFFSET = D3DXVECTOR3( -10.0f, 0.0f, 0.0f );
const D3DXVECTOR3 LARIAT_ROT_OFFSET = D3DXVECTOR3( 0.0f, D3DXToRadian( -20.0f ), 0.0f );

const D3DXVECTOR3 LARIAT_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 LARIAT_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 LARIAT_EFFECT_CHARGE1_OFFSET = D3DXVECTOR3( 0.0f, 60.0f, -15.0f );
const D3DXVECTOR3 LARIAT_EFFECT_CHARGE2_OFFSET = D3DXVECTOR3( 0.0f, 60.0f, 15.0f );
const D3DXVECTOR3 LARIAT_EFFECT_CHARGE_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 LARIAT_EFFECT_HIT_OFFSET = D3DXVECTOR3( +60.0f, +60.0f, 0.0f );
const D3DXVECTOR3 LARIAT_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const D3DXVECTOR3 LARIAT_EFFECT_SLAM_OFFSET = D3DXVECTOR3( -20.0f, 0.0f, 0.0f );
const D3DXVECTOR3 LARIAT_EFFECT_SLAM_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const int LARIAT_DAMAGE = (int)( 100 * DAMAGE_AMP );
const int LARIAT_TENSION = (int)( 10 * TENSION_AMP );

//=================================================
// コンストラクタ
//=================================================
CDirectLariat::CDirectLariat( CManager *pManager, CGame *pGame ) : CDirect( pManager, pGame )
{
	
}

//=================================================
// デストラクタ
//=================================================
CDirectLariat::~CDirectLariat( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectLariat::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 200;		// 技ごとに別

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();
	m_pUIManager = m_pManager->GetUiManager();

	CDirect::SetPlayerID( playerID );
}

//=================================================
// 終了
//=================================================
void CDirectLariat::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirectLariat::Update( void )
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
		m_pUIManager->StartCutIn( m_Player, CUT_IN_JIJII );

		m_pPlayerManager->SetPos( m_Player, pos[m_Player] + TranslateCoord( m_Player, LARIAT_POS_OFFSET ) );
		m_pPlayerManager->SetRot( m_Player, rot[m_Player] + TranslateCoord( m_Player, LARIAT_ROT_OFFSET ) );
		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_LARIAT_LEFT );
		m_pPlayerManager->SetAnimType( m_Enemy, CPlayer::PLAYER_LARIAT_DAMAGE );
		m_pPlayerManager->SetAnimSpd( m_Player, 0.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, 0.0f );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 140.0f, 90.0f, -20.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 140.0f, 90.0f, -20.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			30 );
		break;

	case 30:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.33f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.33f );
		//CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, LARIAT_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)LARIAT_EFFECT_AURA_SCALE );
		if( m_Player == PLAYER_1 )
		{
			CEffect::Create( 60, EFFECT_CHARGE, false, pos[m_Player] + TranslateCoord( m_Player, LARIAT_EFFECT_CHARGE1_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)LARIAT_EFFECT_CHARGE_SCALE );
		}
		if( m_Player == PLAYER_2 )
		{
			CEffect::Create( 60, EFFECT_CHARGE, false, pos[m_Player] + TranslateCoord( m_Player, LARIAT_EFFECT_CHARGE2_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)LARIAT_EFFECT_CHARGE_SCALE );
		}
			m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 140.0f, 90.0f, -20.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 140.0f, 90.0f, 20.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			60 );
		break;

	case 90:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 1.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 1.0f );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 90.0f, 200.0f, -10.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 80.0f, 100.0f, -10.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 90.0f, 20.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 80.0f, 20.0f, 0.0f ) ),
			120 );
		break;

	case 100:
		m_pCameraManager->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pPlayerManager->TakeDamage( m_Enemy, LARIAT_DAMAGE );
		m_pPlayerManager->AddTension( m_Player, LARIAT_TENSION );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, LARIAT_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)LARIAT_EFFECT_HIT_SCALE );
		break;

	case 110:
		CEffect::Create( 30, EFFECT_SHOCK_WAVE, false, pos[m_Enemy] + TranslateCoord( m_Enemy, LARIAT_EFFECT_SLAM_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)LARIAT_EFFECT_SLAM_SCALE );
		break;

	case 170:
		m_pPlayerManager->SetAnimSpd( m_Player, 0.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, 0.0f );
		break;
	}

	// バトルフェードスタート
	if (m_FrameCount == m_TotalFrame)
	{
		m_pManager->GetUiManager()->StartBattleFade();
	}

	/* ここまで個別 */

	CDirect::CheckEnd();
}

//----EOF----