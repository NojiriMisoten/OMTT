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
//=================================================
// コンストラクタ
//=================================================
CDirectBattleIntro::CDirectBattleIntro(CManager *pManager) : CDirect(pManager)
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
	m_TotalFrame = 180;		// 技ごとに別

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
		m_pManager->GetCameraManager()->CameraSetToCoord(D3DXVECTOR3(-150.f, 800.0f, .0f)
														, D3DXVECTOR3(0.f, 0.0f, 0.0f));

		D3DXVECTOR3 cameraPos = m_pManager->GetCameraManager()->GetCameraPos();
		D3DXVECTOR3 cameraPosR = m_pManager->GetCameraManager()->GetPosRCamera();
		m_pManager->GetCameraManager()->CameraMoveToCoord(cameraPos
														, D3DXVECTOR3(-75.f, 275.0f, -125.0f)
														, cameraPosR
														, D3DXVECTOR3(0.f, 0.0f, 0.0f)
														, 60);
		break;
	}
	case 2:
	{
		m_pManager->GetUiManager()->StartOverLay(&COverLay::Data(TEXTURE_LOGO_TITLE, 1.f / 60.f, 20, 30));
		break;
	}
	case 60:
	{
		D3DXVECTOR3 cameraPos = m_pManager->GetCameraManager()->GetCameraPos();
		D3DXVECTOR3 cameraPosR = m_pManager->GetCameraManager()->GetPosRCamera();
		m_pManager->GetCameraManager()->CameraMoveToCoord(cameraPos
														, D3DXVECTOR3(0.0f, 150.0f, -250.0f)
														, cameraPosR
														, D3DXVECTOR3(0.0f, 25.0f, 0.0f)
														, 60);
		break;
		}
	case 120:
		// ここでゲージ類フェードイン開始

		break;

	case 170:
		// ファイト表示
		m_pManager->GetUiManager()->StartOverLay(&COverLay::Data(TEXTURE_LOGO_TITLE, 5, 15, 10));
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
	}
}

//----EOF----