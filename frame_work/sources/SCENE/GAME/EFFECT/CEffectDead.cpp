//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectDead.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int PLAYER_DEAD_COUNT_MAX = 1;
static const float PLAYER_DEAD_WIDTH = 100;
static const float PLAYER_DEAD_HEIGHT = 100;
static const TEXTURE_TYPE PLAYER_DEAD_TEXTURE = TEXTURE_TREASURE_KIRAKIRA;
static const int PLAYER_DEAD_TEXTURE_X = 8;
static const int PLAYER_DEAD_TEXTURE_Y = 1;
static const int PLAYER_DEAD_TEXTURE_LOOP = 100;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectDead::CEffectDead(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectDead ::~CEffectDead(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectDead::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = PLAYER_DEAD_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectDead::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectDead::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			PLAYER_DEAD_WIDTH, PLAYER_DEAD_HEIGHT,
			PLAYER_DEAD_TEXTURE, PLAYER_DEAD_TEXTURE_X, PLAYER_DEAD_TEXTURE_Y,
			PLAYER_DEAD_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();

#ifdef _DEBUG
	CDebugProc::Print("道作成エフェクトなう\n");
#endif
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectDead* CEffectDead::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectDead* p = new CEffectDead(pDevice);

	p->Init(pos);

	return p;
}

//EOF