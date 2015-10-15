//=============================================================================
//
// CScene3D�N���X [CScene3D.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CScene3D.h"

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
D3DXMATRIX CScene3D ::m_mtxWorld;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CScene3D ::CScene3D(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType):CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CScene3D ::~CScene3D(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CScene3D :: Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VF *pVtx;
	
	//���_�̈ʒu
	m_Pos = pos;

	// ���ƍ���
	m_fWidth = width;
	m_fHeight = height;
	
	// ���_�錾�������̂����
	(*m_pD3DDevice)->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	// �|���S���̃������m��
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VF) * 4			// �l�p�`�̒��_���Ȃ̂� * 4
												, D3DUSAGE_WRITEONLY			// �ς��Ȃ�
												, 0					// �g�p���钸�_�t�H�[�}�b�g
												, D3DPOOL_MANAGED				// �ς��Ȃ�
												, &m_pD3DVtxBuff				// �i�[����ꏊ
												, NULL)))						// �ς��Ȃ�
	{
		return;															// ���������m�ۂł��Ȃ������烁�C���ɕԂ�
	}

	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(-m_fWidth * 0.5f								// ���S����̋���
							, 0.f
							, m_fHeight * 0.5f);
	pVtx[1].vtx = D3DXVECTOR3(m_fWidth * 0.5f
							, 0.f	
							, m_fHeight * 0.5f);
	pVtx[2].vtx = D3DXVECTOR3(-m_fWidth * 0.5f
							, 0.f
							, -m_fHeight * 0.5f);
	pVtx[3].vtx = D3DXVECTOR3(m_fWidth * 0.5f
							, 0.f
							, -m_fHeight * 0.5f);
	//--------------------------------------------------------------------------------
	// �@��
	pVtx[0].nor = DEFAULT_UP_VECTOR;
	pVtx[1].nor = DEFAULT_UP_VECTOR;
	pVtx[2].nor = DEFAULT_UP_VECTOR;
	pVtx[3].nor = DEFAULT_UP_VECTOR;
	//-	-------------------------------------------------------------------------------
	//�F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
	pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
	//--------------------------------------------------------------------------------
	//�g�p�������e�N�X�`���̍��W(0�`1)
	pVtx[0].tex = D3DXVECTOR2(MIN_ALPHA, MIN_ALPHA);
	pVtx[1].tex = D3DXVECTOR2(MAX_ALPHA, MIN_ALPHA);
	pVtx[2].tex = D3DXVECTOR2(MIN_ALPHA, MAX_ALPHA);
	pVtx[3].tex = D3DXVECTOR2(MAX_ALPHA, MAX_ALPHA);
	//----------------------------------------------------------------------------------
	m_pD3DVtxBuff->Unlock();											//���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
	
	// �e�N�X�`���ݒ�
	m_pD3DTexBuff = CTexture::GetTexture(texType);


	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CScene3D ::Uninit(void)
{
	SAFE_RELEASE(m_pD3DVtxBuff);
	m_pD3DTexBuff = NULL;
	SAFE_RELEASE(m_pDecl);
	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CScene3D ::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CScene3D ::Draw(void)
{
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

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

	// ���[���h�}�g���b�N�X�̐ݒ�
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
	// �|���S���̕`��
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VF));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);											// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);								// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP							// �v���~�e�B�u�̎��
								, 0												// �`����J�n���钸�_�ԍ�
								, DRAW_SQUARE_PRINITIV_NUM);					// ���������|���S����
}

//*****************************************************************************
// �|���S���Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CScene3D :: SetVertexPolygon(void)
{
	VF *pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(-m_fWidth * 0.5f					// ���S����̋����Ȃ̂Ŕ�����
							, 0.f
							, m_fHeight * 0.5f);
	pVtx[1].vtx = D3DXVECTOR3(m_fWidth * 0.5f
							, 0.f
							, m_fHeight * 0.5f);
	pVtx[2].vtx = D3DXVECTOR3(-m_fWidth * 0.5f
							, 0.f
							, -m_fHeight * 0.5f);
	pVtx[3].vtx = D3DXVECTOR3(m_fWidth * 0.5f
							, 0.f
							, -m_fHeight * 0.5f);
	m_pD3DVtxBuff->Unlock();													//���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
}

//*****************************************************************************
// �|���S���J���[�Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void	CScene3D::SetColorPolygon(D3DXCOLOR color)
{
	VF *pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
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
CScene3D* CScene3D::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	// �쐬
	CScene3D* p = new CScene3D(pDevice);

	// ������
	p->Init(pos, width, height, texType);

	return p;
	
}
//----EOF----