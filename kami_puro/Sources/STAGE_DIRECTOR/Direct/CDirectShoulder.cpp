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
#include "../../MATH/mersenne_twister.h"

const D3DXVECTOR3 SHOULDER_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 SHOULDER_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 SHOULDER_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 SHOULDER_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int SHOULDER_DAMAGE = (int)( 70 * DAMAGE_AMP );
const int SHOULDER_TENSION = (int)( 5 * TENSION_AMP );

//=================================================
// コンストラクタ
//=================================================
CDirectShoulder::CDirectShoulder(CManager *pManager, CGame *pGame) : CDirect( pManager, pGame )
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

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();

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
		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_LEFT );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, SHOULDER_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)SHOULDER_EFFECT_AURA_SCALE );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 150.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 300.0f, -200.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 25.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 100.0f, 0.0f, 0.0f ) ),
			100 );
		break;
	
	case 40:
	{
		m_pPlayerManager->SetAnimType( m_Enemy, CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_DAMAGE_LEFT );
		m_pCameraManager->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pPlayerManager->TakeDamage( m_Enemy, SHOULDER_DAMAGE );
		m_pPlayerManager->AddTension( m_Player, SHOULDER_TENSION );

		int label = mersenne_twister_int(SOUND_LABEL_SE_LARGE_DAMAGE01, SOUND_LABEL_SE_LARGE_DAMAGE03);
		m_pManager->PlaySoundA((SOUND_LABEL)label);
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, SHOULDER_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)SHOULDER_EFFECT_HIT_SCALE );
		break;
	}

	case 60:
	{
		int label = mersenne_twister_int(SOUND_LABEL_SE_DOWN01, SOUND_LABEL_SE_DOWN03);
		m_pManager->PlaySoundA((SOUND_LABEL)label);
		break;
	}
	case 70:
		{
			int label = mersenne_twister_int(SOUND_LABEL_SE_CROWD01, SOUND_LABEL_SE_CROWD03);
			m_pManager->PlaySoundA((SOUND_LABEL)label);

			label = mersenne_twister_int(SOUND_LABEL_SE_DOWN01, SOUND_LABEL_SE_DOWN03);
			m_pManager->PlaySoundA((SOUND_LABEL)label);
			m_pPlayerManager->SetAnimSpd( m_Player, 0.0f );
			m_pPlayerManager->SetAnimSpd( m_Enemy, 0.0f );
			break;
		}
	}

	// バトルフェードスタート
	if (m_FrameCount == m_TotalFrame - 15)
	{
		m_pManager->GetUiManager()->StartBattleFade();
	}
	/* ここまで個別 */



	CDirect::CheckEnd();
}

//----EOF----