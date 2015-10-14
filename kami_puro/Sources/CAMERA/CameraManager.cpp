//=============================================================================
//
// CCameraManagerクラス [CCameraManager.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CameraManager.h"

#include "../EFECT/CEffectManager.h"

//=================================================
// コンストラクタ
//=================================================
CCameraManager::CCameraManager(CEffectManager *pEffectManager_)
{
	m_pEffectManager = pEffectManager_;
	m_pCamera = NULL;
}

//=================================================
// デストラクタ
//=================================================
CCameraManager::~CCameraManager(void)
{
	m_pCamera = NULL;
}

//=================================================
// 終了
//=================================================
void CCameraManager::Uninit(void)
{
	if(m_pCamera)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//=================================================
// 更新
//=================================================
void CCameraManager::Update(void)
{
	m_pCamera->Update();
}

//=================================================
// カメラクリエイト
//=================================================
void CCameraManager::CreateCamera(D3DXVECTOR3& pos, D3DXVECTOR3& posR)
{
	m_pCamera = new CCamera(m_pEffectManager);
	m_pCamera->Init(pos, posR);
}

//=================================================
// カメラセット(描画とかで呼ぶ)
// 引数: デバイス
//=================================================
void CCameraManager::SetCamera(LPDIRECT3DDEVICE9 *pDevice)
{
	if (m_pCamera)
	{
		m_pCamera->SetCamera(pDevice);
	}
}
void CCameraManager::SetLightCamera(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos)
{
	if (m_pCamera)
	{
		m_pCamera->SetLightCamera(pDevice, pos);
	}
};
//----EOF----