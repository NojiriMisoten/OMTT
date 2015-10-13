//=============================================================================
//
// CAttacSpecialSpeedクラス [CAttacSpecialSpeed.cpp]
// Author : 佐藤　諒一
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CAttackSpecialTrap.h"
#include "../EFFECT/CEffectManager.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

// 当たり判定の始まる時間
static const short ATTACK_TRAP_HIT_START_TIME = 1;
// 当たり判定の終わる時間
static const short ATTACK_TRAP_HIT_END_TIME = 30;


// 当たり判 定幅,高さ
static const float ATTACK_TRAP_HIT_WIDTH = 30;
static const float ATTACK_TRAP_HIT_HEIGHT = 30;

// 速度
static const float ATTACK_JAMMER_SPD = 10;

static const float ATTACK_DAMAGE = -240;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAttackSpecialTrap::CAttackSpecialTrap(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// 変数初期化
	m_AttackType = ATTACK_TYPE_NORMAL;

	// この攻撃の固有ステータス初期化
	m_fWidth = ATTACK_TRAP_HIT_WIDTH;
	m_fHeight = ATTACK_TRAP_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_TRAP_END_TIME;
	m_nHitStartTime = ATTACK_TRAP_HIT_START_TIME;
	m_nHitEndTime = ATTACK_TRAP_HIT_END_TIME;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAttackSpecialTrap ::~CAttackSpecialTrap(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CAttackSpecialTrap::Init()
{
	CAttackBase::Init();
	CManager::PlaySoundA(SOUND_LABEL_SE_TRAP_ATTACK);
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CAttackSpecialTrap::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CAttackSpecialTrap::Update(void)
{
	CAttackBase::Update();

	// 弾の移動
	m_vPos += m_vDirection * ATTACK_JAMMER_SPD;
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CAttackSpecialTrap* CAttackSpecialTrap::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// 作成
	CAttackSpecialTrap* p = new CAttackSpecialTrap(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos + velocity * ATTACK_JAMMER_SPD;
	p->m_vDirection = velocity;
	// 初期化
	p->Init();

	// エフェクト生成
	CEffectManager::CreateEffect(pos, EFFECT_MP_ATTACK, velocity);


	return p;
}

//=============================================================================
//	プレイヤーに当たった時の処理
//=============================================================================
void CAttackSpecialTrap::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->AddMp(ATTACK_DAMAGE);
	CAttackBase::HitPlayer(pPlayer);
}

//----EOF-------