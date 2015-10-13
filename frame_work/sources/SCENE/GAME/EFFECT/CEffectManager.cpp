//=============================================================================
//
// CEffectManagerクラス [CEffectManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "CEffectManager.h"
#include "CEffectAttackHit.h"				// 攻撃ヒット
#include "CEffectExplosion.h"				// 爆発
#include "CEffectSpecialAttack.h"			// 攻撃特化攻撃
#include "CEffectSpecialSpeedAttack.h"		// 移動特化攻撃
#include "CEffectSpecialAttackThread.h"		// 攻撃特化糸攻撃
#include "CEffectCaughtTrap.h"				// 罠にかかった時
#include "CEffectCreateRoad.h"				// 道作成時
#include "CEffectFlagGet.h"					// フラグ取得時
#include "CEffectFlagHold.h"				// フラグ所持時
#include "CEffectDead.h"					// プレイヤー体力０
#include "CEffectMPAttack.h"				// MPダメージ時
#include "CEffectNormalAttackCap.h"			// 帽子攻撃
#include "CEffectNormalAttackThread.h"		// 通常糸発射
#include "CEffectPlayerRevive.h"			// プレイヤー復活時
#include "CEffectTrapFoundation.h"			// 罠設置時
#include "CEffectMetamorphose.h"			// 変身

//*****************************************************************************
// マクロ
//*****************************************************************************


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DDEVICE9* CEffectManager::m_pDevice;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectManager::CEffectManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectManager ::~CEffectManager(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectManager::Init()
{
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectManager::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectManager::Update(void)
{
}

//*****************************************************************************
// エフェクト生成
//*****************************************************************************
void CEffectManager::CreateEffect(D3DXVECTOR3 pos, EFFECT_TYPE type, D3DXVECTOR3 velocity)
{
	switch (type)
	{
		// 攻撃がヒットした時のエフェクト
	case EFFECT_ATTACK_HIT:
		CEffectAttackHit::Create(m_pDevice, pos);
		break;
		// 爆発エフェクト
	case EFFECT_EXPLOSION:
		CEffectExplosion::Create(m_pDevice, pos);
		break;
		// 攻撃形態時の攻撃
	case EFFECT_SPECIAL_ATTACK_ATTACK:
		CEffectSpecialAttack::Create(m_pDevice, pos, velocity);
		break;
		// 移動形態時の攻撃
	case EFFECT_SPECIAL_ATTACK_SPEED:
		CEffectSpecialSpeedAttack::Create(m_pDevice, pos, velocity);
		break;
		// 攻撃形態時の糸攻撃
	case EFFECT_SPECIAL_THREAD_ATTACK:
		CEffectSpecialAttackThread::Create(m_pDevice, pos, velocity);
		break;
		// 罠にかかった時のエフェクト
	case EFFECT_CAUGHT_TRAP:
		CEffectCaughtTrap::Create(m_pDevice, pos);
		break;
		// 道作成時のエフェクト
	case EFFECT_CREATE_ROAD:
		CEffectCreateRoad::Create(m_pDevice, pos);
		break;
		// フラグ取得時
	case EFFECT_FLAG_GET:
		CEffectFlagGet::Create(m_pDevice, pos);
		break;
		// フラグ所持時
	case EFFECT_FLAG_HOLD:
		CEffectFlagHold::Create(m_pDevice, pos);
		break;
		// プレイヤーの体力０時のエフェクト
	case EFFECT_PLAYER_DEAD:
		CEffectDead::Create(m_pDevice, pos);
		break;
		// MP攻撃時のエフェクト
	case EFFECT_MP_ATTACK:
		CEffectMPAttack::Create(m_pDevice, pos, velocity);
		break;
		// 通常形態の帽子攻撃エフェクト
	case EFFECT_NORMAL_ATTACK_CAP:
		CEffectNormalAttackCap::Create(m_pDevice, pos);
		break;
		// 通常形態の糸発射エフェクト
	case EFFECT_NORMAL_ATTACK_THREAD:
		CEffectNormalAttackThread::Create(m_pDevice, pos, velocity);
		break;
		// プレイヤー復活エフェクト
	case EFFECT_PLAYER_REVIAVE:
		CEffectPlayerRevive::Create(m_pDevice, pos);
		break;
	default:
		break;
	}
}

//*****************************************************************************
// エフェクト生成
//*****************************************************************************
void CEffectManager::CreateEffectMeta(D3DXVECTOR3 pos, short playerNum)
{
	// 変身エフェクト
	CEffectMetamorphose::Create(m_pDevice,pos,playerNum);
}

//*****************************************************************************
// エフェクト生成
//*****************************************************************************
void CEffectManager::CreateEffectTrap(D3DXVECTOR3 pos, short playerNum)
{
	// トラップエフェクト
	CEffectTrapFoundation::Create(m_pDevice, pos, playerNum);
}
//----EOF-------