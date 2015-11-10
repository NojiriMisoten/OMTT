//=============================================================================
//
// モデル処理 [CShader.h]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CShader.h"
#include "../CONST/const.h"

//*****************************************************************************
// 定数定義
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
// マクロ定義
//*****************************************************************************
static const int LENGTH_MAX = 256;	// 最大文字数

//*****************************************************************************
// スタティックメンバ変数宣言
//*****************************************************************************
LPDIRECT3DVERTEXSHADER9	CShader::m_vs[VS_TYPE_MAX] = { NULL };
LPD3DXCONSTANTTABLE		CShader::m_vsc[VS_TYPE_MAX] = { NULL };		// 定数レジスタのテーブル
LPDIRECT3DPIXELSHADER9	CShader::m_ps[PS_TYPE_MAX] = { NULL };
LPD3DXCONSTANTTABLE		CShader::m_psc[PS_TYPE_MAX] = { NULL };		// 定数レジスタのテーブル
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShader::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	// ---------ピクセルシェーダー部分----------
	HRESULT hr;
	LPD3DXBUFFER err;
	LPD3DXBUFFER code;

	for (int idx = 0; idx < PS_TYPE_MAX; ++idx)
	{
		hr = D3DXCompileShaderFromFile(PS_FILE_NAME[idx]	// シェーダーファイル名
			, NULL
			, NULL
			, "main"				// シェーダープログラムのエントリポイントの関数名
			, "ps_3_0"			// シェーダーバージョン
			, 0
			, &code				// コンパイル後のシェーダーバイナリ
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

	// ---------頂点シェーダー部分----------
	for (int idx = 0; idx < VS_TYPE_MAX; ++idx)
	{
		hr = D3DXCompileShaderFromFile(SHADER_FILE_NAME[idx]	// シェーダーファイル名
			, NULL
			, NULL
			, "main"				// シェーダープログラムのエントリポイントの関数名
			, "vs_3_0"			// シェーダーバージョン
			, 0
			, &code				// コンパイル後のシェーダーバイナリ
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
// 終了処理
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
// モデル作成処理
//=============================================================================
void CShader::Create(LPDIRECT3DDEVICE9 *pDevice)
{
	CShader::Init(pDevice);
}

//----EOF----