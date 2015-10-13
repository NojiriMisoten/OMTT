//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectNormalAttackCap.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int NORMAL_ATTACK_CAP_COUNT_MAX = 1;
static const float NORMAL_ATTACK_CAP_WIDTH = 100;
static const float NORMAL_ATTACK_CAP_HEIGHT = 100;
static const TEXTURE_TYPE NORMAL_ATTACK_CAP_TEXTURE = TEXTURE_ATTACK_AROUND;
static const int NORMAL_ATTACK_CAP_TEXTURE_X = 5;
static const int NORMAL_ATTACK_CAP_TEXTURE_Y = 1;
static const int NORMAL_ATTACK_CAP_TEXTURE_LOOP = 30;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectNormalAttackCap::CEffectNormalAttackCap(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectNormalAttackCap ::~CEffectNormalAttackCap(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectNormalAttackCap::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = NORMAL_ATTACK_CAP_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectNormalAttackCap::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectNormalAttackCap::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			NORMAL_ATTACK_CAP_WIDTH, NORMAL_ATTACK_CAP_HEIGHT,
			NORMAL_ATTACK_CAP_TEXTURE, NORMAL_ATTACK_CAP_TEXTURE_X, NORMAL_ATTACK_CAP_TEXTURE_Y,
			NORMAL_ATTACK_CAP_TEXTURE_LOOP);
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
CEffectNormalAttackCap* CEffectNormalAttackCap::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectNormalAttackCap* p = new CEffectNormalAttackCap(pDevice);

	p->Init(pos);

	return p;
}

//EOF