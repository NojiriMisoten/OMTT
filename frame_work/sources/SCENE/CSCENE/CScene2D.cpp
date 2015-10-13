//=============================================================================
//
// CScene2D�N���X [CScene2D.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CScene2D.h"
#include "../../MATH/matrix.h"
#include "../../MATH/vector.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CScene2D::CScene2D(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene(nPriority, objType)
{
	m_pD3DDevice = pDevice;									// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	m_pD3DVtxBuff = NULL;									// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	m_pD3DTexBuff = NULL;									// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �|���S���̒����̈ʒu
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �|���S���̉�]
	m_AutoDraw = true;
	for (int idx = 0; idx < VERTEX_NUM; ++idx)
	{
		m_vtxPos[idx] = D3DXVECTOR2(0.0f, 0.0f);
	}
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CScene2D ::~CScene2D(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CScene2D::Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VERTEX_2D *pVtx;

	// ���_�̈ʒu
	m_vPos = pos;

	// �p�x������
	m_vRot.z = 0.f;

	// �������ݒ�
	m_fWidth = width;
	m_fHeight = height;

	// �|���S���̃������m��
	if (FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D)* 4			// �l�p�`�̒��_�������� * 4
		, D3DUSAGE_WRITEONLY			// �ς��Ȃ�
		, FVF_VERTEX_2D					// �g�p���钸�_�t�H�[�}�b�g
		, D3DPOOL_MANAGED				// �ς��Ȃ�
		, &m_pD3DVtxBuff				// �i�[����ꏊ
		, NULL)))						// �ς��Ȃ�
	{
		return E_FAIL;															// ���������m�ۂł��Ȃ������烁�C���ɕԂ�
	}

	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(- m_fWidth * 0.5f		// ���S����̋����Ȃ̂Ŕ�����
							, - m_fHeight * 0.5f
							, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(+ m_fWidth * 0.5f
							, - m_fHeight * 0.5f
							, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(- m_fWidth * 0.5f
							, + m_fHeight * 0.5f
							, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(+ m_fWidth * 0.5f
							, + m_fHeight * 0.5f
							, 0.0f);
	//--------------------------------------------------------------------------------
	// ���g�͏��1.0f
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//---------------------------------------------------------------------------------
	// �F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
	pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
	//--------------------------------------------------------------------------------
	// �g�p�������e�N�X�`���̍��W
	pVtx[0].tex = D3DXVECTOR2(MIN_UV_VALUE, MIN_UV_VALUE);
	pVtx[1].tex = D3DXVECTOR2(MAX_UV_VALUE, MIN_UV_VALUE);
	pVtx[2].tex = D3DXVECTOR2(MIN_UV_VALUE, MAX_UV_VALUE);
	pVtx[3].tex = D3DXVECTOR2(MAX_UV_VALUE, MAX_UV_VALUE);
	//----------------------------------------------------------------------------------
	m_pD3DTexBuff = CTexture::GetTexture(texType);
	m_pD3DVtxBuff->Unlock();																	// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************

	// ���W�ϊ�
	SetVertexPolygon();

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CScene2D::Uninit(void)
{
	if (m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	m_pD3DTexBuff = NULL;

	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CScene2D::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CScene2D::Draw(void)
{
	// �����`��ON�Ȃ�
	if (m_AutoDraw)
	{
		// ���W�ϊ�
		SetVertexPolygon();

		// �|���S���̕`��
		(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
		(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// ���_�t�H�[�}�b�g�̐ݒ�
		(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);							// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
		(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// �v���~�e�B�u�̎��
			, 0											// �`����J�n���钸�_�ԍ�
			, DRAW_SQUARE_PRINITIV_NUM);				// ���������|���S����
	}
}

//*****************************************************************************
// �|���S���Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CScene2D::SetVertexPolygon(void)
{
	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v, vertexPos[4];

	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	// ���W�ϊ�
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);

	// �|���S���̐ݒ�
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
								, v.y
								, 0.0f);

		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ���b�N������K���A�����b�N�I�I


	pos = VECTOR2(m_vJudgePos.x, m_vJudgePos.y);
	vertexPos[0] = VECTOR2(-m_fJudgeWidth * 0.5f, -m_fJudgeHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fJudgeWidth * 0.5f, -m_fJudgeHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fJudgeWidth * 0.5f, m_fJudgeHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fJudgeWidth * 0.5f, m_fJudgeHeight * 0.5f);

	// ���W�ϊ�
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		m_vtxJudgePos[i].x = v.x;
		m_vtxJudgePos[i].y = v.y;
	}
}

//*****************************************************************************
// ���_�ύX
//*****************************************************************************
void CScene2D::SetVertexPolygon(D3DXVECTOR3 pos, float width, float height)
{
	m_vPos = pos;
	m_fWidth = width;
	m_fHeight = height;
}

//*****************************************************************************
// �|���S���J���[�Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void	CScene2D::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_2D	*pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	// �F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
	pVtx[0].diffuse = color;
	pVtx[1].diffuse = color;
	pVtx[2].diffuse = color;
	pVtx[3].diffuse = color;
	m_pD3DVtxBuff->Unlock();									// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CScene2D* CScene2D::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int nPriority)
{
	// �쐬
	CScene2D* p = new CScene2D(pDevice, nPriority);

	// ������
	p->Init(pos, width, height, texType);

	return p;
}

//*****************************************************************************
// UV�Z�b�g�֐�
//*****************************************************************************
void	CScene2D::SetUV(UV_INDEX *pUVIndex)
{
	VERTEX_2D *pVtx;
	//********************************************************************************************************************
	//�|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)

	pVtx[0].tex = D3DXVECTOR2(pUVIndex->left, pUVIndex->top);
	pVtx[1].tex = D3DXVECTOR2(pUVIndex->right, pUVIndex->top);
	pVtx[2].tex = D3DXVECTOR2(pUVIndex->left, pUVIndex->bottom);
	pVtx[3].tex = D3DXVECTOR2(pUVIndex->right, pUVIndex->bottom);

	m_pD3DVtxBuff->Unlock();									// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
}

//*****************************************************************************
// UV�l90�x��]�֐�
//*****************************************************************************
void	CScene2D::Rot90_UV(void)
{
	VERTEX_2D *pVtx;
	D3DXVECTOR2 uv[4];
	//********************************************************************************************************************
	//�|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)

	for (int i = 0; i < 4; ++i)
	{
		uv[i] = pVtx[i].tex;
	}

	pVtx[0].tex = uv[1];
	pVtx[1].tex = uv[3];
	pVtx[2].tex = uv[0];
	pVtx[3].tex = uv[2];

	m_pD3DVtxBuff->Unlock();									// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
}

//*****************************************************************************
// �Z�b�g�֐�
//*****************************************************************************
void	CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_vPos = pos;
}

//*****************************************************************************
// �Z�b�g�֐�
//*****************************************************************************
void	CScene2D::SetRot(D3DXVECTOR3 rot)
{
	m_vRot = rot;
}

//*****************************************************************************
// �T�C�Y�ύX�֐�_������_
//*****************************************************************************
void CScene2D::AddWidth_BaseLeft(float addWidth)
{
	// �ύX�ʌv�Z
	m_fWidth += addWidth;

	VECTOR2 vertexPos[4];
	
	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	m_vPos.x += addWidth * 0.5f;

	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v;

	// ���W�ϊ�
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);
	
	// �|���S���̐ݒ�
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
			, v.y
			, 0.0f);
	
		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ���b�N������K���A�����b�N�I�I
}

//*****************************************************************************
// �T�C�Y�ύX�֐�_�E����_
//*****************************************************************************
void CScene2D::AddWidth_BaseRight(float addWidth)
{
	// �ύX�ʌv�Z
	m_fWidth += addWidth;

	VECTOR2 vertexPos[4];

	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	m_vPos.x -= addWidth * 0.5f;

	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v;

	// ���W�ϊ�
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);

	// �|���S���̐ݒ�
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
			, v.y
			, 0.0f);

		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ���b�N������K���A�����b�N�I�I
}

//*****************************************************************************
// ���ύX�֐�_������_�i�㑤�L�т�j
//*****************************************************************************
void CScene2D::AddHeight_BaseBottom(float addHeight)
{
	// �ύX�ʌv�Z
	m_fHeight += addHeight;

	VECTOR2 vertexPos[4];

	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	m_vPos.y -= addHeight * 0.5f;

	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v;

	// ���W�ϊ�
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);

	// �|���S���̐ݒ�
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
			, v.y
			, 0.0f);

		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ���b�N������K���A�����b�N�I�I
}

//*****************************************************************************
// ���ύX�֐�_�㑤��_�i�����L�т�j
//*****************************************************************************
void CScene2D::AddHeight_BaseTop(float addHeight)
{
	// �ύX�ʌv�Z
	m_fHeight += addHeight;

	VECTOR2 vertexPos[4];

	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	m_vPos.y += addHeight * 0.5f;

	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v;

	// ���W�ϊ�
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);

	// �|���S���̐ݒ�
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
			, v.y
			, 0.0f);

		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ���b�N������K���A�����b�N�I�I
}

//----EOF----