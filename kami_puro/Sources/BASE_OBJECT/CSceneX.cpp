//=============================================================================
//
// CSceneXクラス [CSceneX.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneX.h"
#include "../MANAGER/CManager.h"
#include "../RENDERER/CRenderer.h"
#include "../CAMERA/CameraManager.h"
#include "../SHADER/CShader.h"

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
D3DXMATRIX CSceneX ::m_mtxWorld;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneX ::CSceneX(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType):CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_ModelInfo = NULL;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	ZeroMemory(m_cModelFileName, sizeof(m_cModelFileName));
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneX ::~CSceneX(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CSceneX::Init(D3DXVECTOR3& pos, char* modelFileName, CManager* pManager)
{
	// 初期化
	m_Pos = pos;
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// スケールは基本変えないのでこのまま固定。カメラが寄れば大きく見えるし、離れれば小さく見えるから
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	// モデル情報取得
	m_ModelInfo = CModel::GetModel(modelFileName);

	m_pManager = pManager;
}
//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CSceneX::Init(D3DXVECTOR3& pos, MODEL_TYPE type, CManager* pManager)
{
	// 初期化
	m_Pos = pos;
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// スケールは基本変えないのでこのまま固定。カメラが寄れば大きく見えるし、離れれば小さく見えるから
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	// モデル情報取得
	m_ModelInfo = CModel::GetModel(type);
	
	m_pManager = pManager;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneX ::Uninit(void)
{
	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneX ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneX::DrawNormalRender(void)
{
	D3DXMATERIAL		*pD3DXMat;
	D3DMATERIAL9		matDef;		// 元のマテリアル情報を保存しておく
	D3DXMATRIX			mtxScl,mtxRot,mtxTranslate, mtxWVP;
	
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映		*****順番注意*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);	// Y軸を基点に回転しているのでY,X,Zの順に入れる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// シェーダーの適用
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_MODEL);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_MODEL);

	PS_TYPE type = PS_TYPE_MODEL;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*_vs);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	HRESULT hr = 0;
	UINT texSampler = (*_psc)->GetSamplerIndex("texSampler");
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	UINT depthSampler = (*_psc)->GetSamplerIndex("depthSampler");
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	LPDIRECT3DTEXTURE9* pDepthTex = CRenderer::GetRenderTexture(CRenderer::TYPE_RENDER_TOON_OBJECT_DEPTH);
	hr = (*m_pD3DDevice)->SetTexture(depthSampler, *pDepthTex);

	D3DXMATRIX view, proj;
	CCameraManager* pCameraManager = m_pManager->GetCameraManager();
	view = pCameraManager->GetMtxView();
	proj = pCameraManager->GetMtxProj();
	mtxWVP = m_mtxWorld * view * proj;
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gWVP", &mtxWVP);

	// ライトから見たの
	view = pCameraManager->GetMtxLightView();
	proj = pCameraManager->GetMtxLightProj();
	mtxWVP = m_mtxWorld * view * proj;
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gLightWVP", &mtxWVP);


	// 頂点宣言したやつをセット(SetFVFの代わり)
	(*m_pD3DDevice)->SetVertexDeclaration(m_ModelInfo->m_pDecl);

	// マテリアルのバッファ取得
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// 現在のマテリアルの情報を取得
	(*m_pD3DDevice)->GetMaterial(&matDef);

	// マテリアルの描画
	for (int nCntMat = 0; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		LPDIRECT3DTEXTURE9 pTex = m_ModelInfo->pD3DTexBuff[nCntMat];
		hr = (*m_pD3DDevice)->SetTexture(texSampler, pTex);
		m_ModelInfo->pD3DXMeshModel->DrawSubset(nCntMat);					// モデルのパーツを描画
	}

	//*********注意:以下を必ず書くこと******************
	// 書かないとすべての色がおかしくなる
	(*m_pD3DDevice)->SetMaterial(&matDef);									// マテリアル情報を元に戻す
	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetTexture(depthSampler, NULL);
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, char* modelFileName, CManager* pManager)
{
	// 作成
	CSceneX* p = new CSceneX(pDevice);

	// 初期化
	p->Init(pos, modelFileName, pManager);

	p->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	return p;
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, MODEL_TYPE type, CManager* pManager)
{
	// 作成
	CSceneX* p = new CSceneX(pDevice);

	// 初期化
	p->Init(pos, type, pManager);

	p->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	return p;
}

//*****************************************************************************
// アルファ値変更関数
//*****************************************************************************
void CSceneX::SetAlpha(float alpha)
{
	D3DXMATERIAL *pD3DXMat;
	// マテリアルのバッファ取得
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// マテリアルのアルファ値変更
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Diffuse.a = alpha;
	}
}

//*****************************************************************************
// スペキュラー値変更関数
//*****************************************************************************
void CSceneX::SetSpecularPow(float pow)
{
	D3DXMATERIAL *pD3DXMat;
	// マテリアルのバッファ取得
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// マテリアルのアルファ値変更
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Power = pow;
	}
}

//*****************************************************************************
// エッジテクスチャー関数
//*****************************************************************************
void CSceneX::CreateEdgeTexture(void)
{
/*	// レンダーターゲット変更
	CRenderer::ChangeRenderTarget(CRenderer::TYPE_RENDER_EDGE);

	D3DXMATERIAL		*pD3DXMat;
	D3DMATERIAL9		matDef;		// 元のマテリアル情報を保存しておく
	D3DXMATRIX			mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映		*****順番注意*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);	// Y軸を基点に回転しているのでY,X,Zの順に入れる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// マテリアルのバッファ取得
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// 現在のマテリアルの情報を取得
	(*m_pD3DDevice)->GetMaterial(&matDef);

	// ライトベクトルのローカル化
	CLight* light = CLight::GetLightAdr(0);
	D3DXVECTOR3 lightDir = light->GetDir();
	D3DXVec3Normalize(&lightDir, &lightDir);
	D3DXVECTOR3 localLightVec = lightDir;
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, NULL, &m_mtxWorld);
	D3DXVec3TransformCoord(&localLightVec, &localLightVec, &invWorld);

	// シェーダーの適用
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_EDGE);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_EDGE);

	PS_TYPE type = PS_TYPE_EDGE;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*_vs);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	D3DXMATRIX view, proj;
	view = CCameraManager::GetMtxView();
	proj = CCameraManager::GetMtxProj();

	(*_vsc)->SetMatrix((*m_pD3DDevice), "gWorld", &m_mtxWorld);	// こいつらをあらかじめ掛け合わせてシェーダーに送るとコスト削減可
	(*_vsc)->SetMatrix((*m_pD3DDevice), "gView", &view);	// こいつらをあらかじめ掛け合わせてシェーダーに送るとコスト削減可
	(*_vsc)->SetMatrix((*m_pD3DDevice), "gProj", &proj);	// こいつらをあらかじめ掛け合わせてシェーダーに送るとコスト削減可

	// マテリアルの描画
	D3DXMATERIAL* mat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		m_ModelInfo->pD3DXMeshModel->DrawSubset(nCntMat);					// モデルのパーツを描画
	}

	//*********注意:以下を必ず書くこと******************
	// 書かないとすべての色がおかしくなる
	(*m_pD3DDevice)->SetMaterial(&matDef);									// マテリアル情報を元に戻す

	CRenderer::ChangeDefaultRenderTarget();
	*/
}

//*****************************************************************************
// シャドウテクスチャー関数
//*****************************************************************************
void CSceneX::CreateShadowTexture(void)
{/*
	// レンダーターゲット変更
	CRenderer::ChangeRenderTarget(CRenderer::TYPE_RENDER_SHADOW);

	D3DXMATERIAL		*pD3DXMat;
	D3DMATERIAL9		matDef;		// 元のマテリアル情報を保存しておく
	D3DXMATRIX			mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映		*****順番注意*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);	// Y軸を基点に回転しているのでY,X,Zの順に入れる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// マテリアルのバッファ取得
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// 現在のマテリアルの情報を取得
	(*m_pD3DDevice)->GetMaterial(&matDef);

	// ライトベクトルのローカル化
	CLight* light = CLight::GetLightAdr(0);
	D3DXVECTOR3 lightDir = light->GetDir();
	D3DXVec3Normalize(&lightDir, &lightDir);
	D3DXVECTOR3 localLightVec = lightDir;
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, NULL, &m_mtxWorld);
	D3DXVec3TransformCoord(&localLightVec, &localLightVec, &invWorld);

	// シェーダーの適用
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_SHADOW);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_SHADOW);

	PS_TYPE type = PS_TYPE_SHADOW;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*_vs);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	// 頂点宣言したやつをセット(SetFVFの代わり)
	(*m_pD3DDevice)->SetVertexDeclaration(m_ModelInfo->m_pDecl);

	D3DXMATRIX view, proj, lightMatrix;
	D3DXVECTOR3 cameraPos = m_Pos;// ここをライトからにすると照らしている方向からの影になる
	cameraPos.x -= 30.5f;		   // ここをライトからにすると照らしている方向からの影になる
	cameraPos.y += 30.5f;		   // ここをライトからにすると照らしている方向からの影になる
	cameraPos.z -= 5.5f;		   // ここをライトからにすると照らしている方向からの影になる
	D3DXVECTOR3 cameraPosR = m_Pos;
	D3DXVECTOR3 upVec(0.f, 0.f, -1.f);

	CCameraManager::SetOrtho(m_pD3DDevice, cameraPos, cameraPosR, upVec);
	proj = CCameraManager::GetMtxProjOrtho();
	view = CCameraManager::GetMtxViewOrtho();

	// ライトマトリクス作成
	lightMatrix = view * proj;
	light->SetLightMatrix(lightMatrix);

	HRESULT hr = 0;

	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gWorld", &m_mtxWorld);	// こいつらをあらかじめ掛け合わせてシェーダーに送るとコスト削減可
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gView", &view);	// こいつらをあらかじめ掛け合わせてシェーダーに送るとコスト削減可
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gProj", &proj);	// こいつらをあらかじめ掛け合わせてシェーダーに送るとコスト削減可
	
	// マテリアルの描画
	D3DXMATERIAL* mat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		m_ModelInfo->pD3DXMeshModel->DrawSubset(nCntMat);					// モデルのパーツを描画
	}

	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
	//*********注意:以下を必ず書くこと******************
	// 書かないとすべての色がおかしくなる
	(*m_pD3DDevice)->SetMaterial(&matDef);									// マテリアル情報を元に戻す

	CRenderer::ChangeDefaultRenderTarget();
	*/
}
//----EOF----