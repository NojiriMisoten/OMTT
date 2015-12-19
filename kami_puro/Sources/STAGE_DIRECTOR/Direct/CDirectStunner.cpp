//=============================================================================
//
// CDirectStunnerクラス [CDirectStunner.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectStunner.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"
#include "../../MATH/mersenne_twister.h"

const D3DXVECTOR3 STUNNER_POS1_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 STUNNER_POS2_OFFSET = D3DXVECTOR3( 80.0f, 0.0f, 0.0f );
const D3DXVECTOR3 STUNNER_ROT_OFFSET = D3DXVECTOR3( 0.0f, D3DXToRadian( 180.0f ), 0.0f );

const D3DXVECTOR3 STUNNER_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 STUNNER_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 STUNNER_EFFECT_HIT_OFFSET = D3DXVECTOR3( 40.0f, 60.0f, 0.0f );
const D3DXVECTOR3 STUNNER_EFFECT_HIT_SCALE = D3DXVECTOR3( 3.0f, 3.0f, 3.0f );

const int STUNNER_DAMAGE1 = (int)( 10 * DAMAGE_AMP );
const int STUNNER_DAMAGE2 = (int)( 120 * DAMAGE_AMP );
const int STUNNER_TENSION = (int)( 10 * TENSION_AMP );

//=================================================
// コンストラクタ
//=================================================
CDirectStunner::CDirectStunner( CManager *pManager, CGame *pGame ) : CDirect( pManager, pGame )
{
	
}

//=================================================
// デストラクタ
//=================================================
CDirectStunner::~CDirectStunner( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectStunner::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 340;		// 技ごとに別

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();
	m_pUIManager = m_pManager->GetUiManager();

	CDirect::SetPlayerID( playerID );
}

//=================================================
// 終了
//=================================================
void CDirectStunner::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirectStunner::Update( void )
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

		m_pPlayerManager->SetPos( m_Player, pos[m_Player] + TranslateCoord( m_Player, STUNNER_POS1_OFFSET ) );
		m_pPlayerManager->SetRot( m_Enemy, rot[m_Enemy] + TranslateCoord( m_Enemy, STUNNER_ROT_OFFSET ) );
		m_pPlayerManager->SetPos( m_Enemy, pos[m_Enemy] + TranslateCoord( m_Enemy, STUNNER_POS2_OFFSET ) );
		pos[PLAYER_1] = m_pPlayerManager->GetPlayerPos( PLAYER_1 );
		pos[PLAYER_2] = m_pPlayerManager->GetPlayerPos( PLAYER_2 );

		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_STUNNER_RIGHT );
		m_pPlayerManager->SetAnimType( m_Enemy, CPlayer::PLAYER_STUNNER_DAMAGE_RIGHT );
		m_pPlayerManager->SetAnimSpd( m_Player, 0.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, 0.0f );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 20.0f, 80.0f, -5.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 20.0f, 80.0f, 5.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 75.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 75.0f, 0.0f ) ),
			70 );
		break;

	case 30:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.1f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.1f );
		break;

	case 70:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.7f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.7f );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, STUNNER_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)STUNNER_EFFECT_AURA_SCALE );
		m_pPlayerManager->TakeDamage( m_Enemy, STUNNER_DAMAGE1 );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 200.0f, 100.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 200.0f, 100.0f, +100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 40.0f, 0.0f, 0.0f ) ),
			100 );
		break;

	case 140:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 1.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 1.0f );
		break;
	
	case 145:
	{
		m_pCameraManager->StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0 );
		m_pPlayerManager->TakeDamage( m_Enemy, STUNNER_DAMAGE2 );
		m_pPlayerManager->AddTension( m_Player, STUNNER_TENSION );
		int label = mersenne_twister_int(SOUND_LABEL_SE_LARGE_DAMAGE01, SOUND_LABEL_SE_LARGE_DAMAGE03);
		m_pManager->PlaySoundA((SOUND_LABEL)label);
		CEffect::Create( 30, EFFECT_DAGEKI_KYO, false, pos[m_Player] + TranslateCoord( m_Player, STUNNER_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)STUNNER_EFFECT_HIT_SCALE );
		break;
	}

	case 155:
	{
		int label = mersenne_twister_int(SOUND_LABEL_SE_DOWN01, SOUND_LABEL_SE_DOWN03);
		m_pManager->PlaySoundA((SOUND_LABEL)label);
	}
		break;

	case 165:
		{
				int label = mersenne_twister_int(SOUND_LABEL_SE_CROWD01, SOUND_LABEL_SE_CROWD03);
				m_pManager->PlaySoundA((SOUND_LABEL)label);

				label = mersenne_twister_int(SOUND_LABEL_SE_DOWN01, SOUND_LABEL_SE_DOWN03);
				m_pManager->PlaySoundA((SOUND_LABEL)label);

				//m_pPlayerManager->SetAnimSpd(m_Player, 0.0f);
				//m_pPlayerManager->SetAnimSpd(m_Enemy, 0.0f);
		}
		break;

	case 190:
		m_pPlayerManager->SetAnimType(m_Player, CPlayer::PLAYER_APPEAL02, 0.5);
		m_pPlayerManager->SetAnimSpd(m_Enemy, 0.0f);

		m_pManager->GetCameraManager()->CameraMoveToCoord(pos[m_Player] + TranslateCoord(m_Player, D3DXVECTOR3(200.0f, 100.0f, +100.0f))
														, pos[m_Player] + TranslateCoord(m_Player, D3DXVECTOR3(50.0f, 30.0f, 0.0f))
														, pos[m_Player] + TranslateCoord(m_Player, D3DXVECTOR3(40.0f, 0.0f, 0.0f))
														, pos[m_Player] + TranslateCoord(m_Player, D3DXVECTOR3(0.0f, 80.0f, 0.0f))
														, 100);
		break;

	case 300:
		m_pPlayerManager->SetAnimSpd(m_Player, 0.0f);
		break;

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