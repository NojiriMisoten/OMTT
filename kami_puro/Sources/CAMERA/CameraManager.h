//=============================================================================
//
// CCameraManager�N���X [CCameraManager.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CCAMERAMANAGER_H_
#define _CCAMERAMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "CCamera.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEffectManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCameraManager
{
public:
	// �R���X�g���N�^
	CCameraManager(CEffectManager* pEffectManager);

	// �f�X�g���N�^
	~CCameraManager(void);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	//=================================================
	// �J�����N���G�C�g
	//=================================================
	void CreateCamera(D3DXVECTOR3& pos, D3DXVECTOR3& posR);

	//=================================================
	// �J�����Z�b�g(�`��Ƃ��ŌĂ�)
	// ����: �f�o�C�X
	//=================================================
	void SetCamera(LPDIRECT3DDEVICE9 *pDevice);
	void SetLightCamera(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos);

	//=================================================
	// �J�����̍��W�Q�b�g
	// �߂�l: ���W
	//=================================================
	D3DXVECTOR3& GetCameraPos(void){ return m_pCamera->GetCameraPos(); };

	//=================================================
	// �J�����̃t�����g�x�N�g���Q�b�g
	// �߂�l: �t�����g�x�N�g��
	//=================================================
	D3DXVECTOR3& GetVecFront(void){ return m_pCamera->GetVecFront(); };

	//=================================================
	// �J�����̃��C�g�x�N�g���Q�b�g
	// �߂�l: ���C�g�x�N�g��
	//=================================================
	D3DXVECTOR3& GetVecRight(void){ return m_pCamera->GetVecRight(); };

	//=================================================
	// �J������Y�p�x�Q�b�g
	// �߂�l: �p�x
	//=================================================
	float GetRotCamera(void){ return m_pCamera->GetRotCamera(); };

	//=================================================
	// �J�����̒����_�Q�b�g
	// �߂�l: �����_���W
	//=================================================
	D3DXVECTOR3& GetPosRCamera(void){ return m_pCamera->GetPosRCamera(); };
	
	//=================================================
	// �J�����̒����_�Z�b�g
	// ����: ���W
	//=================================================
	void SetPosRCamera(D3DXVECTOR3& posR){ m_pCamera->SetPosRCamera(posR); };
	
	//=================================================
	// �J�����̃A�h���X�Q�b�g
	// �߂�l: �J�����̃|�C���^
	//=================================================
	CCamera* GetAddresCamera(void){ return m_pCamera; };
	
	//=================================================
	// �J�����̍s��Q�b�g
	// �߂�l: �s��
	//=================================================
	D3DXMATRIX& GetMtxView(void){ return m_pCamera->GetMtxView(); };
	D3DXMATRIX& GetMtxViewOrtho(void){ return m_pCamera->GetMtxViewOrtho(); };
	D3DXMATRIX& GetMtxLightView(void){ return m_pCamera->GetMtxLightView(); };

	//=================================================
	// �v���W�F�N�V�����}�g���b�N�X�Q�b�g
	// �߂�l: �s��
	//=================================================
	D3DXMATRIX& GetMtxProj(void){ return m_pCamera->GetMtxProj(); };
	D3DXMATRIX& GetMtxProjOrtho(void){ return m_pCamera->GetMtxProjOrtho(); };
	D3DXMATRIX& GetMtxLightProj(void){ return m_pCamera->GetMtxLightProj(); };

	//=================================================
	// �I���\�[�Z�b�g
	//=================================================
	void SetOrtho(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& cameraPos, D3DXVECTOR3& cameraPosR, D3DXVECTOR3& upVec)
	{
		m_pCamera->SetCameraOrtho(pDevice, cameraPos, cameraPosR, upVec);
	};

	//=================================================
	// �J�����̎�����Q�b�g
	// �߂�l: ������\����
	//=================================================
	FRUSTUM& GetFrustum(void){ return m_pCamera->GetFrustum(); };

	//=================================================
	// �J������FAR�l�Q�b�g
	// �߂�l: �J������FAR�l
	//=================================================
	float GetFar(void){ return m_pCamera->GetFar(); };

private:
	CCamera* m_pCamera;
	CEffectManager			*m_pEffectManager;				//�G�t�F�N�g�}�l�[�W���[
};

#endif
//----EOF----