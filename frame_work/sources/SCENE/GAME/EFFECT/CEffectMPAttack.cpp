//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectMPAttack.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int MP_ATTACK_COUNT_MAX = 29;
static const float MP_ATTACK_WIDTH = 30;
static const float MP_ATTACK_HEIGHT = 30;
static const TEXTURE_TYPE MP_ATTACK_TEXTURE = TEXTURE_THREAD_POISON;
static const int MP_ATTACK_TEXTURE_X = 2;
static const int MP_ATTACK_TEXTURE_Y = 4;
static const int MP_ATTACK_END_TIME = 30;
// 速度
static const float ATTACK_JAMMER_SPD = 10;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectMPAttack::CEffectMPAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
	m_pAnim = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectMPAttack ::~CEffectMPAttack(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectMPAttack::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = MP_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
	m_vVelocity = velocity;
	m_vRot.z = 0.0f;

	m_pAnim = CSceneAnime::Create(
		m_pD3DDevice,
		m_vPos,
		MP_ATTACK_WIDTH, MP_ATTACK_HEIGHT,
		MP_ATTACK_TEXTURE, MP_ATTACK_TEXTURE_X, MP_ATTACK_TEXTURE_Y,
		MP_ATTACK_END_TIME);

	if (m_vVelocity.y == 1)
	{
		m_vRot.z = D3DX_PI*0.5f;
		m_pAnim->SetRot(m_vRot);
	}
	if (m_vVelocity.y == -1)
	{
		m_vRot.z = D3DX_PI*1.5f;
		m_pAnim->SetRot(m_vRot);
	}
	if (m_vVelocity.x == -1)
	{
		m_vRot.z = 0.0f;
	}
	if (m_vVelocity.x == 1)
	{
		m_vRot.z = D3DX_PI;
		m_pAnim->SetRot(m_vRot);
	}
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectMPAttack::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectMPAttack::Update(void)
{
	m_pAnim->SetPos(m_vPos);

	// 自殺の更新
	CEffectBase::Update();

	m_vPos += m_vVelocity * ATTACK_JAMMER_SPD;
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectMPAttack* CEffectMPAttack::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// 作成
	CEffectMPAttack* p = new CEffectMPAttack(pDevice);

	p->Init(pos, velocity);

	return p;
}

//EOF