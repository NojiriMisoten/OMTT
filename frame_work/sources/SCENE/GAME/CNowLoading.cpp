//=============================================================================
//
// CNowLoading�N���X [CNowLoading.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CNowLoading.h"
#include "../../MATH/math.h"
#include "../../CONST/const.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float			DEFAULT_CHAR_SIZE = 75.f;											//�����T�C�Y
static const D3DXVECTOR3	INIT_CHAR_POS = D3DXVECTOR3(50.f, (float)SCREEN_WIDTH * 0.5f, 0.f);	// �����������W
static const float			MOV_POW = D3DX_PI / 120.f;											// �ړ���
static const float			MOV_WIDTH = 0.4f;													// �ړ���
static const float			CULC_INIT_MOVE_COUNTER_COEFFICIENT = 15.f;							// moveCounter�̏����l�v�Z�p�W��

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CNowLoading::CNowLoading(void)
{
	m_pD3DDevice = NULL;
	m_pD3DTexBuff = NULL;
	for(int index = 0; index < MAX_CHAR_NUM; ++index)
	{
		m_vCharPos[index] = D3DXVECTOR3(0,0,0);
		m_pD3DVtxBuff[index] = NULL;
		m_fMoveCounter[index] = (float)index * MOV_POW * CULC_INIT_MOVE_COUNTER_COEFFICIENT;
	}
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CNowLoading::~CNowLoading(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CNowLoading::Init(LPDIRECT3DDEVICE9* pDevice)
{
	VERTEX_2D	*pVtx;
	m_pD3DDevice = pDevice;
	float width = DEFAULT_CHAR_SIZE;
	float height = DEFAULT_CHAR_SIZE;
	float texX = 0.f;
	float movTexX = MAX_UV_VALUE / (float)MAX_CHAR_NUM;

	// �e�N�X�`���ǂݍ���
	if( FAILED( D3DXCreateTextureFromFile((*pDevice), "data/TEXTURE/NowLoading.png", &m_pD3DTexBuff)))
	{
		#ifdef _DEBUG
		MessageBox( NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���\n" , "�G���[", MB_OK | MB_ICONERROR );
		#endif
		m_pD3DTexBuff = NULL;
	}

	for(int index = 0; index < MAX_CHAR_NUM; ++index)
	{
		// �|���S���̃������m��
		if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// �l�p�`�̒��_�����m��
												D3DUSAGE_WRITEONLY,				// �ς��Ȃ�
												FVF_VERTEX_2D,					// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,				// �ς��Ȃ�
												&m_pD3DVtxBuff[index],			// �i�[����ꏊ
												NULL)))							// �ς��Ȃ�
		{
			return;																// ���������m�ۂł��Ȃ������烁�C���ɕԂ�
		}

		// �������W�̐ݒ�
		m_vCharPos[index] = INIT_CHAR_POS;
		m_vCharPos[index].x += index * DEFAULT_CHAR_SIZE;
		m_vCharPos[index].y += MOV_WIDTH * sinf(m_fMoveCounter[index]);

		//********************************************************************************************************************
		// �|���S���̐ݒ�
		m_pD3DVtxBuff[index]->Lock(0, 0, (void**)&pVtx, 0);				//���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
		//--------------------------------------------------------------------------------
		// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
		pVtx[0].vtx = D3DXVECTOR3(m_vCharPos[index].x - width * 0.5f		// ���S����̋����Ȃ̂�0.5�{���Ă���
								, m_vCharPos[index].y - height * 0.5f
								, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(m_vCharPos[index].x + width * 0.5f
								, m_vCharPos[index].y - height * 0.5f
								, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(m_vCharPos[index].x - width * 0.5f
								, m_vCharPos[index].y + height * 0.5f
								, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(m_vCharPos[index].x + width* 0.5f
								, m_vCharPos[index].y + height* 0.5f
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
		// �g�p�������e�N�X�`���̍��W(0�`1)
		pVtx[0].tex = D3DXVECTOR2(texX, MIN_ALPHA);
		pVtx[1].tex = D3DXVECTOR2(texX + movTexX, MIN_ALPHA);
		pVtx[2].tex = D3DXVECTOR2(texX, MAX_ALPHA);
		pVtx[3].tex = D3DXVECTOR2(texX + movTexX, MAX_ALPHA);
		//----------------------------------------------------------------------------------
		m_pD3DVtxBuff[index]->Unlock();							// ���b�N������K���A�����b�N�I�I
		//********************************************************************************************************************

		// UV�l�ύX
		texX += movTexX;
	}
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CNowLoading::Uninit(void)
{
	if(m_pD3DTexBuff)
	{
		m_pD3DTexBuff->Release();
		m_pD3DTexBuff = NULL;
	}
	for(int index = 0; index < MAX_CHAR_NUM; ++index)
	{
		if(m_pD3DVtxBuff[index])
		{
			m_pD3DVtxBuff[index]->Release();
			m_pD3DVtxBuff[index] = NULL;
		}
	}
}


//*****************************************************************************
// �X�V
//*****************************************************************************
void CNowLoading::Update(void)
{
	VERTEX_2D	*pVtx;
	float width = DEFAULT_CHAR_SIZE;
	float height = DEFAULT_CHAR_SIZE;

	// �����̈ړ�
	for(int index = 0; index < MAX_CHAR_NUM; ++index)
	{
		m_fMoveCounter[index] += MOV_POW;
		NormalizeRotation(&m_fMoveCounter[index]);
		m_vCharPos[index].y += MOV_WIDTH * sinf(m_fMoveCounter[index]);
		
		m_pD3DVtxBuff[index]->Lock(0, 0, (void**)&pVtx, 0);				//���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
		// �`�悵�������_���W�̐ݒ�(���W��Z���܂���N�����ɐݒ肷��)
		pVtx[0].vtx = D3DXVECTOR3(m_vCharPos[index].x - width * 0.5f	// ���S����̋����Ȃ̂�0.5�{���Ă���
								, m_vCharPos[index].y - height * 0.5f
								, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(m_vCharPos[index].x + width * 0.5f
								, m_vCharPos[index].y - height * 0.5f
								, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(m_vCharPos[index].x - width * 0.5f
								, m_vCharPos[index].y + height * 0.5f
								, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(m_vCharPos[index].x + width* 0.5f
								, m_vCharPos[index].y + height* 0.5f
								, 0.0f);
		m_pD3DVtxBuff[index]->Unlock();									//���b�N������K���A�����b�N�I�I
	}
}

//*****************************************************************************
// �`��
//*****************************************************************************
void CNowLoading::Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	(*m_pD3DDevice)->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), DEFAULT_POLYGON_COLOR, 1.0f, 0);

	// �`��̊J�n
	if(SUCCEEDED((*m_pD3DDevice)->BeginScene()))
	{
		// �|���S���̕`��
		for(int index = 0; index < MAX_CHAR_NUM; ++index)
		{
			(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff[index], 0, sizeof(VERTEX_2D));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
			(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);												// ���_�t�H�[�}�b�g�̐ݒ�
			(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);										// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
			(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP									// �v���~�e�B�u�̎��
											, 0													// �`����J�n���钸�_�ԍ�
											, DRAW_SQUARE_PRINITIV_NUM);						// ���������|���S����(�O�p�`��E�E�E�l�p�`���Ȃ�|���S�����Q�j
		}
		// �`��̏I��
		(*m_pD3DDevice)->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	(*m_pD3DDevice)->Present(NULL, NULL, NULL, NULL);
}
//----EOF----