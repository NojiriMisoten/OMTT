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
#include "../MATH/mersenne_twister.h"

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

	// �J�����V�F�C�N������
	m_SavePosP = m_PosP;
	m_SavePosR = m_PosR;
	m_IsCameraShake = false;
	m_Epicenter = VECTOR3_ZERO;
	m_Amplitude = 0.0f;
	m_CurrentShakeFrame = 0;
	m_TotalShakeFrame = 0;
	m_Attenuation = 0.0f;
}

//*****************************************************************************
// ������ �����Ȃ�
//*****************************************************************************
void CCamera::Init(void)
{
	Init( (D3DXVECTOR3&)DEFAULT_CAMERA_POS, (D3DXVECTOR3&)DEFAULT_CAMERA_POS_R );
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
	// �J�����y�V�F�C�N�E���[�u�z�e�X�g�p�A�����Ă���
	{
		//�V�F�C�N��
		if( CInputKeyboard::GetKeyboardTrigger( DIK_J ) )
		{
			// �������͌���g���ĂȂ�
			StartCameraShake( VECTOR3_ZERO, 5.0f, 15, 0.8f );
		}
		//�V�F�C�N��
		if( CInputKeyboard::GetKeyboardTrigger( DIK_L ) )
		{
			StartCameraShake( VECTOR3_ZERO, 10.0f, 30, 0.8f );
		}
		// �J�������[�u
		if( CInputKeyboard::GetKeyboardTrigger( DIK_SEMICOLON ) )
		{
			CameraMoveToCoord(
				D3DXVECTOR3( -200.0f, 100.0f, -250.0f ),
				D3DXVECTOR3( 200.0f, 100.0f, -250.0f ),
				VECTOR3_ZERO,
				VECTOR3_ZERO,
				240 );
		}
		// �J�����Z�b�g
		if( CInputKeyboard::GetKeyboardTrigger( DIK_COLON ) )
		{
			CameraSetToCoord(
				D3DXVECTOR3( -200.0f, 100.0f, -250.0f ),
				VECTOR3_ZERO );
		}
	}
	
	// �J�����V�F�C�N�Ǘ�
	ControlShake();

	// �J�����ړ��Ǘ�
	ControlMove();

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
	CDebugProc::Print( "[CAMERA]\n" );
	CDebugProc::Print( "PosP:%5.3f/%5.3f/%5.3f\n", m_PosP.x, m_PosP.y, m_PosP.z );
	CDebugProc::Print( "PosR:%5.3f/%5.3f/%5.3f\n", m_PosR.x, m_PosR.y, m_PosR.z );
	CDebugProc::Print( "Rot:%f/%f/%f\n", m_Rot.x, m_Rot.y, m_Rot.z );
	if( m_IsCameraMove )
	{
		CDebugProc::Print( "Move:true\n" );
	}
	else
	{
		CDebugProc::Print( "Move:false\n" );
	}
	if( m_IsCameraShake )
	{
		CDebugProc::Print( "Shake:true\n" );
	}
	else
	{
		CDebugProc::Print( "Shake:false\n" );
	}
	CDebugProc::Print( "\n" );
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
	if( m_IsCameraShake )
	{
		// �G���[�`�F�b�N�A�ʂ�Ȃ��͂�
		assert( ( ( m_CurrentShakeFrame >= 0 ) && ( m_TotalShakeFrame >= 0 ) ) && "�J�����V�F�C�N�̌Ăяo�������������񂶂�ˁH" );

		// �J�����V�F�C�N�Ăяo��
		CameraShake( m_Epicenter, m_Amplitude, m_CurrentShakeFrame, m_TotalShakeFrame, m_Attenuation );

		// ���݃t���[�����̃J�E���g�A�b�v
		m_CurrentShakeFrame++;

		// ���݃t���[���������t���[�����𒴂�����
		if( m_CurrentShakeFrame > m_TotalShakeFrame )
		{
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
	//�@���݃J�����V�F�C�N���ł���Γ����Ȃ�
	if( !m_IsCameraShake ){
		m_SavePosP = m_PosP;
		m_SavePosR = m_PosR;

		m_IsCameraShake = true;
		m_Epicenter = epicenter;
		m_Amplitude = amplitude;
		m_CurrentShakeFrame = 0;
		m_TotalShakeFrame = totalFrame;
		m_Attenuation = attenuation;
	}
}

//=================================================
// �J�����V�F�C�N�����I��
// ��{�͑��t���[����������������I������̂ŕK�v�Ȃ�
//=================================================
void CCamera::EndCameraShake( void )
{
	m_PosP = m_SavePosP;
	m_PosR = m_SavePosR;

	// �J�����V�F�C�N�p�����o�[�̏�����
	m_IsCameraShake = false;
	m_Epicenter = VECTOR3_ZERO;
	m_Amplitude = 0.0f;
	m_CurrentShakeFrame = 0;
	m_TotalShakeFrame = 0;
	m_Attenuation = 0.0f;
}


//=================================================
// �J�����V�F�C�N
// ����: �k���A�U���A���݃t���[���A���t���[���A������
//=================================================
void CCamera::CameraShake( D3DXVECTOR3 epicenter, float amplitude, int currentFrame, int totalFrame, float attenuation )
{
	// �o�߃p�[�Z���e�[�W
	float percentage = (float)currentFrame / totalFrame;

	// ���������U���̋���
	//float distance = amplitude * ( attenuation + attenuation * percentage + attenuation * percentage * percentage;
	float distance = amplitude * ( 1 - percentage * percentage );

	// �V���W
	float randNum[3];			// -1~1�̊Ԃ̃����_���Ȓl
	for( int i = 0; i < 3; i++ )
	{
		randNum[i] = mersenne_twister_float( -1.0f, 1.0f );
	}
	D3DXVECTOR3 pos = D3DXVECTOR3( distance * randNum[0], distance * randNum[1], distance * randNum[2] );

	m_PosP = m_SavePosP + pos;
	m_PosR = m_SavePosR + pos;
}

//=================================================
// �J�����V�F�C�N���Ǘ�
//=================================================
void CCamera::ControlMove( void )
{
	// �J�������[�u��true�ł����
	if( m_IsCameraMove )
	{
		// ���ړ���
		D3DXVECTOR3 distanceP = m_EndPosP - m_StartPosP;
		D3DXVECTOR3 distanceR = m_EndPosR - m_StartPosR;
		
		// 1�t���[�����Ƃ̈ړ���
		D3DXVECTOR3 movePerFrameP = distanceP / (float)m_TotalMoveFrame;
		D3DXVECTOR3 movePerFrameR = distanceR / (float)m_TotalMoveFrame;

		// �ړ���
		m_PosP += movePerFrameP;
		m_PosR += movePerFrameR;

		m_CurrentMoveFrame++;

		if( m_CurrentMoveFrame > m_TotalMoveFrame )
		{
			EndCameraMove();
		}
	}
}

//=================================================
// �J�����ړ� - �u��
// ����: �ړ��掋�_�A�ړ��撍���_
//=================================================
void CCamera::CameraSetToCoord( D3DXVECTOR3 endPosP, D3DXVECTOR3 endPosR )
{
	m_PosP = endPosP;
	m_PosR = endPosR;

	EndCameraMove();
}

//=================================================
// �J�����ړ� -�@����
// ����: �ړ������_�A�ړ��������_�A�ړ��掋�_�A�ړ��撍���_�A���ԁi�t���[���j
//=================================================
void CCamera::CameraMoveToCoord( D3DXVECTOR3 startPosP, D3DXVECTOR3 endPosP, D3DXVECTOR3 startPosR, D3DXVECTOR3 endPosR, int totalFrame )
{
	m_StartPosP	= startPosP;
	m_StartPosR	= startPosR;
	m_PosP = m_StartPosP;
	m_PosR = m_StartPosR;
	m_EndPosP =	endPosP;
	m_EndPosR =	endPosR;
	m_CurrentMoveFrame = 0;
	m_TotalMoveFrame = totalFrame;

	m_IsCameraMove = true;
}

//=================================================
// �J�������[�u�����I��
// ��{�͑��t���[����������������I��5����̂ŕK�v�Ȃ�
//=================================================
void CCamera::EndCameraMove( void )
{
	m_StartPosP = VECTOR3_ZERO;
	m_StartPosR = VECTOR3_ZERO;
	m_EndPosP = VECTOR3_ZERO;
	m_EndPosR = VECTOR3_ZERO;
	m_CurrentMoveFrame = 0;
	m_TotalMoveFrame = 0;
	
	m_IsCameraMove = false;
}

//-----EOF----