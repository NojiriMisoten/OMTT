//=============================================================================
//
// CCamera�N���X [CCamera.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CCAMERA_H_
#define _CCAMERA_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEffectManager;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���ʍ\����
typedef struct
{
	// ���ʕ�����(a*x)+(b*y)+(c*z)+d = 0(���ʖ@���x�N�g��=[a,b,c])
	float a, b, c, d;
}PLANE;

// ������\����
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
// �N���X��`
//*****************************************************************************
class CCamera
{
public:
	// �R���X�g���N�^
	CCamera(CEffectManager *pEffectManager_);

	// �f�X�g���N�^
	~CCamera(void);

	//=================================================
	// ������
	// ����: ���W�A�����_���W
	//=================================================
	void Init(D3DXVECTOR3& pos, D3DXVECTOR3& posR);

	// ������
	void Init(void);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);


	//=================================================
	// �J�����V�F�C�N�J�n
	// ����: �k���A�U���A���t���[���A������
	//=================================================
	void StartCameraShake( D3DXVECTOR3 epicenter, float amplitude, int totalFrame, float attenuation );

	//=================================================
	// �J�����V�F�C�N�����I��
	// ��{�͑��t���[����������������I������̂ŕK�v�Ȃ�
	//=================================================
	void EndCameraShake( void );

	//=================================================
	// �J�����ړ� - �u��
	// ����: �ړ��掋�_�A�ړ��撍���_
	//=================================================
	void CameraSetToCoord( D3DXVECTOR3 endPosP, D3DXVECTOR3 endPosR );

	//=================================================
	// �J�����ړ� -�@����
	// ����: �ړ������_�A�ړ��������_�A�ړ��掋�_�A�ړ��撍���_�A���ԁi�t���[���j
	//=================================================
	void CameraMoveToCoord( D3DXVECTOR3 startPosP, D3DXVECTOR3 endPosP, D3DXVECTOR3 startPosR, D3DXVECTOR3 endPosR, int totalFrame );

	//=================================================
	// �J�������[�u�����I��
	// ��{�͑��t���[����������������I������̂ŕK�v�Ȃ�
	//=================================================
	void EndCameraMove( void );

	//=================================================
	// �J�����Z�b�g(�`��Ƃ��ŌĂ�)
	// ����: �f�o�C�X
	//=================================================
	void SetCamera(LPDIRECT3DDEVICE9 *pDevice);
	void SetLightCamera(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos);
	// �I���\�[�J�����Z�b�g
	void SetCameraOrtho(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& cameraPos, D3DXVECTOR3& cameraPosR, D3DXVECTOR3& upVec);

	//=================================================
	// �J�����̍��W�Q�b�g
	// �߂�l: ���W
	//=================================================
	D3DXVECTOR3& GetCameraPos(void){return m_PosP;};

	//=================================================
	// �J�����̃t�����g�x�N�g���Q�b�g
	// �߂�l: �t�����g�x�N�g��
	//=================================================
	D3DXVECTOR3& GetVecFront(void){return m_VecFront;};

	//=================================================
	// �J�����̃��C�g�x�N�g���Q�b�g
	// �߂�l: ���C�g�x�N�g��
	//=================================================
	D3DXVECTOR3& GetVecRight(void){return m_VecRight;};

	//=================================================
	// �J�����̈ړ��ʃZ�b�g
	// ����: �ړ��x�N�g��
	//=================================================
	void SetMovCamera(D3DXVECTOR3& movVec){m_MovVec += movVec;};

	//=================================================
	// �J������Y�p�x�Q�b�g
	// �߂�l: �p�x
	//=================================================
	float GetRotCamera(void){return m_Rot.y;};

	//=================================================
	// �J�����̒����_�Q�b�g
	// �߂�l: �����_���W
	//=================================================
	D3DXVECTOR3& GetPosRCamera(void){return m_PosR;};
	
	//=================================================
	// �J�����̒����_�Z�b�g
	// ����: ���W
	//=================================================
	void SetPosRCamera(D3DXVECTOR3& posR){m_DestPosR = posR;};
	
	//=================================================
	// �J�����̃A�h���X�Q�b�g
	// �߂�l: �J�����̃|�C���^
	//=================================================
	CCamera* GetAddresCamera(void){return this;};
	
	//=================================================
	// �J�����̍s��Q�b�g
	// �߂�l: �s��
	//=================================================
	D3DXMATRIX& GetMtxView(void){return m_mtxView;};
	D3DXMATRIX& GetMtxLightView(void){ return m_mtxLightView; };

	//=================================================
	// �v���W�F�N�V�����}�g���b�N�X�Q�b�g
	// �߂�l: �s��
	//=================================================
	D3DXMATRIX& GetMtxProj(void){ return m_mtxProjection; };
	D3DXMATRIX& GetMtxLightProj(void){ return m_mtxLightProjection; };

	//=================================================
	// �J�����̍s��Q�b�g
	// �߂�l: �s��
	//=================================================
	D3DXMATRIX& GetMtxViewOrtho(void){ return m_mtxViewOrtho; };

	//=================================================
	// �v���W�F�N�V�����}�g���b�N�X�Q�b�g
	// �߂�l: �s��
	//=================================================
	D3DXMATRIX& GetMtxProjOrtho(void){ return m_mtxProjectionOrtho; };

	//=================================================
	// �J�����̎�����Q�b�g
	// �߂�l: ������\����
	//=================================================
	FRUSTUM& GetFrustum(void){return m_Frustum;};

	//=================================================
	// �J������FAR�l�Q�b�g
	// �߂�l: �J������FAR�l
	//=================================================
	float GetFar(void);

private:
	//=================================================
	// �J�����V�F�C�N���Ǘ�
	//=================================================
	void ControlShake( void );

	//=================================================
	// �J�������V�F�C�N
	// ����: �k���A�U���A���݃t���[���A���t���[���A������
	//=================================================
	void CameraShake( D3DXVECTOR3 epicenter, float amplitude, int currentFrame, int totalFrame, float attenuation );

	//=================================================
	// �J�����ړ����Ǘ�
	//=================================================
	void ControlMove( void );

	//=============================================
	// 3�_���畽�ʐ���
	// ����: ���W�_�P�A���W�_�Q�A���W�_�R�A���ʍ\����
	//=============================================
	void MakePlaneFrom3Points(D3DXVECTOR3 &P0, D3DXVECTOR3 &P1, D3DXVECTOR3 &P2, PLANE &Plane);

	//=============================================
	// �����䐶��
	// ����: ����p�A�A�X�y�N�g��A�ߌ��E�l�A�����E�l�A������\����
	//=============================================
	void MakeFrustum(float Angle, float Aspect, float NearClip, float FarClip, FRUSTUM &Frustum);

	D3DXVECTOR3			m_PosP;						// �J�����̎��_�i�ꏊ�j
	D3DXVECTOR3			m_DestPosP;					// �J�����̖ڕW�̎��_�i�ꏊ�j
	D3DXVECTOR3			m_SavePosP;					// �J�����̎��_�i�ꏊ�j�i�ߋ��j
	D3DXVECTOR3			m_PosR;						// �J�����̒����_�i�ǂ�����ǂ��܂Ō��Ă�̂��j
	D3DXVECTOR3			m_DestPosR;					// �J�����̖ڕW�̒����_
	D3DXVECTOR3			m_SavePosR;					// �J�����̒����_�i�ߋ��j
	D3DXVECTOR3			m_VecUp;					// �J�����̃x�N�g���̌����i����͏�����j
	D3DXVECTOR3			m_VecFront;					// �J�����̃x�N�g���̌���
	D3DXVECTOR3			m_VecRight;					// �J�����̃x�N�g���̌���
	D3DXVECTOR3			m_Rot;						// �J�����̌����i��]�p�j
	D3DXVECTOR3			m_MovVec;					// �J�����̈ړ��ʂƌ���
	D3DXMATRIX			m_mtxView;					// �r���[�}�g���b�N�X
	D3DXMATRIX			m_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			m_mtxViewOrtho;				// �I���\�[�r���[�}�g���b�N�X
	D3DXMATRIX			m_mtxProjectionOrtho;		// �I���\�[�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			m_mtxLightView;				// ���C�g�r���[�}�g���b�N�X
	D3DXMATRIX			m_mtxLightProjection;		// ���C�g�v���W�F�N�V�����}�g���b�N�X
	float				m_DistanceCamera;			// ���_���璍���_�܂ł̋���
	float				m_angle;					// �p�x
	float				m_fLengthInterval;			// ���_���璍���_�܂ł̋���
	FRUSTUM				m_Frustum;					// ��������

	// �J�����V�F�C�N�p�����o�[
	bool				m_IsCameraShake;				// �J�����V�F�C�N��true��
	D3DXVECTOR3			m_Epicenter;					// �k��
	float				m_Amplitude;					// �U��
	int					m_CurrentShakeFrame;			// ���݃t���[��
	int					m_TotalShakeFrame;				// ���t���[��
	float				m_Attenuation;					// ������
	CEffectManager		*m_pEffectManager;				//�G�t�F�N�g�}�l�[�W���[

	// �J�������[�u�p�����o�[
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