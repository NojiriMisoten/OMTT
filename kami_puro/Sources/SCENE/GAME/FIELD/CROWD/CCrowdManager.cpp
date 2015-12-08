//=============================================================================
//
// CCrowdManagerクラス [CCrowdManager.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCrowdManager.h"
#include "CCrowd.h"
#include "../../../MANAGER/CManager.h"
#include "../../../RENDERER/CRenderer.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// リングの前後の観客の板ポリのおおきさ
static const float CROWD_WIDTH = 300;
static const float CROWD_HEIGHT = 70;
// リング前方の観客の位置
static const D3DXVECTOR3 CROWD_POS_FRONT = D3DXVECTOR3(0, 30, 120);
// リング後方の観客の位置
static const D3DXVECTOR3 CROWD_POS_BACK = D3DXVECTOR3(0, 30, -120);

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCrowdManager::CCrowdManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	m_pD3DDevice = pDevice;
	m_pManager = pManager;
	m_pCrowd3DFront = NULL;
	m_pCrowd3DBack = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCrowdManager ::~CCrowdManager(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CCrowdManager::Init()
{
	// リングの前方の観客の板ポリ生成
	m_pCrowd3DFront = CCrowd::Create(m_pD3DDevice,
		D3DXVECTOR3(CROWD_POS_FRONT),
		CROWD_WIDTH, CROWD_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
	m_pCrowd3DFront->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));

	// リングの後方の観客の板ポリ生成
	m_pCrowd3DBack = CCrowd::Create(m_pD3DDevice,
		D3DXVECTOR3(CROWD_POS_BACK),
		CROWD_WIDTH, CROWD_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
	m_pCrowd3DBack->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));

}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CCrowdManager::Uninit(void)
{
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CCrowdManager::Update(void)
{
}


//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CCrowdManager* CCrowdManager::Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	// 作成
	CCrowdManager* p = new CCrowdManager(pDevice, pManager);

	// 初期化
	p->Init();

	return p;

}

//*****************************************************************************
// 描画しない
//*****************************************************************************
void CCrowdManager::SetImvisible(void)
{
	m_pCrowd3DFront->SetDrawFlag(false);
	m_pCrowd3DBack->SetDrawFlag(false);
}

//*****************************************************************************
// 描画する
//*****************************************************************************
void CCrowdManager::SetVisible(void)
{
	m_pCrowd3DFront->SetDrawFlag(true);
	m_pCrowd3DBack->SetDrawFlag(true);
}
//----EOF----