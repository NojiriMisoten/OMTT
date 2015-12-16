//=============================================================================
//
// CDirectBattleIntroクラス [CDirectBattleIntro.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectBattleIntro.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"
#include "../../SCENE/GAME/UI/COverLay.h"
#include "../../TEXTURE/CTexture.h"
#include "../../SOUND/CSound.h"

//=================================================
// マクロ
//=================================================
static const D3DXVECTOR3 INIT_CAMERA_POS(-150.f, 800.0f, 0.0f);
static const D3DXVECTOR3 INIT_CAMERA_POSR(0.0f, 0.0f, 0.0f);

static const D3DXVECTOR3 FIRST_CAMERA_POS(-75.f, 275.0f, -125.0f);
static const D3DXVECTOR3 FIRST_CAMERA_POSR(0.0f, 0.0f, 0.0f);
static const int FIRST_CAMERA_MOVE_TIME = 60;

static const COverLay::Data READY_FADE_INFO(TEXTURE_READY, 1.f / 30.f, 20, 1.f / 30.f);

static const D3DXVECTOR3 SECOND_CAMERA_POS(0.0f, 150.0f, -250.0f);
static const D3DXVECTOR3 SECOND_CAMERA_POSR(0.0f, 25.0f, 0.0f);
static const int SECOND_CAMERA_MOVE_TIME = 60;

static const COverLay::Data FIGHT_FADE_INFO(TEXTURE_FIGHT, 1.f / 3.f, 30, 1.f / 5.f);
//=================================================
// コンストラクタ
//=================================================
CDirectBattleIntro::CDirectBattleIntro(CManager *pManager, CGame *pGame ) : CDirect(pManager, pGame)
{

}

//=================================================
// デストラクタ
//=================================================
CDirectBattleIntro::~CDirectBattleIntro(void)
{

}

//=================================================
// 初期化
//=================================================
void CDirectBattleIntro::Init(PLAYER_ID playerID)
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 200;		// 技ごとに別

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();

	CDirect::SetPlayerID(playerID);
}

//=================================================
// 終了
//=================================================
void CDirectBattleIntro::Uninit(void)
{

}

//=================================================
// 更新
//=================================================
void CDirectBattleIntro::Update(void)
{
	D3DXVECTOR3 pos[2] = {
		m_pPlayerManager->GetPlayerPos(PLAYER_1),
		m_pPlayerManager->GetPlayerPos(PLAYER_2)
	};

	D3DXVECTOR3 rot[2] = {
		m_pPlayerManager->GetPlayerRot(PLAYER_1),
		m_pPlayerManager->GetPlayerRot(PLAYER_2)
	};

	/* ここから個別 */
	switch (m_FrameCount)
	{
		// フレーム別の処理
		case 0:
		{
			m_pManager->GetCameraManager()->CameraSetToCoord(INIT_CAMERA_POS
															, INIT_CAMERA_POSR);

			m_pManager->GetCameraManager()->CameraMoveToCoord(INIT_CAMERA_POS
															, FIRST_CAMERA_POS
															, INIT_CAMERA_POSR
															, FIRST_CAMERA_POSR
															, FIRST_CAMERA_MOVE_TIME);
			break;
		}
		case 2:
		{
			COverLay::Data readyFadeData = READY_FADE_INFO;
			m_pManager->GetUiManager()->StartOverLay(&readyFadeData);
			//m_pManager->PlaySoundA(SOUND_LABEL_SE_CROWD01);
			break;
		}
		case 60:
		{
			m_pManager->GetCameraManager()->CameraMoveToCoord(FIRST_CAMERA_POS
															, SECOND_CAMERA_POS
															, FIRST_CAMERA_POSR
															, SECOND_CAMERA_POSR
															, SECOND_CAMERA_MOVE_TIME);
			break;
		}
		case 120:
			// ここでゲージ類フェードイン開始

		break;

		case 170:
			// ファイト表示
			COverLay::Data fightFadeData = FIGHT_FADE_INFO;
			m_pManager->GetUiManager()->StartOverLay(&fightFadeData);
			m_pManager->PlaySoundA(SOUND_LABEL_SE_START_GONG);
			break;
	}
	/* ここまで個別 */



	m_FrameCount++;

	if( m_FrameCount > m_TotalFrame )
	{
		// モーションリセット
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_1, CPlayer::PLAYER_WAIT );
		m_pManager->GetPlayerManager()->SetAnimType( PLAYER_2, CPlayer::PLAYER_WAIT );

		//　アニメーション速度リセット
		m_pManager->GetPlayerManager()->SetAnimSpd( PLAYER_1, DEFFAULT_ANIM_SPD );
		m_pManager->GetPlayerManager()->SetAnimSpd( PLAYER_2, DEFFAULT_ANIM_SPD );

		m_pManager->GetDirectorManager()->SetEndDirecting();
		// カメラ上書き
		m_pCameraManager->CameraSetToCoord(
			D3DXVECTOR3( 0.0f, 150.0f, -250.0f ),
			D3DXVECTOR3( 0.0f, 25.0f, 0.0f ) );
	}
}

//----EOF----