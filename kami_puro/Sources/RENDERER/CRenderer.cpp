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
#include "../BASE_OBJECT/CScene.h"
#include "../BASE_OBJECT/CScene2D.h"
#include "../SHADER/CShader.h"
#include "../DEBUG_PROC/CDebugProc.h"

#ifdef _DEBUG
#include "../INPUT/CInputKeyboard.h"
#endif

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const int		NUM_BACK_BUFFER = 1;									// �o�b�N�o�b�t�@�̐�
static const D3DXCOLOR	CLEAR_SCREEN_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// ��ʃN���A�J���[
static const int		GAUSS_WEIGHT_NUM = 5;									// �K�E�X�̏d�݂̐�
static const int		DEFAULT_RENDER_TEXTURE_SIZE = 512;
//*****************************************************************************
// static�����o�ϐ�
//*****************************************************************************
LPDIRECT3DDEVICE9				CRenderer ::m_pD3DDevice = NULL;
LPDIRECT3DTEXTURE9				CRenderer::m_pRenderTexture[TYPE_RENDER_MAX] = {NULL};
LPDIRECT3DSURFACE9				CRenderer::m_pRenderSurface[TYPE_RENDER_MAX] = {NULL};
LPDIRECT3DSURFACE9				CRenderer::m_pRenderSurfaceDepth[TYPE_RENDER_MAX] = {NULL};
LPDIRECT3DSURFACE9				CRenderer::m_pDefaultSurface = NULL;
LPDIRECT3DSURFACE9				CRenderer::m_pDefaultSurfaceDepth = NULL;
LPDIRECT3DSURFACE9				CRenderer::m_pKeepSurface = NULL;
LPDIRECT3DSURFACE9				CRenderer::m_pKeepSurfaceDepth = NULL;
LPDIRECT3DTEXTURE9				CRenderer::m_pKeepTexture = NULL;
D3DVIEWPORT9					CRenderer::m_DefaultViewPort;
D3DVIEWPORT9					CRenderer::m_ChangeViewPort[TYPE_RENDER_MAX];
LPDIRECT3DVERTEXDECLARATION9	CRenderer::m_pDecl = NULL;
bool							CRenderer::m_bBeginScene = false;
VF								CRenderer::m_Vertex[4] = 
{
	D3DXVECTOR3(-2.0f, -2.0f, 0.f), D3DXVECTOR3(0, 1, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0.f, 0.f),
	D3DXVECTOR3((float)SCREEN_WIDTH + 0.5f, -2.0f, 0.f), D3DXVECTOR3(0, 1, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1.f, 0.f),
	D3DXVECTOR3(-2.0f, (float)SCREEN_HEIGHT + 0.5f, 0.f), D3DXVECTOR3(0, 1, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0.f, 1.f),
	D3DXVECTOR3((float)SCREEN_WIDTH + 0.5f, (float)SCREEN_HEIGHT + 0.5f, 0.f), D3DXVECTOR3(0, 1, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1.f, 1.f),
};
float							CRenderer::m_fCameraRot = 0.f;
D3DXMATRIX						CRenderer::m_ProjMtx;
D3DXMATRIX						CRenderer::m_ViewMtx;
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CRenderer ::CRenderer(void)
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_bBeginScene = false;
	m_fCameraRot = 0.f;
	D3DXMatrixIdentity(&m_ProjMtx);
	D3DXMatrixIdentity(&m_ViewMtx);
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
	D3DPRESENT_PARAMETERS d3dpp = {};
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
	d3dpp.hDeviceWindow				= hWnd;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.BackBufferCount			= NUM_BACK_BUFFER;			// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= D3DFMT_A8R8G8B8;			// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	// �E�B���h�E���[�h
	if(bWindow)
	{
		d3dpp.FullScreen_RefreshRateInHz	= 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	// �t���X�N���[�����[�h
	else
	{
		d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
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

	SetDefaultSamplerState();

	// �o�b�N�o�b�t�@�T�[�t�F�X�̎擾
	m_pD3DDevice->GetRenderTarget(0, &m_pDefaultSurface);
	m_pD3DDevice->GetDepthStencilSurface(&m_pDefaultSurfaceDepth);

	// �r���[�|�[�g�̎擾
	m_pD3DDevice->GetViewport(&m_DefaultViewPort);

	////==== �����_�[�^�[�Q�b�g�p ====
	for (int i = 0; i < TYPE_RENDER_MAX; ++i)
	{
		int width = DEFAULT_RENDER_TEXTURE_SIZE;
		int height = DEFAULT_RENDER_TEXTURE_SIZE;
		
		// �f�o�C�X�̐[�x�o�b�t�@�̕��ƍ������擾
		UINT uiDevZBufWidth;    // �f�o�C�X�o�b�t�@��
		UINT uiDevZBufHeight;   // �f�o�C�X�o�b�t�@��
		IDirect3DSurface9 *pTmpSf;
		m_pD3DDevice->GetDepthStencilSurface(&pTmpSf);
		D3DSURFACE_DESC SufDesc;
		pTmpSf->GetDesc(&SufDesc);
		width = uiDevZBufWidth = SufDesc.Width;
		height = uiDevZBufHeight = SufDesc.Height;
		pTmpSf->Release();
		
		if (i == TYPE_RENDER_HALF_SIDE || i == TYPE_RENDER_HALF_GAUS)
		{
			width = (int)(width * 0.5f);
			height = (int)(height * 0.5f);
		}
		else if (i == TYPE_RENDER_QUARTER_SIDE || i == TYPE_RENDER_QUARTER_GAUS)
		{
			width = (int)(width * 0.25f);
			height = (int)(height * 0.25f);
		}

		if (FAILED(m_pD3DDevice->CreateTexture(width					// ��
											, height					// ����
											, 1						// �~�b�v�}�b�v��
											, D3DUSAGE_RENDERTARGET	// �g�p���@
											, D3DFMT_A8R8G8B8		// �t�H�[�}�b�g
											, D3DPOOL_DEFAULT		// �������Ǘ����@
											, &m_pRenderTexture[i]	// �e�N�X�`���C���^�[�t�F�[�X�ւ̃|�C���^
											, NULL)))
		{
			MessageBox(NULL, "�����_�[�^�[�Q�b�g�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK | MB_ICONERROR);
			m_pRenderTexture[i] = NULL;
			return false;
		}
		
		// �T�[�t�F�X�̃e�N�X�`��
		m_pRenderTexture[i]->GetSurfaceLevel(0, &m_pRenderSurface[i]);
	
		// �f�o�C�X�̐[�x�o�b�t�@�̕��ƍ������擾
		m_pD3DDevice->GetDepthStencilSurface(&pTmpSf);
		pTmpSf->GetDesc(&SufDesc);
		width = uiDevZBufWidth = SufDesc.Width;
		height = uiDevZBufHeight = SufDesc.Height;
		pTmpSf->Release();

		// Z�o�b�t�@�A�X�e���V���o�b�t�@�̍쐬
		if (FAILED(m_pD3DDevice->CreateDepthStencilSurface(width
														, height
														, D3DFMT_D24S8
														, D3DMULTISAMPLE_NONE
														, 0
														, FALSE
														, &m_pRenderSurfaceDepth[i]
														, NULL)))
		{
			MessageBox(NULL, "�����_�[�^�[�Q�b�g�T�[�t�F�X���s���܂���\n", "�G���[", MB_OK | MB_ICONERROR);
			m_pRenderSurfaceDepth[i] = NULL;
			return false;
		}
		m_pD3DDevice->SetRenderTarget(0, m_pRenderSurface[i]);
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
		
		// �r���[�|�[�g�̎擾
		m_pD3DDevice->GetViewport(&m_ChangeViewPort[i]);
	}
		
	m_pD3DDevice->SetRenderTarget(0, m_pDefaultSurface);
	m_pD3DDevice->SetDepthStencilSurface(m_pDefaultSurfaceDepth);

	// ���_�錾�������̂����
	m_pD3DDevice->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	m_pD3D->Release();
	m_pD3D = NULL;

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

	for (int i = 0; i < TYPE_RENDER_MAX; ++i)
	{
		if (m_pRenderSurface[i])
		{
			m_pRenderSurface[i]->Release();
			m_pRenderSurface[i] = NULL;
		}

		if (m_pRenderSurfaceDepth[i])
		{
			m_pRenderSurfaceDepth[i]->Release();
			m_pRenderSurfaceDepth[i] = NULL;
		}

		if (m_pRenderTexture[i])
		{
			m_pRenderTexture[i]->Release();
			m_pRenderTexture[i] = NULL;
		}
	}

	if (m_pDefaultSurface)
	{
		m_pDefaultSurface->Release();
		m_pDefaultSurface = NULL;
	}

	if (m_pDefaultSurfaceDepth)
	{
		m_pDefaultSurfaceDepth->Release();
		m_pDefaultSurfaceDepth = NULL;
	}

	if (m_pDecl)
	{
		m_pDecl->Release();
		m_pDecl = NULL;
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
	ChangeDefaultRenderTarget();
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), CLEAR_SCREEN_COLOR, 1.0f, 0);
	for (int i = 0; i < TYPE_RENDER_MAX; ++i)
	{
		TYPE_RENDER_TEX render = (TYPE_RENDER_TEX)i;
		ClearRenderTarget(render);
	}

	// �`��̊J�n
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		m_bBeginScene = true;
	
		// �S�ĕ`��
		CScene::DrawAll();

		// �g�D�[���`��
		//DrawToon();

		// ���C�g�u���[��
		//DrawLightBloom();

		// �o�b�N�o�b�t�@�[�ւ̕`��
		DrawBackBuffer();

#ifdef _DEBUG
		// FPS�`��
		CDebugProc::PrintU("FPS:%d\n", m_nCountFPS);

		// �f�o�b�O���`��
		CDebugProc::Draw();
#endif
	
		// �`��̏I��
		m_pD3DDevice->EndScene();
		m_bBeginScene = false;
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//*****************************************************************************
// �����_�[�^�[�Q�b�g�ύX
//*****************************************************************************
void CRenderer::ChangeRenderTarget(TYPE_RENDER_TEX idx)
{
	if (m_bBeginScene)
	{
		m_pD3DDevice->EndScene();
	}

	HRESULT hr;
	hr = m_pD3DDevice->SetRenderTarget(0, m_pRenderSurface[(int)idx]);
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pRenderSurfaceDepth[(int)idx]);
	hr = m_pD3DDevice->SetViewport(&m_ChangeViewPort[idx]);
	
	if (m_bBeginScene)
	{
		m_pD3DDevice->BeginScene();
	}
}

//*****************************************************************************
// ���̃����_�[�^�[�Q�b�g�ɕύX
//*****************************************************************************
void CRenderer::ChangeDefaultRenderTarget(void)
{
	if (m_bBeginScene)
	{
		m_pD3DDevice->EndScene();
	}

	HRESULT hr;
	hr = m_pD3DDevice->SetRenderTarget(0, m_pDefaultSurface);
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pDefaultSurfaceDepth);
	hr = m_pD3DDevice->SetViewport(&m_DefaultViewPort);

	if (m_bBeginScene)
	{
		m_pD3DDevice->BeginScene();
	}
}

//*****************************************************************************
// �����_�[�e�N�X�`���[�Q�b�g
//*****************************************************************************
LPDIRECT3DTEXTURE9* CRenderer::GetRenderTexture(TYPE_RENDER_TEX idx)
{
	return &m_pRenderTexture[(int)idx];
}

//*****************************************************************************
// �����_�[�^�[�Q�b�g�̃N���A
//*****************************************************************************
void CRenderer::ClearRenderTarget(TYPE_RENDER_TEX idx)
{
	m_pD3DDevice->SetRenderTarget(0, m_pRenderSurface[(int)idx]);
	m_pD3DDevice->SetDepthStencilSurface(m_pRenderSurfaceDepth[(int)idx]);
	m_pD3DDevice->SetViewport(&m_ChangeViewPort[idx]);

	// �e�N�X�`���T�[�t�F�C�X�̃N���A
	if (idx == TYPE_RENDER_NORMAL_VEC)
	{
		m_pD3DDevice->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			1.0f,
			0);
	}
	else if (idx == TYPE_RENDER_TOON_OBJECT_DEPTH)
	{
		m_pD3DDevice->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(255, 255, 255, 255),
			1.0f,
			0);
	}
	else
	{
		m_pD3DDevice->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(255, 0, 0, 0),
			1.0f,
			0);
	}
	ChangeDefaultRenderTarget();
}

//*****************************************************************************
// �o�b�N�o�b�t�@�[�ւ̕`��
//*****************************************************************************
void CRenderer::DrawBackBuffer(void)
{
	ChangeDefaultRenderTarget();

	D3DXMATRIX world;	// ���[���h�}�g���N�X
	// ���[���h�}�g���N�X�쐬
	D3DXMatrixIdentity(&world);

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_BACK_BUFFER);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_BACK_BUFFER);		// �萔���W�X�^�̃e�[�u��
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// �萔���W�X�^�̃e�[�u��
	HRESULT hr = 0;

	// �V�F�[�_�[�̓K�p
	hr = m_pD3DDevice->SetVertexShader(*_vs);
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	LPDIRECT3DTEXTURE9* tex;
	UINT toonSamplerIndex = (*_psc)->GetSamplerIndex("toonRenderSampler");
	UINT lightBloomSamplerIndex = (*_psc)->GetSamplerIndex("lightBloomRenderSampler");
	UINT fadeSamplerIndex = (*_psc)->GetSamplerIndex("fadeRenderSampler");

	// ���ʂ�
	tex = GetRenderTexture(TYPE_RENDER_TOON);
	hr = m_pD3DDevice->SetTexture(toonSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(toonSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(toonSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(toonSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(toonSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	// ���C�g�u���[����
	tex = GetRenderTexture(TYPE_RENDER_LIGHT_BLOOM);
	hr = m_pD3DDevice->SetTexture(lightBloomSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(lightBloomSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(lightBloomSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(lightBloomSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(lightBloomSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �t�F�[�h��
	tex = GetRenderTexture(TYPE_RENDER_FADE);
	hr = m_pD3DDevice->SetTexture(fadeSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(fadeSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(fadeSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(fadeSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(fadeSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ���_�V�F�[�_�[�̒萔���W�X�^�ɒl���Z�b�g
	D3DXMATRIX view, proj, screenProj;

	screenProj = SCREEN_PROJ;
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &screenProj);

	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &screenProj);

	// ���_�錾��������Z�b�g(SetFVF�̑���)
	m_pD3DDevice->SetVertexDeclaration(m_pDecl);

	// �`��
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, DRAW_SQUARE_PRINITIV_NUM, m_Vertex, sizeof(VF));

	hr = m_pD3DDevice->SetTexture(toonSamplerIndex, NULL);
	hr = m_pD3DDevice->SetTexture(fadeSamplerIndex, NULL);
	hr = m_pD3DDevice->SetTexture(lightBloomSamplerIndex, NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
	hr = m_pD3DDevice->SetPixelShader(NULL);
}

//*****************************************************************************
// �g�D�[���ɂ���
//*****************************************************************************
void CRenderer::DrawToon(void)
{
	// �����_�[�^�[�Q�b�g�ύX
	ChangeRenderTarget(TYPE_RENDER_TOON);

	D3DXMATRIX world;	// ���[���h�}�g���N�X
	// ���[���h�}�g���N�X�쐬
	D3DXMatrixIdentity(&world);

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_TOON);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_TOON);		// �萔���W�X�^�̃e�[�u��
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// �萔���W�X�^�̃e�[�u��
	HRESULT hr = 0;

	// �V�F�[�_�[�̓K�p
	hr = m_pD3DDevice->SetVertexShader(*_vs);
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	LPDIRECT3DTEXTURE9* tex;
	UINT normalTexSamplerIndex = (*_psc)->GetSamplerIndex("normalRendereTexSampler");
	UINT normalVecTexSamplerIndex = (*_psc)->GetSamplerIndex("normalVecTexSampler");

	// ���ʂ�
	tex = GetRenderTexture(TYPE_RENDER_NORMAL);
	hr = m_pD3DDevice->SetTexture(normalTexSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(normalTexSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(normalTexSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(normalTexSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(normalTexSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �G�b�W��
	tex = GetRenderTexture(TYPE_RENDER_NORMAL_VEC);
	hr = m_pD3DDevice->SetTexture(normalVecTexSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(normalVecTexSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(normalVecTexSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(normalVecTexSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(normalVecTexSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ���_�V�F�[�_�[�̒萔���W�X�^�ɒl���Z�b�g
	D3DXMATRIX view, proj, screenProj;

	// ���W�ϊ��p�p�����[�^
	screenProj = SCREEN_PROJ;
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &screenProj);
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &screenProj);

	// �𑜓x
	float resolution[2] = { DEFAULT_RENDER_TEXTURE_SIZE, DEFAULT_RENDER_TEXTURE_SIZE };
	(*_psc)->SetFloatArray(m_pD3DDevice, "g_ScreenResolution", &resolution[0], 2);

	// ���_�錾��������Z�b�g(SetFVF�̑���)
	m_pD3DDevice->SetVertexDeclaration(m_pDecl);

	// �`��
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, DRAW_SQUARE_PRINITIV_NUM, m_Vertex, sizeof(VF));

	hr = m_pD3DDevice->SetTexture(normalTexSamplerIndex, NULL);
	hr = m_pD3DDevice->SetTexture(normalVecTexSamplerIndex, NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
	hr = m_pD3DDevice->SetPixelShader(NULL);
}

//================================================
// �K�E�X
//================================================
void CRenderer::DrawGauss(TYPE_RENDER_TEX orijinTex, TYPE_RENDER_TEX sideRender, TYPE_RENDER_TEX gausRender)
{
	D3DXMATRIX world;	// ���[���h�}�g���N�X
	// ���[���h�}�g���N�X�쐬
	D3DXMatrixIdentity(&world);

	float total = 0.f;
	float weightTbl[GAUSS_WEIGHT_NUM] = { 0.f };
	float dispersion = 100.f;		// �K���Ȑ��l 0�֎~

	// �K�E�X�̏d�݂̌v�Z
	for (int i = 0; i < GAUSS_WEIGHT_NUM; ++i)
	{
		weightTbl[i] = expf(-0.5f * (i * i) / dispersion);
		if (i == 0)
		{
			total += weightTbl[i];
		}
		else
		{
			total += 2.0f * weightTbl[i];
		}
	}
	for (int i = 0; i < GAUSS_WEIGHT_NUM; ++i)
	{
		weightTbl[i] /= total;
	}

	// �����_�[�^�[�Q�b�g�ύX
	ChangeRenderTarget(sideRender);

	HRESULT hr;
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// �萔���W�X�^�̃e�[�u��
	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_GAUSS_SIDE);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_GAUSS_SIDE);		// �萔���W�X�^�̃e�[�u��
	// �V�F�[�_�[�̓K�p
	UINT texSamplerIndex = (*_psc)->GetSamplerIndex("texSampler");
	hr = m_pD3DDevice->SetPixelShader(*_ps);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	// ����
	LPDIRECT3DTEXTURE9* tex = GetRenderTexture(orijinTex);
	hr = m_pD3DDevice->SetTexture(texSamplerIndex, *tex);

	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &SCREEN_PROJ);
	float resolution = 1.0f / SCREEN_WIDTH;
	hr = (*_psc)->SetFloat(m_pD3DDevice, "g_Resolution", resolution);
	hr = (*_psc)->SetFloat(m_pD3DDevice, "g_WeihtTabl_Zero", weightTbl[0]);
	hr = (*_psc)->SetFloatArray(m_pD3DDevice, "g_WeightTabl_One_Four", &weightTbl[1], 4);

	// �V�F�[�_�[�̓K�p
	hr = m_pD3DDevice->SetVertexShader(*_vs);

	// �`��
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(VF));

	// ���ɖ߂�
	m_pD3DDevice->SetTexture(texSamplerIndex, NULL);
	m_pD3DDevice->SetPixelShader(NULL);

	// �����_�[�^�[�Q�b�g�ύX
	ChangeRenderTarget(gausRender);

	_ps = CShader::GetPS(PS_TYPE_GAUSS_VERTICAL);
	_psc = CShader::GetPSC(PS_TYPE_GAUSS_VERTICAL);		// �萔���W�X�^�̃e�[�u��
	texSamplerIndex = (*_psc)->GetSamplerIndex("texSampler");
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �K�E�X�|�������
	tex = GetRenderTexture(sideRender);
	hr = m_pD3DDevice->SetTexture(texSamplerIndex, *tex);

	// �R���X�^���g�e�[�u���ɒl����
	resolution = 1.0f / SCREEN_HEIGHT;
	hr = (*_psc)->SetFloat(m_pD3DDevice, "g_Resolution", resolution);
	hr = (*_psc)->SetFloat(m_pD3DDevice, "g_WeihtTabl_Zero", weightTbl[0]);
	hr = (*_psc)->SetFloatArray(m_pD3DDevice, "g_WeightTabl_One_Four", &weightTbl[1], 4);

	// �V�F�[�_�[�̓K�p
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	// �`��
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(VF));

	// ���ɖ߂�
	m_pD3DDevice->SetTexture(texSamplerIndex, NULL);
	m_pD3DDevice->SetPixelShader(NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
}

//================================================
// ���P�x���o
//================================================
void CRenderer::DrawBrightness(void)
{
	ChangeRenderTarget(TYPE_RENDER_HIGH_LUMINANCE);

	D3DXMATRIX world;	// ���[���h�}�g���N�X
	// ���[���h�}�g���N�X�쐬
	D3DXMatrixIdentity(&world);

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_HIGH_LUMINANCE);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_HIGH_LUMINANCE);		// �萔���W�X�^�̃e�[�u��
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// �萔���W�X�^�̃e�[�u��
	HRESULT hr = 0;

	// �V�F�[�_�[�̓K�p
	hr = m_pD3DDevice->SetVertexShader(*_vs);
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	LPDIRECT3DTEXTURE9* tex;
	UINT texSamplerIndex = (*_psc)->GetSamplerIndex("texSampler");
	// ���ʂ�
	tex = GetRenderTexture(TYPE_RENDER_BASE_HIGH_LUMINANCE);
	hr = m_pD3DDevice->SetTexture(texSamplerIndex, *tex);

	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ���_�V�F�[�_�[�̒萔���W�X�^�ɒl���Z�b�g
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &SCREEN_PROJ);

	float resolution[2] = { SCREEN_WIDTH, SCREEN_HEIGHT };
	hr = (*_psc)->SetFloatArray(m_pD3DDevice, "g_ScreenResolution", &resolution[0], 2);

	// ���_�錾��������Z�b�g(SetFVF�̑���)
	m_pD3DDevice->SetVertexDeclaration(m_pDecl);

	// �`��
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(VF));

	hr = m_pD3DDevice->SetTexture(texSamplerIndex, NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
	hr = m_pD3DDevice->SetPixelShader(NULL);
}

//================================================
// ���C�g�u���[��
//================================================
void CRenderer::DrawLightBloom(void)
{
	// ���P�x���o
	DrawBrightness();

	// 1/2�T�C�Y�K�E�X
	DrawGauss(TYPE_RENDER_HIGH_LUMINANCE, TYPE_RENDER_HALF_SIDE, TYPE_RENDER_HALF_GAUS);

	// 1/4�T�C�Y�K�E�X
	DrawGauss(TYPE_RENDER_HALF_GAUS, TYPE_RENDER_QUARTER_SIDE, TYPE_RENDER_QUARTER_GAUS);

	// �����_�[�^�[�Q�b�g�ύX
	ChangeRenderTarget(CRenderer::TYPE_RENDER_LIGHT_BLOOM);

	D3DXMATRIX world;	// ���[���h�}�g���N�X
	// ���[���h�}�g���N�X�쐬
	D3DXMatrixIdentity(&world);

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_LIGHT_BLOOM);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_LIGHT_BLOOM);		// �萔���W�X�^�̃e�[�u��
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// �萔���W�X�^�̃e�[�u��
	HRESULT hr = 0;

	// �V�F�[�_�[�̓K�p
	hr = m_pD3DDevice->SetVertexShader(*_vs);
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	LPDIRECT3DTEXTURE9* tex;
	UINT gausSamplerIndex = (*_psc)->GetSamplerIndex("gausSampler");
	
	// �K�E�X�̂��
	tex = GetRenderTexture(TYPE_RENDER_QUARTER_GAUS);
	hr = m_pD3DDevice->SetTexture(gausSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(gausSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(gausSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(gausSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(gausSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ���_�V�F�[�_�[�̒萔���W�X�^�ɒl���Z�b�g
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);	// ����������炩���ߊ|�����킹�ăV�F�[�_�[�ɑ���ƃR�X�g�팸��
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &SCREEN_PROJ);

	float resolution[2] = { SCREEN_WIDTH, SCREEN_HEIGHT };
	hr = (*_psc)->SetFloatArray(m_pD3DDevice, "g_ScreenResolution", &resolution[0], 2);

	// ���_�錾��������Z�b�g(SetFVF�̑���)
	m_pD3DDevice->SetVertexDeclaration(m_pDecl);

	// �`��
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(VF));

	hr = m_pD3DDevice->SetTexture(gausSamplerIndex, NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
	hr = m_pD3DDevice->SetPixelShader(NULL);
}

//================================================
// �����_�[�^�[�X�e�[�g�̃f�t�H���g�ݒ�
//================================================
void CRenderer::SetDefaultSamplerState(void)
{
	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O������ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	// �A���t�@�e�X�g���s��
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}
//----EOF----