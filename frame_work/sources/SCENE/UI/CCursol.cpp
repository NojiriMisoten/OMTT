//=============================================================================
//
// CCursol�N���X [CCursol.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCursol.h"
#include "../../INPUT/CInputKeyboard.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float DEFFAULT_MOV_ALPFA_SPD = 1.0f / 60.f;				// �ʏ�̓����x�̕ω��X�s�[�h

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCursol ::CCursol(LPDIRECT3DDEVICE9 *pDevice)
{
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		m_pD3DVtxBuff[idx] = NULL;
		m_pD3DTexBuff[idx] = NULL;
	}
	m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_pD3DDevice = pDevice;
	m_fAlpha = 0.f;
	m_bDraw = false;
	m_fLengthOfLeftToRight = 0.f;
	m_fMoveAlphaSign = 1.0f;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCursol ::~CCursol(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CCursol :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, float lengthOfLeftAndRight, int wblock, int hblock )
{
	VERTEX_2D	*pVtx;
	
	// ���_�̈ʒu
	m_vPos = pos;

	// ���ƍ����̐ݒ�
	m_fWidth = width;
	m_fHeight = height;

	// ������E�ւ̒���
	m_fLengthOfLeftToRight = lengthOfLeftAndRight;

	// ����
	bool createLeft = true;

	// ����
	float sign = 1.f;

	// UV�l�̈ړ���
	float movTexX = MAX_UV_VALUE / MENU_CURSOL_OBJ_NUM;

	// UV�l
	float leftTexX = MIN_UV_VALUE;
	float topTexY = MIN_UV_VALUE;
	float rightTexX = movTexX;
	float bottomTexY = MAX_UV_VALUE;
	
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// ���E�̔��f
		if(createLeft)
		{
			sign = -1.f;
		}
		else
		{
			sign = 1.f;
		}

		// �|���S�������̈ʒu�̃I�t�Z�b�g
		float createPosOffset = sign * lengthOfLeftAndRight * 0.5f; // ���S����̋����Ȃ̂Ŕ�����

		// �|���S���̃������m��
		(*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// VERTEX_2D�̌^�łS���_*�\���������m��
												D3DUSAGE_WRITEONLY,			// �ς��Ȃ�
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// �ς��Ȃ�
												&m_pD3DVtxBuff[idx],		// �i�[����ꏊ
												NULL);						// �ς��Ȃ�
		
		//********************************************************************************************************************
		// �|���S���̐ݒ�
		m_pD3DVtxBuff[idx]->Lock(0,0,(void**)&pVtx,0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
		//--------------------------------------------------------------------------------
		// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
		pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// ���S����̋����Ȃ̂� *0.5
								, m_vPos.y - m_fHeight * 0.5f						// Y���W 
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y - m_fHeight * 0.5f						// Y���W ���S����̋����Ȃ̂� *0.5
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y + m_fHeight * 0.5f						// Y���W ���S����̋����Ȃ̂� *0.5
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y + m_fHeight * 0.5f						// Y���W ���S����̋����Ȃ̂� *0.5
								, 0.0f);
		//--------------------------------------------------------------------------------
		//���g�͏��1.0f
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//--------------------------------------------------------------------------------
		//�F�Ɠ����x�̐ݒ�
		pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
		pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
		pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
		pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
		//--------------------------------------------------------------------------------
		//�g�p�������e�N�X�`���̍��W(0�`1)
		pVtx[0].tex = D3DXVECTOR2(leftTexX, topTexY);
		pVtx[1].tex = D3DXVECTOR2(rightTexX, topTexY);
		pVtx[2].tex = D3DXVECTOR2(leftTexX, bottomTexY);
		pVtx[3].tex = D3DXVECTOR2(rightTexX, bottomTexY);
		//----------------------------------------------------------------------------------
		m_pD3DVtxBuff[idx]->Unlock();																	//���b�N������K���A�����b�N�I�I
		//********************************************************************************************************************
		m_pD3DTexBuff[idx] = CTexture::GetTexture( texType );

		// UV�l�ړ�
		leftTexX += movTexX;
		rightTexX += movTexX;

		// �t���O���]
		createLeft = !createLeft;
	}

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CCursol ::Uninit(void)
{
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		if(m_pD3DVtxBuff[idx])
		{
			m_pD3DVtxBuff[idx]->Release();
			m_pD3DVtxBuff[idx] = NULL;		// ���_���W�����i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
		}
		if(m_pD3DTexBuff[idx])
		{
			m_pD3DTexBuff[idx] = NULL;		// �e�N�X�`�������i�[����ꏊ�̃A�h���X���m�ۂ���ꏊ
		}
	}
	m_bDraw = false;
	Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CCursol ::Update(void)
{
	if(!m_bDraw)
	{
		return;
	}
	// �����x�ύX
	m_fAlpha += DEFFAULT_MOV_ALPFA_SPD * m_fMoveAlphaSign;
	
	// ���E�l����
	if(m_fAlpha > MAX_ALPHA)
	{
		m_fAlpha = MAX_ALPHA;

		// �������]
		m_fMoveAlphaSign *= -1.f;
	}
	else if(m_fAlpha < MIN_ALPHA)
	{
		m_fAlpha = MIN_ALPHA;

		// �������]
		m_fMoveAlphaSign *= -1.f;
	}

	// �F�ύX
	SetColorPolygon(D3DXCOLOR(DEFAULT_POLYGON_COLOR.r
					, DEFAULT_POLYGON_COLOR.g
					, DEFAULT_POLYGON_COLOR.b
					, m_fAlpha));
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CCursol ::Draw(void)
{
	// �`�悵�Ȃ��Ȃ牽�����Ȃ�
	if(!m_bDraw)
	{
		return;
	}

	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULL�`�F�b�N
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}

		// �|���S���̕`��
		(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff[idx] ,0, sizeof(VERTEX_2D));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
		(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);											// ���_�t�H�[�}�b�g�̐ݒ�
		(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff[idx]);								// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
		(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP								// �v���~�e�B�u�̎��
									, 0													// �`����J�n���钸�_�ԍ�
									, DRAW_SQUARE_PRINITIV_NUM);						// ���������|���S����(�O�p�`��E�E�E�l�p�`���Ȃ�|���S�����Q�j
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CCursol* CCursol::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, float lengthOfLeftAndRight)
{
	CCursol* p = new CCursol(pDevice);

	// ������
	p->Init(pos, width, height, texType, lengthOfLeftAndRight);

	return p;
}

//*****************************************************************************
// �|���S���J���[�Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CCursol::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_2D	*pVtx;
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULL�`�F�b�N
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}
		
		//�|���S���̐ݒ�
		m_pD3DVtxBuff[idx]->Lock(0,0,(void**)&pVtx,0);							//���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
		
		//�F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
		pVtx[0].diffuse = color;
		pVtx[1].diffuse = color;
		pVtx[2].diffuse = color;
		pVtx[3].diffuse = color;
		m_pD3DVtxBuff[idx]->Unlock();											//���b�N������K���A�����b�N�I�I
	}
}

//*****************************************************************************
// �|���S���Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CCursol :: SetVertexPolygon(D3DXVECTOR3 pos, float width, float height)
{
	// ���W�X�V
	m_vPos = pos;

	// �傫���X�V
	m_fWidth = width;
	m_fHeight = height;

	// ����
	bool createLeft = true;

	// ����
	float sign = 1.f;

	VERTEX_2D	*pVtx;
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULL�`�F�b�N
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}

		// ���E�̔��f
		if(createLeft)
		{
			sign = -1.f;
		}
		else
		{
			sign = 1.f;
		}

		// �|���S�������̈ʒu�̃I�t�Z�b�g
		float createPosOffset = sign * m_fLengthOfLeftToRight * 0.5f; // ���S����̋����Ȃ̂Ŕ�����

		// �|���S���̐ݒ�
		m_pD3DVtxBuff[idx]->Lock(0,0,(void**)&pVtx,0);								// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)

		// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
		pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y - m_fHeight * 0.5f						// Y���W
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y - m_fHeight * 0.5f						// Y���W
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y + m_fHeight * 0.5f						// Y���W
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y + m_fHeight * 0.5f						// Y���W
								, 0.0f);
		m_pD3DVtxBuff[idx]->Unlock();												// ���b�N������K���A�����b�N�I�I
		
		// �t���O���]
		createLeft = !createLeft;
	}
}

//*****************************************************************************
// ���W�Z�b�g�֐�
//*****************************************************************************
void CCursol :: SetPos(D3DXVECTOR3 pos)
{
	// ���W�X�V
	m_vPos = pos;

	// ����
	bool createLeft = true;

	// ����
	float sign = 1.f;

	VERTEX_2D	*pVtx;
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULL�`�F�b�N
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}

		// ���E�̔��f
		if(createLeft)
		{
			sign = -1.f;
		}
		else
		{
			sign = 1.f;
		}

		// �|���S�������̈ʒu�̃I�t�Z�b�g
		float createPosOffset = sign * m_fLengthOfLeftToRight * 0.5f; // ���S����̋����Ȃ̂Ŕ�����

		// �|���S���̐ݒ�
		m_pD3DVtxBuff[idx]->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)

		// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
		pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y - m_fHeight * 0.5f						// Y���W
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y - m_fHeight * 0.5f						// Y���W
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y + m_fHeight * 0.5f						// Y���W
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y + m_fHeight * 0.5f						// Y���W
								, 0.0f);
		m_pD3DVtxBuff[idx]->Unlock();												// ���b�N������K���A�����b�N�I�I
		
		// �t���O���]
		createLeft = !createLeft;
	}
}

//*****************************************************************************
// ���W�Z�b�g�֐�
//*****************************************************************************
void CCursol :: SetPos(float x, float y, float z)
{
	// ���W�X�V
	m_vPos.x = x;
	m_vPos.y = y;
	m_vPos.z = z;

	// ����
	bool createLeft = true;

	// ����
	float sign = 1.f;

	VERTEX_2D	*pVtx;
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULL�`�F�b�N
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}

		// ���E�̔��f
		if(createLeft)
		{
			sign = -1.f;
		}
		else
		{
			sign = 1.f;
		}

		// �|���S�������̈ʒu�̃I�t�Z�b�g
		float createPosOffset = sign * m_fLengthOfLeftToRight * 0.5f;	// ���S����̋����Ȃ̂Ŕ�����

		// �|���S���̐ݒ�
		m_pD3DVtxBuff[idx]->Lock(0, 0, (void**)&pVtx, 0);							// ���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
		
		// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
		pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y - m_fHeight * 0.5f						// Y���W
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y - m_fHeight * 0.5f						// Y���W
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y + m_fHeight * 0.5f						// Y���W
								, 0.0f);											// �Q�c�̂Ƃ���z���W�͕K��0.0
		pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X���W
								, m_vPos.y + m_fHeight * 0.5f						// Y���W
								, 0.0f);
		m_pD3DVtxBuff[idx]->Unlock();												// ���b�N������K���A�����b�N�I�I
		
		// �t���O���]
		createLeft = !createLeft;
	}
}
//----EOF----