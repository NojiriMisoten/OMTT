//=============================================================================
//
// CEffectクラス [CEffect.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CEffectAttackHit.h"
#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const int ATTACK_HIT_COUNT_MAX = 180;
static const float ATTACK_HIT_WIDTH = 100;
static const float ATTACK_HIT_HEIGHT = 100;
static const TEXTURE_TYPE ATTACK_HIT_TEXTURE = TEXTURE_HIT_SMALL;
static const int ATTACK_HIT_TEXTURE_X = 10;
static const int ATTACK_HIT_TEXTURE_Y = 1;
static const int ATTACK_HIT_TEXTURE_LOOP = 10;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectAttackHit::CEffectAttackHit(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectAttackHit ::~CEffectAttackHit(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectAttackHit::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = ATTACK_HIT_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectAttackHit::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectAttackHit::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			ATTACK_HIT_WIDTH, ATTACK_HIT_HEIGHT,
			ATTACK_HIT_TEXTURE, ATTACK_HIT_TEXTURE_X, ATTACK_HIT_TEXTURE_Y,
			ATTACK_HIT_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectAttackHit* CEffectAttackHit::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectAttackHit* p = new CEffectAttackHit(pDevice);

	p->Init(pos);

	return p;
}

//----EOF-------