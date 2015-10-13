//=============================================================================
//
//	移動特化状態の攻撃エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectSpecialSpeedAttack.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int SPEED_ATTACK_COUNT_MAX = 70;
static const float SPEED_ATTACK_WIDTH = 180;
static const float SPEED_ATTACK_HEIGHT = 180;
static const TEXTURE_TYPE SPEED_ATTACK_TEXTURE = TEXTURE_ATTACK_TACKLE;
static const int SPEED_ATTACK_TEXTURE_X = 1;
static const int SPEED_ATTACK_TEXTURE_Y = 7;
static const int SPEED_ATTACK_TEXTURE_LOOP = 70;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectSpecialSpeedAttack::CEffectSpecialSpeedAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
	m_pAnim = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectSpecialSpeedAttack ::~CEffectSpecialSpeedAttack(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectSpecialSpeedAttack::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = SPEED_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vVelocity = velocity;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectSpecialSpeedAttack::Uninit(void)
{
	m_pAnim = NULL;
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectSpecialSpeedAttack::Update(void)
{
	if (m_nCount == 0){
		m_pAnim = CSceneAnime::Create(
					m_pD3DDevice,
					m_vPos,
					SPEED_ATTACK_WIDTH, SPEED_ATTACK_HEIGHT,
					SPEED_ATTACK_TEXTURE, SPEED_ATTACK_TEXTURE_X, SPEED_ATTACK_TEXTURE_Y,
					SPEED_ATTACK_TEXTURE_LOOP);

		if (m_vVelocity.y == -1)
		{
			m_vRot.z = D3DX_PI*0.5f;
			m_pAnim->SetRot(m_vRot);
		}
		if (m_vVelocity.y == 1)
		{
			m_vRot.z = D3DX_PI*1.5f;
			m_pAnim->SetRot(m_vRot);
		}
		if (m_vVelocity.x == 1)
		{
			m_vRot.z = 0.0f;
		}
		if (m_vVelocity.x == -1)
		{
			m_vRot.z = D3DX_PI;
			m_pAnim->SetRot(m_vRot);
		}
	}

	m_vPos += m_vVelocity * 100.0f;

	//m_pAnim->SetPos(m_vPos);

	// 自殺の更新
	CEffectBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectSpecialSpeedAttack* CEffectSpecialSpeedAttack::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// 作成
	CEffectSpecialSpeedAttack* p = new CEffectSpecialSpeedAttack(pDevice);

	p->Init(pos, velocity);

	return p;
}

//EOF