//=============================================================================
//
//	攻撃特化状態の糸攻撃エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectSpecialAttackThread.h"
#include "../../../MANAGER/CManager.h"
#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int THREAD_ATTACK_COUNT_MAX = 100;
static const float THREAD_ATTACK_WIDTH = 100;
static const float THREAD_ATTACK_HEIGHT = 40;
static const float THREAD_ATTACK_WIDTH_SECOND = 220;
static const float THREAD_ATTACK_HEIGHT_SECOND = 80;
static const TEXTURE_TYPE THREAD_ATTACK_TEXTURE = TEXTURE_ATTACK_THUNDERBOLT;
static const int THREAD_ATTACK_TEXTURE_X = 1;
static const int THREAD_ATTACK_TEXTURE_Y = 8;
static const int THREAD_ATTACK_TEXTURE_LOOP1 = 60;
static const int THREAD_ATTACK_TEXTURE_LOOP = 40;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectSpecialAttackThread::CEffectSpecialAttackThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectSpecialAttackThread ::~CEffectSpecialAttackThread(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectSpecialAttackThread::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = THREAD_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vRot.z = 0.0f;
	m_vVelocity = velocity;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectSpecialAttackThread::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectSpecialAttackThread::Update(void)
{
	if (m_nCount == 0){
		CManager::PlaySoundA(SOUND_LABEL_SE_THUNDERBOLT);
		m_pAnim[0] = CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			THREAD_ATTACK_WIDTH, THREAD_ATTACK_HEIGHT,
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP1);
		if (m_vVelocity.y == -1)
		{
			m_vRot.z = D3DX_PI*0.5f;
			m_pAnim[0]->SetRot(m_vRot);
		}
		if (m_vVelocity.y == 1)
		{
			m_vRot.z = D3DX_PI*1.5f;
			m_pAnim[0]->SetRot(m_vRot);
		}
		if (m_vVelocity.x == 1)
		{
			m_vRot.z = 0.0f;
		}
		if (m_vVelocity.x == -1)
		{
			m_vRot.z = D3DX_PI;
			m_pAnim[0]->SetRot(m_vRot);
		}
	}
	else if (m_nCount == 50)
	{
		CManager::PlaySoundA(SOUND_LABEL_SE_THUNDERBOLT_2);
		//m_vPos += m_vVelocity * 50.0f;
		m_pAnim[0] = CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos + D3DXVECTOR3(m_vVelocity.x * 100.0f, (m_vVelocity.y * 100.0f), 0.0f),
			THREAD_ATTACK_WIDTH_SECOND + (abs(m_vVelocity.x) * 100.0f), THREAD_ATTACK_HEIGHT_SECOND + (abs(m_vVelocity.y) * 100.0f),
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP);
		m_vPos.x += m_vVelocity.x * 5.0f;
		m_vPos.y += m_vVelocity.y * 5.0f;

		m_pAnim[1] = CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos + D3DXVECTOR3(m_vVelocity.x * 100.0f, (m_vVelocity.y * 100.0f), 0.0f),
			THREAD_ATTACK_WIDTH_SECOND + (abs(m_vVelocity.x) * 100.0f), THREAD_ATTACK_HEIGHT_SECOND + (abs(m_vVelocity.y) * 100.0f),
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP);
		m_vPos.x += m_vVelocity.x * 5.0f;
		m_vPos.y += m_vVelocity.y * 5.0f;
		m_vPos.y -= 5.0f;
		m_vPos.x -= 5.0f;

		m_pAnim[2] = CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos + D3DXVECTOR3(m_vVelocity.x * 100.0f, (m_vVelocity.y * 100.0f), 0.0f),
			THREAD_ATTACK_WIDTH_SECOND + (abs(m_vVelocity.x) * 100.0f), THREAD_ATTACK_HEIGHT_SECOND + (abs(m_vVelocity.y) * 100.0f),
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP);
		m_vPos.x += m_vVelocity.x * 5.0f;
		m_vPos.y += m_vVelocity.y * 5.0f;
		m_vPos.y -= 10.0f;
		m_vPos.x -= 10.0f;

		m_pAnim[3] = CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos + D3DXVECTOR3(m_vVelocity.x * 100.0f, (m_vVelocity.y * 100.0f), 0.0f),
			THREAD_ATTACK_WIDTH_SECOND + (abs(m_vVelocity.x) * 100.0f), THREAD_ATTACK_HEIGHT_SECOND + (abs(m_vVelocity.y) * 100.0f),
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP);

		for (int i = 0; i < 4; i++)
		{
			if (m_vVelocity.y == -1)
			{
				m_vRot.z = D3DX_PI*0.5f;
				m_pAnim[i]->SetRot(m_vRot);
			}
			if (m_vVelocity.y == 1)
			{
				m_vRot.z = D3DX_PI*1.5f;
				m_pAnim[i]->SetRot(m_vRot);
			}
			if (m_vVelocity.x == 1)
			{
				m_vRot.z = 0.0f;
			}
			if (m_vVelocity.x == -1)
			{
				m_vRot.z = D3DX_PI;
				m_pAnim[i]->SetRot(m_vRot);
			}
		}
	}

	// 自殺の更新
	CEffectBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectSpecialAttackThread* CEffectSpecialAttackThread::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// 作成
	CEffectSpecialAttackThread* p = new CEffectSpecialAttackThread(pDevice);

	p->Init(pos, velocity);

	return p;

#ifdef _DEBUG
	CDebugProc::Print("攻撃特化状態の糸のエフェクト作ったよ\n");
#endif
}

//EOF