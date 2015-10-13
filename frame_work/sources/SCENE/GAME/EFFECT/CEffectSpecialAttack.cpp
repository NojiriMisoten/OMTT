//=============================================================================
//
//	攻撃特化状態の攻撃エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectSpecialAttack.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int ATTACK_ATTACK_COUNT_MAX = 30;
static const float ATTACK_ATTACK_WIDTH = 100;
static const float ATTACK_ATTACK_HEIGHT = 100;
static const TEXTURE_TYPE ATTACK_ATTACK_TEXTURE = TEXTURE_FIRE_1;
static const int ATTACK_ATTACK_TEXTURE_X = 10;
static const int ATTACK_ATTACK_TEXTURE_Y = 1;
static const int ATTACK_ATTACK_TEXTURE_LOOP = 10;
static const float ATTACK_ATTACK_RADIUS = 75;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectSpecialAttack::CEffectSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectSpecialAttack ::~CEffectSpecialAttack(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectSpecialAttack::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = ATTACK_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vRot.z = 0.0f;
	m_vVelocity = velocity;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectSpecialAttack::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectSpecialAttack::Update(void)
{
	if (m_nCount % 3 == 0){
		D3DXVECTOR3 pos = m_vPos;
		pos.x += ATTACK_ATTACK_WIDTH * 0.5f * cosf(m_vRot.z * D3DX_PI);
		pos.y -= ATTACK_ATTACK_WIDTH * 0.5f * sinf(m_vRot.z * D3DX_PI);
		CSceneAnime::Create(
			m_pD3DDevice,
			pos,
			ATTACK_ATTACK_WIDTH, ATTACK_ATTACK_HEIGHT,
			ATTACK_ATTACK_TEXTURE, ATTACK_ATTACK_TEXTURE_X, ATTACK_ATTACK_TEXTURE_Y,
			ATTACK_ATTACK_TEXTURE_LOOP);
	}

	m_vRot.z += 0.2831853f;

	// 自殺の更新
	CEffectBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectSpecialAttack* CEffectSpecialAttack::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// 作成
	CEffectSpecialAttack* p = new CEffectSpecialAttack(pDevice);

	p->Init(pos, velocity);

	return p;
}

//EOF