//=============================================================================
//
// CSceneMeshSky�N���X [CSceneMeshSky.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneMeshSky.h"
#include "../SHADER/CShader.h"
#include "../CAMERA/CameraManager.h"
#include "../RENDERER/CRenderer.h"
#include "../MANAGER/CManager.h"
//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float DEFAULT_ROT_SPD = 0.0001f;		// ��̓�������
static const float CULC_TEX_Y_COEFFICIENT = 1.3f;	// �e�N�X�`����Y���W�v�Z�̌W��
static const float CULC_POS_Y_COEFFICIENT = 8.f;	// Y���W�v�Z�̌W��
static const float MIN_TEX_POS = 0.01f;				// �e�N�X�`���̍ŏ����W

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSceneMeshSky::CSceneMeshSky(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) :CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fAngle = 0.0f;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSceneMeshSky ::~CSceneMeshSky(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CSceneMeshSky :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VF *pVtx;
	
	float DistanceXVertex,DistanceZVertex;	// ���_�Ԃ̋���
	float lineX=0;
	float lineZ=0;
	float nNumDig = 0;						// �p�x�p
	float PlusDig = 0;
	float tex_X = MAX_UV_VALUE;
	float tex_Y = MAX_UV_VALUE;
	float Add_tex_X = MAX_UV_VALUE;
	float Add_tex_Y = MAX_UV_VALUE;
	float r = width;
	int counter =0;
	m_nNumWidthBlock = wblock;
	m_nNumHeightBlock = hblock;
	m_fRadius = width;
	m_fHeight = height;
	// �p�x�̌v�Z
	PlusDig = D3DX_PI * 2.f / (float)m_nNumWidthBlock;	// 360�x�ɂ���������Q�{���Ă���
	nNumDig = 0;

	// �e�N�X�`���̑����ʌv�Z
	Add_tex_X = MAX_UV_VALUE / (float)(m_nNumWidthBlock);
	Add_tex_Y = MAX_UV_VALUE / (float)(m_nNumHeightBlock / CULC_TEX_Y_COEFFICIENT);

	// ���|���S�����̌v�Z             ���l�p�`�͎O�p�`�Q�Ȃ̂�
	m_nNumPolygon = wblock * hblock * 2 + ((hblock - 1) * 4);	// +~�̕����͏k�ރ|���S����

	// ���C���f�b�N�X���̌v�Z  �R�͍ŏ��̎O�p�`�̒��_��
	m_nNumVertexIndex = 3 + (m_nNumPolygon - 1);	// �ŏ��̂P�ȍ~�͂P���_�v���X�Ń|���S����������

	// ���_�Ԃ̋����Z�o
	DistanceXVertex = width / wblock;
	DistanceZVertex = height / hblock;

	// ���_�̈ʒu
	m_vPos = pos;
	
	// ���_�̐���ݒ�
	m_nNumVtxNum = (wblock + 1) * (hblock + 1);

	// ���_�錾�������̂����
	(*m_pD3DDevice)->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	// �|���S���̃������m��
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VF) * m_nNumVtxNum	// VERTEX_2D�̌^�łS���_*�\���������m��
												, D3DUSAGE_WRITEONLY				// �ς��Ȃ�
												, 0						// �g�p���钸�_�t�H�[�}�b�g
												, D3DPOOL_MANAGED					// �ς��Ȃ�
												, &m_pD3DVtxBuff					// �i�[����ꏊ
												, NULL)))							// �ς��Ȃ�
	{
		return E_FAIL;																// ���������m�ۂł��Ȃ������烁�C���ɕԂ�
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	if(FAILED((*m_pD3DDevice)->CreateIndexBuffer(sizeof(WORD) * m_nNumVertexIndex	// ���C���f�b�N�X��
												, D3DUSAGE_WRITEONLY
												, D3DFMT_INDEX16
												, D3DPOOL_MANAGED
												, &m_pD3DIndexBuff
												, NULL)))
	{
		return E_FAIL;
	}

	float posY;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);		// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	// ���_�̐ݒ�(�����Ɍ������ĕ`�悵�����̂ŕ��i�Ƌt)
	for(int i = 0, count = 0;i < m_nNumVtxNum; i++)
	{
		posY = (float)(DistanceZVertex * count) * cosf(count * D3DX_PI / CULC_POS_Y_COEFFICIENT / (float)m_nNumHeightBlock);
		if(i == m_nNumVtxNum - 1 - m_nNumWidthBlock)
		{
			count = m_nNumHeightBlock - 1;
			posY = (float)(DistanceZVertex * (count)) * cosf((count) * D3DX_PI / CULC_POS_Y_COEFFICIENT / (float)m_nNumHeightBlock);
		}

		pVtx[i].vtx = D3DXVECTOR3(r * cosf(nNumDig)			// (���a * ������) * cos
								, posY
								, (r * sinf(nNumDig)));
		// �p�x����
		nNumDig += PlusDig;

		// �����������
		if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
		{
			// �s�ԍ�����
			count++;

			// �p�x��߂�
			nNumDig = 0;
										// 90�x���ق����̂Ŕ�����
			r = m_fRadius * cosf(count * D3DX_PI * 0.5f / (float)m_nNumHeightBlock);
		}
	}

	for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
	{
		// �e�N�X�`���̐ݒ�(�����Ɍ������ĕ`�悵�����̂ŕ��i�Ƌt)
		// �}�C�i�X1�ɂȂ�Ȃ��悤�ɂ���
		if(tex_X < MIN_UV_VALUE)
		{
			tex_X = MAX_UV_VALUE;
		}

		// �P��������
		if(i - 1 == m_nNumWidthBlock + count * (m_nNumWidthBlock + 1))
		{
			// �s�ԍ�����
			count++;

			// �e�N�X�`���w�����Ƃɂ��ǂ�
			tex_X = MAX_UV_VALUE;
		
			// �e�N�X�`���x��ύX
			tex_Y -= Add_tex_Y;

			// �}�C�i�X1�ɂȂ�Ȃ��悤�ɂ���
			if(tex_Y < MIN_UV_VALUE)
			{
				tex_Y = MIN_UV_VALUE;
			}
		}

		if(tex_Y == MIN_UV_VALUE)
		{
			tex_Y = MIN_TEX_POS;
			tex_X = MIN_TEX_POS;
		}

		// �e�N�X�`���ݒ�
		pVtx[i].tex = D3DXVECTOR2(tex_X, tex_Y);

		// �e�N�X�`���w���W�𑝂₷
		tex_X += Add_tex_X;

		// �@���̐ݒ�
		pVtx[i].nor = DEFAULT_UP_VECTOR;

		// �F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
		pVtx[i].diffuse = DEFAULT_POLYGON_COLOR;
	}
	
	m_pD3DVtxBuff->Unlock();																	//���b�N������K���A�����b�N�I�I
	m_pD3DTexBuff = CTexture::GetTexture( texType );
	//********************************************************************************************************************
	WORD *pIndex;
	// ���b�N
	m_pD3DIndexBuff->Lock(0, 0, (void**)&pIndex, 0);
	
	// �C���f�b�N�X�̐ݒ�																	// 2�Â݂Ă���̂�+2
	for(int i = 0, first = wblock + 1, second = 0, counter = 0 ; i < m_nNumVertexIndex - 1 ; i += 2, first++, second++)
	{
		// ���Ɍ����Ƃ��ɂP�Â����Ă������Ƃ𗘗p
		pIndex[i] = first;
		pIndex[i+1] = second;

		if(counter < hblock - 1)
		{
			// �܂�Ԃ��̏���
			if(second == wblock + (wblock + 1) * counter)
			{
				i += 2;	// 2�Â݂Ă���̂�+2
				pIndex[i] = second;
				pIndex[i+1] = first + 1;

				// �܂�Ԃ����J�E���g�A�b�v
				counter++;
			}
		}
	}

	// �A�����b�N
	m_pD3DIndexBuff->Unlock();


	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	CScene::AddLinkList(CRenderer::TYPE_RENDER_BASE_HIGH_LUMINANCE);
	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CSceneMeshSky ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	m_pD3DTexBuff = NULL;

	if(m_pD3DIndexBuff)
	{
		m_pD3DIndexBuff->Release();
		m_pD3DIndexBuff = NULL;
	}

	SAFE_RELEASE(m_pDecl);

	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CSceneMeshSky ::Update(void)
{
	m_vRot.y += DEFAULT_ROT_SPD;
	NormalizeRotation(&m_vRot.y);
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneMeshSky::DrawNormalRender(void)
{
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f		*****���Ԓ���*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.y, m_vRot.x, m_vRot.z);	// Y������_�ɉ�]���Ă���̂�Y,X,Z�̏��ɓ����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �V�F�[�_�[�̓K�p
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_TEX);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_TEX);

	PS_TYPE type = PS_TYPE_TEX;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

	HRESULT hr = D3D_OK;
	UINT texSamplerIndex = (*_psc)->GetSamplerIndex("texSampler");

	hr = (*m_pD3DDevice)->SetSamplerState(texSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	D3DXMATRIX view, proj, wvp;
	view = m_pManager->GetCameraManager()->GetMtxView();
	proj = m_pManager->GetCameraManager()->GetMtxProj();
	wvp = m_mtxWorld * view * proj;

	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gWVP", &wvp);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��

	// ���_�錾��������Z�b�g(SetFVF�̑���)
	(*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);

	// �|���S���̕`��
	hr = (*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);											// ���_�t�H�[�}�b�g�̐ݒ�
	hr = (*m_pD3DDevice)->SetTexture(texSamplerIndex, m_pD3DTexBuff);
	hr = (*m_pD3DDevice)->SetVertexShader(*_vs);
	hr = (*m_pD3DDevice)->SetPixelShader(*_ps);
	
	// �|���S���̕`��
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VF));		// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetIndices(m_pD3DIndexBuff);							// �C���f�b�N�X�Z�b�g
	(*m_pD3DDevice)->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP				// �v���~�e�B�u�̎��
											,0
											,0
											,m_nNumVtxNum					// �����_��
											,0
											,m_nNumPolygon);

	hr = (*m_pD3DDevice)->SetTexture(texSamplerIndex, NULL);
	hr = (*m_pD3DDevice)->SetVertexShader(NULL);
	hr = (*m_pD3DDevice)->SetPixelShader(NULL);
}

//*****************************************************************************
// ���P�x���o�������z�`��
//*****************************************************************************
void CSceneMeshSky::DrawBaseHighLuminanceRender(void)
{
	DrawNormalRender();
}

//*****************************************************************************
// �|���S���Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CSceneMeshSky :: SetVertexPolygon(void)
{
	VF	*pVtx;
	int lineX = 0, lineZ = 0;
	float DistanceXVertex,DistanceZVertex; // ���_�Ԃ̋���
	DistanceXVertex = m_fRadius / m_nNumWidthBlock;
	DistanceZVertex = m_fHeight / m_nNumHeightBlock;

	// ���_���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum; i++)
	{
		// ���_�̐ݒ�
		for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
		{
			pVtx[i].vtx = D3DXVECTOR3((m_vPos.x - m_fRadius * 0.5f) + lineX	// ���S����̋����Ȃ̂Ŕ�����
									, 0.0f
									, (m_vPos.z + m_fHeight * 0.5f) - lineZ);

			lineX += (int)DistanceXVertex;
			if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
			{
				lineX = 0;
				lineZ += (int)DistanceZVertex;
				count++;
			}
		}
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// �|���S���J���[�Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CSceneMeshSky::SetColorPolygon(D3DXCOLOR color)
{
	VF *pVtx;
	int lineX = 0, lineZ = 0;

	// ���_���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum ; i++)
	{
		pVtx[i].diffuse = color;
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneMeshSky* CSceneMeshSky::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float r, float height, int wblock, int hblock, TEXTURE_TYPE texType, CManager* pManager, D3DXVECTOR3 rot)
{
	// �쐬
	CSceneMeshSky* p = new CSceneMeshSky(pDevice);

	// ������
	p->Init(pos, r, height, texType, wblock, hblock);
	p->m_pManager = pManager;
	p->SetRot(rot);

	return p;
}
//----EOF----