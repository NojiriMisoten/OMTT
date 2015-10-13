//=============================================================================
//
// CSceneX�N���X [CSceneX.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneX.h"
#include "../RENDERER/CRenderer.h"
#include "../CAMERA/CameraManager.h"
#include "../SHADER/CShader.h"

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
D3DXMATRIX CSceneX ::m_mtxWorld;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSceneX ::CSceneX(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType):CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_ModelInfo = NULL;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	ZeroMemory(m_cModelFileName, sizeof(m_cModelFileName));
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSceneX ::~CSceneX(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CSceneX :: Init(D3DXVECTOR3& pos, char* modelFileName)
{
	// ������
	m_Pos = pos;
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// �X�P�[���͊�{�ς��Ȃ��̂ł��̂܂܌Œ�B�J���������Α傫�������邵�A�����Ώ����������邩��
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	// ���f�����擾
	m_ModelInfo = CModel::GetModel(modelFileName);
}
//*****************************************************************************
// �������֐�
//*****************************************************************************
void CSceneX :: Init(D3DXVECTOR3& pos, MODEL_TYPE type)
{
	// ������
	m_Pos = pos;
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// �X�P�[���͊�{�ς��Ȃ��̂ł��̂܂܌Œ�B�J���������Α傫�������邵�A�����Ώ����������邩��
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	// ���f�����擾
	m_ModelInfo = CModel::GetModel(type);

}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CSceneX ::Uninit(void)
{
	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CSceneX ::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneX ::Draw(void)
{
	/*D3DXMATERIAL		*pD3DXMat;
	D3DMATERIAL9		matDef;		// ���̃}�e���A������ۑ����Ă���
	D3DXMATRIX			mtxScl,mtxRot,mtxTranslate;
	
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f		*****���Ԓ���*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);	// Y������_�ɉ�]���Ă���̂�Y,X,Z�̏��ɓ����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���C�g�x�N�g���̃��[�J����
	CLight* light = CLight::GetLightAdr(0);
	D3DXVECTOR3 lightDir = light->GetDir();
	D3DXVec3Normalize(&lightDir, &lightDir);
	D3DXVECTOR3 localLightVec = lightDir;
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, NULL, &m_mtxWorld);
	D3DXVec3TransformCoord(&localLightVec, &localLightVec, &invWorld);
	D3DXCOLOR lightColor = light->GetColor();

	// �V�F�[�_�[�̓K�p
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_FUR);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_FUR);

	PS_TYPE type = PS_TYPE_FUR;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*_vs);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	HRESULT hr = 0;
	hr = (*_vsc)->SetFloatArray((*m_pD3DDevice), "gLightDir", (float*)&localLightVec, 3);
	hr = (*_vsc)->SetFloatArray((*m_pD3DDevice), "gLightDiffuse", (float*)&lightColor, 4);

	hr = (*m_pD3DDevice)->SetSamplerState((*_psc)->GetSamplerIndex("furSampler"), D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState((*_psc)->GetSamplerIndex("furSampler"), D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState((*_psc)->GetSamplerIndex("furSampler"), D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState((*_psc)->GetSamplerIndex("furSampler"), D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	D3DXMATRIX view, proj;
	view = CCameraManager::GetMtxView();
	proj = CCameraManager::GetMtxProj();

	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gWorld", &m_mtxWorld);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gView", &view);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gProj", &proj);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��

	// �J�����x�N�g���Z�b�g
	D3DXVECTOR3 cameraPos = CCameraManager::GetCameraPos();
	D3DXVECTOR3 cameraPosR = CCameraManager::GetPosRCamera();
	D3DXVECTOR3 cameraVec = cameraPosR - cameraPos;
	D3DXVec3Normalize(&cameraVec, &cameraVec);
	
	// �J�����x�N�g���̃��[�J����(�@�������[�J���Ȃ̂�)
	D3DXVECTOR3 localCameraVec = cameraVec;
	D3DXVec3TransformCoord(&localCameraVec, &localCameraVec, &invWorld);
	hr = (*_vsc)->SetFloatArray((*m_pD3DDevice), "gCameraDir", (float*)&localCameraVec, 3);
	
	// ���_�錾��������Z�b�g(SetFVF�̑���)
	(*m_pD3DDevice)->SetVertexDeclaration(m_ModelInfo->m_pDecl);

	// �}�e���A���̃o�b�t�@�擾
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// ���݂̃}�e���A���̏����擾
	(*m_pD3DDevice)->GetMaterial(&matDef);

	// �т̃e�N�X�`��
	LPDIRECT3DTEXTURE9 pTex = CTexture::GetTexture(TEXTURE_FUR);

//	(*m_pD3DDevice)->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);				// ���u�����h���s��

	for (int loop = 0; loop < 100; loop++)
	{
		// �I�t�Z�b�g�̌v�Z
		hr = (*_vsc)->SetFloat((*m_pD3DDevice), "gOffset", loop * 0.02f);

		// �}�e���A���̕`��
		for (int nCntMat = 0; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
		{
			hr = (*_vsc)->SetFloatArray((*m_pD3DDevice), "gMatDiffuse", (float*)&pD3DXMat[nCntMat].MatD3D.Diffuse, 4);
			hr = (*m_pD3DDevice)->SetTexture((*_psc)->GetSamplerIndex("furSampler")			// �T���v���[�ԍ����V�F�[�_�[�ɕ���
				, pTex);
			m_ModelInfo->pD3DXMeshModel->DrawSubset(nCntMat);					// ���f���̃p�[�c��`��
		}
	}

	//*********����:�ȉ���K����������******************
	// �����Ȃ��Ƃ��ׂĂ̐F�����������Ȃ�
	(*m_pD3DDevice)->SetMaterial(&matDef);									// �}�e���A���������ɖ߂�
//	(*m_pD3DDevice)->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
*/
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, char* modelFileName)
{
	// �쐬
	CSceneX* p = new CSceneX(pDevice);

	// ������
	p->Init(pos, modelFileName);

	return p;
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, MODEL_TYPE type)
{
	// �쐬
	CSceneX* p = new CSceneX(pDevice);

	// ������
	p->Init(pos, type);

	return p;
}

//*****************************************************************************
// �A���t�@�l�ύX�֐�
//*****************************************************************************
void CSceneX::SetAlpha(float alpha)
{
	D3DXMATERIAL *pD3DXMat;
	// �}�e���A���̃o�b�t�@�擾
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// �}�e���A���̃A���t�@�l�ύX
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Diffuse.a = alpha;
	}
}

//*****************************************************************************
// �X�y�L�����[�l�ύX�֐�
//*****************************************************************************
void CSceneX::SetSpecularPow(float pow)
{
	D3DXMATERIAL *pD3DXMat;
	// �}�e���A���̃o�b�t�@�擾
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// �}�e���A���̃A���t�@�l�ύX
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Power = pow;
	}
}

//*****************************************************************************
// �G�b�W�e�N�X�`���[�֐�
//*****************************************************************************
void CSceneX::CreateEdgeTexture(void)
{
/*	// �����_�[�^�[�Q�b�g�ύX
	CRenderer::ChangeRenderTarget(CRenderer::TYPE_RENDER_EDGE);

	D3DXMATERIAL		*pD3DXMat;
	D3DMATERIAL9		matDef;		// ���̃}�e���A������ۑ����Ă���
	D3DXMATRIX			mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f		*****���Ԓ���*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);	// Y������_�ɉ�]���Ă���̂�Y,X,Z�̏��ɓ����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �}�e���A���̃o�b�t�@�擾
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// ���݂̃}�e���A���̏����擾
	(*m_pD3DDevice)->GetMaterial(&matDef);

	// ���C�g�x�N�g���̃��[�J����
	CLight* light = CLight::GetLightAdr(0);
	D3DXVECTOR3 lightDir = light->GetDir();
	D3DXVec3Normalize(&lightDir, &lightDir);
	D3DXVECTOR3 localLightVec = lightDir;
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, NULL, &m_mtxWorld);
	D3DXVec3TransformCoord(&localLightVec, &localLightVec, &invWorld);

	// �V�F�[�_�[�̓K�p
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_EDGE);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_EDGE);

	PS_TYPE type = PS_TYPE_EDGE;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*_vs);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	D3DXMATRIX view, proj;
	view = CCameraManager::GetMtxView();
	proj = CCameraManager::GetMtxProj();

	(*_vsc)->SetMatrix((*m_pD3DDevice), "gWorld", &m_mtxWorld);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��
	(*_vsc)->SetMatrix((*m_pD3DDevice), "gView", &view);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��
	(*_vsc)->SetMatrix((*m_pD3DDevice), "gProj", &proj);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��

	// �}�e���A���̕`��
	D3DXMATERIAL* mat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		m_ModelInfo->pD3DXMeshModel->DrawSubset(nCntMat);					// ���f���̃p�[�c��`��
	}

	//*********����:�ȉ���K����������******************
	// �����Ȃ��Ƃ��ׂĂ̐F�����������Ȃ�
	(*m_pD3DDevice)->SetMaterial(&matDef);									// �}�e���A���������ɖ߂�

	CRenderer::ChangeDefaultRenderTarget();
	*/
}

//*****************************************************************************
// �V���h�E�e�N�X�`���[�֐�
//*****************************************************************************
void CSceneX::CreateShadowTexture(void)
{/*
	// �����_�[�^�[�Q�b�g�ύX
	CRenderer::ChangeRenderTarget(CRenderer::TYPE_RENDER_SHADOW);

	D3DXMATERIAL		*pD3DXMat;
	D3DMATERIAL9		matDef;		// ���̃}�e���A������ۑ����Ă���
	D3DXMATRIX			mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f		*****���Ԓ���*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);	// Y������_�ɉ�]���Ă���̂�Y,X,Z�̏��ɓ����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �}�e���A���̃o�b�t�@�擾
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// ���݂̃}�e���A���̏����擾
	(*m_pD3DDevice)->GetMaterial(&matDef);

	// ���C�g�x�N�g���̃��[�J����
	CLight* light = CLight::GetLightAdr(0);
	D3DXVECTOR3 lightDir = light->GetDir();
	D3DXVec3Normalize(&lightDir, &lightDir);
	D3DXVECTOR3 localLightVec = lightDir;
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, NULL, &m_mtxWorld);
	D3DXVec3TransformCoord(&localLightVec, &localLightVec, &invWorld);

	// �V�F�[�_�[�̓K�p
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_SHADOW);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_SHADOW);

	PS_TYPE type = PS_TYPE_SHADOW;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*_vs);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	// ���_�錾��������Z�b�g(SetFVF�̑���)
	(*m_pD3DDevice)->SetVertexDeclaration(m_ModelInfo->m_pDecl);

	D3DXMATRIX view, proj, lightMatrix;
	D3DXVECTOR3 cameraPos = m_Pos;// ���������C�g����ɂ���ƏƂ炵�Ă����������̉e�ɂȂ�
	cameraPos.x -= 30.5f;		   // ���������C�g����ɂ���ƏƂ炵�Ă����������̉e�ɂȂ�
	cameraPos.y += 30.5f;		   // ���������C�g����ɂ���ƏƂ炵�Ă����������̉e�ɂȂ�
	cameraPos.z -= 5.5f;		   // ���������C�g����ɂ���ƏƂ炵�Ă����������̉e�ɂȂ�
	D3DXVECTOR3 cameraPosR = m_Pos;
	D3DXVECTOR3 upVec(0.f, 0.f, -1.f);

	CCameraManager::SetOrtho(m_pD3DDevice, cameraPos, cameraPosR, upVec);
	proj = CCameraManager::GetMtxProjOrtho();
	view = CCameraManager::GetMtxViewOrtho();

	// ���C�g�}�g���N�X�쐬
	lightMatrix = view * proj;
	light->SetLightMatrix(lightMatrix);

	HRESULT hr = 0;

	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gWorld", &m_mtxWorld);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gView", &view);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gProj", &proj);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��
	
	// �}�e���A���̕`��
	D3DXMATERIAL* mat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		m_ModelInfo->pD3DXMeshModel->DrawSubset(nCntMat);					// ���f���̃p�[�c��`��
	}

	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
	//*********����:�ȉ���K����������******************
	// �����Ȃ��Ƃ��ׂĂ̐F�����������Ȃ�
	(*m_pD3DDevice)->SetMaterial(&matDef);									// �}�e���A���������ɖ߂�

	CRenderer::ChangeDefaultRenderTarget();
	*/
}
//----EOF----