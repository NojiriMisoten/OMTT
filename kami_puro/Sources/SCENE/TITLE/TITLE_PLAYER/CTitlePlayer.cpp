//=============================================================================
//
// CTitlePlayerクラス [CTitlePlayer.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CTitlePlayer.h"
#include "../../../INPUT/CInputKeyboard.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../MANAGER/CManager.h"
#include "../../../SHADER/CShader.h"
#include "../../../SKINMESH/CSkinMeshHolder.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTitlePlayer::CTitlePlayer(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType) : CPlayer(pDevice, m_objType)
{
	// 姿勢ベクトル
	m_vecFront = DEFAULT_FRONT_VECTOR;
	m_vecRight = DEFAULT_RIGHT_VECTOR;
	m_vecUp = DEFAULT_UP_VECTOR;

	// 移動初期化
	m_MovPos = D3DXVECTOR3(0, 0, 0);

	m_pManager = NULL;

	m_ID = -1;
	m_AnimState = PLAYER_WAIT;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTitlePlayer::~CTitlePlayer(void)
{

}

//*****************************************************************************
// 作成
//*****************************************************************************
CTitlePlayer* CTitlePlayer::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID)
{
	CTitlePlayer* p = new CTitlePlayer(pDevice);

	p->Init(pDevice, pos, rot, type, pManager, ID);

	return p;
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CTitlePlayer::Init(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID)
{
	m_DestPos = m_Pos = pos;
	m_pManager = pManager;
	m_Rot = rot;

	// アニメ
	m_AnimState = PLAYER_WAIT;

	// ID
	m_ID = ID;

	// スキンメッシュモデル読み込み
	SKIN_MESH_INFO skinMeshInfo = (SKIN_MESH_INFO)m_ID;
	m_pCSkinMesh = CSkinMeshHolder::GetSkinMesh(skinMeshInfo);

	// 姿勢ベクトル
	m_vecFront = DEFAULT_FRONT_VECTOR;
	m_vecRight = DEFAULT_RIGHT_VECTOR;
	m_vecUp = DEFAULT_UP_VECTOR;

	// リンクに追加
	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// スケール
	m_vScl = D3DXVECTOR3(50, 50, 50);

	// ここで更新してからじゃないとアニメーション変えられないのでしている
	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);
	SetAnimType(PLAYER_WAIT);
	SetAnimMortionOfTime(0);
	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);

	m_DrawMode = DRAW_MODE_GRAY;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CTitlePlayer::Uninit(void)
{
	CPlayer::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CTitlePlayer::Update(void)
{
	// フロントベクトルの設定
	m_vecFront.x = -sinf(-m_Rot.y);
	m_vecFront.z = -cosf(m_Rot.y - D3DX_PI);

	// ライトベクトルの設定
	m_vecRight.x = cosf(m_Rot.y - D3DX_PI);
	m_vecRight.z = sinf(m_Rot.y);

	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);
}

//*****************************************************************************
// 描画
//*****************************************************************************
void CTitlePlayer::DrawNormalRender(void)
{
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

	// シェーダーの適用
	m_pVS = CShader::GetVS(VS_TYPE_NORMAL_RENDERER_SKINMESH);
	m_pVSC = CShader::GetVSC(VS_TYPE_NORMAL_RENDERER_SKINMESH);

	PS_TYPE type = PS_TYPE_TITLE_PLAYER;

	m_pPS = CShader::GetPS(type);
	m_pPSC = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*m_pVS);
	(*m_pD3DDevice)->SetPixelShader(*m_pPS);

	HRESULT hr = 0;

	UINT texSampler = (*m_pPSC)->GetSamplerIndex("texSampler");
	UINT toonSampler = (*m_pPSC)->GetSamplerIndex("toonSampler");
	
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	// カメラベクトルセット
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, NULL, &m_mtxWorld);
	D3DXVECTOR3 cameraPos = m_pManager->GetCameraManager()->GetCameraPos();
	D3DXVECTOR3 cameraPosR = m_pManager->GetCameraManager()->GetPosRCamera();
	D3DXVECTOR3 cameraVec = cameraPosR - cameraPos;
	D3DXVec3Normalize(&cameraVec, &cameraVec);
	// カメラベクトルのローカル化(法線がローカルなので)
	D3DXVECTOR3 localCameraVec = cameraVec;
	D3DXVec3TransformCoord(&localCameraVec, &localCameraVec, &invWorld);
	(*m_pVSC)->SetFloatArray((*m_pD3DDevice), "gCameraDir", (float*)&localCameraVec, 3);

	D3DXVECTOR3 grayScale(0.298912f, 0.586611f, 0.114478f);
	(*m_pPSC)->SetFloatArray((*m_pD3DDevice), "gMonochromeScale", (float*)&grayScale, 3);
	(*m_pPSC)->SetInt((*m_pD3DDevice), "gMode", m_DrawMode);
	// 頂点宣言したやつをセット(SetFVFの代わり)
	(*m_pD3DDevice)->SetVertexDeclaration((*m_pCSkinMesh->GetDecl()));

	// トゥーン用
	hr = (*m_pD3DDevice)->SetTexture(toonSampler, CTexture::GetTexture(TEXTURE_MONO));

	// 描画
	m_pCSkinMesh->Draw(this, RENDERER_TYPE_NORMAL);

	//*********注意:以下を必ず書くこと******************
	(*m_pD3DDevice)->SetTexture(toonSampler, NULL);
	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);

}
//*****************************************************************************
// スキンメッシュで計算したワールドマトリクスをもとにセットする
//*****************************************************************************
void CTitlePlayer::SetWorldMtxForNormalRender(D3DXMATRIX* worldMtx, int blendBoneNum)
{
	// 座標変換用のパラメータを送る
	D3DXMATRIX view, proj;
	HRESULT hr;
	hr = (*m_pVSC)->SetInt((*m_pD3DDevice), "gBlendNum", blendBoneNum);
	view = m_pManager->GetCameraManager()->GetMtxView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxProj();
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gView", &view);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gProj", &proj);
	hr = (*m_pVSC)->SetMatrixArray((*m_pD3DDevice), "gWorld", worldMtx, MAX_BONE_MATRIX);

	view = m_pManager->GetCameraManager()->GetMtxLightView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxLightProj();
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gLightView", &view);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gLightProj", &proj);
}

//----EOF----