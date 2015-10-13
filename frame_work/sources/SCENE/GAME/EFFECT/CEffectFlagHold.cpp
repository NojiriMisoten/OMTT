//=============================================================================
//
// CEffectクラス [CEffect.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CEffectFlagHold.h"
#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const int FLAG_HOLD_COUNT_MAX = 180;
static const float FLAG_HOLD_WIDTH = 100;
static const float FLAG_HOLD_HEIGHT = 100;
static const TEXTURE_TYPE FLAG_HOLD_TEXTURE = TEXTURE_HIT_SMALL;
static const int FLAG_HOLD_TEXTURE_X = 10;
static const int FLAG_HOLD_TEXTURE_Y = 1;
static const int FLAG_HOLD_TEXTURE_LOOP = 10;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectFlagHold::CEffectFlagHold(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectFlagHold ::~CEffectFlagHold(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectFlagHold::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = FLAG_HOLD_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectFlagHold::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectFlagHold::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			FLAG_HOLD_WIDTH, FLAG_HOLD_HEIGHT,
			FLAG_HOLD_TEXTURE, FLAG_HOLD_TEXTURE_X, FLAG_HOLD_TEXTURE_Y,
			FLAG_HOLD_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectFlagHold* CEffectFlagHold::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectFlagHold* p = new CEffectFlagHold(pDevice);

	p->Init(pos);

	return p;
}

//----EOF-------