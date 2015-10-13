//=============================================================================
//
// CRenderer�N���X [CRenderer.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CRenderer.h"
#include "../SCENE/CSCENE/CScene2D.h"
#include "../DEBUG_PROC/CDebugProc.h"
#ifdef _DEBUG
#include "../INPUT/CInputKeyboard.h"
#endif

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float		POLYGON_WIDTH = 200.0f;
static const float		POLYGON_HEIGHT = 200.0f;
static const int		NUM_BACK_BUFFER = 1;									// �o�b�N�o�b�t�@�̐�
static const D3DXCOLOR	CLEAR_SCREEN_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ��ʃN���A�J���[

//*****************************************************************************
// static�����o�ϐ�
//*****************************************************************************
LPDIRECT3DDEVICE9 CRenderer ::m_pD3DDevice = NULL;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CRenderer ::CRenderer(void)
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CRenderer ::~CRenderer(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CRenderer :: Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount			= NUM_BACK_BUFFER;			// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= d3ddm.Format;		// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.hDeviceWindow				= hWnd;
	
	// �E�B���h�E���[�h
	if(bWindow)
	{
		d3dpp.FullScreen_RefreshRateInHz	= 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	// �t���X�N���[�����[�h
	else
	{
		d3dpp.FullScreen_RefreshRateInHz	= 60;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
								, D3DDEVTYPE_HAL
								, hWnd
								, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED
								, &d3dpp
								, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
									, D3DDEVTYPE_HAL
									, hWnd
									, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED
									, &d3dpp
									, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
										, D3DDEVTYPE_REF
										, hWnd
										, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED
										, &d3dpp
										, &m_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O������ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	// �A���t�@�e�X�g���s��
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CRenderer ::Uninit(void)
{
	// �f�o�C�X�I�u�W�F�N�g�̊J��
	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	
	// Direct3D�I�u�W�F�N�g�̊J��
	if(m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CRenderer ::Update()
{
	// �S�čX�V
	CScene::UpdateAll();
}

void CRenderer ::Update(int priority)
{
	// �w�肵�����̂̂ݍX�V
	CScene::UpdateChoice(priority);
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CRenderer ::Draw()
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), CLEAR_SCREEN_COLOR, 1.0f, 0);

	// �`��̊J�n
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// �S�ĕ`��
		CScene::DrawAll();
#ifdef _DEBUG
		// FPS�`��
		CDebugProc::Print("FPS:%d\n", m_nCountFPS);

		// �f�o�b�O���`��
		CDebugProc::Draw();
#endif
		// �`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

// �w�肵�����̂̂ݕ`��
void CRenderer ::Draw(int priority, int priority2, int priority3, int priority4, int priority5, int priority6, int priority7, int priority8)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), CLEAR_SCREEN_COLOR, 1.0f, 0);

	// �`��̊J�n
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
#ifdef _DEBUG
		// FPS�`��
		CDebugProc::Print("FPS:%d\n",m_nCountFPS);

		// �f�o�b�O���`��
		CDebugProc::Draw();

		// ���C���[�t���[���`��
		if(CInputKeyboard::GetKeyboardPress(DIK_1))
		{
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}

		// �\���b�h�`��
		if(CInputKeyboard::GetKeyboardPress(DIK_0))
		{
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
#endif
		// �w��`��
		CScene::DrawChoice(priority);
		if(priority2 > -1)
		{
			CScene::DrawChoice(priority2);
		}
		if(priority3 > -1)
		{
			CScene::DrawChoice(priority3);
		}
		if(priority4 > -1)
		{
			CScene::DrawChoice(priority4);
		}
		if(priority5 > -1)
		{
			CScene::DrawChoice(priority5);
		}
		if(priority6 > -1)
		{
			CScene::DrawChoice(priority6);
		}
		if(priority7 > -1)
		{
			CScene::DrawChoice(priority7);
		}
		if(priority8 > -1)
		{
			CScene::DrawChoice(priority8);
		}

		// �`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//----EOF----