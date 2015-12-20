//=============================================================================
//
// スキンメッシュ処理 [CSkinMeshHolder.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSkinMeshHolder.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const SKIN_MESH_ANIM_MODEL ANIM_MODEL_LIST[SKIN_MESH_INFO_MAX] =
{
	SKIN_MESH_TYPE_PLAYER01,
	SKIN_MESH_TYPE_PLAYER02,
};

//*****************************************************************************
// staticメンバ変数
//*****************************************************************************
CSkinMesh*				CSkinMeshHolder::m_pSkinMeshHolder[SKIN_MESH_INFO_MAX];
CCallBackHandlerPlayer*	CSkinMeshHolder::m_pFunc[SKIN_MESH_INFO_MAX];
CALLBACK_TIMING			CSkinMeshHolder::m_pCallBackTimiming[CPlayer::PLAYER_ANIM_MAX];

//=============================================================================
// コンストラクタ
//=============================================================================
CSkinMeshHolder::CSkinMeshHolder(void)
{
	for (int i = 0; i < SKIN_MESH_INFO_MAX; i++)
	{
		m_pSkinMeshHolder[SKIN_MESH_INFO_MAX];
		m_pFunc[SKIN_MESH_INFO_MAX];
	}

	m_pCallBackTimiming[CPlayer::PLAYER_ANIM_MAX];
}

//=============================================================================
// デストラクタ
//=============================================================================
CSkinMeshHolder::~CSkinMeshHolder(void)
{

}
//=============================================================================
// 終了
//=============================================================================
void CSkinMeshHolder::Uninit(void)
{
	for (int i = 0; i < SKIN_MESH_INFO_MAX; i++)
	{
		SAFE_DELETE(m_pFunc[i]);

		// スキンメッシュモデル読み込み
		m_pSkinMeshHolder[i]->Uninit();
		delete m_pSkinMeshHolder[i];
	}
}

//=============================================================================
// クリエイト
//=============================================================================
void CSkinMeshHolder::Create(LPDIRECT3DDEVICE9 *pDevice)
{
	InitCallBackTiming();

	for (int i = 0; i < SKIN_MESH_INFO_MAX; i++)
	{
		// コールバック次呼び出す関数
		m_pFunc[i] = new CCallBackHandlerPlayer;

		// スキンメッシュモデル読み込み
		m_pSkinMeshHolder[i] = new CSkinMesh;
		m_pSkinMeshHolder[i]->Init(pDevice, &m_pCallBackTimiming[0], m_pFunc[i], ANIM_MODEL_LIST[i]);
	}
}

//=============================================================================
// コールバックタイミングの初期化
//=============================================================================
void CSkinMeshHolder::InitCallBackTiming(void)
{
	m_pCallBackTimiming[CPlayer::PLAYER_WAIT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_WAIT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_LARIAT_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_LARIAT_LEFT].CallBackTiming = 0.f;

	m_pCallBackTimiming[CPlayer::PLAYER_LARIAT_RIGHT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_LARIAT_RIGHT].CallBackTiming = 0.f;

	m_pCallBackTimiming[CPlayer::PLAYER_ELBOW_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_ELBOW_LEFT].CallBackTiming = 0.5f;

	m_pCallBackTimiming[CPlayer::PLAYER_ELBOW_RIGHT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_ELBOW_RIGHT].CallBackTiming = 0.3f;

	m_pCallBackTimiming[CPlayer::PLAYER_DAMAGE_SMALL].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_DAMAGE_SMALL].CallBackTiming = 0.99f;

	m_pCallBackTimiming[CPlayer::PLAYER_CHOP_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_CHOP_LEFT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_CHOP_RIGHT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_CHOP_RIGHT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_LARIAT_DAMAGE].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_LARIAT_DAMAGE].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_BACKDROP].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_BACKDROP].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_BACKDROP_DAMAGE].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_BACKDROP_DAMAGE].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_SLAPPING_RIGHT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_SLAPPING_RIGHT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_SLAPPING_DAMAGE_RIGHT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_SLAPPING_DAMAGE_RIGHT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_STUNNER_RIGHT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_STUNNER_RIGHT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_STUNNER_DAMAGE_RIGHT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_STUNNER_DAMAGE_RIGHT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_DROP_KICK_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_DROP_KICK_LEFT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_DROP_KICK_DAMAGE_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_DROP_KICK_DAMAGE_LEFT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_LEFT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_DAMAGE_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_FLYING_SHOULDER_ATTACK_DAMAGE_LEFT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_ROLLING_ELBOW_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_ROLLING_ELBOW_LEFT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_ROLLING_ELBOW_DAMAGE_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_ROLLING_ELBOW_DAMAGE_LEFT].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_ELBOW_DAMAGE].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_ELBOW_DAMAGE].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_FINISH].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_FINISH].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_FINISH_DAMAGE].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_FINISH_DAMAGE].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_ROPE].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_ROPE].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL01].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL01].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL02].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL02].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL03].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL03].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL04].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL04].CallBackTiming = 0.0f;

	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL05].nCallBackNum = 1;
	m_pCallBackTimiming[CPlayer::PLAYER_APPEAL05].CallBackTiming = 0.0f;

}

//----EOF----