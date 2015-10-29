//=============================================================================
//
// CCameraクラス [CCamera.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CCAMERA_H_
#define _CCAMERA_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEffectManager;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 平面構造体
typedef struct
{
	// 平面方程式(a*x)+(b*y)+(c*z)+d = 0(平面法線ベクトル=[a,b,c])
	float a, b, c, d;
}PLANE;

// 視錐台構造体
typedef struct
{
	PLANE LeftPlane;
	PLANE RightPlane;
	PLANE TopPlane;
	PLANE BottomPlane;
	float NearClip;
	float FarClip;
}FRUSTUM;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCamera
{
public:
	// コンストラクタ
	CCamera(CEffectManager *pEffectManager_);

	// デストラクタ
	~CCamera(void);

	//=================================================
	// 初期化
	// 引数: 座標、注視点座標
	//=================================================
	void Init(D3DXVECTOR3& pos, D3DXVECTOR3& posR);

	// 初期化
	void Init(void);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);


	//=================================================
	// カメラシェイク開始
	// 引数: 震源、振幅、総フレーム、減衰率
	//=================================================
	void StartCameraShake( D3DXVECTOR3 epicenter, float amplitude, int totalFrame, float attenuation );

	//=================================================
	// カメラシェイク強制終了
	// 基本は総フレーム数分が完了次第終了するので必要なし
	//=================================================
	void EndCameraShake( void );

	//=================================================
	// カメラ移動 - 瞬間
	// 引数: 移動先視点、移動先注視点
	//=================================================
	void CameraSetToCoord( D3DXVECTOR3 endPosP, D3DXVECTOR3 endPosR );

	//=================================================
	// カメラ移動 -　時間
	// 引数: 移動元視点、移動元注視点、移動先視点、移動先注視点、時間（フレーム）
	//=================================================
	void CameraMoveToCoord( D3DXVECTOR3 startPosP, D3DXVECTOR3 endPosP, D3DXVECTOR3 startPosR, D3DXVECTOR3 endPosR, int totalFrame );

	//=================================================
	// カメラムーブ強制終了
	// 基本は総フレーム数分が完了次第終了するので必要なし
	//=================================================
	void EndCameraMove( void );

	//=================================================
	// カメラセット(描画とかで呼ぶ)
	// 引数: デバイス
	//=================================================
	void SetCamera(LPDIRECT3DDEVICE9 *pDevice);
	void SetLightCamera(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos);
	// オルソーカメラセット
	void SetCameraOrtho(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& cameraPos, D3DXVECTOR3& cameraPosR, D3DXVECTOR3& upVec);

	//=================================================
	// カメラの座標ゲット
	// 戻り値: 座標
	//=================================================
	D3DXVECTOR3& GetCameraPos(void){return m_PosP;};

	//=================================================
	// カメラのフロントベクトルゲット
	// 戻り値: フロントベクトル
	//=================================================
	D3DXVECTOR3& GetVecFront(void){return m_VecFront;};

	//=================================================
	// カメラのライトベクトルゲット
	// 戻り値: ライトベクトル
	//=================================================
	D3DXVECTOR3& GetVecRight(void){return m_VecRight;};

	//=================================================
	// カメラの移動量セット
	// 引数: 移動ベクトル
	//=================================================
	void SetMovCamera(D3DXVECTOR3& movVec){m_MovVec += movVec;};

	//=================================================
	// カメラのY角度ゲット
	// 戻り値: 角度
	//=================================================
	float GetRotCamera(void){return m_Rot.y;};

	//=================================================
	// カメラの注視点ゲット
	// 戻り値: 注視点座標
	//=================================================
	D3DXVECTOR3& GetPosRCamera(void){return m_PosR;};
	
	//=================================================
	// カメラの注視点セット
	// 引数: 座標
	//=================================================
	void SetPosRCamera(D3DXVECTOR3& posR){m_DestPosR = posR;};
	
	//=================================================
	// カメラのアドレスゲット
	// 戻り値: カメラのポインタ
	//=================================================
	CCamera* GetAddresCamera(void){return this;};
	
	//=================================================
	// カメラの行列ゲット
	// 戻り値: 行列
	//=================================================
	D3DXMATRIX& GetMtxView(void){return m_mtxView;};
	D3DXMATRIX& GetMtxLightView(void){ return m_mtxLightView; };

	//=================================================
	// プロジェクションマトリックスゲット
	// 戻り値: 行列
	//=================================================
	D3DXMATRIX& GetMtxProj(void){ return m_mtxProjection; };
	D3DXMATRIX& GetMtxLightProj(void){ return m_mtxLightProjection; };

	//=================================================
	// カメラの行列ゲット
	// 戻り値: 行列
	//=================================================
	D3DXMATRIX& GetMtxViewOrtho(void){ return m_mtxViewOrtho; };

	//=================================================
	// プロジェクションマトリックスゲット
	// 戻り値: 行列
	//=================================================
	D3DXMATRIX& GetMtxProjOrtho(void){ return m_mtxProjectionOrtho; };

	//=================================================
	// カメラの視錐台ゲット
	// 戻り値: 視錐台構造体
	//=================================================
	FRUSTUM& GetFrustum(void){return m_Frustum;};

	//=================================================
	// カメラのFAR値ゲット
	// 戻り値: カメラのFAR値
	//=================================================
	float GetFar(void);

private:
	//=================================================
	// カメラシェイクを管理
	//=================================================
	void ControlShake( void );

	//=================================================
	// カメラをシェイク
	// 引数: 震源、振幅、現在フレーム、総フレーム、減衰率
	//=================================================
	void CameraShake( D3DXVECTOR3 epicenter, float amplitude, int currentFrame, int totalFrame, float attenuation );

	//=================================================
	// カメラ移動を管理
	//=================================================
	void ControlMove( void );

	//=============================================
	// 3点から平面生成
	// 引数: 座標点１、座標点２、座標点３、平面構造体
	//=============================================
	void MakePlaneFrom3Points(D3DXVECTOR3 &P0, D3DXVECTOR3 &P1, D3DXVECTOR3 &P2, PLANE &Plane);

	//=============================================
	// 視錐台生成
	// 引数: 視野角、アスペクト比、近限界値、遠限界値、視錐台構造体
	//=============================================
	void MakeFrustum(float Angle, float Aspect, float NearClip, float FarClip, FRUSTUM &Frustum);

	D3DXVECTOR3			m_PosP;						// カメラの視点（場所）
	D3DXVECTOR3			m_DestPosP;					// カメラの目標の視点（場所）
	D3DXVECTOR3			m_SavePosP;					// カメラの視点（場所）（過去）
	D3DXVECTOR3			m_PosR;						// カメラの注視点（どこからどこまで見てるのか）
	D3DXVECTOR3			m_DestPosR;					// カメラの目標の注視点
	D3DXVECTOR3			m_SavePosR;					// カメラの注視点（過去）
	D3DXVECTOR3			m_VecUp;					// カメラのベクトルの向き（今回は上方向）
	D3DXVECTOR3			m_VecFront;					// カメラのベクトルの向き
	D3DXVECTOR3			m_VecRight;					// カメラのベクトルの向き
	D3DXVECTOR3			m_Rot;						// カメラの向き（回転角）
	D3DXVECTOR3			m_MovVec;					// カメラの移動量と向き
	D3DXMATRIX			m_mtxView;					// ビューマトリックス
	D3DXMATRIX			m_mtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX			m_mtxViewOrtho;				// オルソービューマトリックス
	D3DXMATRIX			m_mtxProjectionOrtho;		// オルソープロジェクションマトリックス
	D3DXMATRIX			m_mtxLightView;				// ライトビューマトリックス
	D3DXMATRIX			m_mtxLightProjection;		// ライトプロジェクションマトリックス
	float				m_DistanceCamera;			// 視点から注視点までの距離
	float				m_angle;					// 角度
	float				m_fLengthInterval;			// 視点から注視点までの距離
	FRUSTUM				m_Frustum;					// 視錐台情報

	// カメラシェイク用メンバー
	bool				m_IsCameraShake;				// カメラシェイクがtrueか
	D3DXVECTOR3			m_Epicenter;					// 震源
	float				m_Amplitude;					// 振幅
	int					m_CurrentShakeFrame;			// 現在フレーム
	int					m_TotalShakeFrame;				// 総フレーム
	float				m_Attenuation;					// 減衰率
	CEffectManager		*m_pEffectManager;				//エフェクトマネージャー

	// カメラムーブ用メンバー
	bool				m_IsCameraMove;
	D3DXVECTOR3			m_StartPosP;
	D3DXVECTOR3			m_StartPosR;
	D3DXVECTOR3			m_EndPosP;
	D3DXVECTOR3			m_EndPosR;
	int					m_CurrentMoveFrame;
	int					m_TotalMoveFrame;
};

#endif
//----EOF----