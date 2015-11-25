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
CCameraManager::CCameraManager(CEffectManager* pEffectManager)
{
	m_pCamera = NULL;
	m_pEffectManager = pEffectManager;
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

//=================================================
// カメラシェイク開始
// 引数: 震源、振幅、総フレーム、減衰率
//=================================================
void CCameraManager::StartCameraShake( D3DXVECTOR3 epicenter, float amplitude, int totalFrame, float attenuation )
{
	m_pCamera->StartCameraShake( epicenter, amplitude, totalFrame, attenuation );
}

//=================================================
// カメラシェイク強制終了
// 基本は総フレーム数分が完了次第終了するので必要なし
//=================================================
void CCameraManager::EndCameraShake( void )
{
	m_pCamera->EndCameraShake();
}

//=================================================
// カメラ移動 - 瞬間
// 引数: 移動先視点、移動先注視点
//=================================================
void CCameraManager::CameraSetToCoord( D3DXVECTOR3 endPosP, D3DXVECTOR3 endPosR )
{
	m_pCamera->CameraSetToCoord( endPosP, endPosR );
}

//=================================================
// カメラ移動 -　時間
// 引数: 移動元視点、移動先視点、移動元注視点、移動先注視点、時間（フレーム）
//=================================================
void CCameraManager::CameraMoveToCoord( D3DXVECTOR3 startPosP, D3DXVECTOR3 endPosP, D3DXVECTOR3 startPosR, D3DXVECTOR3 endPosR, int totalFrame )
{
	m_pCamera->CameraMoveToCoord( startPosP, endPosP, startPosR, endPosR, totalFrame );
}

//----EOF----