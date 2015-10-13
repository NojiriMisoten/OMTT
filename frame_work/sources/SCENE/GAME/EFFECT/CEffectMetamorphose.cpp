//=============================================================================
//
// CEffectクラス [CEffect.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CEffectMetamorphose.h"
#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const int METAMORPHOSE_COUNT_MAX = 10;
static const float METAMORPHOSE_WIDTH = 100;
static const float METAMORPHOSE_HEIGHT = 100;
//static const TEXTURE_TYPE METAMORPHOSE_TEXTURE = TEXTURE_HIT_SMALL;
static const int METAMORPHOSE_TEXTURE_X = 5;
static const int METAMORPHOSE_TEXTURE_Y = 2;
static const int METAMORPHOSE_TEXTURE_LOOP = 10;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectMetamorphose::CEffectMetamorphose(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectMetamorphose ::~CEffectMetamorphose(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectMetamorphose::Init(D3DXVECTOR3 pos, short playerNum)
{
	m_nCount = 0;
	m_nCountMax = METAMORPHOSE_COUNT_MAX;
	m_vPos = pos;

	switch (playerNum)
	{
	case 0:
		m_Texture = TEXTURE_PLAYER_ETAMORPHOSE_YELLOW;
		break;
	case 1:
		m_Texture = TEXTURE_PLAYER_ETAMORPHOSE_RED;
		break;
	case 2:
		m_Texture = TEXTURE_PLAYER_ETAMORPHOSE_BLUE;
		break;
	case 3:
		m_Texture = TEXTURE_PLAYER_ETAMORPHOSE_GREEN;
		break;
	default:
		break;
	}
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectMetamorphose::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectMetamorphose::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			METAMORPHOSE_WIDTH, METAMORPHOSE_HEIGHT,
			m_Texture, METAMORPHOSE_TEXTURE_X, METAMORPHOSE_TEXTURE_Y,
			METAMORPHOSE_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectMetamorphose* CEffectMetamorphose::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos , short playerNum)
{
	// 作成
	CEffectMetamorphose* p = new CEffectMetamorphose(pDevice);

	p->Init(pos, playerNum);

	return p;
}

//----EOF-------