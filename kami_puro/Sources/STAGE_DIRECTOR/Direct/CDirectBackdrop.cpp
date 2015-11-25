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

const D3DXVECTOR3 BACKDROP_HIT_OFFSET[2] = {
	D3DXVECTOR3( 40.0f, 60.0f, 0.0f ),
	D3DXVECTOR3( -40.0f, 60.0f, 0.0f )
};

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
		m_pManager->GetPlayerManager()->GetPlayerPos( m_Player ),
		m_pManager->GetPlayerManager()->GetPlayerPos( m_Enemy )
	};

	D3DXVECTOR3 rot[2] = {
		m_pManager->GetPlayerManager()->GetPlayerRot( m_Player ),
		m_pManager->GetPlayerManager()->GetPlayerRot( m_Enemy )
	};

	/* ここから個別 */
	switch( m_FrameCount )
	{
		// フレーム別の処理
	case 0:
		m_pManager->GetPlayerManager()->SetAnimType( m_Player, CPlayer::PLAYER_BACKDROP );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), VECTOR3_ZERO, D3DXVECTOR3( 10.0f, 10.0f, 10.0f ) );
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_BACKDROP_DAMAGE );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, 1 );
		m_pManager->GetPlayerManager()->SetRot( m_Enemy, rot[m_Enemy] + D3DXVECTOR3( 0.0f, D3DXToRadian( 180.0f ), 0.0f ) );
		m_pManager->GetCameraManager()->CameraMoveToCoord(
			pos[m_Player] + D3DXVECTOR3( 0.0f, 100.0f, -100.0f ),
			pos[m_Player] + D3DXVECTOR3( -40.0f, 200.0f, -140.0f ),
			pos[m_Player] + D3DXVECTOR3( 0.0f, 70.0f, 0.0f ),
			pos[m_Player] + D3DXVECTOR3( -40.0f, 0.0f, 0.0f ),
			100 );
		break;
	
	case 90:
		m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_BACKDROP_DAMAGE );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, 19 );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + BACKDROP_HIT_OFFSET[m_Player], VECTOR3_ZERO, D3DXVECTOR3( 3.0f, 3.0f, 3.0f ) );
		break;
	}
	/* ここまで個別 */



	CDirect::CheckEnd();
}

//----EOF----