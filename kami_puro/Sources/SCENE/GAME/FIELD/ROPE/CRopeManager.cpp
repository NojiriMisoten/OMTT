//=============================================================================
//
// CRopeManagerManagerクラス [CRopeManagerManager.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CRopeManager.h"
#include "CRope.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// Ｙ座標
static const float ROPE_Y = 45;
// 幅
static const float ROPE_WIDTH = 130;
// 高さ
static const float ROPE_HEIGHT = 40;


//=============================================================================
// コンストラクタ
//=============================================================================
CRopeManager::CRopeManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	m_pD3DDevice = pDevice;
	m_pManager = pManager;

	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRope[i] = NULL;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CRopeManager::~CRopeManager(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CRopeManager::Init()
{
	// 手前
	m_pRope[0] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, -40), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRope[0]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));

	// 奥
	m_pRope[1] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, 40), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRope[1]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI, 0));

	// 左
	m_pRope[2] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(-40, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRope[2]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));

	// 右
	m_pRope[3] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(40, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRope[3]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));
}

//=============================================================================
// 終了
//=============================================================================
void CRopeManager::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CRopeManager::Update(void)
{
}

//=============================================================================
// ゴムアニメ―ションスタート
// どのロープか、引っ張る力、その手を離すまでのフレームカウント
//=============================================================================
void CRopeManager::Pull(RopeNum num, float pullPower, int pullInterval)
{
#ifdef _DEBUG
	assert((num >= 0) && (num < RopeNumMax) && "ロープの番号をいれてね！");
#endif
	m_pRope[num]->Pull(pullPower, pullInterval);
}

//=============================================================================
// 描画しない
//=============================================================================
void CRopeManager::SetImvisible(void)
{
	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRope[i]->SetDrawFlag(false);
	}
}

//=============================================================================
// 描画する
//=============================================================================
void CRopeManager::SetVisible(void)
{
	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRope[i]->SetDrawFlag(true);
	}
}

//----EOF----