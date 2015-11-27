//=============================================================================
//
// CCameraManager�N���X [CCameraManager.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CameraManager.h"
#include "../EFECT/CEffectManager.h"
//=================================================
// �R���X�g���N�^
//=================================================
CCameraManager::CCameraManager(CEffectManager* pEffectManager)
{
	m_pCamera = NULL;
	m_pEffectManager = pEffectManager;
}

//=================================================
// �f�X�g���N�^
//=================================================
CCameraManager::~CCameraManager(void)
{
	m_pCamera = NULL;
}

//=================================================
// �I��
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
// �X�V
//=================================================
void CCameraManager::Update(void)
{
	m_pCamera->Update();
}

//=================================================
// �J�����N���G�C�g
//=================================================
void CCameraManager::CreateCamera(D3DXVECTOR3& pos, D3DXVECTOR3& posR)
{
	m_pCamera = new CCamera(m_pEffectManager);
	m_pCamera->Init(pos, posR);
}

//=================================================
// �J�����Z�b�g(�`��Ƃ��ŌĂ�)
// ����: �f�o�C�X
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
// �J�����V�F�C�N�J�n
// ����: �k���A�U���A���t���[���A������
//=================================================
void CCameraManager::StartCameraShake( D3DXVECTOR3 epicenter, float amplitude, int totalFrame, float attenuation )
{
	m_pCamera->StartCameraShake( epicenter, amplitude, totalFrame, attenuation );
}

//=================================================
// �J�����V�F�C�N�����I��
// ��{�͑��t���[����������������I������̂ŕK�v�Ȃ�
//=================================================
void CCameraManager::EndCameraShake( void )
{
	m_pCamera->EndCameraShake();
}

//=================================================
// �J�����ړ� - �u��
// ����: �ړ��掋�_�A�ړ��撍���_
//=================================================
void CCameraManager::CameraSetToCoord( D3DXVECTOR3 endPosP, D3DXVECTOR3 endPosR )
{
	m_pCamera->CameraSetToCoord( endPosP, endPosR );
}

//=================================================
// �J�����ړ� -�@����
// ����: �ړ������_�A�ړ��掋�_�A�ړ��������_�A�ړ��撍���_�A���ԁi�t���[���j
//=================================================
void CCameraManager::CameraMoveToCoord( D3DXVECTOR3 startPosP, D3DXVECTOR3 endPosP, D3DXVECTOR3 startPosR, D3DXVECTOR3 endPosR, int totalFrame )
{
	m_pCamera->CameraMoveToCoord( startPosP, endPosP, startPosR, endPosR, totalFrame );
}

//----EOF----