//=============================================================================
//
// CTitlePlayer�N���X [CTitlePlayer.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CTitlePlayer.h"
#include "../../../INPUT/CInputKeyboard.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../MANAGER/CManager.h"
#include "../../../SHADER/CShader.h"
#include "../../../SKINMESH/CSkinMeshHolder.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTitlePlayer::CTitlePlayer(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType) : CPlayer(pDevice, m_objType)
{
	// �p���x�N�g��
	m_vecFront = DEFAULT_FRONT_VECTOR;
	m_vecRight = DEFAULT_RIGHT_VECTOR;
	m_vecUp = DEFAULT_UP_VECTOR;

	// �ړ�������
	m_MovPos = D3DXVECTOR3(0, 0, 0);

	m_pManager = NULL;

	m_ID = -1;
	m_AnimState = PLAYER_WAIT;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTitlePlayer::~CTitlePlayer(void)
{

}

//*****************************************************************************
// �쐬
//*****************************************************************************
CTitlePlayer* CTitlePlayer::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID)
{
	CTitlePlayer* p = new CTitlePlayer(pDevice);

	p->Init(pDevice, pos, rot, type, pManager, ID);

	return p;
}

//*****************************************************************************
// ������
//*****************************************************************************
void CTitlePlayer::Init(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID)
{
	m_DestPos = m_Pos = pos;
	m_pManager = pManager;
	m_Rot = rot;

	// �A�j��
	m_AnimState = PLAYER_WAIT;

	// ID
	m_ID = ID;

	// �X�L�����b�V�����f���ǂݍ���
	SKIN_MESH_INFO skinMeshInfo = (SKIN_MESH_INFO)m_ID;
	m_pCSkinMesh = CSkinMeshHolder::GetSkinMesh(skinMeshInfo);

	// �p���x�N�g��
	m_vecFront = DEFAULT_FRONT_VECTOR;
	m_vecRight = DEFAULT_RIGHT_VECTOR;
	m_vecUp = DEFAULT_UP_VECTOR;

	// �����N�ɒǉ�
	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// �X�P�[��
	m_vScl = D3DXVECTOR3(50, 50, 50);

	// �����ōX�V���Ă��炶��Ȃ��ƃA�j���[�V�����ς����Ȃ��̂ł��Ă���
	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);
	SetAnimType(PLAYER_WAIT);
	SetAnimMortionOfTime(0);
	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);

	m_DrawMode = DRAW_MODE_GRAY;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CTitlePlayer::Uninit(void)
{
	CPlayer::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CTitlePlayer::Update(void)
{
	// �t�����g�x�N�g���̐ݒ�
	m_vecFront.x = -sinf(-m_Rot.y);
	m_vecFront.z = -cosf(m_Rot.y - D3DX_PI);

	// ���C�g�x�N�g���̐ݒ�
	m_vecRight.x = cosf(m_Rot.y - D3DX_PI);
	m_vecRight.z = sinf(m_Rot.y);

	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);
}

//*****************************************************************************
// �`��
//*****************************************************************************
void CTitlePlayer::DrawNormalRender(void)
{
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

	// �V�F�[�_�[�̓K�p
	m_pVS = CShader::GetVS(VS_TYPE_NORMAL_RENDERER_SKINMESH);
	m_pVSC = CShader::GetVSC(VS_TYPE_NORMAL_RENDERER_SKINMESH);

	PS_TYPE type = PS_TYPE_TITLE_PLAYER;

	m_pPS = CShader::GetPS(type);
	m_pPSC = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*m_pVS);
	(*m_pD3DDevice)->SetPixelShader(*m_pPS);

	HRESULT hr = 0;

	UINT texSampler = (*m_pPSC)->GetSamplerIndex("texSampler");
	UINT toonSampler = (*m_pPSC)->GetSamplerIndex("toonSampler");
	
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	// �J�����x�N�g���Z�b�g
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, NULL, &m_mtxWorld);
	D3DXVECTOR3 cameraPos = m_pManager->GetCameraManager()->GetCameraPos();
	D3DXVECTOR3 cameraPosR = m_pManager->GetCameraManager()->GetPosRCamera();
	D3DXVECTOR3 cameraVec = cameraPosR - cameraPos;
	D3DXVec3Normalize(&cameraVec, &cameraVec);
	// �J�����x�N�g���̃��[�J����(�@�������[�J���Ȃ̂�)
	D3DXVECTOR3 localCameraVec = cameraVec;
	D3DXVec3TransformCoord(&localCameraVec, &localCameraVec, &invWorld);
	(*m_pVSC)->SetFloatArray((*m_pD3DDevice), "gCameraDir", (float*)&localCameraVec, 3);

	D3DXVECTOR3 grayScale(0.298912f, 0.586611f, 0.114478f);
	(*m_pPSC)->SetFloatArray((*m_pD3DDevice), "gMonochromeScale", (float*)&grayScale, 3);
	(*m_pPSC)->SetInt((*m_pD3DDevice), "gMode", m_DrawMode);
	// ���_�錾��������Z�b�g(SetFVF�̑���)
	(*m_pD3DDevice)->SetVertexDeclaration((*m_pCSkinMesh->GetDecl()));

	// �g�D�[���p
	hr = (*m_pD3DDevice)->SetTexture(toonSampler, CTexture::GetTexture(TEXTURE_MONO));

	// �`��
	m_pCSkinMesh->Draw(this, RENDERER_TYPE_NORMAL);

	//*********����:�ȉ���K����������******************
	(*m_pD3DDevice)->SetTexture(toonSampler, NULL);
	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);

}
//*****************************************************************************
// �X�L�����b�V���Ōv�Z�������[���h�}�g���N�X�����ƂɃZ�b�g����
//*****************************************************************************
void CTitlePlayer::SetWorldMtxForNormalRender(D3DXMATRIX* worldMtx, int blendBoneNum)
{
	// ���W�ϊ��p�̃p�����[�^�𑗂�
	D3DXMATRIX view, proj;
	HRESULT hr;
	hr = (*m_pVSC)->SetInt((*m_pD3DDevice), "gBlendNum", blendBoneNum);
	view = m_pManager->GetCameraManager()->GetMtxView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxProj();
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gView", &view);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gProj", &proj);
	hr = (*m_pVSC)->SetMatrixArray((*m_pD3DDevice), "gWorld", worldMtx, MAX_BONE_MATRIX);

	view = m_pManager->GetCameraManager()->GetMtxLightView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxLightProj();
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gLightView", &view);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gLightProj", &proj);
}

//----EOF----