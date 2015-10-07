//=============================================================================
//
// CCameraクラス [CCamera.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MANAGER/CManager.h"
#include "CCamera.h"
#include "../INPUT/CInputKeyboard.h"
#include "../INPUT/CInputGamePad.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const float			NEAR_VAL = 1.0f;										// near値 カメラに映る近さの限界
static const float			FAR_VAL = 1000.0f;										// far値 カメラに映る遠さの限界
static const float			DEFAULT_CAMERA_MOV_R_SPD = 80.0f;						// カメラの注視点変更量
static const float			DEFAULT_CAMERA_MOV_SPD = 1.8f;							// カメラの移動スピード
static const float			DEFAULT_CAMERA_ANGL_SPD = 0.03f;						// カメラの回転スピード
static const float			LIMIT_UNDER_ANGLE = -1.35f;								// カメラの下への角度の限界値
static const float			LIMIT_UP_ANGLE = -0.014f;								// カメラの上への角度の限界値
static const float			BASE_HIGHT_POS = 100.0f;								// カメラの基本の高さ
static const float			VIEW_ANGLE = D3DX_PI / 4.f;								// 視野角
static const D3DXVECTOR3	DEFAULT_CAMERA_POS(0.0f, 40.0f, -160.0f);				// デフォルトのカメラ座標
static const D3DXVECTOR3	DEFAULT_CAMERA_POS_R(0.f, 0.f, 0.f);					// デフォルトのカメラ注視点座標
static const float			MAX_CAMERA_MOV_COEFFICIENT = 1.0f;						// カメラ移動係数の最大値
static const float			MIN_CAMERA_MOV_COEFFICIENT = 0.1f;						// カメラ移動係数の最小値

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCamera::CCamera(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCamera::~CCamera(void)
{
}

//*****************************************************************************
// 初期化 引数あり
//*****************************************************************************
void CCamera::Init(D3DXVECTOR3& pos, D3DXVECTOR3& posR)
{
	// 座標
	m_PosP = pos;
	m_DestPosP = pos;
	
	// カメラの回転（見ている場所 ex:この場合m_PosPの座標からm_PosRの座標を見ている）
	m_PosR = posR;
	m_DestPosR = posR;
	
	// カメラの方向
	m_VecUp = DEFAULT_UP_VECTOR;
	m_VecFront = DEFAULT_FRONT_VECTOR;
	m_VecRight = DEFAULT_RIGHT_VECTOR;

	// 注視点と視点の距離
	D3DXVECTOR3 length = m_PosR - m_PosP;
	m_DistanceCamera = sqrt(length.x * length.x + length.z * length.z);
	m_fLengthInterval = sqrtf((m_PosR.x - m_PosP.x) * (m_PosR.x - m_PosP.x)
								+ (m_PosR.y - m_PosP.y) * (m_PosR.y - m_PosP.y)
								+ (m_PosR.z - m_PosP.z) * (m_PosR.z - m_PosP.z));

	// 角度の初期化
	m_Rot = D3DXVECTOR3(0,0,0);
	m_Rot.y = atan2f((m_PosR.x - m_PosP.x), (m_PosR.z - m_PosP.z));
	m_Rot.x = atan2f((m_PosR.y - m_PosP.y), m_DistanceCamera);

	// 移動量の初期化
	m_MovVec = D3DXVECTOR3(0,0,0);

	// 視錐台作成
	MakeFrustum(VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_VAL, FAR_VAL, m_Frustum);
}

//*****************************************************************************
// 初期化 引数なし
//*****************************************************************************
void CCamera::Init(void)
{
	// 座標
	m_PosP = DEFAULT_CAMERA_POS;
	m_DestPosP = DEFAULT_CAMERA_POS;
	
	// カメラの回転（見ている場所 ex:この場合m_PosPの座標からm_PosRの座標を見ている）
	m_PosR = DEFAULT_CAMERA_POS_R;
	m_DestPosR = DEFAULT_CAMERA_POS_R;
	
	// カメラの方向
	m_VecUp = DEFAULT_UP_VECTOR;
	m_VecFront = DEFAULT_FRONT_VECTOR;
	m_VecRight = DEFAULT_RIGHT_VECTOR;

	// 注視点と視点の距離
	D3DXVECTOR3 length = m_PosR - m_PosP;
	m_DistanceCamera = sqrt(length.x * length.x + length.z * length.z);
	m_fLengthInterval = sqrtf((m_PosR.x - m_PosP.x) * (m_PosR.x - m_PosP.x)
								+ (m_PosR.y - m_PosP.y) * (m_PosR.y - m_PosP.y)
								+ (m_PosR.z - m_PosP.z) * (m_PosR.z - m_PosP.z));

	// 角度の初期化
	m_Rot = D3DXVECTOR3(0,0,0);
	m_Rot.y = atan2f((m_PosR.x - m_PosP.x), (m_PosR.z - m_PosP.z));
	m_Rot.x = atan2f((m_PosR.y - m_PosP.y), m_DistanceCamera);

	// 移動量の初期化
	m_MovVec = D3DXVECTOR3(0,0,0);

	// 視錐台作成
	MakeFrustum(VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_VAL, FAR_VAL, m_Frustum);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CCamera::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CCamera::Update(void)
{
	MovePos();

	// フロントベクトルの設定
	m_VecFront = m_PosR - m_PosP;
	D3DXVec3Normalize(&m_VecFront, &m_VecFront);

	// ライトベクトルの設定
	D3DXVec3Cross(&m_VecRight, &m_VecUp, &m_VecFront);
	D3DXVec3Normalize(&m_VecRight, &m_VecRight);

	// レンダーに角度教える
	CRenderer::TeachCameraRot(m_Rot.y);
	CRenderer::TeachProjMtx(m_mtxProjection);
	CRenderer::TeachViewMtx(m_mtxView);

#ifdef _DEBUG
	CDebugProc::Print("CameraX:%f\nCameraY:%f\nCameraZ:%f\n", m_PosP.x, m_PosP.y, m_PosP.z);
	CDebugProc::Print("CameraRotX:%f\nCameraRotY:%f\nCameraRotZ:%f\n", m_Rot.x, m_Rot.y, m_Rot.z);
#endif
}

//*****************************************************************************
// セットカメラ
//*****************************************************************************
void CCamera::SetCamera(LPDIRECT3DDEVICE9 *pDevice)
{
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);
	
	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_PosP, &m_PosR, &m_VecUp);
	
	// ビューマトリックスの設定
	(*pDevice)->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection
							, VIEW_ANGLE						// 視野角
							, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT	// アスペクト比
							, NEAR_VAL							// near値
							, FAR_VAL);							// far値
	
	// プロジェクションマトリックスの設定
	(*pDevice)->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}
void CCamera::SetLightCamera(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos)
{
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxLightView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxLightView, &pos, &m_PosR, &m_VecUp);

	// ビューマトリックスの設定
	(*pDevice)->SetTransform(D3DTS_VIEW, &m_mtxLightView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxLightProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxLightProjection
		, VIEW_ANGLE						// 視野角
		, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT	// アスペクト比
		, NEAR_VAL							// near値
		, FAR_VAL);							// far値

	// プロジェクションマトリックスの設定
	(*pDevice)->SetTransform(D3DTS_PROJECTION, &m_mtxLightProjection);
}

//*****************************************************************************
// オルソーカメラセット
//*****************************************************************************
void CCamera::SetCameraOrtho(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& cameraPos, D3DXVECTOR3& cameraPosR, D3DXVECTOR3& upVec)
{
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxViewOrtho);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxViewOrtho, &cameraPos, &cameraPosR, &upVec);

	// ビューマトリックスの設定
	(*pDevice)->SetTransform(D3DTS_VIEW, &m_mtxViewOrtho);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjectionOrtho);

	// プロジェクションマトリックスの作成
	D3DXMatrixOrthoLH(&m_mtxProjectionOrtho
					, 80.f
					, 80.f
					, -40.f
					, 40.f);

	// プロジェクションマトリックスの設定
	(*pDevice)->SetTransform(D3DTS_PROJECTION, &m_mtxProjectionOrtho);
}

//*****************************************************************************
// 3点から平面作成関数
//*****************************************************************************
void CCamera::MakePlaneFrom3Points(D3DXVECTOR3 &P0, D3DXVECTOR3 &P1, D3DXVECTOR3 &P2, PLANE &Plane)
{
	D3DXVECTOR3 V0, V1, V2;
	V0 = P1 - P0;
	V1 = P2 - P0;
	D3DXVec3Cross(&V2, &V0, &V1);
	D3DXVec3Normalize(&V2, &V2);
	Plane.a = V2.x;
	Plane.b = V2.y;
	Plane.c = V2.z;
	Plane.d = -((V2.x * P0.x) + (V2.y * P0.y) + (V2.z * P0.z));
}

//*****************************************************************************
// 視錐台生成関数
//*****************************************************************************
void CCamera::MakeFrustum(float Angle, float Aspect, float NearClip, float FarClip, FRUSTUM &Frustum)
{
	D3DXVECTOR3 P0, P1, P2;
	float fTan = tan(Angle * 0.5f);	// 正面から半分にしたいから
	float fTanDivAspect = fTan / Aspect;
	float fFarMulTan = FarClip * fTan;
	float fFarMulTanDivAspect = FarClip * fTanDivAspect;

	// 左平面生成
	P0.x = 0.f;
	P0.y = 0.f;
	P0.z = 0.f;
	P1.x = -fFarMulTanDivAspect;
	P1.y = -fFarMulTan;
	P1.z = FarClip;
	P2.x = P1.x;
	P2.y = -P1.y;
	P2.z = P1.z;
	MakePlaneFrom3Points(P0, P1, P2, Frustum.LeftPlane);

	// 右平面生成
	P0.x = 0.f;
	P0.y = 0.f;
	P0.z = 0.f;
	P1.x = fFarMulTanDivAspect;
	P1.y = fFarMulTan;
	P1.z = FarClip;
	P2.x = P1.x;
	P2.y = -P1.y;
	P2.z = P1.z;
	MakePlaneFrom3Points(P0, P1, P2, Frustum.RightPlane);

	// 上平面生成
	P0.x = 0.f;
	P0.y = 0.f;
	P0.z = 0.f;
	P1.x = -fFarMulTanDivAspect;
	P1.y = fFarMulTan;
	P1.z = FarClip;
	P2.x = -P1.x;
	P2.y = P1.y;
	P2.z = P1.z;
	MakePlaneFrom3Points(P0, P1, P2, Frustum.TopPlane);

	// 下平面生成
	P0.x = 0.f;
	P0.y = 0.f;
	P0.z = 0.f;
	P1.x = fFarMulTanDivAspect;
	P1.y = -fFarMulTan;
	P1.z = FarClip;
	P2.x = -P1.x;
	P2.y = P1.y;
	P2.z = P1.z;
	MakePlaneFrom3Points(P0, P1, P2, Frustum.BottomPlane);

	Frustum.NearClip = NearClip;
	Frustum.FarClip = FarClip;
}


//*****************************************************************************
// 視点移動関数
//*****************************************************************************
void CCamera::MovePos(void)
{
	if (CInputKeyboard::GetKeyboardPress(DIK_I))
	{// 視点移動「上」
		m_Rot.x -= DEFAULT_CAMERA_ANGL_SPD;
		if (m_Rot.x < (-D3DX_PI * 0.5f + D3DX_PI * 0.02f))
		{
			m_Rot.x = (-D3DX_PI * 0.5f + D3DX_PI * 0.02f);
		}

		m_PosP.y = m_PosR.y - sinf(m_Rot.x) * m_fLengthInterval;

		m_DistanceCamera = cosf(m_Rot.x) * m_fLengthInterval;
		m_PosP.x = m_PosR.x - sinf(m_Rot.y) * m_DistanceCamera;
		m_PosP.z = m_PosR.z - cosf(m_Rot.y) * m_DistanceCamera;
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_K))
	{// 視点移動「下」
		m_Rot.x += DEFAULT_CAMERA_ANGL_SPD;
		if (m_Rot.x > (-0.45f))
		{
			m_Rot.x = (-0.45f);
		}

		m_PosP.y = m_PosR.y - sinf(m_Rot.x) * m_fLengthInterval;

		m_DistanceCamera = cosf(m_Rot.x) * m_fLengthInterval;
		m_PosP.x = m_PosR.x - sinf(m_Rot.y) * m_DistanceCamera;
		m_PosP.z = m_PosR.z - cosf(m_Rot.y) * m_DistanceCamera;
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_U))
	{// 視点移動「左」
		m_Rot.y += DEFAULT_CAMERA_ANGL_SPD;
		if (m_Rot.y > D3DX_PI)
		{
			m_Rot.y -= D3DX_PI * 2.0f;
		}

		m_PosP.x = m_PosR.x - sinf(m_Rot.y) * m_DistanceCamera;
		m_PosP.z = m_PosR.z - cosf(m_Rot.y) * m_DistanceCamera;
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_O))
	{// 視点移動「右」
		m_Rot.y -= DEFAULT_CAMERA_ANGL_SPD;
		if (m_Rot.y < -D3DX_PI)
		{
			m_Rot.y += D3DX_PI * 2.0f;
		}

		m_PosP.x = m_PosR.x - sinf(m_Rot.y) * m_DistanceCamera;
		m_PosP.z = m_PosR.z - cosf(m_Rot.y) * m_DistanceCamera;
	}

	D3DXVECTOR3 front = m_VecFront;
	front.y = 0;
	D3DXVECTOR3 right = m_VecRight;
	right.y = 0;

}

//=================================================
// カメラのFAR値ゲット
//=================================================
float CCamera::GetFar(void)
{
	return FAR_VAL;
}

//-----EOF----