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
#include "../../../CONTROLLER/CControllerManager.h"
#include "../../../EFECT/CEffect.h"
#include "../JUDGE/CJudgeManager.h"
#include "../UI/CUiManager.h"
#include "../UI/CHpBar.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float	DEFFAULT_MOV_SPD = 0.3f;								// �ʏ펞�ړ����x
static const float	DEFFAULT_ROT_SPD = 0.01f;
static const float	DEST_CAMERA_POS_COEFFICIENT = 3.f;						// �J�����Ɉڂ��Ăق����Ƃ���v�Z�p�W��
static const float	DEST_CAMERA_POS_Y_COEFFICIENT = 0.8f;					// �J�����Ɉڂ��Ăق����Ƃ���Y���W�v�Z�p�W��
static const int	DEFFAULT_JAMP_POWER = 3;								// �W�����v�̗�
static const int	DEFFAULT_HP_PARAMETER = 100;							// HP�̗�


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

	m_ID = -1;
	m_HP = 0;
	m_JampPower = 0;
	m_JampFlag = false;
	m_AnimState = PLAYER_WAIT;
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
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID)
{
	CPlayer* p = new CPlayer(pDevice);

	p->Init(pDevice, pos, type, pManager, ID);

	return p;
}

//*****************************************************************************
// ������
//*****************************************************************************
void CPlayer::Init(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID)
{
	m_DestPos = m_Pos = pos;
	m_pManager = pManager;

	// �W�����v�p�����[�^������
	m_JampPower = 0;
	m_JampFlag = true;

	// �A�j��
	m_AnimState = PLAYER_WAIT;

	// �v���C���[HP
	m_HP = DEFFAULT_HP_PARAMETER;

	// ID
	m_ID = ID;

	// �X�L�����b�V���̏�����
	// =====�R�[���o�b�N�̃^�C�~���O�ݒ�=========
	m_pCallBackTimiming = new CALLBACK_TIMING[PLAYER_ANIM_MAX];

	m_pCallBackTimiming[PLAYER_WAIT].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_WAIT].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_WAIT].nCallBackNum];
	m_pCallBackTimiming[PLAYER_WAIT].pCallBackTiming[0] = 0.0f;

	m_pCallBackTimiming[PLAYER_LARIAT].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_LARIAT].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_LARIAT].nCallBackNum];
	m_pCallBackTimiming[PLAYER_LARIAT].pCallBackTiming[0] = 0.f;

	m_pCallBackTimiming[PLAYER_ELBOW_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_ELBOW_LEFT].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_ELBOW_LEFT].nCallBackNum];
	m_pCallBackTimiming[PLAYER_ELBOW_LEFT].pCallBackTiming[0] = 0.5f;

	m_pCallBackTimiming[PLAYER_ELBOW_RIGHT].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_ELBOW_RIGHT].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_ELBOW_RIGHT].nCallBackNum];
	m_pCallBackTimiming[PLAYER_ELBOW_RIGHT].pCallBackTiming[0] = 0.3f;

	m_pCallBackTimiming[PLAYER_DAMAGE_SMALL].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_DAMAGE_SMALL].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_DAMAGE_SMALL].nCallBackNum];
	m_pCallBackTimiming[PLAYER_DAMAGE_SMALL].pCallBackTiming[0] = 0.99f;

	m_pCallBackTimiming[PLAYER_CHOP_LEFT].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_CHOP_LEFT].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_CHOP_LEFT].nCallBackNum];
	m_pCallBackTimiming[PLAYER_CHOP_LEFT].pCallBackTiming[0] = 0.0f;

	m_pCallBackTimiming[PLAYER_CHOP_RIGHT].nCallBackNum = 1;
	m_pCallBackTimiming[PLAYER_CHOP_RIGHT].pCallBackTiming = new float[m_pCallBackTimiming[PLAYER_CHOP_RIGHT].nCallBackNum];
	m_pCallBackTimiming[PLAYER_CHOP_RIGHT].pCallBackTiming[0] = 0.0f;

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

	// �X�P�[��
	m_vScl = D3DXVECTOR3( 50, 50, 50 );

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

	// Get�Ō��݂̃t�F�[�Y�������Ă���
	BATTLE_MODE mode = m_pManager->GetJudgeManager()->GetBattleMode();


	if (mode == BATTLE_MOVE)
	{
		// �ړ��t�F�[�Y
		MovePhase();
	}
	else if (mode == BATTLE_FIGHT)
	{
		// �U���t�F�[�Y
		AttackPhase();
	}

	// �t�����g�x�N�g���̐ݒ�
	m_vecFront.x = -sinf(-m_Rot.y);
	m_vecFront.z = -cosf(m_Rot.y - D3DX_PI);

	// ���C�g�x�N�g���̐ݒ�
	m_vecRight.x = cosf(m_Rot.y - D3DX_PI);
	m_vecRight.z = sinf(m_Rot.y);

	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);
#ifdef _DEBUG
	if (m_ID == 0)
	{
		CDebugProc::PrintDL("[PLAYER]\n");
		CDebugProc::PrintDL("Pos: %+10.3f/%+10.3f/%+10.3f\n", m_Pos.x, m_Pos.y, m_Pos.z);
		CDebugProc::PrintDL("\n");
	}
	if (m_ID == 1)
	{
		CDebugProc::PrintDR("[PLAYER]\n");
		CDebugProc::PrintDR("Pos: %+10.3f/%+10.3f/%+10.3f\n", m_Pos.x, m_Pos.y, m_Pos.z);
		CDebugProc::PrintDR("\n");
	}


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

	(*m_pD3DDevice)->SetTexture(texSampler, *m_pCSkinMesh->GetTexture());

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
	D3DXMATRIX view, proj;
	HRESULT hr;

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

	D3DXMATRIX view, proj;
	view = m_pManager->GetCameraManager()->GetMtxLightView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxLightProj();
	hr = (*m_pVSC)->SetMatrixArray((*m_pD3DDevice), "gWorld", &worldMtx[0], MAX_BONE_MATRIX);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gView", &view);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gProj", &proj);
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
	if (pCallData->nAnimationID == CPlayer::PLAYER_WAIT)
	{

	}

	// �C�⃂�[�V����
	else if (pCallData->nAnimationID == CPlayer::PLAYER_LARIAT)
	{

	}

	// �_���[�W���[�V����
	else if (pCallData->nAnimationID == CPlayer::PLAYER_ELBOW_LEFT)
	{
	
	}

	// �������[�V����
	else if (pCallData->nAnimationID == CPlayer::PLAYER_ELBOW_RIGHT)
	{

	}

	// �ҋ@���[�V����
	else if (pCallData->nAnimationID == CPlayer::PLAYER_DAMAGE_SMALL)
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

//*****************************************************************************
// �ړ��t�F�[�Y�֐�
//*****************************************************************************
void CPlayer::MovePhase()
{
	const bool isPlayer1ForwardKeyboard = (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_LEFT_UP)
											|| CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_RIGHT_UP)
											)&& m_ID == 0;
	const bool isPlayer1BackKeyboard = (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_LEFT_DOWN)
											|| CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_RIGHT_DOWN)
											)&& m_ID == 0;

	const bool isPlayer2ForwardKeyboard = (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_LEFT_UP)
											|| CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_RIGHT_UP)
											)&& m_ID == 1;
	const bool isPlayer2BackKeyboard = (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_LEFT_DOWN)
											|| CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_RIGHT_DOWN)
											)&& m_ID == 1;

	const bool isForword = CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_UP, m_ID)
						|| CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_UP, m_ID)
						|| isPlayer1ForwardKeyboard
						|| isPlayer2ForwardKeyboard;

	const bool isBack = CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_DOWN, m_ID)
						|| CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_DOWN, m_ID)
						|| isPlayer1BackKeyboard
						|| isPlayer2BackKeyboard;

	if (isForword)
	{
		m_DestPos.x += m_vecFront.x;
		m_DestPos.z += m_vecFront.z;
	}
	if (isBack)
	{
		m_DestPos.x -= m_vecFront.x;
		m_DestPos.z -= m_vecFront.z;
	}

	// true�̏ꍇ�W�����v�ł���
	if (m_JampFlag)
	{
		if (isForword || isBack)
		{
			m_JampPower = DEFFAULT_JAMP_POWER;
			m_JampFlag = false;
		}
	}

	PlayerJamp();

	m_Pos.x += (m_DestPos.x - m_Pos.x) * DEFFAULT_MOV_SPD;
	m_Pos.z += (m_DestPos.z - m_Pos.z) * DEFFAULT_MOV_SPD;

	if (m_Pos.y < 0)
	{
		m_Pos.y = 0;
		// ���n
		if( !m_JampFlag )
		{
			D3DXVECTOR3 pos = m_Pos;
			CEffect::Create(30, EFFECT_DAGEKI_KYO, false, pos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(10, 10, 10));
			CEffect::Create(30, EFFECT_FOOTSTEP_WAVE, false, pos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(20, 20, 20));

			m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 1.0f, 5, 0.0f );
		}
		m_JampFlag = true;
	}
}

//*****************************************************************************
// �U���t�F�[�Y�֐�
//*****************************************************************************
void CPlayer::AttackPhase()
{
	// �W�����v���ɍU���t�F�C�Y���n�܂����Ƃ��p�̏C��
	if (m_Pos.y > 0.0f)
	{
		m_Pos.y = 0.0f;
	}
}

//*****************************************************************************
// �v���C���[�W�����v�֐�
//*****************************************************************************
void CPlayer::PlayerJamp()
{
	float tempPosy = m_Pos.y;
	m_Pos.y += (m_Pos.y - m_DestPos.y) + m_JampPower;
	m_DestPos.y = tempPosy;
	m_JampPower = -1;
}

//*****************************************************************************
// �Q�b�^�[
//*****************************************************************************
int CPlayer::GetHP()
{
	return m_HP;
}

CPlayer::PLAYER_ANIM_TYPE CPlayer::GetAnimState()
{
	return m_AnimState;
}

//*****************************************************************************
// �A�j���[�V�����Z�b�^�[
//*****************************************************************************
void CPlayer::SetAnimType( int type )
{
	if( type < 0 )
	{
		type = 0;
	}
	if( type >= (int)PLAYER_ANIM_MAX )
	{
		type = (int)PLAYER_ANIM_MAX;
	}
	m_AnimState = (PLAYER_ANIM_TYPE)type;
	m_pCSkinMesh->ChangeMotion( m_AnimState, DEFFAULT_CHANGE_ANIM_SPD );
}

//*****************************************************************************
// �_���[�W����
//*****************************************************************************
void CPlayer::TakeDamage( int damage )
{
	m_HP -= damage;
	switch( m_ID )
	{
	case 0:
		m_pManager->GetUiManager()->GetHpBar()->SubLeft( damage );
		break;

	case 1:
		m_pManager->GetUiManager()->GetHpBar()->SubRight( damage );
		break;
	}
}

//*****************************************************************************
// �񕜏���
//*****************************************************************************
void CPlayer::TakeHeal( int heal )
{
	m_HP += heal;
	switch( m_ID )
	{
	case 0:
		m_pManager->GetUiManager()->GetHpBar()->AddLeft( heal );
		break;

	case 1:
		m_pManager->GetUiManager()->GetHpBar()->AddRight( heal );
		break;
	}
}


//----EOF----