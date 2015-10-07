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
#include "../RENDERER/CRenderer.h"
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CScene2D ::CScene2D(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType):CScene(objType)
{
	m_pD3DDevice = pDevice;									// �f�o�C�X�I�u�W�F�N�g(�`��ɕK�v)
	m_pD3DVtxBuff = NULL;									// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	m_pD3DTexBuff = NULL;									// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �|���S���̒����̈ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �|���S���̉�]
	m_fLength = 0.0f;										// �|���S���̑Ίp���̒���
	m_fAngle = 0.0f;										// �|���S���̑Ίp���̊p�x
	m_AutoDraw = true;
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
HRESULT CScene2D :: Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VERTEX_2D *pVtx;
	
	// ���S����Ȃ̂Ŕ����ɂ��Ă���
	m_fLength = sqrtf(((width * 0.5f) * (width * 0.5f)) + ((height * 0.5f) * (height * 0.5f)));

	// ���_�̈ʒu
	m_Pos = pos;
	
	// �Ίp���̊p�x
	m_fAngle = D3DX_PI * 0.25f;	// 45�x���ق����̂�1/4�ɂ��Ă���

	// �p�x������
	m_Rot.z = 0.f;

	// �������ݒ�
	m_fWidth = width;
	m_fHeight = height;

	// �|���S���̃������m��
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D) * 4			// �l�p�`�̒��_�������� * 4
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
	pVtx[0].vtx = D3DXVECTOR3(m_Pos.x - m_fWidth * 0.5f		// ���S����̋����Ȃ̂Ŕ�����
							, m_Pos.y - m_fHeight * 0.5f
							, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_Pos.x + m_fWidth * 0.5f
							, m_Pos.y - m_fHeight * 0.5f
							, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_Pos.x - m_fWidth * 0.5f
							, m_Pos.y + m_fHeight * 0.5f
							, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_Pos.x + m_fWidth * 0.5f
							, m_Pos.y + m_fHeight * 0.5f
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

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CScene2D ::Uninit(void)
{
	SAFE_RELEASE(m_pD3DVtxBuff);
	m_pD3DTexBuff = NULL;
	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CScene2D ::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CScene2D::DrawUI(void)
{
	// �����`��ON�Ȃ�
	if(m_AutoDraw)
	{
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
void CScene2D :: SetVertexPolygon(void)
{
	VERTEX_2D *pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(m_Pos.x + sinf(m_Rot.z - m_fAngle) * m_fLength
							, m_Pos.y - cosf(m_Rot.z - m_fAngle) * m_fLength
							, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_Pos.x + sinf(m_Rot.z + m_fAngle) * m_fLength
							, m_Pos.y - cosf(m_Rot.z + m_fAngle) * m_fLength
							, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_Pos.x - sinf(m_Rot.z + m_fAngle) * m_fLength
							, m_Pos.y + cosf(m_Rot.z + m_fAngle) * m_fLength
							, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_Pos.x - sinf(m_Rot.z - m_fAngle) * m_fLength
							, m_Pos.y + cosf(m_Rot.z - m_fAngle) * m_fLength
							, 0.0f);
	m_pD3DVtxBuff->Unlock();		// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
}

//*****************************************************************************
// ���_�ύX
//*****************************************************************************
void CScene2D :: SetVertexPolygon(D3DXVECTOR3 pos, float width, float height)
{
	m_Pos = pos;
	m_fWidth = width;
	m_fHeight = height;

	VERTEX_2D *pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	//�`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(pos.x - width * 0.5f				// ���S����̋����Ȃ̂Ŕ�����
							, pos.y - height * 0.5f
							, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(pos.x + width * 0.5f
							, pos.y - height * 0.5f
							, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(pos.x - width * 0.5f
							, pos.y + height * 0.5f
							, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(pos.x + width * 0.5f
							, pos.y + height * 0.5f
							, 0.0f);
	m_pD3DVtxBuff->Unlock();									// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
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
CScene2D* CScene2D::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	// �쐬
	CScene2D* p = new CScene2D(pDevice);

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
	m_pD3DVtxBuff->Lock(0,0,(void**)&pVtx,0);					// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	
	pVtx[0].tex = D3DXVECTOR2(pUVIndex->left, pUVIndex->top);
	pVtx[1].tex = D3DXVECTOR2(pUVIndex->right, pUVIndex->top);
	pVtx[2].tex = D3DXVECTOR2(pUVIndex->left, pUVIndex->bottom);
	pVtx[3].tex = D3DXVECTOR2(pUVIndex->right, pUVIndex->bottom);
	
	m_pD3DVtxBuff->Unlock();									// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
}

//*****************************************************************************
// �Z�b�g�֐�
//*****************************************************************************
void	CScene2D::SetPos(D3DXVECTOR3& pos)
{
	m_Pos = pos;
	if(m_pD3DVtxBuff == NULL)
	{
		return;
	}

	VERTEX_2D *pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(m_Pos.x - m_fWidth * 0.5f		// ���S����̋����Ȃ̂Ŕ�����
							, m_Pos.y - m_fHeight * 0.5f
							, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_Pos.x + m_fWidth * 0.5f
							, m_Pos.y - m_fHeight * 0.5f
							, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_Pos.x - m_fWidth * 0.5f
							, m_Pos.y + m_fHeight * 0.5f
							, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_Pos.x + m_fWidth * 0.5f
							, m_Pos.y + m_fHeight * 0.5f
							, 0.0f);
	//--------------------------------------------------------------------------------
	m_pD3DVtxBuff->Unlock();									// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
};

//*****************************************************************************
// �Z�b�g�֐�
//*****************************************************************************
void	CScene2D::SetRot(D3DXVECTOR3& rot)
{
	m_Rot = rot;
	if(m_pD3DVtxBuff == NULL)
	{
		return;
	}

	VERTEX_2D *pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(m_Pos.x + sinf(m_Rot.z - m_fAngle) * m_fLength
							, m_Pos.y - cosf(m_Rot.z - m_fAngle) * m_fLength
							, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_Pos.x + sinf(m_Rot.z + m_fAngle) * m_fLength
							, m_Pos.y - cosf(m_Rot.z + m_fAngle) * m_fLength
							, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_Pos.x - sinf(m_Rot.z + m_fAngle) * m_fLength
							, m_Pos.y + cosf(m_Rot.z + m_fAngle) * m_fLength
							, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_Pos.x - sinf(m_Rot.z - m_fAngle) * m_fLength
							, m_Pos.y + cosf(m_Rot.z - m_fAngle) * m_fLength
							, 0.0f);
	//--------------------------------------------------------------------------------
	
	m_pD3DVtxBuff->Unlock();					// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
};
//----EOF----