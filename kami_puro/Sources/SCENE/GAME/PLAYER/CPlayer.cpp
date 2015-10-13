//=============================================================================
//
// CPlayer�N���X [CPlayer.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CPlayer.h"
#include "../../../INPUT/CInputKeyboard.h"
#include "../../../CAMERA/CameraManager.h"
#include "../CGame.h"
#include "../../../MANAGER/CManager.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float	DEFFAULT_MOV_SPD = 0.3f;								// �ʏ펞�ړ����x
static const float	DEFFAULT_ROT_SPD = 0.9f;
static const float	DEST_CAMERA_POS_COEFFICIENT = 3.f;						// �J�����Ɉڂ��Ăق����Ƃ���v�Z�p�W��
static const float	DEST_CAMERA_POS_Y_COEFFICIENT = 0.8f;					// �J�����Ɉڂ��Ăق����Ƃ���Y���W�v�Z�p�W��

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPlayer::CPlayer(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType) : CSceneX(pDevice, m_objType)
{
	// �p���x�N�g��
	m_vecFront = DEFAULT_FRONT_VECTOR;
	m_vecRight = DEFAULT_RIGHT_VECTOR;
	m_vecUp = DEFAULT_UP_VECTOR;

	// �ړ�������
	m_MovPos = D3DXVECTOR3(0, 0, 0);

	D3DXMatrixIdentity(&m_OldWorldMtx);
	m_pManager = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPlayer::~CPlayer(void)
{

}

//*****************************************************************************
// �쐬
//*****************************************************************************
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type, CManager* pManager)
{
	CPlayer* p = new CPlayer(pDevice);

	p->Init(pDevice, pos, type, pManager);

	return p;
}

//*****************************************************************************
// ������
//*****************************************************************************
void CPlayer::Init(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type, CManager* pManager)
{
	m_Pos = pos;
	m_pManager = pManager;

	// �X�L�����b�V���̏�����
	// =====�R�[���o�b�N�̃^�C�~���O�ݒ�=========
	m_pCallBackTimiming = new CALLBACK_TIMING[PLAYER_ANIM_MAX];

	m_pCallBackTimiming[PLAYER_SAMMON].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_SAMMON].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_SAMMON].nCallBackNum];
	m_pCallBackTimiming[PLAYER_SAMMON].pCallBackTiming[0] = 0.0f;

	m_pCallBackTimiming[PLAYER_STAN].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_STAN].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_STAN].nCallBackNum];
	m_pCallBackTimiming[PLAYER_STAN].pCallBackTiming[0] = 0.f;

	m_pCallBackTimiming[PLAYER_DAMAGE].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_DAMAGE].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_DAMAGE].nCallBackNum];
	m_pCallBackTimiming[PLAYER_DAMAGE].pCallBackTiming[0] = 0.5f;

	m_pCallBackTimiming[PLAYER_WALK].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_WALK].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_WALK].nCallBackNum];
	m_pCallBackTimiming[PLAYER_WALK].pCallBackTiming[0] = 0.3f;

	m_pCallBackTimiming[PLAYER_WAIT].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_WAIT].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_WAIT].nCallBackNum];
	m_pCallBackTimiming[PLAYER_WAIT].pCallBackTiming[0] = 0.99f;

	// ==================================================

	// �R�[���o�b�N���Ăяo���֐�
	m_pFunc = new CCallBackHandlerPlayer;
	m_pFunc->m_pMyAddress = this;

	// �X�L�����b�V�����f���ǂݍ���
	m_pCSkinMesh = new CSkinMesh;
	m_pCSkinMesh->Init(m_pD3DDevice, &m_pCallBackTimiming[0], m_pFunc, SKIN_MESH_TYPE_TEST);

	// �p���x�N�g��
	m_vecFront = DEFAULT_FRONT_VECTOR;
	m_vecRight = DEFAULT_RIGHT_VECTOR;
	m_vecUp = DEFAULT_UP_VECTOR;

	// �����N�ɒǉ�
	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL_VEC);
	CScene::AddLinkList(CRenderer::TYPE_RENDER_TOON_OBJECT_DEPTH);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CPlayer::Uninit(void)
{
	if (m_pCSkinMesh)
	{
		m_pCSkinMesh->Uninit();
		delete m_pCSkinMesh;
		m_pCSkinMesh = NULL;
	}
	if (m_pCallBackTimiming)
	{
		for (int i = 0; i < PLAYER_ANIM_MAX; i++)
		{
			if (m_pCallBackTimiming[i].pCallBackTiming)
			{
				delete[] m_pCallBackTimiming[i].pCallBackTiming;
				m_pCallBackTimiming[i].pCallBackTiming = NULL;
			}
		}
		delete[] m_pCallBackTimiming;
		m_pCallBackTimiming = NULL;
	}

	if (m_pFunc)
	{
		delete m_pFunc;
		m_pFunc = NULL;
	}

	Release();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CPlayer::Update(void)
{
	m_OldWorldMtx = m_mtxWorld;

	if (CInputKeyboard::GetKeyboardPress(DIK_A))
	{
		m_Rot.y += 0.01f;
		NormalizeRotation(&m_Rot.y);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_D))
	{
		m_Rot.y -= 0.01f;
		NormalizeRotation(&m_Rot.y);
	}

	// �t�����g�x�N�g���̐ݒ�
	m_vecFront.x = sinf(-m_Rot.y);
	m_vecFront.z = cosf(m_Rot.y - D3DX_PI);

	// ���C�g�x�N�g���̐ݒ�
	m_vecRight.x = cosf(m_Rot.y - D3DX_PI);
	m_vecRight.z = sinf(m_Rot.y);

	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);
#ifdef _DEBUG
	CDebugProc::Print("plyer���WX:%f\nplyer���WY:%f\nplyer���WZ:%f\n", m_Pos.x, m_Pos.y, m_Pos.z);
#endif
}

//*****************************************************************************
// �`��
//*****************************************************************************
void CPlayer::DrawNormalRender(void)
{
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

	// �V�F�[�_�[�̓K�p
	m_pVS = CShader::GetVS(VS_TYPE_NORMAL_RENDERER_SKINMESH);
	m_pVSC = CShader::GetVSC(VS_TYPE_NORMAL_RENDERER_SKINMESH);

	PS_TYPE type = PS_TYPE_NORMAL_RENDERER;

	m_pPS = CShader::GetPS(type);
	m_pPSC = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*m_pVS);
	(*m_pD3DDevice)->SetPixelShader(*m_pPS);

	HRESULT hr = 0;

	UINT texSampler = (*m_pPSC)->GetSamplerIndex("texSampler");
	UINT toonSampler = (*m_pPSC)->GetSamplerIndex("toonSampler");
	UINT depthSampler = (*m_pPSC)->GetSamplerIndex("depthSampler");

	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_BORDERCOLOR, 0xffffffff);

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

	// ���_�錾��������Z�b�g(SetFVF�̑���)
	(*m_pD3DDevice)->SetVertexDeclaration((*m_pCSkinMesh->GetDecl()));

	// �g�D�[���p
	hr = (*m_pD3DDevice)->SetTexture(toonSampler, CTexture::GetTexture(TEXTURE_MONO));

	// �[�x�l
	LPDIRECT3DTEXTURE9 *tex = CRenderer::GetRenderTexture(CRenderer::TYPE_RENDER_TOON_OBJECT_DEPTH);
	hr = (*m_pD3DDevice)->SetTexture(depthSampler, *tex);

	(*m_pD3DDevice)->SetTexture(texSampler, CTexture::GetTexture(TEXTURE_PLAYER));

	// �`��
	m_pCSkinMesh->Draw(this, RENDERER_TYPE_NORMAL);

	//*********����:�ȉ���K����������******************
	(*m_pD3DDevice)->SetTexture(toonSampler, NULL);
	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetTexture(depthSampler, NULL);
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);

}
//*****************************************************************************
// �X�L�����b�V���Ōv�Z�������[���h�}�g���N�X�����ƂɃZ�b�g����
//*****************************************************************************
void CPlayer::SetWorldMtxForNormalRender(D3DXMATRIX* worldMtx)
{
	// ���W�ϊ��p�̃p�����[�^�𑗂�
	D3DXMATRIX view, proj, lightVP;
	HRESULT hr;

	view = m_pManager->GetCameraManager()->GetMtxView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxProj();
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gView", &view);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gProj", &proj);
	hr = (*m_pVSC)->SetMatrixArray((*m_pD3DDevice), "gWorld", &worldMtx[0], MAX_BONE_MATRIX);

	view = m_pManager->GetCameraManager()->GetMtxLightView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxLightProj();
	lightVP = view * proj;
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gLightVP", &lightVP);
}

//*****************************************************************************
// �@���`��
//*****************************************************************************
void CPlayer::DrawNormalVecRender(void)
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
	m_pVS = CShader::GetVS(VS_TYPE_NORMAL_VEC_RENDERER_SKINMESH);
	m_pVSC = CShader::GetVSC(VS_TYPE_NORMAL_VEC_RENDERER_SKINMESH);

	PS_TYPE type = PS_TYPE_NORMAL_VEC_RENDERER;

	m_pPS = CShader::GetPS(type);
	m_pPSC = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*m_pVS);
	(*m_pD3DDevice)->SetPixelShader(*m_pPS);

	HRESULT hr = 0;
	
	// ���_�錾��������Z�b�g(SetFVF�̑���)
	(*m_pD3DDevice)->SetVertexDeclaration((*m_pCSkinMesh->GetDecl()));

	// �`��
	m_pCSkinMesh->Draw(this, RENDERER_TYPE_NORMAL_VEC);

	//*********����:�ȉ���K����������******************
	// �����Ȃ��Ƃ��ׂĂ̐F�����������Ȃ�
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}
//*****************************************************************************
// �X�L�����b�V���Ōv�Z�������[���h�}�g���N�X�����ƂɃZ�b�g����
//*****************************************************************************
void CPlayer::SetWorldMtxForNormalVecRender(D3DXMATRIX* worldMtx)
{
	D3DXMATRIX view, proj;
	HRESULT hr;

	view = m_pManager->GetCameraManager()->GetMtxView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxProj();

	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gProj", &proj);
	hr = (*m_pVSC)->SetMatrixArray((*m_pD3DDevice), "gWorld", &worldMtx[0], MAX_BONE_MATRIX);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gView", &view);
}

//*****************************************************************************
// �[�x�l�`��
//*****************************************************************************
void CPlayer::DrawToonObjectDepthRender(void)
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
	m_pVS = CShader::GetVS(VS_TYPE_TOON_OBJECT_DEPTH_SKINMESH);
	m_pVSC = CShader::GetVSC(VS_TYPE_TOON_OBJECT_DEPTH_SKINMESH);

	PS_TYPE type = PS_TYPE_TOON_OBJECT_DEPTH;

	m_pPS = CShader::GetPS(type);
	m_pPSC = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*m_pVS);
	(*m_pD3DDevice)->SetPixelShader(*m_pPS);
	
	// ���_�錾��������Z�b�g(SetFVF�̑���)
	(*m_pD3DDevice)->SetVertexDeclaration((*m_pCSkinMesh->GetDecl()));

	// �`��
	m_pCSkinMesh->Draw(this, RENDERER_TYPE_DEPTH);

	//*********����:�ȉ���K����������******************
	// �����Ȃ��Ƃ��ׂĂ̐F�����������Ȃ�
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);

	m_pManager->GetCameraManager()->SetCamera(m_pD3DDevice);
}
//*****************************************************************************
// �X�L�����b�V���Ōv�Z�������[���h�}�g���N�X�����ƂɃZ�b�g����
//*****************************************************************************
void CPlayer::SetWorldMtxForToonObjectDepthRender(D3DXMATRIX* worldMtx)
{
	HRESULT hr = 0;

	D3DXVECTOR3	cameraPos(0.0f, 400.0f, -400.0f);
	m_pManager->GetCameraManager()->SetLightCamera(m_pD3DDevice, cameraPos);

	D3DXMATRIX view, proj, vp;
	view = m_pManager->GetCameraManager()->GetMtxLightView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxLightProj();
	vp = view * proj;
	hr = (*m_pVSC)->SetMatrixArray((*m_pD3DDevice), "gWorld", &worldMtx[0], MAX_BONE_MATRIX);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gVP", &vp);
}


//*****************************************************************************
// �ړ��֐�
//*****************************************************************************
D3DXVECTOR3 CPlayer::Move(void)
{
	return D3DXVECTOR3(0, 0, 0);
}

//*****************************************************************************
//�A�j���[�V�����R�[���o�b�N�p�n���h���[�N���X��`
//*****************************************************************************
HRESULT CALLBACK CCallBackHandlerPlayer::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
{
	// �󂯎����void�|�C���^�𐳂����W�J
	CUSTOM_CALLBACK_DATA *pCallData = (CUSTOM_CALLBACK_DATA*)pCallbackData;

	// �������[�V����
	if (pCallData->nAnimationID == CPlayer::PLAYER_SAMMON)
	{

	}

	// �C�⃂�[�V����
	else if (pCallData->nAnimationID == CPlayer::PLAYER_STAN)
	{

	}

	// �_���[�W���[�V����
	else if (pCallData->nAnimationID == CPlayer::PLAYER_DAMAGE)
	{

	}

	// �������[�V����
	else if (pCallData->nAnimationID == CPlayer::PLAYER_WALK)
	{

	}

	// �ҋ@���[�V����
	else if (pCallData->nAnimationID == CPlayer::PLAYER_WAIT)
	{

	}

	return S_OK;
}

//*****************************************************************************
// ���[���h�}�g���b�N�X���Z�b�g����
//*****************************************************************************
void CPlayer::SetWorldMtx(D3DXMATRIX* worldMtx, PLAYER_RENDERER_TYPE type)
{
	switch (type)
	{
	case RENDERER_TYPE_NORMAL:
		SetWorldMtxForNormalRender(worldMtx);
		break;

	case RENDERER_TYPE_NORMAL_VEC:
		SetWorldMtxForNormalVecRender(worldMtx);
		break;

	case RENDERER_TYPE_DEPTH:
		SetWorldMtxForToonObjectDepthRender(worldMtx);
		break;

	default:
		assert(!"�s���ȃ^�C�v�I�I");
		break;
	}
}
//----EOF----