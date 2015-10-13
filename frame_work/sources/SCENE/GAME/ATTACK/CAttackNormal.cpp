//=============================================================================
//
// CAttacjManagerクラス [CAttacManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CAttackNormal.h"
#include "../EFFECT/CEffectManager.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// 当たり判定の始まる時間
static const short ATTACK_NORMAL_HIT_START_TIME = 5;
// 当たり判定の終わる時間
static const short ATTACK_NORMAL_HIT_END_TIME = 30;

// 当たり判 定幅,高さ
static const float ATTACK_NORMAL_HIT_WIDTH = 50;
static const float ATTACK_NORMAL_HIT_HEIGHT = 50;

// プレイヤと攻撃エフェクトの距離
static const float ATTACK_NORMAL_RANGE = 50;

static const float ATTACK_DAMAGE = -200.f;
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAttackNormal::CAttackNormal(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// 変数初期化
	m_AttackType = ATTACK_TYPE_NORMAL;

	// この攻撃の固有ステータス初期化
	m_fWidth = ATTACK_NORMAL_HIT_WIDTH;
	m_fHeight = ATTACK_NORMAL_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_NORMAL_END_TIME;
	m_nHitStartTime = ATTACK_NORMAL_HIT_START_TIME;
	m_nHitEndTime = ATTACK_NORMAL_HIT_END_TIME;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAttackNormal ::~CAttackNormal(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CAttackNormal::Init()
{
	CAttackBase::Init();
	CManager::PlaySoundA(SOUND_LABEL_SE_NORMAL_ATTACK);
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CAttackNormal::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CAttackNormal::Update(void)
{
	CAttackBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CAttackNormal* CAttackNormal::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// 作成
	CAttackNormal* p = new CAttackNormal(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos + (velocity * ATTACK_NORMAL_RANGE);

	// 初期化
	p->Init();

	// エフェクト生成
	CEffectManager::CreateEffect(p->m_vPos, EFFECT_NORMAL_ATTACK_CAP, velocity);

	return p;
}

//*****************************************************************************
//	攻撃がヒットした際に呼び出される関数
//	 引数　　ヒットしたプレイヤーのプレイヤー番号
//*****************************************************************************
void CAttackNormal::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->AddHp(ATTACK_DAMAGE);
	CAttackBase::HitPlayer(pPlayer);
}

//----EOF-------