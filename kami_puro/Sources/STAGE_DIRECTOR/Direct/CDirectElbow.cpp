//=============================================================================
//
// CDirectElbowクラス [CDirectElbow.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectElbow.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"
#include "../../SOUND/CSound.h"
#include "../../MATH/mersenne_twister.h"

const D3DXVECTOR3 ELBOW_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 ELBOW_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 ELBOW_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 ELBOW_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int ELBOW_DAMAGE = (int)( 50 * DAMAGE_AMP );
const int ELBOW_TENSION = (int)( 3 * TENSION_AMP );

//=================================================
// コンストラクタ
//=================================================
CDirectElbow::CDirectElbow( CManager *pManager, CGame *pGame ) : CDirect( pManager, pGame )
{
	
}

//=================================================
// デストラクタ
//=================================================
CDirectElbow::~CDirectElbow( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectElbow::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 100;		// 技ごとに別

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();

	CDirect::SetPlayerID( playerID );
}

//=================================================
// 終了
//=================================================
void CDirectElbow::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirectElbow::Update( void )
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
		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_ELBOW_LEFT );
		//CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, ELBOW_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)ELBOW_EFFECT_AURA_SCALE );
		break;

	case 5:
		m_pPlayerManager->SetAnimType(m_Enemy, CPlayer::PLAYER_ELBOW_DAMAGE);
		break;

	
	case 50:
	{
		m_pCameraManager->StartCameraShake( VECTOR3_ZERO, 3.0f, 10, 0 );
		m_pPlayerManager->TakeDamage( m_Enemy, ELBOW_DAMAGE );
		m_pPlayerManager->AddTension( m_Player, ELBOW_TENSION );
		int label = mersenne_twister_int(SOUND_LABEL_SE_ELBOW01, SOUND_LABEL_SE_ELBOW03);
		m_pManager->PlaySoundA((SOUND_LABEL)label);
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, ELBOW_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)ELBOW_EFFECT_HIT_SCALE );
		break;
	}
	case 90:
		m_pPlayerManager->SetAnimType( m_Enemy, CPlayer::PLAYER_WAIT );
		break;
	}
	/* ここまで個別 */



	CDirect::CheckEnd();
}

//----EOF----