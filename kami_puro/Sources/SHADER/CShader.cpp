//=============================================================================
//
// ���f������ [CShader.h]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CShader.h"
#include "../CONST/const.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
static const char *SHADER_FILE_NAME[VS_TYPE_MAX] =
{
	"../data/SHADER/Draw2DVS.hlsl",
	"../data/SHADER/NormalRendereVS.hlsl",
	"../data/SHADER/NormalVecRendereVS.hlsl",
	"../data/SHADER/ToonObjectDepthRendereVS.hlsl",
	"../data/SHADER/NormalRendereSkinMeshVS.hlsl",
	"../data/SHADER/NormalVecRendereSkinMeshVS.hlsl",
	"../data/SHADER/ToonObjectDepthRendereSkinMeshVS.hlsl",
	"../data/SHADER/FieldVS.hlsl",
	"../data/SHADER/DrawModelVS.hlsl",
	"../data/SHADER/PointLightVS.hlsl",
	"../data/SHADER/DrawTexVS.hlsl",
};

static const char *PS_FILE_NAME[PS_TYPE_MAX] =
{
	"../data/SHADER/BackBufferPS.hlsl",
	"../data/SHADER/NormalRenderePS.hlsl",
	"../data/SHADER/NormalVecRenderePS.hlsl",
	"../data/SHADER/ToonPS.hlsl",
	"../data/SHADER/ToonObjectDepthRenderePS.hlsl", 
	"../data/SHADER/FieldPS.hlsl",
	"../data/SHADER/GaussPS_Side.hlsl",
	"../data/SHADER/GaussPS_Vertical.hlsl",
	"../data/SHADER/HighLuminancePS.hlsl",
	"../data/SHADER/LightBloomPS.hlsl",
	"../data/SHADER/DrawModelPS.hlsl",
	"../data/SHADER/PointLightPS.hlsl",
	"../data/SHADER/DrawTexPS.hlsl",
};

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const int LENGTH_MAX = 256;	// �ő啶����

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DVERTEXSHADER9	CShader::m_vs[VS_TYPE_MAX] = { NULL };
LPD3DXCONSTANTTABLE		CShader::m_vsc[VS_TYPE_MAX] = { NULL };		// �萔���W�X�^�̃e�[�u��
LPDIRECT3DPIXELSHADER9	CShader::m_ps[PS_TYPE_MAX] = { NULL };
LPD3DXCONSTANTTABLE		CShader::m_psc[PS_TYPE_MAX] = { NULL };		// �萔���W�X�^�̃e�[�u��
//=============================================================================
// ����������
//=============================================================================
HRESULT CShader::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	// ---------�s�N�Z���V�F�[�_�[����----------
	HRESULT hr;
	LPD3DXBUFFER err;
	LPD3DXBUFFER code;

	for (int idx = 0; idx < PS_TYPE_MAX; ++idx)
	{
		hr = D3DXCompileShaderFromFile(PS_FILE_NAME[idx]	// �V�F�[�_�[�t�@�C����
			, NULL
			, NULL
			, "main"				// �V�F�[�_�[�v���O�����̃G���g���|�C���g�̊֐���
			, "ps_3_0"			// �V�F�[�_�[�o�[�W����
			, 0
			, &code				// �R���p�C����̃V�F�[�_�[�o�C�i��
			, &err
			, &m_psc[idx]);
		if (FAILED(hr))
		{
			MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile", MB_OK);
			err->Release();
			return false;
		}

		hr = (*pDevice)->CreatePixelShader((DWORD*)code->GetBufferPointer(), &m_ps[idx]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "FAILED", "CreatePixelShader", MB_OK);
			return false;
		}
	}

	//----------------------------------------------------------------------------

	// ---------���_�V�F�[�_�[����----------
	for (int idx = 0; idx < VS_TYPE_MAX; ++idx)
	{
		hr = D3DXCompileShaderFromFile(SHADER_FILE_NAME[idx]	// �V�F�[�_�[�t�@�C����
			, NULL
			, NULL
			, "main"				// �V�F�[�_�[�v���O�����̃G���g���|�C���g�̊֐���
			, "vs_3_0"			// �V�F�[�_�[�o�[�W����
			, 0
			, &code				// �R���p�C����̃V�F�[�_�[�o�C�i��
			, &err
			, &m_vsc[idx]);
		if (FAILED(hr))
		{
			MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile", MB_OK);
			err->Release();
			return false;
		}

		hr = (*pDevice)->CreateVertexShader((DWORD*)code->GetBufferPointer(), &m_vs[idx]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "FAILED", "CreateVertexShader", MB_OK);
			return false;
		}
	}
	//---------------------------------------------------------------------

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShader::Uninit(void)
{
	for (int i = 0; i < VS_TYPE_MAX; ++i)
	{
		SAFE_RELEASE(m_vs[i]);
		SAFE_RELEASE(m_vsc[i]);
	}
	for (int i = 0; i < PS_TYPE_MAX; ++i)
	{
		SAFE_RELEASE(m_ps[i]);
		SAFE_RELEASE(m_psc[i]);
	}
}

//=============================================================================
// ���f���쐬����
//=============================================================================
void CShader::Create(LPDIRECT3DDEVICE9 *pDevice)
{
	CShader::Init(pDevice);
}

//----EOF----