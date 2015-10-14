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
CCameraManager::CCameraManager(CEffectManager *pEffectManager_)
{
	m_pEffectManager = pEffectManager_;
	m_pCamera = NULL;
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
//----EOF----