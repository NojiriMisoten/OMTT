//=============================================================================
//
// CDirectShoulderクラス [CDirectShoulder.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectShoulder.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 SHOULDER_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 SHOULDER_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 SHOULDER_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 SHOULDER_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int SHOULDER_DAMAGE = 20;

//=================================================
// コンストラクタ
//=================================================
CDirectShoulder::CDirectShoulder(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// デストラクタ
//=================================================
CDirectShoulder::~CDirectShoulder( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectShoulder::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 120;		// 技ごとに別

	CDirect::SetPlayerID( playerID );
}

//=================================================
// 終了
//=================================================
void CDirectShoulder::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirectShoulder::Update( void )
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
		m_pManager->GetPlayerManager()->SetAnimType( m_Player, CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_LEFT );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, SHOULDER_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)SHOULDER_EFFECT_AURA_SCALE );
		m_pManager->GetCameraManager()->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 150.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 300.0f, -200.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 100.0f, 0.0f, 0.0f ) ),
			100 );
		break;
	
	case 40:
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_DAMAGE_LEFT );
		m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, SHOULDER_DAMAGE );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, SHOULDER_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)SHOULDER_EFFECT_HIT_SCALE );
		break;

	case 70:
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Player, 0.0f );
		m_pManager->GetPlayerManager()->SetAnimSpd( m_Enemy, 0.0f );
		break;
	}
	/* ここまで個別 */



	CDirect::CheckEnd();
}

//----EOF----