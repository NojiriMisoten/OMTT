//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectExplosion.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int EXPLOSION_COUNT_MAX = 75;
static const float EXPLOSION_WIDTH = 200;
static const float EXPLOSION_HEIGHT = 200;
static const TEXTURE_TYPE EXPLOSION_TEXTURE = TEXTURE_FIRE_0;
static const int EXPLOSION_TEXTURE_X = 10;
static const int EXPLOSION_TEXTURE_Y = 1;
static const int EXPLOSION_TEXTURE_LOOP = 100;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectExplosion::CEffectExplosion(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectExplosion ::~CEffectExplosion(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectExplosion::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = EXPLOSION_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectExplosion::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectExplosion::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			EXPLOSION_WIDTH, EXPLOSION_HEIGHT,
			EXPLOSION_TEXTURE, EXPLOSION_TEXTURE_X, EXPLOSION_TEXTURE_Y,
			EXPLOSION_TEXTURE_LOOP);
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
CEffectExplosion* CEffectExplosion::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectExplosion* p = new CEffectExplosion(pDevice);

	p->Init(pos);

	return p;
}

//EOF