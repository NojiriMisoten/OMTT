//=============================================================================
//
// CNumber�N���X [CNumber.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CNumber.h"
#include "../../CONST/const.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float NUMBER_WIDTH_TEX = 0.1f;		// �P�̐����̃e�N�X�`���̉���
static const int MAX_NUMBER_NUM = 10;			// �����̐�

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CNumber ::CNumber(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CNumber ::~CNumber(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
void CNumber :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	VERTEX_2D	*pVtx;
	
	// ���_�̈ʒu
	m_vPos = pos;

	// ���ƍ����̐ݒ�
	m_fWidth = width;
	m_fHeight = height;
	
	// �|���S���̃������m��
	(*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D) * 4		// �l�p�`�̒��_�����m��
										, D3DUSAGE_WRITEONLY		// �ς��Ȃ�
										, FVF_VERTEX_2D				// �g�p���钸�_�t�H�[�}�b�g
										, D3DPOOL_MANAGED			// �ς��Ȃ�
										, &m_pD3DVtxBuff			// �i�[����ꏊ
										, NULL);					// �ς��Ȃ�
	
	//********************************************************************************************************************
	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					//���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	//--------------------------------------------------------------------------------
	// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
	pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f		// ���S����̋����Ȃ̂� *0.5
							, m_vPos.y - m_fHeight * 0.5f
							, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f
							, m_vPos.y - m_fHeight * 0.5f
							, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f
							, m_vPos.y + m_fHeight * 0.5f
							, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f
							, m_vPos.y + m_fHeight * 0.5f
							, 0.0f);
	//--------------------------------------------------------------------------------
	//���g�͏��1.0f
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//-	-------------------------------------------------------------------------------
	//�F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
	pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
	//--------------------------------------------------------------------------------
	//�g�p�������e�N�X�`���̍��W(0�`1)
	pVtx[0].tex = D3DXVECTOR2(MIN_UV_VALUE, MIN_UV_VALUE);
	pVtx[1].tex = D3DXVECTOR2(NUMBER_WIDTH_TEX, MIN_UV_VALUE);
	pVtx[2].tex = D3DXVECTOR2(MIN_UV_VALUE, MAX_UV_VALUE);
	pVtx[3].tex = D3DXVECTOR2(NUMBER_WIDTH_TEX, MAX_UV_VALUE);
	//----------------------------------------------------------------------------------
	m_pD3DVtxBuff->Unlock();									//���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************
	m_pD3DTexBuff = CTexture::GetTexture( texType );
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CNumber ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}
	if(m_pD3DTexBuff)
	{
		m_pD3DTexBuff = NULL;
	}
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CNumber ::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CNumber ::Draw(void)
{
	// �|���S���̕`��
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);										// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);								// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP							// �v���~�e�B�u�̎��
								, 0												// �`����J�n���钸�_�ԍ�
								, DRAW_SQUARE_PRINITIV_NUM);					// ���������|���S����(�O�p�`��E�E�E�l�p�`���Ȃ�|���S�����Q�j
}

//*****************************************************************************
// �����Z�b�g�֐�
//*****************************************************************************
void CNumber :: SetNumber(int number)
{
	float leftX,rightX;
		
	// �������o����
	rightX = (float)((float)number / (float)MAX_NUMBER_NUM) + NUMBER_WIDTH_TEX;
	leftX = (float)((float)number / (float)MAX_NUMBER_NUM);

	VERTEX_2D	*pVtx;

	// �|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);		//���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	
	// �g�p�������e�N�X�`���̍��W(0�`1)
	pVtx[0].tex = D3DXVECTOR2(leftX, MIN_UV_VALUE);
	pVtx[1].tex = D3DXVECTOR2(rightX, MIN_UV_VALUE);
	pVtx[2].tex = D3DXVECTOR2(leftX, MAX_UV_VALUE);
	pVtx[3].tex = D3DXVECTOR2(rightX, MAX_UV_VALUE);
	
	m_pD3DVtxBuff->Unlock();							//���b�N������K���A�����b�N�I�I
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CNumber* CNumber::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	// �쐬
	CNumber* p = new CNumber(pDevice);

	// ������
	p->Init(pos, width, height, texType);

	return p;
	
}
//----EOF----