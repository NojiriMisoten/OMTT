//=============================================================================
//
// CInheritanceHierarchy�̏��� [CInheritanceHierarchy.cpp]
// Author : ��K�@����
//
//=============================================================================
#include "CInheritanceHierarchy.h"
#include "../CONST/const.h"
#include "../RENDERER/CRenderer.h"

//==============================================
// �}�N��
//==============================================
static const D3DXCOLOR	COLOR_OF_NOT_EXIST_MATERIAL = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// �}�e���A�����Ȃ��������̐ݒ�F

//==============================================================================
// �t���[���̍쐬(XFile�̊e�p�[�c�P�ʂ̂��Ɓ@�r�Ƃ����Ƃ��̃p�[�c���C���[�W)
//==============================================================================
HRESULT CInheritanceHierarchy::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
	//�t���[���p��
	MYFRAME *pFrame;

	//�����̐V�����t���[���ւ̃|�C���^�����������Ƃ�
	*ppNewFrame = NULL;

	//�t���[���̊m�ۂ����݂�
	pFrame = new MYFRAME;

	//�t���[���̃������m�ۂł��Ȃ�������
	if (pFrame == NULL)
	{
		//�X�L�����b�V���p�̃������s����ʒB���ďI��
		return E_OUTOFMEMORY;
	}

	//���x�� ���O�̃������m�ۂ����݂�
	pFrame->Name= new TCHAR[lstrlen(Name) + 1];	// \0����+1

	//�������s���Ȃ玸�s
	if (!pFrame->Name)
	{
		return E_FAIL;
	}

	//�������m�ۂ����t���[�����̖��O���ɏ����ʂ��Ă���
	strcpy(pFrame->Name,Name);

	//���͍s�������������
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);

	// �s��̏�����
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	//�e��|�C���^�̏�����
	pFrame->pMeshContainer = NULL;	//���b�V���R���e�i������
	pFrame->pFrameSibling = NULL;	//�Ԃ炳�����Ă�t���[��������
	pFrame->pFrameFirstChild = NULL;//�t���[���̎q�v�f������

	//�V�����t���[���̃|�C���^�ւ̃|�C���^�����F�X������t���[���ɂ��Ă���
	*ppNewFrame = pFrame;

	return S_OK;
}

//====================================================================================
// ���b�V���R���e�i�̍쐬(XFile�̊e�p�[�c�̏W��)�悤�̓X�L�����b�V�����f���������ō��
//====================================================================================
HRESULT CInheritanceHierarchy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
													CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
													DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
													LPD3DXMESHCONTAINER *ppMeshContainer )
{
	//========================================================================
	// �R���e�i�쐬�ɓ������ĕK�v�ȕϐ���p��
	//========================================================================
	MYMESHCONTAINER *pMeshContainer = NULL;
	int iFacesAmount;
	DWORD iMaterial;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	LPD3DXMESH pMesh = NULL;
	*ppMeshContainer = NULL;
	DWORD dwBoneNum=0;

	//========================================================================
	// �R���e�i�𓮓I�m�ۂ��Ēl�����Ă�����Ƃ�
	//========================================================================
	pMesh = pMeshData->pMesh;
	pMeshContainer = new MYMESHCONTAINER;

	//�������s���̂Ƃ��͏I��
	if (pMeshContainer == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//�쐬�ɐ��������璆�g��0�ŏ��������Ă���
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));

	//���b�V���R���e�i�Ƃ���XFile�ł����Ă閼�O���E��
	pMeshContainer->Name=new TCHAR[lstrlen(Name) + 1];	// \0����+1

	//���O���Ȃ������炨�������̂ŏI��
	if (!pMeshContainer->Name)
	{
		return E_FAIL;
	}

	//���O������Ȃ炻��ɂ���
	strcpy(pMeshContainer->Name,Name);

	// �f�o�C�X�Q�b�g
	pMesh->GetDevice(&pDevice);

	// �ʂ̐��Q�b�g
	iFacesAmount = pMesh->GetNumFaces();

	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	//�ʏ탁�b�V���̏ꍇ�͂��ꂪ�K�v�B�X�L���̏ꍇ�A���������ƃ��������[�N�ɂȂ�B
	if (pSkinInfo == NULL)
	{
		pMesh->AddRef();
	}

	//���b�V���̃}�e���A���ݒ�
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * NUM_POLYGON_CREATE_TRIANGLE];

	//�אڐ����܂��̓}�e���A�����Ȃ���ΏI��
	if( pMeshContainer->pAdjacency == NULL
	||	pMeshContainer->pMaterials == NULL )
	{
		return E_FAIL;
	}

	//�������b�V���R���e�i�ɗ�������
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount * NUM_POLYGON_CREATE_TRIANGLE);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	//========================================================================
	// ���f���̃}�e���A�����
	//========================================================================
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			// �}�e���A���̃f�B�t�[�Y�F�������I�ɕύX
			pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse = DEFAULT_POLYGON_COLOR;

			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				TCHAR strTexturePath[MAX_PATH];
				strcpy( strTexturePath,TEX_FOLDER_PATH);
				strcat_s( strTexturePath, sizeof( char ) * MAX_LENGTH_FILE_PATH, pMeshContainer->pMaterials[iMaterial].pTextureFilename );
				if( FAILED( D3DXCreateTextureFromFile( pDevice, strTexturePath, 
														&pMeshContainer->ppTextures[iMaterial] ) ) )
				pMeshContainer->ppTextures[iMaterial] = NULL;
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = COLOR_OF_NOT_EXIST_MATERIAL.r;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = COLOR_OF_NOT_EXIST_MATERIAL.g;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = COLOR_OF_NOT_EXIST_MATERIAL.b;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	//========================================================================
	//�Y�����b�V�����X�L�����������Ă���ꍇ
	//========================================================================
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		for(DWORD i= 0; i < dwBoneNum; i++)
		{	
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i),sizeof(D3DMATRIX));
		}
		if(FAILED(	pMeshContainer->pSkinInfo->ConvertToBlendedMesh(pMesh,
																	NULL,
																	pMeshContainer->pAdjacency,
																	NULL,
																	NULL,
																	NULL,
																	&pMeshContainer->dwWeight,
																	&pMeshContainer->dwBoneNum,
																	&pMeshContainer->pBoneBuffer,
																	&pMeshContainer->MeshData.pMesh)))
		{
			return E_FAIL;
		}

		// �R���o�[�g
		ConvertMesh(&pMeshContainer->MeshData.pMesh);
	}
	//���[�J���ɐ����������b�V���R���e�i�[���Ăяo�����ɃR�s�[���� �i�R�s�[����Ȃ����ǁE�E�E�j
	*ppMeshContainer = pMeshContainer;

	//�Q�ƃJ�E���^�𑝂₵���̂Ō��炷
	SAFE_RELEASE(pDevice);
	return S_OK;
}

//====================================================================================
// �t���[���̍폜(XFile�̊e�p�[�c�P�ʂ̂��Ɓ@�r�Ƃ����Ƃ��̃p�[�c���C���[�W)
//====================================================================================
HRESULT CInheritanceHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);

	// �n�܂�
	if(pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}

	// �Ȃ���
	if(pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	// ��g
	if(pFrameToFree->pMeshContainer)
	{
		DestroyMeshContainer(pFrameToFree->pMeshContainer);
	}

	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

//==============================================================================
// ���b�V���R���e�i�̍폜
//==============================================================================
HRESULT CInheritanceHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	DWORD iMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);				// �m�ۂ������O�J��
	SAFE_RELEASE(pMeshContainer->pSkinInfo);				// �X�L�����J��
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);			// �אڏ��J��
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);			// �}�e���A�����J��

	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);		// �{�[���̃}�g���N�X�J��
	
	// �e�N�X�`���[����Ȃ�
	if (pMeshContainer->ppTextures != NULL)
	{
		// �}�e���A���̐������[�v
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	if(pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}

	SAFE_DELETE(pMeshContainer);
	return S_OK;
}

//=============================================================================
// ���b�V���̃R���o�[�g�֐�
//============================================================================
void CInheritanceHierarchy::ConvertMesh(LPD3DXMESH* pMesh)
{
	LPDIRECT3DDEVICE9 *pDevice = CRenderer::GetDevice();
	// �N���[���쐻
	D3DVERTEXELEMENT9 elements[] =
	{
		// ���_�X�g���[��(�p�C�v���C��)�ԍ�, �I�t�Z�b�g(���_�̌^�̐擪����̃o�C�g��), �f�[�^�^, DEFAULT�łn�j, �g�p�p�r, �g�p�p�r���������̂𕡐��g���Ƃ��Ɏd������ԍ�
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		{ 0, 24, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()																					// ��`�I�� ��ΕK�v
	};
	LPD3DXMESH pOldMesh = *pMesh;
	pOldMesh->CloneMesh(D3DXMESH_MANAGED
		, elements
		, *pDevice
		, pMesh);
	pOldMesh->Release();
}

//----EOF----