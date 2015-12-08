//=============================================================================
//
// CFieldManagerクラス [CFieldManager.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MANAGER/CManager.h"
#include "CFieldManager.h"
#include "ROPE\CRopeManager.h"
#include "../../../BASE_OBJECT/CSceneX.h"

//*****************************************************************************
// 定数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CFieldManager::CFieldManager(
	LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	m_pDevice = pDevice;
	m_pManager = pManager;

	m_pRopeManger = NULL;
	m_pDome = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CFieldManager::~CFieldManager(void)
{

}

//*****************************************************************************
// 初期化処理
//*****************************************************************************
void CFieldManager::Init(void)
{
	// ロープの生成
	m_pRopeManger = new CRopeManager(m_pDevice, m_pManager);
	m_pRopeManger->Init();

	// 観客席
	m_pDome = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_DOME, m_pManager);
	m_pDome->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pDome->SetRot(D3DXVECTOR3(0, D3DX_PI*0.5f, 0));
}

//*****************************************************************************
// 作成
//*****************************************************************************
CFieldManager *CFieldManager::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	CManager *pManager)
{
	CFieldManager* p = new CFieldManager(pDevice, pManager);
	p->Init();
	return p;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CFieldManager::Uninit(void)
{
	m_pRopeManger->Uninit();
	SAFE_DELETE(m_pRopeManger);
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CFieldManager::Update(void)
{
	// test
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_ROPE_BIYON))
	{
		m_pRopeManger->Pull(CRopeManager::RopeNumLeft, 5.6f, 90);
	}
}

//*****************************************************************************
// 描画しない
//*****************************************************************************
void CFieldManager::SetImvisible(void)
{
	m_pRopeManger->SetImvisible();
	m_pDome->SetDrawFlag(false);
}

//*****************************************************************************
// 描画する
//*****************************************************************************
void CFieldManager::SetVisible(void)
{
	m_pRopeManger->SetVisible();
	m_pDome->SetDrawFlag(true);
}
//----EOF----