//=============================================================================
//
// CCameraManagerクラス [CCameraManager.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CCAMERAMANAGER_H_
#define _CCAMERAMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "CCamera.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEffectManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCameraManager
{
public:
	// コンストラクタ
	CCameraManager(CEffectManager* pEffectManager);

	// デストラクタ
	~CCameraManager(void);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//=================================================
	// カメラクリエイト
	//=================================================
	void CreateCamera(D3DXVECTOR3& pos, D3DXVECTOR3& posR);

	//=================================================
	// カメラセット(描画とかで呼ぶ)
	// 引数: デバイス
	//=================================================
	void SetCamera(LPDIRECT3DDEVICE9 *pDevice);
	void SetLightCamera(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos);

	//=================================================
	// カメラの座標ゲット
	// 戻り値: 座標
	//=================================================
	D3DXVECTOR3& GetCameraPos(void){ return m_pCamera->GetCameraPos(); };

	//=================================================
	// カメラのフロントベクトルゲット
	// 戻り値: フロントベクトル
	//=================================================
	D3DXVECTOR3& GetVecFront(void){ return m_pCamera->GetVecFront(); };

	//=================================================
	// カメラのライトベクトルゲット
	// 戻り値: ライトベクトル
	//=================================================
	D3DXVECTOR3& GetVecRight(void){ return m_pCamera->GetVecRight(); };

	//=================================================
	// カメラのY角度ゲット
	// 戻り値: 角度
	//=================================================
	float GetRotCamera(void){ return m_pCamera->GetRotCamera(); };

	//=================================================
	// カメラの注視点ゲット
	// 戻り値: 注視点座標
	//=================================================
	D3DXVECTOR3& GetPosRCamera(void){ return m_pCamera->GetPosRCamera(); };
	
	//=================================================
	// カメラの注視点セット
	// 引数: 座標
	//=================================================
	void SetPosRCamera(D3DXVECTOR3& posR){ m_pCamera->SetPosRCamera(posR); };
	
	//=================================================
	// カメラのアドレスゲット
	// 戻り値: カメラのポインタ
	//=================================================
	CCamera* GetAddresCamera(void){ return m_pCamera; };
	
	//=================================================
	// カメラの行列ゲット
	// 戻り値: 行列
	//=================================================
	D3DXMATRIX& GetMtxView(void){ return m_pCamera->GetMtxView(); };
	D3DXMATRIX& GetMtxViewOrtho(void){ return m_pCamera->GetMtxViewOrtho(); };
	D3DXMATRIX& GetMtxLightView(void){ return m_pCamera->GetMtxLightView(); };

	//=================================================
	// プロジェクションマトリックスゲット
	// 戻り値: 行列
	//=================================================
	D3DXMATRIX& GetMtxProj(void){ return m_pCamera->GetMtxProj(); };
	D3DXMATRIX& GetMtxProjOrtho(void){ return m_pCamera->GetMtxProjOrtho(); };
	D3DXMATRIX& GetMtxLightProj(void){ return m_pCamera->GetMtxLightProj(); };

	//=================================================
	// オルソーセット
	//=================================================
	void SetOrtho(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& cameraPos, D3DXVECTOR3& cameraPosR, D3DXVECTOR3& upVec)
	{
		m_pCamera->SetCameraOrtho(pDevice, cameraPos, cameraPosR, upVec);
	};

	//=================================================
	// カメラの視錐台ゲット
	// 戻り値: 視錐台構造体
	//=================================================
	FRUSTUM& GetFrustum(void){ return m_pCamera->GetFrustum(); };

	//=================================================
	// カメラのFAR値ゲット
	// 戻り値: カメラのFAR値
	//=================================================
	float GetFar(void){ return m_pCamera->GetFar(); };

private:
	CCamera* m_pCamera;
	CEffectManager			*m_pEffectManager;				//エフェクトマネージャー
};

#endif
//----EOF----