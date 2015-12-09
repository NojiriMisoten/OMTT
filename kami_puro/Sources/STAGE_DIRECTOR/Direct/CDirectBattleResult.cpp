//=============================================================================
//
// CDirectBattleResultクラス [CDirectBattleResult.cpp]
// Author : 池島　大樹
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CDirectBattleResult.h"
#include "../../MANAGER/CManager.h"
#include "../../CAMERA/CameraManager.h"
#include "../../EFECT/CEffect.h"
#include "../../EFECT/CEffectManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../CDirectorManager.h"
#include "../../SCENE/GAME/UI/COverLay.h"
#include "../../TEXTURE/CTexture.h"

//=================================================
// マクロ
//=================================================
static const D3DXVECTOR3 INIT_WINNER_POS(-50.f, -10.f, 0.f);
static const D3DXVECTOR3 INIT_LOOSER_POS(50.f, 0.f, 50.f);
static const D3DXVECTOR3 INIT_CAMERA_POS(0.f, 0.0f, -130.0f);
static const D3DXVECTOR3 INIT_CAMERA_POSR(50.f, 50.0f, 0.0f);
static const int WINNER_POSE_TIME = 450;
static const int LOOSER_POSE_TIME = 450;

static const D3DXVECTOR3 SECOND_CAMERA_POS(50.f, 20.f, -80.f);
static const D3DXVECTOR3 SECOND_CAMERA_POSR(50.f, 0.f, 50.f);
static const int SECOND_CAMERA_MOVE_TIME = 30;

static const D3DXVECTOR3 THIRD_CAMERA_POS(30.f, 30.f, -30.f);
static const D3DXVECTOR3 THIRD_CAMERA_POSR(0.f, 50.f, 0.f);
static const int THIRD_CAMERA_MOVE_TIME = 120;

static const D3DXVECTOR3 FOURTH_CAMERA_POS(30.f, 50.f, -30.f);
static const D3DXVECTOR3 FOURTH_CAMERA_POSR(0.f, 80.f, 0.f);
static const int FOURTH_CAMERA_MOVE_TIME = 10;
//=================================================
// コンストラクタ
//=================================================
CDirectBattleResult::CDirectBattleResult(CManager *pManager) : CDirect(pManager)
{

}

//=================================================
// デストラクタ
//=================================================
CDirectBattleResult::~CDirectBattleResult(void)
{

}

//=================================================
// 初期化
//=================================================
void CDirectBattleResult::Init(PLAYER_ID playerID)
{
	m_FrameCount = 0;		// 固定
	m_TotalFrame = 300;		// 技ごとに別

	m_pPlayerManager = m_pManager->GetPlayerManager();
	m_pCameraManager = m_pManager->GetCameraManager();

	CDirect::SetPlayerID(playerID);
}

//=================================================
// 終了
//=================================================
void CDirectBattleResult::Uninit(void)
{

}

//=================================================
// 更新
//=================================================
void CDirectBattleResult::Update(void)
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

		m_pPlayerManager->SetPos(m_Player, INIT_WINNER_POS);
		m_pPlayerManager->SetPos(m_Enemy, INIT_LOOSER_POS);

		// ポーズ設定
		m_pPlayerManager->SetAnimType(m_Player, CPlayer::PLAYER_FINISH);
		m_pPlayerManager->SetAnimType(m_Enemy, CPlayer::PLAYER_FINISH_DAMAGE);
		m_pPlayerManager->SetAnimMortionOfTime(m_Player, WINNER_POSE_TIME);
		m_pPlayerManager->SetAnimMortionOfTime(m_Enemy, LOOSER_POSE_TIME);

		// ポーズ固定
		m_pPlayerManager->SetAnimSpd(m_Player, 0.0);
		m_pPlayerManager->SetAnimSpd(m_Enemy, 0.0);
		break;
	}
	case 30:
		m_pManager->GetCameraManager()->CameraMoveToCoord(INIT_CAMERA_POS
														, SECOND_CAMERA_POS
														, INIT_CAMERA_POSR
														, SECOND_CAMERA_POSR
														, SECOND_CAMERA_MOVE_TIME);
		break;
	
	case 75:
		m_pManager->GetCameraManager()->CameraMoveToCoord(SECOND_CAMERA_POS
														, THIRD_CAMERA_POS
														, SECOND_CAMERA_POSR
														, THIRD_CAMERA_POSR
														, THIRD_CAMERA_MOVE_TIME);
		break;
	
	case 195:
		m_pManager->GetCameraManager()->CameraMoveToCoord(THIRD_CAMERA_POS
														, FOURTH_CAMERA_POS
														, THIRD_CAMERA_POSR
														, FOURTH_CAMERA_POSR
														, FOURTH_CAMERA_MOVE_TIME);
		break;
	}
	/* ここまで個別 */



	CDirect::CheckEnd();
}

//----EOF----