//=============================================================================
//
// ���f������ [CModel.h]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CModel.h"
#include "../CONST/const.h"
#include "../TEXTURE/CTexture.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
// ���f���̃t�@�C���p�X
static const char *MODEL_PATH[ MODEL_MAX ] = 
{
	"../data/MODEL/tank000.x",
}; 

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const int LENGTH_MAX = 256;	// �ő啶����

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ��錾
//*****************************************************************************
X_MODEL					CModel::m_Model[MODEL_MAX] = {NULL};

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init( LPDIRECT3DDEVICE9 *pDevice )
{
	LPD3DXBUFFER pD3DXAdjacencyBuff;		// �אڏ��o�b�t�@

	// �Q�[���Ŏg���S���f�����쐬
	for(int modelCnt = 0; modelCnt < MODEL_MAX; modelCnt++)
	{
		// ���f���̏�����
		if(FAILED(D3DXLoadMeshFromX(MODEL_PATH[modelCnt]	// �ǂݍ��ރt�@�C����
									, D3DXMESH_SYSTEMMEM
									, *pDevice
									, &pD3DXAdjacencyBuff
									, &m_Model[modelCnt].pD3DXBuffMatModel
									, NULL
									, &m_Model[modelCnt].nNumMatModel
									, &m_Model[modelCnt].pD3DXMeshModel)))
		{
			return E_FAIL;
		}
		
		// �I�v�e�B�}�C�Y
		if(FAILED(m_Model[modelCnt].pD3DXMeshModel->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE
														, (DWORD*)pD3DXAdjacencyBuff->GetBufferPointer()
														, NULL
														, NULL
														, NULL)))
		{
			return E_FAIL;
		}
		pD3DXAdjacencyBuff->Release();

		// ���_�v�f�z������
		D3DVERTEXELEMENT9 elements[] =
		{
			// ���_�X�g���[��(�p�C�v���C��)�ԍ�, �I�t�Z�b�g(���_�̌^�̐擪����̃o�C�g��), �f�[�^�^, DEFAULT�łn�j, �g�p�p�r, �g�p�p�r���������̂𕡐��g���Ƃ��Ɏd������ԍ�
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}		// ���_ (D3DDECLUSAGE_POSITIONT�͍��W�ϊ��ςݒ��_��\��)
			, {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0}	// �@��
			, {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0}	// UV
			, D3DDECL_END()																	// �錾�̏I��
		};

		// ���_�錾�������̂����
		(*pDevice)->CreateVertexDeclaration(elements, &m_Model[modelCnt].m_pDecl);

		// �N���[���쐻
		LPD3DXMESH pOldMesh = m_Model[modelCnt].pD3DXMeshModel;
		pOldMesh->CloneMesh(D3DXMESH_MANAGED
							, elements
							, *pDevice
							, &m_Model[modelCnt].pD3DXMeshModel);
		pOldMesh->Release();

		// �}�e���A���̐������
		m_Model[modelCnt].pD3DTexBuff = new LPDIRECT3DTEXTURE9[(int)m_Model[modelCnt].nNumMatModel];

		D3DXMATERIAL* d3dxmatrs = (D3DXMATERIAL*)m_Model[modelCnt].pD3DXBuffMatModel->GetBufferPointer();

		for(int i = 0; i < (int)m_Model[modelCnt].nNumMatModel; i++)
		{
			//�e�N�X�`�������[�h
			m_Model[modelCnt].pD3DTexBuff[i] = NULL;
		
			d3dxmatrs[i].MatD3D.Ambient = d3dxmatrs[i].MatD3D.Diffuse;
			d3dxmatrs[i].MatD3D.Ambient.r *= 0.3f;
			d3dxmatrs[i].MatD3D.Ambient.g *= 0.3f;
			d3dxmatrs[i].MatD3D.Ambient.b *= 0.3f;

			// �e�N�X�`���̏���
			char temp[LENGTH_MAX];
			if(d3dxmatrs[i].pTextureFilename != NULL)
			{
				//���g������
				ZeroMemory(temp, sizeof(char) * LENGTH_MAX);

				//������A��
				strcat(temp, TEX_FOLDER_PATH);
				strcat(temp, d3dxmatrs[i].pTextureFilename);
				
				//�e�N�X�`���ǂݍ���
				if(FAILED(D3DXCreateTextureFromFile(*pDevice, temp, &m_Model[modelCnt].pD3DTexBuff[i])))
				{
					//�e�N�X�`���ǂݍ���
					if (FAILED(D3DXCreateTextureFromFile(*pDevice, temp, &m_Model[modelCnt].pD3DTexBuff[i])))
					{
						assert(!"���f���̃e�N�X�`����data�ɂȂ��I");
					}
				}
			}
			else
			{
				//���g������
				ZeroMemory(temp, sizeof(char) * LENGTH_MAX);
			
				//������A��
				strcat(temp, TEX_FOLDER_PATH);
				strcat(temp, "default.png");
				
				//�e�N�X�`���ǂݍ���
				if(FAILED(D3DXCreateTextureFromFile(*pDevice, temp, &m_Model[modelCnt].pD3DTexBuff[i])))
				{
					assert(!"default.png��data�ɂȂ��I");
				}
			}
		}

		// X�t�@�C���ɖ@�����Ȃ��ꍇ�́A�@������������
		if (!(m_Model[modelCnt].pD3DXMeshModel->GetFVF() & D3DFVF_NORMAL))
		{
			ID3DXMesh* pTempMesh = NULL;
			
			m_Model[modelCnt].pD3DXMeshModel->CloneMeshFVF(m_Model[modelCnt].pD3DXMeshModel->GetOptions()
															, m_Model[modelCnt].pD3DXMeshModel->GetFVF()|D3DFVF_NORMAL
															, *pDevice
															, &pTempMesh);
		
			D3DXComputeNormals(pTempMesh, NULL);
			m_Model[modelCnt].pD3DXMeshModel->Release();
			m_Model[modelCnt].pD3DXMeshModel = pTempMesh;
		}

		// �C���f�b�N�X����ۑ�
		LPDIRECT3DINDEXBUFFER9 index_buffer;	// �C���f�b�N�X�o�b�t�@
		WORD* pIndices;							// �|���S���\���i���_�����N�j�f�[�^�ւ̃|�C���^
		WORD* pIndices_2;						// �|���S���\���i���_�����N�j�f�[�^�ւ̃|�C���^
		
		// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^���Q�b�g
		m_Model[modelCnt].pD3DXMeshModel->GetIndexBuffer(&index_buffer);

		// �ʂ̐��擾
		int numFace = m_Model[modelCnt].pD3DXMeshModel->GetNumFaces();

		// �C���f�b�N�X�𒸓_�������
		m_Model[modelCnt].pIndex = new WORD[numFace * NUM_POLYGON_CREATE_TRIANGLE];
		pIndices_2 = m_Model[modelCnt].pIndex;

		// �C���f�b�N�X�o�b�t�@�����b�N
		index_buffer ->Lock(0, 0, (void**)&pIndices , 0);

		// �C���f�b�N�X�f�[�^�����[�N�ɃR�s�[
		memcpy(pIndices_2, pIndices, sizeof(WORD)*numFace * NUM_POLYGON_CREATE_TRIANGLE);
		
		// �C���f�b�N�X�o�b�t�@���A�����b�N
		index_buffer ->Unlock();

		// ���
		index_buffer->Release();

		// ���_���ۑ�
		LPDIRECT3DVERTEXBUFFER9 vertex_buffer;	// ���_�o�b�t�@
		VERTEX* pVertices;						// ���_�f�[�^�ւ̃|�C���^
		VERTEX* pVertices_2;					// ���_�f�[�^�ւ̃|�C���^

		// ���_�o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^���Q�b�g
		m_Model[modelCnt].pD3DXMeshModel->GetVertexBuffer(&vertex_buffer);

		// ���_�����Q�b�g
		int numVertex = m_Model[modelCnt].pD3DXMeshModel->GetNumVertices();
		
		// ���_�̐������
		m_Model[modelCnt].pVertex = new VERTEX[numVertex];
		pVertices_2 = m_Model[modelCnt].pVertex;

		// ���_�o�b�t�@�����b�N
		vertex_buffer->Lock(0, 0, (void**)&pVertices, 0);

		// ���_�f�[�^�����[�N�ɃR�s�[
		memcpy(pVertices_2, pVertices, sizeof(VERTEX)*numVertex);

		// ���_�o�b�t�@���A�����b�N
		vertex_buffer->Unlock();

		// ���
		vertex_buffer->Release();
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit( void )
{
	// �g�����S�e�N�X�`���|�C���^���J��
	for( int modelCnt = 0; modelCnt < MODEL_MAX; modelCnt++ )
	{
		if(m_Model[modelCnt].pD3DTexBuff)
		{
			for (int i = 0; i < (int)m_Model[modelCnt].nNumMatModel; i++)
			{
				if(m_Model[modelCnt].pD3DTexBuff[i])
				{
					(m_Model[modelCnt].pD3DTexBuff[i])->Release();
					(m_Model[modelCnt].pD3DTexBuff[i]) = NULL;
				}
			}
			delete[] m_Model[modelCnt].pD3DTexBuff;
			m_Model[modelCnt].pD3DTexBuff = NULL;
		}

		if(m_Model[modelCnt].pD3DXMeshModel)
		{
			m_Model[modelCnt].pD3DXMeshModel->Release();
			m_Model[modelCnt].pD3DXMeshModel = NULL;
		}

		if(m_Model[modelCnt].pD3DXBuffMatModel)
		{
			m_Model[modelCnt].pD3DXBuffMatModel->Release();
			m_Model[modelCnt].pD3DXBuffMatModel = NULL;
		}

		if(m_Model[modelCnt].pIndex)
		{
			delete[] m_Model[modelCnt].pIndex;
			m_Model[modelCnt].pIndex = NULL;
		}

		if(m_Model[modelCnt].pVertex)
		{
			delete[] m_Model[modelCnt].pVertex;
			m_Model[modelCnt].pVertex = NULL;
		}

		if (m_Model[modelCnt].m_pDecl)
		{
			m_Model[modelCnt].m_pDecl->Release();
			m_Model[modelCnt].m_pDecl = NULL;
		}
	}
}

//=============================================================================
// ���f���쐬����
//=============================================================================
void CModel::CreateModel( LPDIRECT3DDEVICE9 *pDevice)
{
	CModel::Init( pDevice);
}

//=============================================================================
// ���f�����Q�b�g
//=============================================================================
X_MODEL* CModel::GetModel(MODEL_TYPE type)
{
	assert((type >= 0 && type < MODEL_MAX)&& "�s���ȃ��f���^�C�v�I");
	return &m_Model[type];
}

X_MODEL* CModel::GetModel(char* fileName)
{
	// ����Ă�����̂̃t�@�C���p�X�Ɣ�r���Ă�������Ԃ�
	for(int i = 0; i < MODEL_MAX; i++)
	{
		if(strcmp(MODEL_PATH[i], fileName) == 0)
		{
			return &m_Model[i];
		}
	}
	assert(!"�s���ȃt�@�C���p�X�I");
	return NULL;
}
//----EOF----