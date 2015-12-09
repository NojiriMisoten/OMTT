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
	m_pRing = NULL;
	m_pFrontChair = NULL;
	m_pOverChair = NULL;
	m_pLeftChair = NULL;
	m_pStage = NULL;
	m_pRightChair = NULL;
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

	// リング
	m_pRing = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_RING, m_pManager);
	m_pRing->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pRing->SetRot(D3DXVECTOR3(0, D3DX_PI, 0));

	// 椅子
	m_pFrontChair = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_FRONT_CHAIR, m_pManager);
	m_pFrontChair->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pOverChair = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_OVER_CHAIR, m_pManager);
	m_pOverChair->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pLeftChair = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_SIDE_CHAIR, m_pManager);
	m_pLeftChair->SetScl(D3DXVECTOR3(50, 50, 50));
	m_pLeftChair->SetRot(D3DXVECTOR3(0, D3DX_PI, 0));
	m_pRightChair = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_SIDE_CHAIR, m_pManager);
	m_pRightChair->SetScl(D3DXVECTOR3(50, 50, 50));

	// ステージ
	m_pStage = CSceneX::Create(m_pDevice, D3DXVECTOR3(0, 0, 0), MODEL_STAGE, m_pManager);
	m_pStage->SetScl(D3DXVECTOR3(50, 50, 50));
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
	m_pRing->SetDrawFlag(false);
	m_pFrontChair->SetDrawFlag(false);
	m_pOverChair->SetDrawFlag(false);
	m_pLeftChair->SetDrawFlag(false);
	m_pStage->SetDrawFlag(false);
	m_pRightChair->SetDrawFlag(false);
}

//*****************************************************************************
// 描画する
//*****************************************************************************
void CFieldManager::SetVisible(void)
{
	m_pRopeManger->SetVisible();
	m_pRing->SetDrawFlag(true);
	m_pFrontChair->SetDrawFlag(true);
	m_pOverChair->SetDrawFlag(true);
	m_pLeftChair->SetDrawFlag(true);
	m_pStage->SetDrawFlag(true);
	m_pRightChair->SetDrawFlag(true);
}
//----EOF----