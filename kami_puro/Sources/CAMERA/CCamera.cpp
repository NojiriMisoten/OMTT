//=============================================================================
//
// CCamera�N���X [CCamera.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MANAGER/CManager.h"
#include "CCamera.h"
#include "../INPUT/CInputKeyboard.h"
#include "../INPUT/CInputGamePad.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float			NEAR_VAL = 1.0f;										// near�l �J�����ɉf��߂��̌��E
static const float			FAR_VAL = 1000.0f;										// far�l �J�����ɉf�鉓���̌��E
static const float			DEFAULT_CAMERA_MOV_R_SPD = 80.0f;						// �J�����̒����_�ύX��
static const float			DEFAULT_CAMERA_MOV_SPD = 1.8f;							// �J�����̈ړ��X�s�[�h
static const float			DEFAULT_CAMERA_ANGL_SPD = 0.03f;						// �J�����̉�]�X�s�[�h
static const float			LIMIT_UNDER_ANGLE = -1.35f;								// �J�����̉��ւ̊p�x�̌��E�l
static const float			LIMIT_UP_ANGLE = -0.014f;								// �J�����̏�ւ̊p�x�̌��E�l
static const float			BASE_HIGHT_POS = 100.0f;								// �J�����̊�{�̍���
static const float			VIEW_ANGLE = D3DX_PI / 4.f;								// ����p
static const D3DXVECTOR3	DEFAULT_CAMERA_POS(0.0f, 40.0f, -160.0f);				// �f�t�H���g�̃J�������W
static const D3DXVECTOR3	DEFAULT_CAMERA_POS_R(0.f, 0.f, 0.f);					// �f�t�H���g�̃J���������_���W
static const float			MAX_CAMERA_MOV_COEFFICIENT = 1.0f;						// �J�����ړ��W���̍ő�l
static const float			MIN_CAMERA_MOV_COEFFICIENT = 0.1f;						// �J�����ړ��W���̍ŏ��l

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCamera::CCamera(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCamera::~CCamera(void)
{
}

//*****************************************************************************
// ������ ��������
//*****************************************************************************
void CCamera::Init(D3DXVECTOR3& pos, D3DXVECTOR3& posR)
{
	// ���W
	m_PosP = pos;
	m_DestPosP = pos;
	
	// �J�����̉�]�i���Ă���ꏊ ex:���̏ꍇm_PosP�̍��W����m_PosR�̍��W�����Ă���j
	m_PosR = posR;
	m_DestPosR = posR;
	
	// �J�����̕���
	m_VecUp = DEFAULT_UP_VECTOR;
	m_VecFront = DEFAULT_FRONT_VECTOR;
	m_VecRight = DEFAULT_RIGHT_VECTOR;

	// �����_�Ǝ��_�̋���
	D3DXVECTOR3 length = m_PosR - m_PosP;
	m_DistanceCamera = sqrt(length.x * length.x + length.z * length.z);
	m_fLengthInterval = sqrtf((m_PosR.x - m_PosP.x) * (m_PosR.x - m_PosP.x)
								+ (m_PosR.y - m_PosP.y) * (m_PosR.y - m_PosP.y)
								+ (m_PosR.z - m_PosP.z) * (m_PosR.z - m_PosP.z));

	// �p�x�̏�����
	m_Rot = D3DXVECTOR3(0,0,0);
	m_Rot.y = atan2f((m_PosR.x - m_PosP.x), (m_PosR.z - m_PosP.z));
	m_Rot.x = atan2f((m_PosR.y - m_PosP.y), m_DistanceCamera);

	// �ړ��ʂ̏�����
	m_MovVec = D3DXVECTOR3(0,0,0);

	// ������쐬
	MakeFrustum(VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_VAL, FAR_VAL, m_Frustum);

	// �J�����V�F�C�N�p�ϐ��̏�����
	m_IsCameraShake = false;
	m_Epicenter = VECTOR3_ZERO;
	m_Amplitude = 0.0f;
	m_CurrentFrame = 0;
	m_TotalFrame = 0;
	m_Attenuation = 0.0f;
}

//*****************************************************************************
// ������ �����Ȃ�
//*****************************************************************************
void CCamera::Init(void)
{
	// ���W
	m_PosP = DEFAULT_CAMERA_POS;
	m_DestPosP = DEFAULT_CAMERA_POS;
	
	// �J�����̉�]�i���Ă���ꏊ ex:���̏ꍇm_PosP�̍��W����m_PosR�̍��W�����Ă���j
	m_PosR = DEFAULT_CAMERA_POS_R;
	m_DestPosR = DEFAULT_CAMERA_POS_R;
	
	// �J�����̕���
	m_VecUp = DEFAULT_UP_VECTOR;
	m_VecFront = DEFAULT_FRONT_VECTOR;
	m_VecRight = DEFAULT_RIGHT_VECTOR;

	// �����_�Ǝ��_�̋���
	D3DXVECTOR3 length = m_PosR - m_PosP;
	m_DistanceCamera = sqrt(length.x * length.x + length.z * length.z);
	m_fLengthInterval = sqrtf((m_PosR.x - m_PosP.x) * (m_PosR.x - m_PosP.x)
								+ (m_PosR.y - m_PosP.y) * (m_PosR.y - m_PosP.y)
								+ (m_PosR.z - m_PosP.z) * (m_PosR.z - m_PosP.z));

	// �p�x�̏�����
	m_Rot = D3DXVECTOR3(0,0,0);
	m_Rot.y = atan2f((m_PosR.x - m_PosP.x), (m_PosR.z - m_PosP.z));
	m_Rot.x = atan2f((m_PosR.y - m_PosP.y), m_DistanceCamera);

	// �ړ��ʂ̏�����
	m_MovVec = D3DXVECTOR3(0,0,0);

	// ������쐬
	MakeFrustum(VIEW_ANGLE, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_VAL, FAR_VAL, m_Frustum);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CCamera::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CCamera::Update(void)
{
	MovePos();

	// �J�����V�F�C�N�e�X�g�p�I������OK
	{
		// �V�F�C�N��
		static bool IsFlagTrue = false;
		if( !IsFlagTrue ){
			if( CInputKeyboard::GetKeyboardRelease( DIK_J ) ){
				IsFlagTrue = true;
				StartCameraShake( VECTOR3_ZERO, 10.0f, 20, 0.4f );
			}
			// �V�F�C�N��
			if( CInputKeyboard::GetKeyboardRelease( DIK_L ) ){
				StartCameraShake( VECTOR3_ZERO, 20.0f, 40, 0.1f );
			}
		}
	}

	// �J�����V�F�C�N�֐�
	ControlShake();

	// �t�����g�x�N�g���̐ݒ�
	m_VecFront = m_PosR - m_PosP;
	D3DXVec3Normalize(&m_VecFront, &m_VecFront);

	// ���C�g�x�N�g���̐ݒ�
	D3DXVec3Cross(&m_VecRight, &m_VecUp, &m_VecFront);
	D3DXVec3Normalize(&m_VecRight, &m_VecRight);

	// �����_�[�Ɋp�x������
	CRenderer::TeachCameraRot(m_Rot.y);
	CRenderer::TeachProjMtx(m_mtxProjection);
	CRenderer::TeachViewMtx(m_mtxView);

#ifdef _DEBUG
	CDebugProc::Print("CameraX:%f\nCameraY:%f\nCameraZ:%f\n", m_PosP.x, m_PosP.y, m_PosP.z);
	CDebugProc::Print("CameraRotX:%f\nCameraRotY:%f\nCameraRotZ:%f\n", m_Rot.x, m_Rot.y, m_Rot.z);
#endif
}

//*****************************************************************************
// �Z�b�g�J����
//*****************************************************************************
void CCamera::SetCamera(LPDIRECT3DDEVICE9 *pDevice)
{
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);
	
	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_PosP, &m_PosR, &m_VecUp);
	
	// �r���[�}�g���b�N�X�̐ݒ�
	(*pDevice)->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection
							, VIEW_ANGLE						// ����p
							, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT	// �A�X�y�N�g��
							, NEAR_VAL							// near�l
							, FAR_VAL);							// far�l
	
	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	(*pDevice)->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}
void CCamera::SetLightCamera(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos)
{
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxLightView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxLightView, &pos, &m_PosR, &m_VecUp);

	// �r���[�}�g���b�N�X�̐ݒ�
	(*pDevice)->SetTransform(D3DTS_VIEW, &m_mtxLightView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxLightProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxLightProjection
		, VIEW_ANGLE						// ����p
		, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT	// �A�X�y�N�g��
		, NEAR_VAL							// near�l
		, FAR_VAL);							// far�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	(*pDevice)->SetTransform(D3DTS_PROJECTION, &m_mtxLightProjection);
}

//*****************************************************************************
// �I���\�[�J�����Z�b�g
//*****************************************************************************
void CCamera::SetCameraOrtho(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& cameraPos, D3DXVECTOR3& cameraPosR, D3DXVECTOR3& upVec)
{
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxViewOrtho);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxViewOrtho, &cameraPos, &cameraPosR, &upVec);

	// �r���[�}�g���b�N�X�̐ݒ�
	(*pDevice)->SetTransform(D3DTS_VIEW, &m_mtxViewOrtho);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjectionOrtho);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixOrthoLH(&m_mtxProjectionOrtho
					, 80.f
					, 80.f
					, -40.f
					, 40.f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	(*pDevice)->SetTransform(D3DTS_PROJECTION, &m_mtxProjectionOrtho);
}

//*****************************************************************************
// 3�_���畽�ʍ쐬�֐�
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
// �����䐶���֐�
//*****************************************************************************
void CCamera::MakeFrustum(float Angle, float Aspect, float NearClip, float FarClip, FRUSTUM &Frustum)
{
	D3DXVECTOR3 P0, P1, P2;
	float fTan = tan(Angle * 0.5f);	// ���ʂ��甼���ɂ���������
	float fTanDivAspect = fTan / Aspect;
	float fFarMulTan = FarClip * fTan;
	float fFarMulTanDivAspect = FarClip * fTanDivAspect;

	// �����ʐ���
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

	// �E���ʐ���
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

	// �㕽�ʐ���
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

	// �����ʐ���
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
// ���_�ړ��֐�
//*****************************************************************************
void CCamera::MovePos(void)
{
	if (CInputKeyboard::GetKeyboardPress(DIK_I))
	{// ���_�ړ��u��v
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
	{// ���_�ړ��u���v
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
	{// ���_�ړ��u���v
		m_Rot.y += DEFAULT_CAMERA_ANGL_SPD;
		if (m_Rot.y > D3DX_PI)
		{
			m_Rot.y -= D3DX_PI * 2.0f;
		}

		m_PosP.x = m_PosR.x - sinf(m_Rot.y) * m_DistanceCamera;
		m_PosP.z = m_PosR.z - cosf(m_Rot.y) * m_DistanceCamera;
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_O))
	{// ���_�ړ��u�E�v
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
// �J������FAR�l�Q�b�g
//=================================================
float CCamera::GetFar(void)
{
	return FAR_VAL;
}

//=================================================
// �J�����V�F�C�N���Ǘ�
//=================================================
void CCamera::ControlShake( void )
{
	// �J�����V�F�C�N��true�ł����
	if( m_IsCameraShake ){
		// �G���[�`�F�b�N�A�ʂ�Ȃ��͂�
		assert( ( ( m_CurrentFrame >= 0 ) && ( m_TotalFrame >= 0 ) ) && "�J�����V�F�C�N�̌Ăяo�������������񂶂�ˁH" );

		// �J�����V�F�C�N�Ăяo��
		CameraShake( m_Epicenter, m_Amplitude, m_CurrentFrame, m_TotalFrame, m_Attenuation );

		// ���݃t���[�����̃J�E���g�A�b�v
		m_CurrentFrame++;

		// ���݃t���[���������t���[�����𒴂�����
		if( m_CurrentFrame > m_TotalFrame )
		{
			// �V�F�C�N�I������
			EndCameraShake();
		}
	}
}

//=================================================
// �J�����V�F�C�N�J�n
// ����: �k���A�U���A���t���[���A������
//=================================================
void CCamera::StartCameraShake( D3DXVECTOR3 epicenter, float amplitude, int totalFrame, float attenuation )
{
	// �ϐ��������o�[�Ɋi�[
	m_IsCameraShake = true;
	m_Epicenter = epicenter;
	m_Amplitude = amplitude;
	m_CurrentFrame = 0;
	m_TotalFrame = totalFrame;
	m_Attenuation = attenuation;

	// �J�������W��ޔ�
	m_SavePosP = m_PosP;
	m_SavePosR = m_PosR;
}

//=================================================
// �J�����V�F�C�N�����I��
// ��{�͑��t���[����������������I������̂ŕK�v�Ȃ�
//=================================================
void CCamera::EndCameraShake( void )
{
	// �J�����V�F�C�N�p�����o�[�̏�����
	m_IsCameraShake = false;
	m_Epicenter = VECTOR3_ZERO;
	m_Amplitude = 0.0f;
	m_CurrentFrame = 0;
	m_TotalFrame = 0;
	m_Attenuation = 0.0f;
}


//=================================================
// �J�����V�F�C�N
// ����: �k���A�U���A���݃t���[���A���t���[���A������
//=================================================
void CCamera::CameraShake( D3DXVECTOR3 epicenter, float amplitude, int currentFrame, int totalFrame, float attenuation )
{
	// �o�ߎ��Ԃ̊���
	float percentage = (float)currentFrame / (float)totalFrame;

	// ���������U���̋���
	// ���Ԉ���Ă�L�� 0�`distance�̊Ԃ���Ȃ��Ƃ�������
	float distance = amplitude - amplitude * ( attenuation + attenuation * percentage + attenuation * percentage * percentage );

	// �V���W
	float randRatio[3];				// 0�`1�̊Ԃ̃����_���Ȑ��l
	for( int i = 0; i < 3; i++ ){
		randRatio[i] = rand() / RAND_MAX;
	}
	D3DXVECTOR3 pos = epicenter + D3DXVECTOR3(
		(float)( ( randRatio[0] - 0.5f ) * 2 * distance ),
		(float)( ( randRatio[1] - 0.5f ) * 2 * distance ),
		(float)( ( randRatio[2] - 0.5f ) * 2 * distance ) );

	m_PosP = m_SavePosP + pos;
	m_PosR = m_SavePosR + pos;
}

//-----EOF----