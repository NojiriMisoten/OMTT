//=============================================================================
//
// �X�L�����b�V������ [SkinMesh.cpp]
// Author : ��K�@����
//
//=============================================================================
//==============================================
// �C���N���[�h
//==============================================
#include "CSkinMesh.h"
#include "../CONST/const.h"
#include "../SCENE/GAME/PLAYER/CPlayer.h"
//==============================================
// �}�N��
//==============================================
static const float COMMPRESS_COEFFICIENT = 0.5f;							// �R���v���X�̌W��

// �X�L�����b�V���̃p�X
static const char *SKIN_MODEL_PATH[SKIN_MESH_TYPE_MAX] =
{
	"../data/MODEL/SKIN_MODEL/Player02.x",
	"../data/MODEL/SKIN_MODEL/Player02_b.x",
};
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSkinMesh::CSkinMesh(void)
{
	m_pFrameRoot = NULL;
	m_pController = NULL;
	m_apAnimation = NULL;
	m_dwAnimType = 0;
	m_apAnimSetEx = NULL;
	m_pCallBackHandler = NULL;
	m_dAnimSpd = DEFFAULT_ANIM_SPD;
	m_pDevice = NULL;
	m_nAnimType = 0;
	m_AnimTime = 0;
	m_pTexture = NULL;
	m_isCulcBone = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSkinMesh::~CSkinMesh(void)
{
}

//=============================================================================
// �{�[���s��̌ʊ��蓖��
// ����   : ���b�V���R���e�i���
// ����2  : �t���[�����
// �߂�l : ���蓖�Ăɐ���������
//=============================================================================
HRESULT CSkinMesh::AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase ,LPD3DXFRAME p_FrameRoot)
{
	MYFRAME *pFrame = NULL;
	DWORD dwBoneNum = 0;		// �{�[���̐�

	// �x�[�X������
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}

	// �{�[���̐��擾
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();

	// �{�[���̐����}�g���N�X�쐬
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];
	// �{�[�������[�v
	for (DWORD i = 0; i < dwBoneNum; ++i)
	{
		// �R���e�i�̒��ɂ��̖��O�̂��̂����邩
		pFrame = (MYFRAME*)D3DXFrameFind(p_FrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		if (pFrame == NULL)
		{
			return E_FAIL;
		}

		// �}�g���N�X������
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
	}

	return S_OK;
}

//=============================================================================
// �S�{�[���s��̊��蓖��
// �������������Ă��邪�A�ċA�̓s���セ�����Ă���
//=============================================================================
HRESULT CSkinMesh::AllocateAllBoneMatrices(LPD3DXFRAME pFrame ,LPD3DXFRAME p_FrameRoot)
{
	p_FrameRoot->pMeshContainer;
	// �R���e�i����Ȃ�{�[���s��̌ʊ��蓖��
	if (pFrame->pMeshContainer != NULL)
	{
		if(FAILED(AllocateBoneMatrix(pFrame->pMeshContainer,p_FrameRoot)))
		{
			return E_FAIL;
		}
	}
	
	// �Ȃ��肪����Ȃ�������
	if (pFrame->pFrameSibling != NULL)
	{
		if(FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling,p_FrameRoot)))
		{
			return E_FAIL;
		}
	}

	// �q������Ȃ�������
	if (pFrame->pFrameFirstChild != NULL)
	{
		if(FAILED(AllocateAllBoneMatrices(pFrame->pFrameFirstChild,p_FrameRoot)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// ���b�V���R���e�i�̕`��
// �t���[�����̂��ꂼ��̃��b�V���������_�����O����
//=============================================================================
VOID CSkinMesh::RenderMeshContainer(MYMESHCONTAINER* pMeshContainer
									, MYFRAME* pFrame
									, CPlayer* pPlayer
									, int type)
{
	DWORD i,k,m;
	DWORD dwBoneIndex,dwBlendMatrixNum;
	DWORD dwPrevBoneID;
	LPD3DXBONECOMBINATION pBoneCombination;
	UINT iMatrixIndex;
	D3DXMATRIX mStack;

	//�x���폜
	(void*)m;
	(void*)dwBoneIndex;

	DWORD boneNum = pMeshContainer->pSkinInfo->GetNumBones();
	
	//�X�L�����b�V���̏ꍇ
	if(pMeshContainer->pSkinInfo != NULL)
	{	
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();
	
		dwPrevBoneID = UINT_MAX;
		for(i = 0; i < pMeshContainer->dwBoneNum; ++i)
		{
			dwBlendMatrixNum = 0;
			for (k = 0; k< pMeshContainer->dwWeight; k++)
			{
				if (pBoneCombination[i].BoneId[k] != UINT_MAX)
				{
					dwBlendMatrixNum = k;
				}
			}

			// ���_�ɑ΂��Ă̏d�݂Â�
			for (k = 0; k < boneNum; k++)
			{
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				if (iMatrixIndex != UINT_MAX)
				{
					m_arrayWorldMtx[k] = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
				}
			}
			
			dwPrevBoneID = pBoneCombination[i].AttribId;

			// ���[���h�}�g���N�X�K�p
			pPlayer->SetWorldMtx(&m_arrayWorldMtx[0], (CPlayer::PLAYER_RENDERER_TYPE)type, dwBlendMatrixNum);

			m_pTexture = pMeshContainer->ppTextures;

			pPlayer->SetTextureForPS((CPlayer::PLAYER_RENDERER_TYPE)type, m_pTexture);

			// �`��
			pMeshContainer->MeshData.pMesh->DrawSubset(i);

			(*m_pDevice)->SetRenderState(D3DRS_VERTEXBLEND, 0);		// �����Ȃ���XP�œ����Ȃ�
			//(*m_pDevice)->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, 0);
		}
	}

	//�ʏ탁�b�V���̏ꍇ
	else
	{
		return;
	}

	//D3DXMATRIX mtxRot, worldMtx;
	//D3DXMatrixIdentity(&mtxRot);
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	////================================================
	//// �ʒu�Ɖ�]�s��ƃX�P�[���̌v�Z
	//// D3DXMatrixMultiply�ł�float�̖��ʌv�Z������
	//// �������邽�߂ɂ������Ă�ʂɍ����multiply�ł���
	////=================================================
	//worldMtx._11 = m_Scl.x * mtxRot._11;
	//worldMtx._12 = m_Scl.x * mtxRot._12;
	//worldMtx._13 = m_Scl.x * mtxRot._13;
	//worldMtx._14 = 0.0f;
	//
	//worldMtx._21 = m_Scl.y * mtxRot._21;
	//worldMtx._22 = m_Scl.y * mtxRot._22;
	//worldMtx._23 = m_Scl.y * mtxRot._23;
	//worldMtx._24 = 0.0f;
	//
	//worldMtx._31 = m_Scl.z * mtxRot._31;
	//worldMtx._32 = m_Scl.z * mtxRot._32;
	//worldMtx._33 = m_Scl.z * mtxRot._33;
	//worldMtx._34 = 0.0f;
	//
	//worldMtx._41 = m_Pos.x;
	//worldMtx._42 = m_Pos.y;
	//worldMtx._43 = m_Pos.z;
	//worldMtx._44 = 1.0f;
	//
	//// TODO
	//for (int i = 0; i < MAX_BONE_MATRIX; i++)
	//{
	//	m_arrayWorldMtx[i] = worldMtx;
	//}
}

//=============================================================================
// ���b�V���t���[���̕`��
// �t���[���������_�����O����
//============================================================================
VOID CSkinMesh::DrawFrame(LPD3DXFRAME pFrameBase, CPlayer* pPlayer, int type)
{
	MYFRAME* pFrame=(MYFRAME*)pFrameBase;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	// �R���e�i�[������Ȃ�
	while(pMeshContainer != NULL)
	{
		RenderMeshContainer(pMeshContainer, pFrame, pPlayer, type);

		pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
	}

	// �Ԃ牺�����Ă�̂���Ȃ�
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling, pPlayer, type);
	}

	// �ŏ��̎q������Ȃ�
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild, pPlayer, type);
	}
}

//=============================================================================
// ���b�V���t���[���̍X�V
// �t���[�����X�V����
//============================================================================
//�t���[�����̃��b�V�����Ƀ��[���h�ϊ��s����X�V����
VOID CSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	// �����Ă����t���[���������
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;

	// �e�̃}�g���N�X������Ȃ獇��
	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}

	// ���Ȃ������炻�̂܂�
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	// �Ȃ����Ă�����Ȃ�t���[���}�g���N�X�X�V
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	// �q������Ȃ�t���[���}�g���N�X�X�V
	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

//==============================================
// ������
//==============================================
void CSkinMesh::Init(LPDIRECT3DDEVICE9 *pDevice, CALLBACK_TIMING* pCallBackTimimig, LPD3DXANIMATIONCALLBACKHANDLER pHandl, SKIN_MESH_ANIM_MODEL modelType)
{
	m_pDevice = pDevice;

	for (int i = 0; i < MAX_BONE_MATRIX; i++)
	{
		D3DXMatrixIdentity(&m_arrayWorldMtx[i]);
	}

	// ���_�v�f�z������
	D3DVERTEXELEMENT9 elements[] =
	{
		// ���_�X�g���[��(�p�C�v���C��)�ԍ�, �I�t�Z�b�g(���_�̌^�̐擪����̃o�C�g��), �f�[�^�^, DEFAULT�łn�j, �g�p�p�r, �g�p�p�r���������̂𕡐��g���Ƃ��Ɏd������ԍ�
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		{ 0, 24, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()																					// ��`�I�� ��ΕK�v
	};
	// ���_�錾�������̂����
	(*pDevice)->CreateVertexDeclaration(elements, &m_pDecl);

	//�X�L�����b�V���Ɏg���f�[�^�̃����������������Ă���
	ZeroMemory(&m_pFrameRoot	,sizeof(LPD3DXFRAME));
	ZeroMemory(&m_pController	,sizeof(LPD3DXANIMATIONCONTROLLER));

	if(FAILED(D3DXLoadMeshHierarchyFromX(
								SKIN_MODEL_PATH[modelType],							// �t�@�C����
								D3DXMESH_MANAGED,					// �Ǘ��p�t���O�萔
								(*m_pDevice),						// �f�o�C�X�ւ̃|�C���^
								&m_InheritHierarchy,				// �K�w���f���\���̂ւ̃|�C���^
								NULL,
								&m_pFrameRoot,
								&m_pController
								)))
	{
		MessageBox(NULL,"�ǂݍ��݂Ɏ��s","�x��",MB_OK);
	}
	//�Ō��AdvanceTime ���ɌĂяo���R�[���o�b�N�C���X�^���X�𐶐�
	m_pCallBackHandler = pHandl;

	//�����ɐ������R���g���[���ɕϊ�����
	m_pController = GetTruthController();

	//�{�[���s��̊��蓖��
	AllocateAllBoneMatrices(m_pFrameRoot,m_pFrameRoot);

	//�e�A�j���[�V�����f�[�^�̍쐬
	DWORD nAnimMax = m_pController->GetNumAnimationSets();	//�A�j���[�V�����̌����R���g���[��������
	m_apAnimation		= new LPD3DXANIMATIONSET			[ nAnimMax ];
	m_apAnimSetEx		= new LPD3DXCOMPRESSEDANIMATIONSET	[ nAnimMax ];
	m_apCallBackDatas	= new CUSTOM_CALLBACK_DATA			[ nAnimMax ];

	//--�A�j���[�V�����������Ɋe�p�����[�^���--
	for(DWORD i=0; i<nAnimMax;i++)
	{
		m_pController->GetAnimationSet(i, &m_apAnimation[i]);
		m_apCallBackDatas[i].nAnimationID = i;
	}

	//�R�[���o�b�N�t���A�j���[�V�����̍Đ�
	CreateCommpressAnimation(pCallBackTimimig);

	//�����̃��[�V�����ɃZ�b�g
	m_pController->SetTrackAnimationSet(0, m_apAnimSetEx[0]);

	//==========================================
	// �t���[���s��X�V����(�����̎p���ɐݒ肵�Ă���)
	//==========================================
	D3DXMATRIX IdentityMtx;
	D3DXMatrixIdentity(&IdentityMtx);
	UpdateFrameMatrices(m_pFrameRoot,&IdentityMtx);

	// �A�j���[�V�����ԍ�������
	m_dwAnimType = 0;
}

//==============================================
// �I��
//==============================================
void CSkinMesh::Uninit(void)
{
	SAFE_RELEASE(m_pController);
	SAFE_RELEASE(m_pDecl);

	delete [] m_apAnimation;
	delete [] m_apAnimSetEx;
	delete [] m_apCallBackDatas;
	
	//�X�L�����b�V���p�̃t���[�����R�[���o�b�N�ŉ��
	//�Y���Ƃ�΂����ƂɂȂ�
	m_InheritHierarchy.DestroyFrame( m_pFrameRoot);
}

//==============================================
// �X�V
//==============================================
void CSkinMesh::Update(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxRot, worldMtx;
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	m_Pos = pos;
	m_Rot = rot;
	m_Scl = scl;
	//================================================
	// �ʒu�Ɖ�]�s��ƃX�P�[���̌v�Z
	// D3DXMatrixMultiply�ł�float�̖��ʌv�Z������
	// �������邽�߂ɂ������Ă�ʂɍ����multiply�ł���
	//=================================================
	worldMtx._11 = scl.x * mtxRot._11;
	worldMtx._12 = scl.x * mtxRot._12;
	worldMtx._13 = scl.x * mtxRot._13;
	worldMtx._14 = 0.0f;

	worldMtx._21 = scl.y * mtxRot._21;
	worldMtx._22 = scl.y * mtxRot._22;
	worldMtx._23 = scl.y * mtxRot._23;
	worldMtx._24 = 0.0f;

	worldMtx._31 = scl.z * mtxRot._31;
	worldMtx._32 = scl.z * mtxRot._32;
	worldMtx._33 = scl.z * mtxRot._33;
	worldMtx._34 = 0.0f;

	worldMtx._41 = pos.x;
	worldMtx._42 = pos.y;
	worldMtx._43 = pos.z;
	worldMtx._44 = 1.0f;

	// TODO
	for (int i = 0; i < MAX_BONE_MATRIX; i++)
	{
		//D3DXMatrixIdentity(&m_arrayWorldMtx[i]);
		m_arrayWorldMtx[i] = worldMtx;
	}

	//==========================================
	// �A�j���[�V�����X�V����  kDefaultSpan���i�߂Ă���
	//==========================================
	const double kDefaultSpan = m_dAnimSpd;							// �A�j���[�V�������x�@�����ϐ��ɂ���ƃA�j���[�V�������x�𓮓I�ɕς�����
	m_pController->AdvanceTime(kDefaultSpan,m_pCallBackHandler);	// AdvanceTime�����[�V������i�s�����Ă���

	//==========================================
	// �t���[���s��X�V����
	//==========================================
	UpdateFrameMatrices(m_pFrameRoot, &worldMtx);	// �e�̃t���[������ċA�I�Ɋe�t���[���̈ʒu���v�Z�A�`�悵�Ă���

	m_AnimTime += GetAnimSpd();
	//if (m_AnimTime > 1.0)
	//{
	//	m_AnimTime = 0.0;
	//}
}

//==============================================
// �`��
//==============================================
void CSkinMesh::Draw(CPlayer* pPlayer, int type)
{
	DrawFrame(m_pFrameRoot, pPlayer, type);	// �e�̃t���[������ċA�I�Ɋe�t���[���̈ʒu���v�Z�A�`�悵�Ă���
}

//==============================================
// �A�j���[�V�����ύX
//==============================================
void CSkinMesh::ChangeMotion(int nChangeAnimID,double dMoveRate)
{
	// ���ƈႤ�A�j���[�V�����ւ̃`�F���W�Ȃ�
	if(m_nAnimType != nChangeAnimID)
	{
		// �A�j���[�V�����̎�ޕۑ�
		m_nAnimType = nChangeAnimID;

		//�O��ƈقȂ�g���b�N�ʒu�𓾂�
		DWORD dwNewTrack = (m_dwAnimType == 0 ? 1 : 0);
		m_pController->SetTrackAnimationSet(dwNewTrack,m_apAnimSetEx[nChangeAnimID]);

		//�L�[���̃��Z�b�g
		m_pController->UnkeyAllTrackEvents(m_dwAnimType);
		m_pController->UnkeyAllTrackEvents(dwNewTrack);

		//�����݂̃g���b�N���̃f�[�^�����Z�b�g
		m_pController->KeyTrackSpeed (m_dwAnimType ,0.0f, m_pController->GetTime() ,dMoveRate,D3DXTRANSITION_LINEAR);
		m_pController->KeyTrackWeight(m_dwAnimType ,0.0f, m_pController->GetTime() ,dMoveRate,D3DXTRANSITION_LINEAR);
		m_pController->KeyTrackEnable(m_dwAnimType ,FALSE, m_pController->GetTime() + dMoveRate);

		//���ꂩ��J�ڂ���f�[�^��ݒ�
		m_pController->KeyTrackSpeed (dwNewTrack, 1.0f, m_pController->GetTime(), dMoveRate, D3DXTRANSITION_LINEAR);
		m_pController->KeyTrackWeight(dwNewTrack, 1.0f, m_pController->GetTime(), dMoveRate, D3DXTRANSITION_LINEAR);
		m_pController->SetTrackEnable(dwNewTrack, TRUE);
		m_pController->KeyTrackPosition(dwNewTrack, 0.0f, m_pController->GetTime());

		//���݂̃g���b�N���X�V
		m_dwAnimType = dwNewTrack;
	}
}

//==============================================================================
// �������A�j���[�V�����R���g���[�����擾����
//==============================================================================
LPD3DXANIMATIONCONTROLLER CSkinMesh::GetTruthController(void)
{
	//�^���̃R���g���[����p��
	ID3DXAnimationController* newTruthController = NULL;

	//���O�ɐ������R���g���[��������ł����ꍇ�̂ݎ��s
	if(m_pController != NULL)
	{
		//�N���[���Ȃ̂ɂ������Ȃ���
		//�������A�j���[�V�����R���g���[�������Ȃ�
		m_pController->CloneAnimationController(m_pController->GetMaxNumAnimationOutputs()
												, m_pController->GetMaxNumAnimationSets()
												, m_pController->GetMaxNumTracks()
												, m_pController->GetMaxNumEvents()
												, &newTruthController);
	}

	//�U�R���g���[���ɐ^���̃R���g���[�����������Ⴄ�̂ŁA
	//�O�̃R���g���[�����E�� �łȂ��ƃ����[�X���[�N���N����
	m_pController->Release();
	m_pController = nullptr;

	//�^���̃R���g���[���𓾂�
	return newTruthController;
}

//==============================================================================
// �R�[���o�b�N�Ăяo���@�\���̃��[�V�����f�[�^�����  ******* �������łP��̂݌Ă�(�����܂ō��������) ***********
//==============================================================================
void CSkinMesh::CreateCommpressAnimation(CALLBACK_TIMING* pCallBackTimimig)
{
	//===================================
	//�����ݒ�
	//===================================

	//�L�[�t���[���p�̃A�j���[�V�����Z�b�g��p��
	ID3DXKeyframedAnimationSet* keyFramedanimSet = NULL;
	
	//�o�b�t�@�ɑ΂��Ĉ��k�p�̃f�[�^��p��
	ID3DXBuffer* compressedData = NULL;
	UINT max = m_pController->GetNumAnimationSets();
	int timing = 0;
	for(UINT animIndex = 0; animIndex < max; animIndex++)
	{
		//�R�[���o�b�N�p�̐��ƃf�[�^��p��
		//�����̕��̃L�[�t���[�����ƂɕK�v�Ȃ��̂ƍl����
		//���A�j�����ɃR�[���o�b�N����񐔁@���[�V�������ʂ�OK
		int numCallbacks = pCallBackTimimig[animIndex].nCallBackNum;
		D3DXKEY_CALLBACK* keys;										// �����P�A�j���[�V�����ɂR��炷�Ȃ�R���
		keys = new D3DXKEY_CALLBACK[numCallbacks];

		m_pController	->GetAnimationSet(animIndex, (ID3DXAnimationSet**)&keyFramedanimSet);
		keyFramedanimSet->Compress(D3DXCOMPRESS_DEFAULT, COMMPRESS_COEFFICIENT, NULL, &compressedData);
		timing = 0;
		for(timing = 0; timing < numCallbacks; timing++)
		{
			// �R�[���o�b�N�p�Ɍ��X�̍Đ����ԓI�Ȃ̂�������Ă銴��
			double ticks = keyFramedanimSet->GetSourceTicksPerSecond();
			double period = keyFramedanimSet->GetPeriod();

			// �R�[���o�b�N���Ăяo����鎞�Ԃ��w��
			// �Ⴆ�΁��̂悤��/2.0f���Ă�̂Ń��[�V������50%���炢���Đ����ꂽ�Ƃ��ɔ�������
			//keys[0].Time = float(period/2.0f*ticks);

			keys[timing].Time = float(period * pCallBackTimimig[animIndex].CallBackTiming * ticks);	// 1.0���l�`�w(�A�j���[�V�����̎��Ԃ�0.0�`1.0�Ƃ���)
			keys[timing].pCallbackData = (LPVOID)&m_apCallBackDatas[animIndex];	// m_apCallBackDatas[0]�̂O���A�j���[�V�����ԍ�
		}
	
		// �R�[���o�b�N���ݒ肳�ꂽ�V���ȃA�j���[�V�����Z�b�g���쐬
		D3DXCreateCompressedAnimationSet(keyFramedanimSet->GetName()
										, keyFramedanimSet->GetSourceTicksPerSecond()
										, keyFramedanimSet->GetPlaybackType()
										, compressedData,numCallbacks
										, &keys[0]
										, &m_apAnimSetEx[animIndex]);

		//���k�f�[�^�͂�������Ȃ��̂ŎE��
		compressedData->Release();
		keyFramedanimSet->Release();

		SAFE_DELETE_ARRAY(keys);
	}

	for(UINT animIndex = 0; animIndex < max; animIndex++)
	{
		//=============================================
		// �e�ݒ��V�K�̃A�j���[�V�����Z�b�g�ɔ��f
		//=============================================
		//�ʏ�̃A�j���[�V�����Z�b�g�𖕏�
		m_pController->UnregisterAnimationSet(m_apAnimation[animIndex]);
	
		// �R�[���o�b�N�t���̃A�j���Z�b�g��V���ɓo�^����(�����ւ�)
		m_pController->RegisterAnimationSet(m_apAnimSetEx[animIndex]);
	}
}

//================================================================================================
// �A�j���[�V�����w�莞�Ԃ̏�Ԃɂ���
//================================================================================================
void CSkinMesh::SetAnimMotion(double time)
{
	//assert(time >= 0.0 && time <= 1.0 && "�Z�b�g���Ԃ��s���ł�");

	InitAnim();

	const double kDefaultSpan = time;
	m_AnimTime += time;
	m_pController->AdvanceTime(kDefaultSpan, m_pCallBackHandler);
}

//================================================================================================
// �A�j���[�V�����̌��ݎ����Q�b�g
//================================================================================================
double CSkinMesh::GetAnimTime(void)
{
	return m_AnimTime;
}

//=============================================================================
// �A�j���[�V�����������֐�
//============================================================================
void CSkinMesh::InitAnim(void)
{
	//�O��ƈقȂ�g���b�N�ʒu�𓾂�
	DWORD dwNewTrack = (m_dwAnimType == 0 ? 1 : 0);
	m_pController->SetTrackAnimationSet(dwNewTrack, m_apAnimSetEx[m_nAnimType]);

	//�L�[���̃��Z�b�g
	m_pController->UnkeyAllTrackEvents(m_dwAnimType);
	m_pController->UnkeyAllTrackEvents(dwNewTrack);

	//�����݂̃g���b�N���̃f�[�^�����Z�b�g
	m_pController->KeyTrackSpeed(m_dwAnimType, 0.0f, m_pController->GetTime(), 0, D3DXTRANSITION_LINEAR);
	m_pController->KeyTrackWeight(m_dwAnimType, 0.0f, m_pController->GetTime(), 0, D3DXTRANSITION_LINEAR);
	m_pController->KeyTrackEnable(m_dwAnimType, FALSE, m_pController->GetTime());

	//���ꂩ��J�ڂ���f�[�^��ݒ�
	m_pController->KeyTrackSpeed(dwNewTrack, 1.0f, m_pController->GetTime(), 0, D3DXTRANSITION_LINEAR);
	m_pController->KeyTrackWeight(dwNewTrack, 1.0f, m_pController->GetTime(), 0, D3DXTRANSITION_LINEAR);
	m_pController->SetTrackEnable(dwNewTrack, TRUE);
	m_pController->KeyTrackPosition(dwNewTrack, 0.0f, m_pController->GetTime());

	//���݂̃g���b�N���X�V
	m_dwAnimType = dwNewTrack;

	m_AnimTime = 0.0;
}
//----EOF----