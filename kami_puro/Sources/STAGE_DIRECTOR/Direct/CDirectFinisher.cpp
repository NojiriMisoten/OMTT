//=============================================================================
//
// CDirectFinisherクラス [CDirectFinisher.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectFinisher.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"
#include "../../SCENE/GAME/CGame.h"
#include "../../SCENE/GAME/COMMANDCHART/CCommandChartManager.h"
#include "../../SOUND/CSound.h"

const D3DXVECTOR3 FINISHER_POS_OFFSET = D3DXVECTOR3( 0.0f, 1000.0f, 0.0f );
const D3DXVECTOR3 FINISHER_ROT_OFFSET = D3DXVECTOR3( 0.0f, D3DXToRadian( 180.0f ), 0.0f );

const D3DXVECTOR3 FINISHER_EFFECT_AURA_OFFSET = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
const D3DXVECTOR3 FINISHER_EFFECT_AURA_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 FINISHER_EFFECT_HIT_OFFSET = D3DXVECTOR3( 60.0f, 220.0f, 0.0f );
const D3DXVECTOR3 FINISHER_EFFECT_HIT_SCALE = D3DXVECTOR3( 10.0f, 10.0f, 10.0f );

const D3DXVECTOR3 FINISHER_EFFECT_SLAM_OFFSET = D3DXVECTOR3( 0.0f, 2.0f, 0.0f );
const D3DXVECTOR3 FINISHER_EFFECT_SLAM_SCALE = D3DXVECTOR3( 20.0f, 20.0f, 20.0f );

const int FINISHER_DAMAGE1 = (int)( 10 * DAMAGE_AMP );
const int FINISHER_DAMAGE2 = (int)( 30 * DAMAGE_AMP );
const int FINISHER_DAMAGE3 = (int)( 1000 * DAMAGE_AMP );

static const COverLay::Data KO_FADE_INFO(TEXTURE_KO, 1.f / 30.f, 20, 1.f / 30.f);
//=================================================
// コンストラクタ
//=================================================
CDirectFinisher::CDirectFinisher( CManager *pManager, CGame *pGame ) : CDirect( pManager, pGame )
{
	
}

//=================================================
// デストラクタ
//=================================================
CDirectFinisher::~CDirectFinisher( void )
{

}

//=================================================
// 初期化
//=================================================
void CDirectFinisher::Init( PLAYER_ID playerID )
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 1000;		// 技ごとに別

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();
	m_pUIManager = m_pManager->GetUiManager();

	CDirect::SetPlayerID( playerID );
}

//=================================================
// 終了
//=================================================
void CDirectFinisher::Uninit( void )
{

}

//=================================================
// 更新
//=================================================
void CDirectFinisher::Update( void )
{
	D3DXVECTOR3 pos[2] = {
		m_pPlayerManager->GetPlayerPos( PLAYER_1 ),
		m_pPlayerManager->GetPlayerPos( PLAYER_2 )
	};

	D3DXVECTOR3 rot[2] = {
		m_pPlayerManager->GetPlayerRot( PLAYER_1 ),
		m_pPlayerManager->GetPlayerRot( PLAYER_2 )
	};


	D3DXVECTOR3 yes;

	/* ここから個別 */
	switch( m_FrameCount )
	{
		// フレーム別の処理
	case 0:
		m_pUIManager->StartCutIn( m_Player, CUT_IN_JIJII );

		m_pPlayerManager->SetAnimType( m_Player, CPlayer::PLAYER_FINISH );
		m_pPlayerManager->SetAnimType( m_Enemy, CPlayer::PLAYER_FINISH_DAMAGE );
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.3f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.3f );
		CEffect::Create( 60, EFFECT_AURA_START, false, pos[m_Player] + TranslateCoord( m_Player, FINISHER_EFFECT_AURA_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)FINISHER_EFFECT_AURA_SCALE );
		m_pPlayerManager->TakeDamage( m_Enemy, FINISHER_DAMAGE1 );
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 200.0f, 100.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 100.0f, 100.0f, +100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			60 );
		CManager::StopSound();
		m_pManager->PlaySoundA(SOUND_LABEL_BGM_FINISH);
		break;

	case 60:
		m_pManager->GetUiManager()->GetGame()->SetImvisible();
		m_pManager->GetUiManager()->GetCommandChartManager()->SetCommandChartMode( m_Player, CCommandChart::MODE_VANISH );
	//	m_pPlayerManager->SetPos( m_Player, pos[m_Player] + TranslateCoord( m_Player, FINISHER_POS_OFFSET ) );
	//	m_pPlayerManager->SetPos( m_Enemy, pos[m_Enemy] + TranslateCoord( m_Enemy, FINISHER_POS_OFFSET ) );
		pos[0] = m_pPlayerManager->GetPlayerPos( PLAYER_1 );
		pos[1] = m_pPlayerManager->GetPlayerPos( PLAYER_2 );

		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 100.0f, 100.0f, +100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( -100.0f, 100.0f, +100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			100 );

		m_pManager->PlaySoundA(SOUND_LABEL_SE_FINISH_START);
		m_pManager->PlaySoundA(SOUND_LABEL_SE_SLOW);
		break;

	case 160:
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( -100.0f, 100.0f, +100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 200.0f, 100.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			100 );
		
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.4f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.4f );
		break;

	case 180:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.5f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.5f );
		break;

	case 190:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.6f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.6f );
		break;

	case 200:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.7f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.7f );
		break;

	case 210:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.8f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.8f );
		break;

	case 220:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.9f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.9f );
		break;

	case 230:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 1.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 1.0f );
		break;

	case 260:
		// ジャンプのとこ
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 200.0f, 100.0f, -100.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 140.0f, -500.0f, -80.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 70.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 250.0f, 0.0f ) ),
			35 );
		break;

	case 299:
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.1f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.1f );
		
		m_pCameraManager->CameraSetToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 100.0f, 500.0f, -150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 400.0f, 0.0f ) ) );
		break;

	case 300:
		// ジャンプ頂上
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 100.0f, 500.0f, -150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 110.0f, 400.0f, +150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 400.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 300.0f, 0.0f ) ),
			60 );
		break;

	case 360:
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 110.0f, 400.0f, 150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( -150.0f, 400.0f, +150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 300.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 300.0f, 0.0f ) ),
			60 );
		break;

	case 420:
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( -150.0f, 400.0f, 150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( -150.0f, 300.0f, -150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 300.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 300.0f, 0.0f ) ),
			50 );
		break;

	case 470:
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( -150.0f, 300.0f, -150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 150.0f, 250.0f, -150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 300.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 250.0f, 0.0f ) ),
			50 );
		break;

	case 520:
		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 150.0f, 250.0f, -150.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 300.0f, 180.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 250.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 250.0f, 0.0f ) ),
			30 );
		break;
		
	case 550:
		// ホーリーランス！
		m_pPlayerManager->SetAnimSpd( m_Player, 0 );
		m_pPlayerManager->SetAnimSpd( m_Enemy, 0 );
		m_pEffect = CEffect::Create( 30, EFFECT_HOLYLANCE, true, pos[m_Player] + TranslateCoord( m_Player, FINISHER_EFFECT_HIT_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)FINISHER_EFFECT_HIT_SCALE );
		m_pEffect->SetRotSpd( D3DXVECTOR3( 0.0f, D3DXToRadian(50.0f), 0.0f ) );
		m_pPlayerManager->TakeDamage( m_Enemy, FINISHER_DAMAGE2 );
		m_pManager->PlaySoundA(SOUND_LABEL_SE_FALL01);
		break;

	case 560:
		m_pEffect->AddPos( D3DXVECTOR3( 0.0f, -10.0f, 0.0f ) );
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 1.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 1.0f );
		break;
		
	case 561: case 562: case 563: case 564: case 565: case 566: case 567: case 568: case 569:
		m_pEffect->AddPos( D3DXVECTOR3( 0.0f, -10.0f, 0.0f ) );
		break;

	case 570:
		m_pPlayerManager->SetAnimSpd( m_Player, 0.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, 0.0f );
		m_pEffect->AddPos( D3DXVECTOR3( 0.0f, -10.0f, 0.0f ) );
		break;

	case 571: case 572: case 573: case 574: case 575: case 576: case 577: case 578: case 579: case 580:
	case 581: case 582: case 583: case 584: case 585: case 586: case 587: case 588: case 589:
		m_pEffect->AddPos( D3DXVECTOR3( 0.0f, -10.0f, 0.0f ) );
		break;

	case 590:
		// 落下中
		m_pEffect->AddPos( D3DXVECTOR3( 0.0f, +100.0f, 0.0f ) );
		m_pPlayerManager->SetAnimSpd( m_Player, 0.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, 0.0f );

		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 300.0f, 100.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 300.0f, 100.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 50.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 50.0f, 0.0f ) ),
			30 );
		break;

	case 700:
		// 接地
		m_pPlayerManager->SetAnimSpd( m_Player, DEFFAULT_ANIM_SPD * 0.5f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, DEFFAULT_ANIM_SPD * 0.5f );
		m_pEffect->Destroy();
		m_pCameraManager->StartCameraShake( VECTOR3_ZERO, 40.0f, 120, 0.0f );
		CEffect::Create( 200, EFFECT_TEST1, false, pos[m_Player] + TranslateCoord( m_Player, FINISHER_EFFECT_SLAM_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)FINISHER_EFFECT_SLAM_SCALE );
		CEffect::Create( 100, EFFECT_TEST2, false, pos[m_Player] + TranslateCoord( m_Player, FINISHER_EFFECT_SLAM_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)FINISHER_EFFECT_SLAM_SCALE / 2 );
		CEffect::Create( 300, EFFECT_BROKEN_FIELD, false, pos[m_Player] + TranslateCoord( m_Player, FINISHER_EFFECT_SLAM_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)FINISHER_EFFECT_SLAM_SCALE );
		CEffect::Create( 300, EFFECT_RING, false, pos[m_Player] + TranslateCoord( m_Player, FINISHER_EFFECT_SLAM_OFFSET ), VECTOR3_ZERO, (D3DXVECTOR3)FINISHER_EFFECT_SLAM_SCALE );
		m_pPlayerManager->TakeDamageFinish( m_Enemy, FINISHER_DAMAGE3 );
		m_pManager->GetUiManager()->GetGame()->SetVisible();
		// KO表示
		m_pManager->GetUiManager()->StartOverLay(&(COverLay::Data)KO_FADE_INFO);

		m_pManager->PlaySoundA(SOUND_LABEL_SE_FINISH_END);
		break;

	case 800:
		//m_pPlayerManager->SetPos( m_Player, pos[m_Player] - FINISHER_POS_OFFSET );
		//m_pPlayerManager->SetPos( m_Enemy, pos[m_Enemy] - FINISHER_POS_OFFSET );
		pos[0] = m_pPlayerManager->GetPlayerPos( PLAYER_1 );
		pos[1] = m_pPlayerManager->GetPlayerPos( PLAYER_2 );

		m_pCameraManager->CameraMoveToCoord(
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 300.0f, 100.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 300.0f, 100.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 50.0f, 0.0f ) ),
			pos[m_Player] + TranslateCoord( m_Player, D3DXVECTOR3( 0.0f, 50.0f, 0.0f ) ),
			10 );
		break;

	case 900:
		m_pPlayerManager->SetAnimSpd( m_Player, 0.0f );
		m_pPlayerManager->SetAnimSpd( m_Enemy, 0.0f );
		m_pManager->PlaySoundA(SOUND_LABEL_SE_FINISH_GONG);
	}

	/* ここまで個別 */
#ifdef _DEBUG
	CDebugProc::PrintU( "[ANIM]\n" );
	CDebugProc::PrintU( "Frame:%d\n", m_FrameCount );
#endif

	CDirect::CheckEnd();
}

//----EOF----