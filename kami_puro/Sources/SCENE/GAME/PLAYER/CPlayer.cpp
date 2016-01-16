//=============================================================================
//
// CPlayerクラス [CPlayer.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CPlayer.h"
#include "../../../INPUT/CInputKeyboard.h"
#include "../../../CAMERA/CameraManager.h"
#include "../CGame.h"
#include "../../../MANAGER/CManager.h"
#include "../../../SHADER/CShader.h"
#include "../../../CONTROLLER/CControllerManager.h"
#include "../../../EFECT/CEffect.h"
#include "../JUDGE/CJudgeManager.h"
#include "../UI/CUiManager.h"
#include "../UI/CHpBar.h"
#include "../UI/CCrowdBar.h"
#include "../../../SKINMESH/CSkinMeshHolder.h"
#include "../../../MATH/mersenne_twister.h"
#include "../../../SOUND/CSound.h"

#ifdef _DEBUG
#include <time.h>
#endif

//*****************************************************************************
// マクロ
//*****************************************************************************
static const float	DEFFAULT_MOV_SPD = 0.3f;								// 通常時移動速度
static const float	DEFFAULT_ROT_SPD = 0.01f;
static const float	DEST_CAMERA_POS_COEFFICIENT = 3.f;						// カメラに移してほしいところ計算用係数
static const float	DEST_CAMERA_POS_Y_COEFFICIENT = 0.8f;					// カメラに移してほしいところY座標計算用係数

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPlayer::CPlayer(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType) : CSceneX(pDevice, m_objType)
{
	// 姿勢ベクトル
	m_vecFront = DEFAULT_FRONT_VECTOR;
	m_vecRight = DEFAULT_RIGHT_VECTOR;
	m_vecUp = DEFAULT_UP_VECTOR;

	// 移動初期化
	m_MovPos = D3DXVECTOR3(0, 0, 0);

	D3DXMatrixIdentity(&m_OldWorldMtx);
	m_pManager = NULL;

	m_ID = -1;
	m_HP = 0;
	m_JampPower = 0;
	m_JampFlag = false;
	m_AnimState = PLAYER_WAIT;
	m_isUseFinish = false;
	m_isRope = true;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPlayer::~CPlayer(void)
{

}

//*****************************************************************************
// 作成
//*****************************************************************************
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID)
{
	CPlayer* p = new CPlayer(pDevice);

	p->Init(pDevice, pos, rot, type, pManager, ID);

	return p;
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CPlayer::Init(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID)
{
	m_DestPos = m_Pos = pos;
	m_pManager = pManager;
	m_pUiManager = pManager->GetUiManager();
	m_Rot = rot;

	// ジャンプパラメータ初期化
	m_JampPower = 0;
	m_JampFlag = true;

	// アニメ
	m_AnimState = PLAYER_WAIT;

	// プレイヤーHP
	m_HP = DEFAULT_HP_PARAMETER;

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
	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL_VEC);
	CScene::AddLinkList(CRenderer::TYPE_RENDER_TOON_OBJECT_DEPTH);

	// スケール
	m_vScl = D3DXVECTOR3( 50, 50, 50 );

	// ここで更新してからじゃないとアニメーション変えられないのでしている
	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);
	SetAnimType(PLAYER_WAIT);
	SetAnimMortionOfTime(0);
	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CPlayer::Uninit(void)
{
	Release();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CPlayer::Update(void)
{
	m_OldWorldMtx = m_mtxWorld;

	// Getで現在のフェーズを持ってくる
	BATTLE_MODE mode = m_pManager->GetJudgeManager()->GetBattleMode();


	if (mode == BATTLE_MOVE)
	{
		// 移動フェーズ
		MovePhase();
	}
	else if (mode == BATTLE_FIGHT)
	{
		// 攻撃フェーズ
		AttackPhase();
	}

	// フロントベクトルの設定
	m_vecFront.x = -sinf(-m_Rot.y);
	m_vecFront.z = -cosf(m_Rot.y - D3DX_PI);

	// ライトベクトルの設定
	m_vecRight.x = cosf(m_Rot.y - D3DX_PI);
	m_vecRight.z = sinf(m_Rot.y);

	m_pCSkinMesh->Update(m_Pos, m_Rot, m_vScl);
#ifdef _DEBUG
	if (m_ID == 0)
	{
		CDebugProc::PrintDL("[PLAYER]\n");
		CDebugProc::PrintDL("Pos: %+10.3f/%+10.3f/%+10.3f\n", m_Pos.x, m_Pos.y, m_Pos.z);
		CDebugProc::PrintDL("\n");
		if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_FINISH_FLAG_REVERSAL))
		{
			m_isRope = !m_isRope;
		}
	}
	if (m_ID == 1)
	{
		CDebugProc::PrintDR("[PLAYER]\n");
		CDebugProc::PrintDR("Pos: %+10.3f/%+10.3f/%+10.3f\n", m_Pos.x, m_Pos.y, m_Pos.z);
		CDebugProc::PrintDR("\n");
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_FINISH_FLAG_REVERSAL))
	{
		m_isUseFinish = !m_isUseFinish;
	}

#endif
}

//*****************************************************************************
// 描画
//*****************************************************************************
void CPlayer::DrawNormalRender(void)
{
	// シェーダーの適用
	m_pVS = CShader::GetVS(VS_TYPE_NORMAL_RENDERER_SKINMESH);
	m_pVSC = CShader::GetVSC(VS_TYPE_NORMAL_RENDERER_SKINMESH);

	PS_TYPE type = PS_TYPE_NORMAL_RENDERER;

	m_pPS = CShader::GetPS(type);
	m_pPSC = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*m_pVS);
	(*m_pD3DDevice)->SetPixelShader(*m_pPS);

	HRESULT hr = 0;

	UINT texSampler = (*m_pPSC)->GetSamplerIndex("texSampler");
	UINT toonSampler = (*m_pPSC)->GetSamplerIndex("toonSampler");
	UINT depthSampler = (*m_pPSC)->GetSamplerIndex("depthSampler");

	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(toonSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	hr = (*m_pD3DDevice)->SetSamplerState(depthSampler, D3DSAMP_BORDERCOLOR, 0xffffffff);

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

	// 頂点宣言したやつをセット(SetFVFの代わり)
	(*m_pD3DDevice)->SetVertexDeclaration((*m_pCSkinMesh->GetDecl()));

	// トゥーン用
	hr = (*m_pD3DDevice)->SetTexture(toonSampler, CTexture::GetTexture(TEXTURE_MONO));

	// 深度値
	LPDIRECT3DTEXTURE9 *tex = CRenderer::GetRenderTexture(CRenderer::TYPE_RENDER_TOON_OBJECT_DEPTH);
	hr = (*m_pD3DDevice)->SetTexture(depthSampler, *tex);

	// 描画
	m_pCSkinMesh->Draw(this, RENDERER_TYPE_NORMAL);

	//*********注意:以下を必ず書くこと******************
	(*m_pD3DDevice)->SetTexture(toonSampler, NULL);
	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetTexture(depthSampler, NULL);
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);

}
//*****************************************************************************
// スキンメッシュで計算したワールドマトリクスをもとにセットする
//*****************************************************************************
void CPlayer::SetWorldMtxForNormalRender(D3DXMATRIX* worldMtx, int blendBoneNum)
{
	// 座標変換用のパラメータを送る
	D3DXMATRIX view, proj;
	HRESULT hr;
	hr = (*m_pVSC)->SetInt((*m_pD3DDevice), "gBlendNum", blendBoneNum);

	view = m_pManager->GetCameraManager()->GetMtxView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxProj();
	hr = (*m_pVSC)->SetMatrixArray((*m_pD3DDevice), "gWorld", worldMtx, MAX_BONE_MATRIX);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gView", &view);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gProj", &proj);

	view = m_pManager->GetCameraManager()->GetMtxLightView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxLightProj();
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gLightView", &view);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gLightProj", &proj);
}

//*****************************************************************************
// テクスチャーをＰＳにセットする
//*****************************************************************************
void CPlayer::SetTextureForPS(PLAYER_RENDERER_TYPE type, LPDIRECT3DTEXTURE9* tex)
{
	if (type != RENDERER_TYPE_NORMAL)
	{
		return;
	}
	UINT texSampler = (*m_pPSC)->GetSamplerIndex("texSampler");
	(*m_pD3DDevice)->SetTexture(texSampler, *tex);
}

//*****************************************************************************
// 法線描画
//*****************************************************************************
void CPlayer::DrawNormalVecRender(void)
{
	// シェーダーの適用
	m_pVS = CShader::GetVS(VS_TYPE_NORMAL_VEC_RENDERER_SKINMESH);
	m_pVSC = CShader::GetVSC(VS_TYPE_NORMAL_VEC_RENDERER_SKINMESH);

	PS_TYPE type = PS_TYPE_NORMAL_VEC_RENDERER;

	m_pPS = CShader::GetPS(type);
	m_pPSC = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*m_pVS);
	(*m_pD3DDevice)->SetPixelShader(*m_pPS);

	HRESULT hr = 0;
	
	// 頂点宣言したやつをセット(SetFVFの代わり)
	(*m_pD3DDevice)->SetVertexDeclaration((*m_pCSkinMesh->GetDecl()));

	// 描画
	m_pCSkinMesh->Draw(this, RENDERER_TYPE_NORMAL_VEC);
	
	//*********注意:以下を必ず書くこと******************
	// 書かないとすべての色がおかしくなる
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}
//*****************************************************************************
// スキンメッシュで計算したワールドマトリクスをもとにセットする
//*****************************************************************************
void CPlayer::SetWorldMtxForNormalVecRender(D3DXMATRIX* worldMtx, int blendBoneNum)
{
	D3DXMATRIX view, proj;
	HRESULT hr;
	hr = (*m_pVSC)->SetInt((*m_pD3DDevice), "gBlendNum", blendBoneNum);

	view = m_pManager->GetCameraManager()->GetMtxView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxProj();

	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gProj", &proj);
	hr = (*m_pVSC)->SetMatrixArray((*m_pD3DDevice), "gWorld", &worldMtx[0], MAX_BONE_MATRIX);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gView", &view);
}

//*****************************************************************************
// 深度値描画
//*****************************************************************************
void CPlayer::DrawToonObjectDepthRender(void)
{
	// シェーダーの適用
	m_pVS = CShader::GetVS(VS_TYPE_TOON_OBJECT_DEPTH_SKINMESH);
	m_pVSC = CShader::GetVSC(VS_TYPE_TOON_OBJECT_DEPTH_SKINMESH);
	

	PS_TYPE type = PS_TYPE_TOON_OBJECT_DEPTH;

	m_pPS = CShader::GetPS(type);
	m_pPSC = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*m_pVS);
	(*m_pD3DDevice)->SetPixelShader(*m_pPS);
	
	// 頂点宣言したやつをセット(SetFVFの代わり)
	(*m_pD3DDevice)->SetVertexDeclaration((*m_pCSkinMesh->GetDecl()));

	// 描画
	m_pCSkinMesh->Draw(this, RENDERER_TYPE_DEPTH);
	
	//*********注意:以下を必ず書くこと******************
	// 書かないとすべての色がおかしくなる
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);

//	m_pManager->GetCameraManager()->SetCamera(m_pD3DDevice);
}
//*****************************************************************************
// スキンメッシュで計算したワールドマトリクスをもとにセットする
//*****************************************************************************
void CPlayer::SetWorldMtxForToonObjectDepthRender(D3DXMATRIX* worldMtx, int blendBoneNum)
{
	HRESULT hr = 0;
	hr = (*m_pVSC)->SetInt((*m_pD3DDevice), "gBlendNum", blendBoneNum);
	D3DXMATRIX view, proj;
	view = m_pManager->GetCameraManager()->GetMtxLightView();
	proj = m_pManager->GetCameraManager()->CCameraManager::GetMtxLightProj();
	hr = (*m_pVSC)->SetMatrixArray((*m_pD3DDevice), "gWorld", &worldMtx[0], MAX_BONE_MATRIX);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gView", &view);
	hr = (*m_pVSC)->SetMatrix((*m_pD3DDevice), "gProj", &proj);
}


//*****************************************************************************
// 移動関数
//*****************************************************************************
D3DXVECTOR3 CPlayer::Move(void)
{
	return D3DXVECTOR3(0, 0, 0);
}

//*****************************************************************************
//アニメーションコールバック用ハンドラークラス定義
//*****************************************************************************
HRESULT CALLBACK CCallBackHandlerPlayer::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
{
	//// 受け取ったvoidポインタを正しく展開
	//CUSTOM_CALLBACK_DATA *pCallData = (CUSTOM_CALLBACK_DATA*)pCallbackData;
	//
	//// 召喚モーション
	//if (pCallData->nAnimationID == CPlayer::PLAYER_WAIT)
	//{
	//
	//}
	//
	//// 気絶モーション
	//else if (pCallData->nAnimationID == CPlayer::PLAYER_LARIAT_LEFT)
	//{
	//
	//}
	//
	//// ダメージモーション
	//else if (pCallData->nAnimationID == CPlayer::PLAYER_ELBOW_LEFT)
	//{
	//
	//}
	//
	//// 歩きモーション
	//else if (pCallData->nAnimationID == CPlayer::PLAYER_ELBOW_RIGHT)
	//{
	//
	//}
	//
	//// 待機モーション
	//else if (pCallData->nAnimationID == CPlayer::PLAYER_DAMAGE_SMALL)
	//{
	//
	//}

	return S_OK;
}

//*****************************************************************************
// ワールドマトリックスをセットする
//*****************************************************************************
void CPlayer::SetWorldMtx(D3DXMATRIX* worldMtx, PLAYER_RENDERER_TYPE type, int blendBoneNum)
{
	switch (type)
	{
	case RENDERER_TYPE_NORMAL:
		SetWorldMtxForNormalRender(worldMtx, blendBoneNum);
		break;

	case RENDERER_TYPE_NORMAL_VEC:
		SetWorldMtxForNormalVecRender(worldMtx, blendBoneNum);
		break;

	case RENDERER_TYPE_DEPTH:
		SetWorldMtxForToonObjectDepthRender(worldMtx, blendBoneNum);
		break;

	default:
#ifdef _DEBUG
		assert(!"不正なタイプ！！");
#endif
		break;
	}
}

//*****************************************************************************
// 移動フェーズ関数
//*****************************************************************************
void CPlayer::MovePhase()
{
	const bool isPlayer1ForwardKeyboard = (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_LEFT_UP)
											|| CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_RIGHT_UP)
											)&& m_ID == 0;
	const bool isPlayer1BackKeyboard = (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_LEFT_DOWN)
											|| CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_RIGHT_DOWN)
											)&& m_ID == 0;

	const bool isPlayer2ForwardKeyboard = (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_LEFT_UP)
											|| CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_RIGHT_UP)
											)&& m_ID == 1;
	const bool isPlayer2BackKeyboard = (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_LEFT_DOWN)
											|| CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_RIGHT_DOWN)
											)&& m_ID == 1;

	const bool isForword = CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_UP, m_ID)
						|| CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_UP, m_ID)
						|| isPlayer1ForwardKeyboard
						|| isPlayer2ForwardKeyboard;

	const bool isBack = CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_DOWN, m_ID)
						|| CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_DOWN, m_ID)
						|| isPlayer1BackKeyboard
						|| isPlayer2BackKeyboard;

	switch( m_ID )
	{
	case 0:
		if( m_Pos.x <= 60.0f )
		{
			if( isForword )
			{
				m_DestPos.x += m_vecFront.x;
				m_DestPos.z += m_vecFront.z;
				AddTension( MOVE_TENSION_AMOUNT );

			}
		}
	
		if( m_Pos.x > -70.0f )
		{

			if( isBack )
			{
				m_DestPos.x -= m_vecFront.x;
				m_DestPos.z -= m_vecFront.z;
				TakeHeal( MOVE_HEAL_AMOUNT );
			}
		}
		break;

	case 1:
		if( m_Pos.x >= -60.0f )
		{
			if( isForword )
			{
				m_DestPos.x += m_vecFront.x;
				m_DestPos.z += m_vecFront.z;
				AddTension( MOVE_TENSION_AMOUNT );

			}
		}
		if( m_Pos.x < 70.0f )
		{
			if( isBack )
			{
				m_DestPos.x -= m_vecFront.x;
				m_DestPos.z -= m_vecFront.z;
				TakeHeal( MOVE_HEAL_AMOUNT );
			}
		}
		break;
	}

	// trueの場合ジャンプできる
	if (m_JampFlag)
	{
		if (isForword || isBack)
		{
			m_JampPower = DEFFAULT_JAMP_POWER;
			m_JampFlag = false;
		}
	}

	PlayerJamp();

	m_Pos.x += (m_DestPos.x - m_Pos.x) * DEFFAULT_MOV_SPD;
	m_Pos.z += (m_DestPos.z - m_Pos.z) * DEFFAULT_MOV_SPD;

	if (m_Pos.y < 0)
	{
		m_Pos.y = 0;
		// 着地
		if( !m_JampFlag )
		{
			D3DXVECTOR3 pos = m_Pos;
			CEffect::Create(30, EFFECT_FOOTSTEP_WAVE, false, pos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(20, 20, 20));

			m_pManager->GetCameraManager()->StartCameraShake( VECTOR3_ZERO, 1.0f, 5, 0.0f );

			int label = mersenne_twister_int(SOUND_LABEL_SE_FOOT_STEP01, SOUND_LABEL_SE_FOOT_STEP03);
			m_pManager->PlaySoundA((SOUND_LABEL)label);
		}
		m_JampFlag = true;
	}
}

//*****************************************************************************
// 攻撃フェーズ関数
//*****************************************************************************
void CPlayer::AttackPhase()
{
	
}

//*****************************************************************************
// プレイヤージャンプ関数
//*****************************************************************************
void CPlayer::PlayerJamp()
{
	float tempPosy = m_Pos.y;
	m_Pos.y += (m_Pos.y - m_DestPos.y) + m_JampPower;
	m_DestPos.y = tempPosy;
	m_JampPower = -1;
}

//*****************************************************************************
// ゲッター
//*****************************************************************************
int CPlayer::GetHP()
{
	return m_HP;
}

CPlayer::PLAYER_ANIM_TYPE CPlayer::GetAnimState()
{
	return m_AnimState;
}

//*****************************************************************************
// アニメーションセッター
//*****************************************************************************
void CPlayer::SetAnimType( int type , double moveRate)
{
	if( type < 0 )
	{
		type = 0;
	}
	if( type >= (int)PLAYER_ANIM_MAX )
	{
		type = (int)PLAYER_ANIM_MAX - 1;
	}
	m_AnimState = (PLAYER_ANIM_TYPE)type;
	m_pCSkinMesh->ChangeMotion( m_AnimState, moveRate );
}

//*****************************************************************************
// ダメージ処理
//*****************************************************************************
void CPlayer::TakeDamage( int damage )
{
	int HPOld = m_HP;
	m_HP -= damage;
	if ( m_HP < 50 )
	{
		m_HP = 50;
	}
	damage = HPOld - m_HP;


	switch( m_ID )
	{
	case 0:
		m_pManager->GetUiManager()->GetHpBar()->SubLeft( (float)damage );
		break;

	case 1:
		m_pManager->GetUiManager()->GetHpBar()->SubRight( (float)damage );
		break;
	}
}
//*****************************************************************************
// ダメージ処理
//*****************************************************************************
void CPlayer::TakeDamageFinish(int damage)
{
	int HPOld = m_HP;
	m_HP -= damage;
	if (m_HP < 0)
	{
		m_HP = 0;
	}
	damage = HPOld - m_HP;

	switch (m_ID)
	{
	case 0:
		m_pManager->GetUiManager()->GetHpBar()->SubLeft((float)damage);
		break;

	case 1:
		m_pManager->GetUiManager()->GetHpBar()->SubRight((float)damage);
		break;
	}
}

//*****************************************************************************
// 回復処理
//*****************************************************************************
void CPlayer::TakeHeal( int heal )
{
	m_HP += heal;
	if ( m_HP > DEFAULT_HP_PARAMETER )
	{
		m_HP = DEFAULT_HP_PARAMETER;
	}
	switch( m_ID )
	{
	case 0:
		m_pManager->GetUiManager()->GetHpBar()->AddLeft( (float)heal );
		break;

	case 1:
		m_pManager->GetUiManager()->GetHpBar()->AddRight( (float)heal );
		break;
	}
	// エフェクトを作る
	m_pUiManager->CreateJumpEffect(m_Pos, TEXTURE_JUMP_EFFECT_HP, m_ID);
}


//*****************************************************************************
// テンションゲージ
//*****************************************************************************
void CPlayer::AddTension( int tension )
{
	switch( m_ID )
	{
	case 0:
		m_pManager->GetUiManager()->GetCrowdBar()->Add(tension);
		// エフェクトを作る
		m_pUiManager->CreateJumpEffect(m_Pos, TEXTURE_JUMP_EFFECT_CROWD_R, m_ID);
		break;

	case 1:
		m_pManager->GetUiManager()->GetCrowdBar()->Add(-tension);
		// エフェクトを作る
		m_pUiManager->CreateJumpEffect(m_Pos, TEXTURE_JUMP_EFFECT_CROWD_B, m_ID);
		break;
	}
	
}

//*****************************************************************************
// アニメーションを時間の状態に指定
//*****************************************************************************
void CPlayer::SetAnimMortionOfTime(double time)
{
	//if (animTime < 0.0)
	//{
	//	animTime = 0.0;
	//}
	//if (animTime > 1.0)
	//{
	//	animTime = 1.0;
	//}

	m_pCSkinMesh->SetAnimMotion( time );
}

//*****************************************************************************
// アニメーション速度セット
//*****************************************************************************
void CPlayer::SetAnimSpd(double spd)
{
	m_pCSkinMesh->SetAnimSpd(spd);
}

//----EOF----