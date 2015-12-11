//=============================================================================
//
// CDirectSlapクラス [CDirectSlap.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectSlap.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 SLAP_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 SLAP_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 SLAP_EFFECT_GRAB_OFFSET = D3DXVECTOR3( 40.0f, 70.0f, 0.0f );
const D3DXVECTOR3 SLAP_EFFECT_GRAB_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const D3DXVECTOR3 SLAP_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 70.0f, 0.0f );
const D3DXVECTOR3 SLAP_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int SLAP_DAMAGE1 = 10 * DAMAGE_AMP;
const int SLAP_DAMAGE2 = 40 * DAMAGE_AMP;

//=================================================
// コンストラクタ
//=================================================
CDirectSlap::CDirectSlap( CManager *pManager, CGame *pGame ) : CDirect( pManager, pGame )
{
	
}

//=================================================
// デストラクタ
//=================================================
CDirectSlap::~CDirectSlap( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectSlap::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 130;		// 技ごとに別

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();

	CDirect::SetPlayerID( playerID );
}

//=================================================
// 終了
//=================================================
void CDirectSlap::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirectSlap::Update( void )
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
		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_SLAPPING_RIGHT );
		m_pPlayerManager->SetAnimType( m_Enemy, CPlayer::PLAYER_SLAPPING_DAMAGE_RIGHT );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, SLAP_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)SLAP_EFFECT_AURA_SCALE );
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.7f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.7f );

		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( -30.0f, 90.0f, -70.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 30.0f, 90.0f, -70.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 60.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 60.0f, 0.0f ) ),
			80 );
		break;

	case 20:
		CEffect::Create( 30, EFFECT_DAGEKI_TYU, false, pos[m_Player] + TranslateCoord( m_Player, SLAP_EFFECT_GRAB_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)SLAP_EFFECT_GRAB_SCALE );
		m_pPlayerManager->TakeDamage( m_Enemy, SLAP_DAMAGE1 );
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 1.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 1.0f );
		break;

	case 60:
		m_pCameraManager->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pPlayerManager->TakeDamage( m_Enemy, SLAP_DAMAGE2 );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, SLAP_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)SLAP_EFFECT_HIT_SCALE );
		break;
	}
	/* ここまで個別 */



	CDirect::CheckEnd();
}

//----EOF----