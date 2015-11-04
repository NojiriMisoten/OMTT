//=============================================================================
//
// CRope�N���X [CRope.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CRope.h"
#include "../../../MANAGER/CManager.h"
#include "../../../RENDERER/CRenderer.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �S���̒�R ����������ƌł��S���ɂȂ�
//static const float GOMU_RESIST = 0.85f;
static const float GOMU_RESIST = 0.6f;
// �����������Ƃ����̎��͂ɂǂ̂��炢���f���邩 2~3���炢
static const float AROUND_RESIST = 2.0f;
// ��������͂̊��� ����ŃS�����������Ă���͂���߂���
static const float PULL_RESIST = 0.92f;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CRope::CRope(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) :CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_pManager = NULL;
	m_PosMax = 0;
	m_PosMin = 0;
	m_PullPower = 0;
	m_PullInterval = 0;
	m_PullCount = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CRope ::~CRope(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CRope::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager, int wblock, int hblock)
{
	m_pManager = pManager;

	m_PosMin = pos.y;
	m_PosMax = 0;

	VF  *pVtx;
	float DistanceXVertex, DistanceZVertex; // ���_�Ԃ̋���
	float lineX = 0;
	float lineZ = 0;
	float tex_X = 0;
	float tex_Y = 0;
	int counter = 0;
	int i;
	float Add_tex_X = 1.f;
	float Add_tex_Y = 1.f;
	m_nNumWidthBlock = wblock;
	m_nNumHeightBlock = hblock;
	m_fWidth = width;
	m_fHeight = height;
	m_fGridWidth = m_fWidth / m_nNumWidthBlock;
	m_fGridHeight = m_fHeight / m_nNumHeightBlock;
	Add_tex_X = 1.f / (float)(m_nNumWidthBlock);
	Add_tex_Y = 1.f / (float)(m_nNumHeightBlock);

	// ���|���S�����̌v�Z             ���l�p�`�͎O�p�`�Q�Ȃ̂�
	m_nNumPolygon = wblock * hblock * 2 + ((hblock - 1) * 4);	// +~�̕����͏k�ރ|���S����

	// ���C���f�b�N�X���̌v�Z
	m_nNumVertexIndex = 3 + (m_nNumPolygon - 1);	// �ŏ��̂P�ȍ~�͂P���_�v���X�Ń|���S����������

	// ���_�Ԃ̋����Z�o
	DistanceXVertex = m_fGridWidth;
	DistanceZVertex = m_fGridHeight;

	//���_�̈ʒu
	m_Pos = pos;

	// ���_�̐���ݒ�
	m_nNumVtxNum = (wblock + 1) * (hblock + 1);

	// �ʖ@���o�b�t�@�쐬
	m_vFaceNormalBuf = new D3DXVECTOR3[wblock * hblock * 2];
	for (int i = 0; i < wblock * hblock * 2; i++)
	{
		m_vFaceNormalBuf[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// ���_�@���o�b�t�@�̍쐬
	m_vVtxNormalBuf = new D3DXVECTOR3[m_nNumVtxNum];
	for (int i = 0; i < m_nNumVtxNum; i++)
	{
		m_vVtxNormalBuf[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// �|���S���̃������m��
	if (FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VF) * m_nNumVtxNum	// VERTEX_2D�̌^�Ŋm��
		, D3DUSAGE_WRITEONLY				// �ς��Ȃ�
		, 0						// �g�p���钸�_�t�H�[�}�b�g
		, D3DPOOL_MANAGED					// �ς��Ȃ�
		, &m_pD3DVtxBuff					// �i�[����ꏊ
		, NULL)))							// �ς��Ȃ�
	{
		return;																// ���������m�ۂł��Ȃ������烁�C���ɕԂ�
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED((*m_pD3DDevice)->CreateIndexBuffer(sizeof(WORD) * m_nNumVertexIndex	// ���C���f�b�N�X��
		, D3DUSAGE_WRITEONLY
		, D3DFMT_INDEX16
		, D3DPOOL_MANAGED
		, &m_pD3DIndexBuff
		, NULL)))
	{
		return;
	}

	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	// ���_�̐ݒ�
	for (int i = 0, count = 0; i < m_nNumVtxNum; i++)
	{
		pVtx[i].vtx = D3DXVECTOR3((-width * 0.5f) + lineX		// ���S����̋����Ȃ̂Ŕ�����
			, pos.y
			, (height * 0.5f) - lineZ);

		pVtx[i].nor = D3DXVECTOR3(0.f, 0.f, 0.f);

		// �F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
		pVtx[i].diffuse = DEFAULT_POLYGON_COLOR;

		lineX += DistanceXVertex;
		if (i == wblock + (wblock + 1) * count)
		{
			lineX = 0;
			lineZ += DistanceZVertex;
			count++;
		}
		if (i == 0)
		{
			m_vCornerPos[0] = pVtx[i].vtx;
			m_vCornerPos[0].x += m_Pos.x;
			m_vCornerPos[0].y += m_Pos.y;
			m_vCornerPos[0].z += m_Pos.z;
		}

		if (i == wblock)
		{
			m_vCornerPos[1] = pVtx[i].vtx;
			m_vCornerPos[1].x += m_Pos.x;
			m_vCornerPos[1].y += m_Pos.y;
			m_vCornerPos[1].z += m_Pos.z;
		}

		if (i == m_nNumVtxNum - 1)
		{
			m_vCornerPos[2] = pVtx[i].vtx;
			m_vCornerPos[2].x += m_Pos.x;
			m_vCornerPos[2].y += m_Pos.y;
			m_vCornerPos[2].z += m_Pos.z;
		}

		if (i == m_nNumVtxNum - wblock - 1)
		{
			m_vCornerPos[3] = pVtx[i].vtx;
			m_vCornerPos[3].x += m_Pos.x;
			m_vCornerPos[3].y += m_Pos.y;
			m_vCornerPos[3].z += m_Pos.z;
		}
	}

	// �ʖ@���̎Z�o
	int faceNormalCounter = 0, hblockCounter = 0, wblockCounter = 0, vtxIndexOne = 1, vtxIndexSecond = wblock + 1;
	D3DXVECTOR3 norOne, norSecond, vecOneA, vecOneB, vecSecondA, vecSecondB;

	// �c�̃��[�v
	for (hblockCounter = 0; hblockCounter < hblock; hblockCounter++)
	{
		// ���̃��[�v
		for (wblockCounter = 0; wblockCounter < wblock; wblockCounter++, vtxIndexOne++, vtxIndexSecond++)
		{
			// ���Ɍ��Ă����Q�����
			// �P��
			vecOneA = pVtx[vtxIndexSecond - (wblock + 1)].vtx - pVtx[vtxIndexSecond].vtx;
			vecOneB = pVtx[vtxIndexSecond + 1].vtx - pVtx[vtxIndexSecond].vtx;
			D3DXVec3Cross(&norOne, &vecOneA, &vecOneB);
			D3DXVec3Normalize(&norOne, &norOne);
			m_vFaceNormalBuf[faceNormalCounter] = norOne;

			// ���_�̖@���̐ݒ�
			pVtx[vtxIndexSecond - (wblock + 1)].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexSecond - (wblock + 1)].nor, &pVtx[vtxIndexSecond - (wblock + 1)].nor);

			pVtx[vtxIndexSecond].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexSecond].nor, &pVtx[vtxIndexSecond].nor);

			pVtx[vtxIndexSecond + 1].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexSecond + 1].nor, &pVtx[vtxIndexSecond + 1].nor);

			faceNormalCounter++;

			// �Q��
			vecSecondA = pVtx[vtxIndexOne + (wblock + 1)].vtx - pVtx[vtxIndexOne].vtx;
			vecSecondB = pVtx[vtxIndexOne - 1].vtx - pVtx[vtxIndexOne].vtx;
			D3DXVec3Cross(&norSecond, &vecSecondA, &vecSecondB);
			D3DXVec3Normalize(&norSecond, &norSecond);
			m_vFaceNormalBuf[faceNormalCounter] = norSecond;

			// ���_�̖@���̐ݒ�
			pVtx[vtxIndexOne + (wblock + 1)].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexOne + (wblock + 1)].nor, &pVtx[vtxIndexOne + (wblock + 1)].nor);

			pVtx[vtxIndexOne].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexOne].nor, &pVtx[vtxIndexOne].nor);

			pVtx[vtxIndexOne - 1].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexOne - 1].nor, &pVtx[vtxIndexOne - 1].nor);

			faceNormalCounter++;
		}
		vtxIndexOne++;
		vtxIndexSecond++;
	}

	// �g�p�������e�N�X�`���̍��W
	for (i = 0; i < m_nNumVtxNum; i++)
	{
		if (i != 0)
		{
			if (pVtx[i].vtx.x != pVtx[i - 1].vtx.x)
			{
				tex_X += Add_tex_X;
				// 1�𒴂��Ȃ��悤�ɂ���
				if (tex_X > 1.f)
				{
					tex_X = 1.f;
				}
			}

			// �P��������
			//if (i - 1 == wblock + counter * (wblock + 1))
			if (pVtx[i].vtx.z != pVtx[i - 1].vtx.z)
			{
				counter++;

				tex_Y += Add_tex_Y;
				tex_X = 0.f;
				if (tex_Y > 1.f)
				{
					tex_Y = 1.f;
				}
			}
		}

		// �e�N�X�`���ݒ�
		pVtx[i].tex = D3DXVECTOR2((float)tex_X, (float)tex_Y);
	}

	m_pD3DVtxBuff->Unlock();												// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************

	WORD *pIndex;
	// ���b�N
	m_pD3DIndexBuff->Lock(0, 0, (void**)&pIndex, 0);

	// �C���f�b�N�X�̐ݒ�																	2�Â��Ă�̂łQ�Âi��
	for (int i = 0, first = wblock + 1, second = 0, counter = 0; i < m_nNumVertexIndex - 1; i += 2, first++, second++)
	{
		// ���Ɍ����Ƃ��ɂP�Â����Ă������Ƃ𗘗p
		pIndex[i] = first;
		pIndex[i + 1] = second;

		if (counter < hblock - 1)
		{
			// �܂�Ԃ��̏���
			if (second == wblock + (wblock + 1) * counter)
			{
				i += 2;	// 2�Â��Ă�̂łQ�Âi��
				pIndex[i] = second;
				pIndex[i + 1] = first + 1;

				// �܂�Ԃ����J�E���g�A�b�v
				counter++;
			}
		}
	}

	// �A�����b�N
	m_pD3DIndexBuff->Unlock();

	// ���_�錾�������̂����
	(*m_pD3DDevice)->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	// ���X�g�ɒǉ�
	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// �e�N�X�`���ݒ�
	m_pD3DTexBuff = CTexture::GetTexture(texType);
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CRope::Uninit(void)
{
	if (m_vFaceNormalBuf)
	{
		delete[] m_vFaceNormalBuf;
		m_vFaceNormalBuf = NULL;
	}

	if (m_vVtxNormalBuf)
	{
		delete[] m_vVtxNormalBuf;
		m_vVtxNormalBuf = NULL;
	}

	SAFE_RELEASE(m_pD3DIndexBuff);
	m_pD3DIndexBuff = NULL;
	SAFE_RELEASE(m_pD3DVtxBuff);
	m_pD3DTexBuff = NULL;
	SAFE_RELEASE(m_pDecl);
	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CRope::Update(void)
{
	// ��������͂̎��ԊǗ�
	if (m_PullCount < m_PullInterval)
	{
		m_PullCount++;

		// ���������
		m_PosMax += m_PullPower;

		// ��������͂̊���
		m_PullPower *= PULL_RESIST;
	}
	else
	{
		// �S���̊���
		m_PosMax *= GOMU_RESIST;
	}

	VF *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// �������̔���
	int numBlockHalf = static_cast<int>(m_nNumWidthBlock * 0.5f);
	for (int i = 0; i < m_nNumVtxNum; i++)
	{
		// �ŏ��̐܂�Ԃ��Ƃ��Ɛ܂�Ԃ����͂��߂͂O
		int num = i % (m_nNumWidthBlock+1);
		// �[��0�Œ����̒��_�ɍs���ق�1�ɂȂ�v�Z
		float max_ratio = 1.0f -
			static_cast<float>(abs(num - numBlockHalf)) /
			static_cast<float>(numBlockHalf);
		// �񎟊֐��݂����Ɋɂ₩�ɂ���
		max_ratio = max_ratio * (AROUND_RESIST - max_ratio) / AROUND_RESIST;

		// �����̒��_�ق�Max�̍��W�ɋ߂��Ȃ�悤�Z�b�g
		pVtx[i].vtx.y = m_PosMin + m_PosMax * max_ratio;
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CRope::DrawNormalRender(void)
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
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_TEX);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_TEX);

	PS_TYPE type = PS_TYPE_TEX;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

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

	// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);

	// �|���S���̕`��	
	hr = (*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);											// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VF));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetIndices(m_pD3DIndexBuff);								// �C���f�b�N�X�̐ݒ�
	(*m_pD3DDevice)->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP					// �v���~�e�B�u�̎��
		, 0
		, 0
		, m_nNumVtxNum							// �����_��
		, 0
		, m_nNumPolygon);
	
	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}

//*****************************************************************************
// �S���A�j���\�V�����X�^�[�g
// ��������́A���̎�𗣂��܂ł̃t���[���J�E���g
//*****************************************************************************
void CRope::Pull(float pullPower, int pullInterval)
{
	m_PullCount = 0;
	m_PullInterval = pullInterval;
	m_PullPower = pullPower;
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CRope* CRope::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3 pos, float width, float height,
	int wBlock, int hBlock,
	TEXTURE_TYPE texType,
	CManager* pManager)
{
	// �쐬
	CRope* p = new CRope(pDevice);

	// ������
	p->Init(pos, width, height, texType, pManager, wBlock, hBlock);

	p->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	return p;

}
//----EOF----