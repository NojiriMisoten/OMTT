//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectCaughtTrap.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int TRAP_CAUGHT_COUNT_MAX = 300;
static const float TRAP_CAUGHT_WIDTH = 50;
static const float TRAP_CAUGHT_HEIGHT = 50;
static const TEXTURE_TYPE TRAP_CAUGHT_TEXTURE = TEXTURE_SLOW;
static const int TRAP_CAUGHT_TEXTURE_X = 10;
static const int TRAP_CAUGHT_TEXTURE_Y = 1;
static const int TRAP_CAUGHT_TEXTURE_LOOP = 100;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectCaughtTrap::CEffectCaughtTrap(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectCaughtTrap ::~CEffectCaughtTrap(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectCaughtTrap::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = TRAP_CAUGHT_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y-30.0f;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectCaughtTrap::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectCaughtTrap::Update(void)
{
	if (m_nCount %100 == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			TRAP_CAUGHT_WIDTH, TRAP_CAUGHT_HEIGHT,
			TRAP_CAUGHT_TEXTURE, TRAP_CAUGHT_TEXTURE_X, TRAP_CAUGHT_TEXTURE_Y,
			TRAP_CAUGHT_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();

#ifdef _DEBUG
	CDebugProc::Print("爆発エフェクトなう\n");
#endif
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectCaughtTrap* CEffectCaughtTrap::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectCaughtTrap* p = new CEffectCaughtTrap(pDevice);

	p->Init(pos);

	return p;
}

//EOF