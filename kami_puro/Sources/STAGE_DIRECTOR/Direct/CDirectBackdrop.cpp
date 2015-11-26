//=============================================================================
//
// CDirectBackdropクラス [CDirectBackdrop.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectBackdrop.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 BACKDROP_ROT_OFFSET = D3DXVECTOR3( 0.0f, D3DXToRadian( 180.0f ), 0.0f );

const D3DXVECTOR3 BACKDROP_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 BACKDROP_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 BACKDROP_EFFECT_SLAM_OFFSET = D3DXVECTOR3( -90.0f, 0.0f, 15.0f );
const D3DXVECTOR3 BACKDROP_EFFECT_SLAM_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const int BACKDROP_DAMAGE1 = 1;
const int BACKDROP_DAMAGE2 = 19;

//=================================================
// コンストラクタ
//=================================================
CDirectBackdrop::CDirectBackdrop(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// デストラクタ
//=================================================
CDirectBackdrop::~CDirectBackdrop( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectBackdrop::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 150;		// 技ごとに別

	CDirect::SetPlayerID( playerID );
}

//=================================================
// 終了
//=================================================
void CDirectBackdrop::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirectBackdrop::Update( void )
{
	D3DXVECTOR3 pos[2] = {
		m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_1 ),
		m_pManager->GetPlayerManager()->GetPlayerPos( PLAYER_2 )
	};

	D3DXVECTOR3 rot[2] = {
		m_pManager->GetPlayerManager()->GetPlayerRot( PLAYER_1 ),
		m_pManager->GetPlayerManager()->GetPlayerRot( PLAYER_2 )
	};

	/* ここから個別 */
	switch( m_FrameCount )
	{
		// フレーム別の処理
	case 0:
		m_pManager->GetPlayerManager()->SetAnimType( m_Player, CPlayer::PLAYER_BACKDROP );
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_BACKDROP_DAMAGE );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, BACKDROP_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)BACKDROP_EFFECT_AURA_SCALE );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, BACKDROP_DAMAGE1 );
		m_pManager->GetPlayerManager()->SetRot( m_Enemy, rot[m_Enemy] + TranslateCoord( m_Enemy, BACKDROP_ROT_OFFSET ) );
		m_pManager->GetCameraManager()->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 100.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( -40.0f, 200.0f, -140.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( -40.0f, 0.0f, 0.0f ) ),
			100 );
		break;
	
	case 90:
		m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, BACKDROP_DAMAGE1 );
		CEffect::Create( 30, EFFECT_SHOCK_WAVE, false, pos[m_Player] + TranslateCoord( m_Player, BACKDROP_EFFECT_SLAM_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)BACKDROP_EFFECT_SLAM_SCALE );
		break;
	}
	/* ここまで個別 */



	CDirect::CheckEnd();
}

//----EOF----