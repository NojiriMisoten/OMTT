//=============================================================================
//
// CEffectクラス [CEffect.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CEffectPlayerRevive.h"
#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const int PLAYER_REVIVE_COUNT_MAX = 180;
static const float PLAYER_REVIVE_WIDTH = 100;
static const float PLAYER_REVIVE_HEIGHT = 100;
static const TEXTURE_TYPE PLAYER_REVIVE_TEXTURE = TEXTURE_HIT_SMALL;
static const int PLAYER_REVIVE_TEXTURE_X = 10;
static const int PLAYER_REVIVE_TEXTURE_Y = 1;
static const int PLAYER_REVIVE_TEXTURE_LOOP = 10;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectPlayerRevive::CEffectPlayerRevive(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectPlayerRevive ::~CEffectPlayerRevive(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectPlayerRevive::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = PLAYER_REVIVE_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectPlayerRevive::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectPlayerRevive::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			PLAYER_REVIVE_WIDTH, PLAYER_REVIVE_HEIGHT,
			PLAYER_REVIVE_TEXTURE, PLAYER_REVIVE_TEXTURE_X, PLAYER_REVIVE_TEXTURE_Y,
			PLAYER_REVIVE_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectPlayerRevive* CEffectPlayerRevive::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectPlayerRevive* p = new CEffectPlayerRevive(pDevice);

	p->Init(pos);

	return p;
}

//----EOF-------