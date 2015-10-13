//=============================================================================
//
// CThreadManagerクラス [CThreadManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "CThreadManager.h"
#include "CThreadBase.h"
#include "CThreadNormal.h"
#include "CThreadSpecialAttack.h"
#include "CThreadSpecialSpeed.h"
#include "CThreadSpecialTrap.h"

//********************************************** *******************************
// マクロ
//*****************************************************************************


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************


//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CThreadManager::CThreadManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CThreadManager ::~CThreadManager(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CThreadManager::Init()
{
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CThreadManager::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CThreadManager::Update(void)
{
}

//*****************************************************************************
// 糸を生成する
//*****************************************************************************
void CThreadManager::CreateThread(ThreadType type, int nPlayerNum, D3DXVECTOR3 pos, DIRECTION_PLAYER_FACING playerFacing, CEffectManager *pEffectManager)
{
	switch (type)
	{
		// 通常形態の糸
	case THREAD_TYPE_NORMAL:
		CThreadNormal::Create(m_pDevice, nPlayerNum, pos, PLAYER_DIRECTION_VECTOR[playerFacing]);
		break;
		// 攻撃特化形態の糸
	case THREAD_TYPE_ATTACK:
		CThreadSpecialAttack::Create(m_pDevice, nPlayerNum, pos , PLAYER_DIRECTION_VECTOR[playerFacing]);
		break;
		// 移動特化形態の糸
	case THREAD_TYPE_SPEED:
		CThreadSpecialSpeed::Create(m_pDevice, nPlayerNum, pos, playerFacing);
		break;
		// 罠特化形態の糸
	case THREAD_TYPE_TRAP:
		CThreadSpecialTrap::Create(m_pDevice, nPlayerNum, pos);
		break;
	default:
		break;
	}
}
//----EOF-------