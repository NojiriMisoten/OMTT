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
static const float ROPE_Y = 37;
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
		m_pRopeOut[i] = NULL;
		m_pRopeIn[i] = NULL;
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
	m_pRopeOut[0] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, -10), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeOut[0]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));
	m_pRopeIn[0] = CRope::Create(m_pD3DDevice,
		// ここ -5 にしないとロープがずれる、原因不明
		D3DXVECTOR3(0, ROPE_Y - 5, -157), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeIn[0]->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0, 0));


	// 奥
	m_pRopeOut[1] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, 10), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeOut[1]->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0, 0));
	m_pRopeIn[1] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(0, ROPE_Y, 157), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeIn[1]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));


	// 左
	m_pRopeOut[2] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(-10, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeOut[2]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));
	m_pRopeIn[2] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(-157, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeIn[2]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));


	// 右
	m_pRopeOut[3] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(10, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeOut[3]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));
	m_pRopeIn[3] = CRope::Create(m_pD3DDevice,
		D3DXVECTOR3(157, ROPE_Y, 0), ROPE_WIDTH, ROPE_HEIGHT, 10, 1, TEXTURE_ROPE, m_pManager);
	m_pRopeIn[3]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));

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
	assert((num >= 0) && (num < RopeNumMax) && "ロープの番号をいれてね！");

	m_pRopeOut[num]->Pull(pullPower, pullInterval);
	m_pRopeIn[num]->Pull(-pullPower, pullInterval);
}

//=============================================================================
// 描画しない
//=============================================================================
void CRopeManager::SetImvisible(void)
{
	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRopeOut[i]->SetDrawFlag(false);
		m_pRopeIn[i]->SetDrawFlag(false);
	}
}

//=============================================================================
// 描画する
//=============================================================================
void CRopeManager::SetVisible(void)
{
	for (int i = 0; i < RopeNumMax; i++)
	{
		m_pRopeOut[i]->SetDrawFlag(true);
		m_pRopeIn[i]->SetDrawFlag(true);
	}
}

//----EOF----