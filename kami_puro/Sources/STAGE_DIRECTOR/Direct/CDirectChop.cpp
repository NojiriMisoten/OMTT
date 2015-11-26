//=============================================================================
//
// CDirectChopクラス [CDirectChop.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectChop.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"

const D3DXVECTOR3 CHOP_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 CHOP_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.f );

const D3DXVECTOR3 CHOP_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 CHOP_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int CHOP_DAMAGE = 5;

//=================================================
// コンストラクタ
//=================================================
CDirectChop::CDirectChop(CManager *pManager) : CDirect( pManager )
{
	
}

//=================================================
// デストラクタ
//=================================================
CDirectChop::~CDirectChop( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectChop::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 120;		// 技ごとに別

	CDirect::SetPlayerID( playerID );
}

//=================================================
// 終了
//=================================================
void CDirectChop::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirectChop::Update( void )
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
		m_pManager->GetPlayerManager()->SetAnimType( m_Player, CPlayer::PLAYER_CHOP_LEFT );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, CHOP_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)CHOP_EFFECT_AURA_SCALE );
		break;
	
	case 50:
		m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 3.0f, 10, 0 );
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_DAMAGE_SMALL );
		m_pManager->GetPlayerManager()->TakeDamage( m_Enemy, CHOP_DAMAGE );
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, CHOP_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)CHOP_EFFECT_HIT_SCALE );
		break;

	case 110:
		m_pManager->GetPlayerManager()->SetAnimType( m_Enemy, CPlayer::PLAYER_WAIT );
		break;
	}
	/* ここまで個別 */



	CDirect::CheckEnd();
}

//----EOF----