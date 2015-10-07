//=============================================================================
//
// CSceneBillBoard�N���X [CSceneBillBoard.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneBillBoard.h"
#include "../CAMERA/CameraManager.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSceneBillBoard::CSceneBillBoard(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) :CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_Scl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_pCameraManager = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSceneBillBoard ::~CSceneBillBoard(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CSceneBillBoard::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CCameraManager* pCameraManager)
{
	m_pCameraManager = pCameraManager;

	VF *pVtx;

	//���_�̈ʒu
	m_Pos = pos;
	
	//�Ίp���̊p�x
	m_fAngle = D3DX_PI / 6;		// 30�x�ɂ��������� / 6

	m_Rot.z = 0;

	// ���_�錾�������̂����
	(*m_pD3DDevice)->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	// �|���S���̃������m��
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VF) * 4			// �l�p�`�̒��_���Ȃ̂�4
												, D3DUSAGE_WRITEONLY			// �ς��Ȃ�
												, 0					// �g�p���钸�_�t�H�[�}�b�g
												, D3DPOOL_MANAGED				// �ς��Ȃ�
												, &m_pD3DVtxBuff				// �i�[����ꏊ
												, NULL)))						// �ς��Ȃ�
	{
		return;
	}

	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(-width
							, height
							, 0.f);
	pVtx[1].vtx = D3DXVECTOR3(width
							, height
							, 0.f);
	pVtx[2].vtx = D3DXVECTOR3(-width
							, -height
							, 0.f);
	pVtx[3].vtx = D3DXVECTOR3(width
							, -height
							, 0.f);
	//--------------------------------------------------------------------------------
	// �@��
	pVtx[0].nor = DEFAULT_UP_VECTOR;
	pVtx[1].nor = DEFAULT_UP_VECTOR;
	pVtx[2].nor = DEFAULT_UP_VECTOR;
	pVtx[3].nor = DEFAULT_UP_VECTOR;
	//---------------------------------------------------------------------------------
	// �F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
	pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
	//---------------------------------------------------------------------------------
	// �g�p�������e�N�X�`���̍��W
	pVtx[0].tex = D3DXVECTOR2(MIN_UV_VALUE, MIN_UV_VALUE);
	pVtx[1].tex = D3DXVECTOR2(MAX_UV_VALUE, MIN_UV_VALUE);
	pVtx[2].tex = D3DXVECTOR2(MIN_UV_VALUE, MAX_UV_VALUE);
	pVtx[3].tex = D3DXVECTOR2(MAX_UV_VALUE, MAX_UV_VALUE);
	//----------------------------------------------------------------------------------
	m_pD3DTexBuff = CTexture::GetTexture( texType );
	m_pD3DVtxBuff->Unlock();								// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************

}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CSceneBillBoard ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	m_pD3DTexBuff = NULL;

	SAFE_RELEASE(m_pDecl);

	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CSceneBillBoard ::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneBillBoard ::Draw(void)
{
	D3DXMATRIX mtxView;
	
	// �J�����̈ʒu�擾
	mtxView = m_pCameraManager->GetMtxView();

	D3DXMATRIX mtxScl,mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// �덷�̏���
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 3D�|���S���̕`��
	// �r���{�[�h�|���S���̕`��
	(*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);											// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VF));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);								// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP							// �v���~�e�B�u�̎��
							, 0
							, DRAW_SQUARE_PRINITIV_NUM);						// ���������|���S����(�O�p�`��E�E�E�l�p�`���Ȃ�|���S�����Q�j
}

//*****************************************************************************
// �|���S���Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CSceneBillBoard :: SetVertexPolygon(void)
{
	VF *pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(m_Pos.x - m_fLength
							, m_Pos.y + m_fLength
							, m_Pos.z);
	pVtx[1].vtx = D3DXVECTOR3(m_Pos.x + m_fLength
							, m_Pos.y + m_fLength
							, m_Pos.z);
	pVtx[2].vtx = D3DXVECTOR3(m_Pos.x - m_fLength
							, m_Pos.y - m_fLength
							, m_Pos.z);
	pVtx[3].vtx = D3DXVECTOR3(m_Pos.x + m_fLength
							, m_Pos.y - m_fLength
							, m_Pos.z);
	m_pD3DVtxBuff->Unlock();												// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
}

//*****************************************************************************
// �|���S���J���[�Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CSceneBillBoard::SetColorPolygon(D3DXCOLOR& color)
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
// �|���S���J���[�Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CSceneBillBoard::SetTexcordPolygon(D3DXVECTOR2& leftTop, D3DXVECTOR2& rightTop, D3DXVECTOR2& leftBottom, D3DXVECTOR2& rightBottom)
{
	VF *pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);			// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	// �F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
	pVtx[0].tex = leftTop;
	pVtx[1].tex = rightTop;
	pVtx[2].tex = leftBottom;
	pVtx[3].tex = rightBottom;
	m_pD3DVtxBuff->Unlock();								// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneBillBoard* CSceneBillBoard::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CCameraManager* pCameraManager, OBJTYPE objType)
{
	// �쐬
	CSceneBillBoard* p = new CSceneBillBoard(pDevice, objType);

	// ������
	p->Init(pos, width, height, texType, pCameraManager);

	return p;
}

//*****************************************************************************
// UV�ݒ�֐�
//*****************************************************************************
void CSceneBillBoard::SetUV(float left, float right, float top, float bottom)
{
	VF *pVtx;
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	// �g�p�������e�N�X�`���̍��W(0�`1)
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, bottom);
	pVtx[3].tex = D3DXVECTOR2(right, bottom);
	m_pD3DVtxBuff->Unlock();									// ���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
}
//----EOF----