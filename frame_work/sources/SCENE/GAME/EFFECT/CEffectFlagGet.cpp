//=============================================================================
//
// CEffectクラス [CEffect.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CEffectFlagGet.h"
#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const int FLAG_GET_COUNT_MAX = 180;
static const float FLAG_GET_WIDTH = 100;
static const float FLAG_GET_HEIGHT = 100;
static const TEXTURE_TYPE FLAG_GET_TEXTURE = TEXTURE_HIT_SMALL;
static const int FLAG_GET_TEXTURE_X = 10;
static const int FLAG_GET_TEXTURE_Y = 1;
static const int FLAG_GET_TEXTURE_LOOP = 10;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectFlagGet::CEffectFlagGet(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectFlagGet ::~CEffectFlagGet(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectFlagGet::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = FLAG_GET_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectFlagGet::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectFlagGet::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			FLAG_GET_WIDTH, FLAG_GET_HEIGHT,
			FLAG_GET_TEXTURE, FLAG_GET_TEXTURE_X, FLAG_GET_TEXTURE_Y,
			FLAG_GET_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectFlagGet* CEffectFlagGet::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectFlagGet* p = new CEffectFlagGet(pDevice);

	p->Init(pos);

	return p;
}

//----EOF-------