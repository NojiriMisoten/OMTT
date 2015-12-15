//=============================================================================
//
// CCrowd�N���X [CCrowd.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCrowd.h"
#include "../../../MANAGER/CManager.h"
#include "../../../RENDERER/CRenderer.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../SHADER/CShader.h"
#include "../../../../LIGHT/CLightManager.h"

//*****************************************************************************
// �萔
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCrowd::CCrowd(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) :CScene3D(pDevice, objType)
{
	m_pD3DDevice = pDevice;
	m_TextureOffsetV = 0;

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCrowd ::~CCrowd(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CCrowd::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager)
{
	m_pManager = pManager;

	CScene3D::Init(pos, width, height, texType, pManager);
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CCrowd::Uninit(void)
{
	CScene3D::Uninit();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CCrowd::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CCrowd::DrawNormalRender(void)
{
	D3DXMATRIX			mtxScl, mtxRot, mtxTranslate, mtxWVP;

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

	// �V�F�[�_�[�̓K�p
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_POINT);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_POINT);
	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_POINT);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_POINT);

	(*m_pD3DDevice)->SetVertexShader(*_vs);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	UINT texSampler = (*_psc)->GetSamplerIndex("texSampler");
	HRESULT hr = 0;
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	D3DXMATRIX view, proj;
	CCameraManager* pCameraManager = m_pManager->GetCameraManager();
	view = pCameraManager->GetMtxView();
	proj = pCameraManager->GetMtxProj();

	mtxWVP = m_mtxWorld * view * proj;
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gWVP", &mtxWVP);
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "g_World", &m_mtxWorld);

	// UV�̃I�t�Z�b�g�����ăe�N�X�`���A�j���[�V���������
	hr = (*_vsc)->SetFloat((*m_pD3DDevice), "shiftV", m_TextureOffsetV);

	// ���C�g�̎擾
	D3DXVECTOR4 pos[CLightManager::LIGHT_NUM];
	D3DXVECTOR4 color[CLightManager::LIGHT_NUM];
	for (int i = 0; i < CLightManager::LIGHT_NUM; i++){
		CLightManager::CLight *light = m_pManager->GetLightManager()->GetLight(i);
		pos[i] = D3DXVECTOR4(light->GetPos(), 1);
		color[i] = D3DXVECTOR4(light->GetColor());
	}
	(*_psc)->SetVectorArray(*m_pD3DDevice, "worldLightPos", pos, CLightManager::LIGHT_NUM);
	(*_psc)->SetVectorArray(*m_pD3DDevice, "lightColor", color, CLightManager::LIGHT_NUM);
	(*_psc)->SetFloat(*m_pD3DDevice, "light_num", static_cast<float>(CLightManager::LIGHT_NUM));

	// ���Ăɂウ�\�V����
	(*_psc)->SetFloat(*m_pD3DDevice, "attenuation0", 0.0000001f);
	(*_psc)->SetFloat(*m_pD3DDevice, "attenuation1", 0.008f);
	(*_psc)->SetFloat(*m_pD3DDevice, "attenuation2", 0.0001f);

	// �|���S���̕`��
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VF));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);											// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetTexture(texSampler, m_pD3DTexBuff);
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP							// �v���~�e�B�u�̎��
		, 0												// �`����J�n���钸�_�ԍ�
		, DRAW_SQUARE_PRINITIV_NUM);					// ���������|���S����

	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CCrowd* CCrowd::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3 pos, float width, float height,
	TEXTURE_TYPE texType,
	CManager* pManager)
{
	// �쐬
	CCrowd* p = new CCrowd(pDevice);

	// ������
	p->Init(pos, width, height, texType, pManager);

	p->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	return p;

}
//----EOF----