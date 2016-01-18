//=============================================================================
//
// CJumpEffectBillbordクラス [CJumpEffectBillbord.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CJumpEffectBillbord.h"
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../../../SHADER/CShader.h"
#include "../../../MATH/mersenne_twister.h"
#include "../../../CAMERA/CameraManager.h"
#include "CUiManager.h"

//*****************************************************************************
// 定数
//*****************************************************************************

// 大きさ
static const float JUMP_EFFECT_SIZE = SCREEN_WIDTH / 1280.f * 50.0f;
// 透過スピード
static const float ALPHA_SPEED = 0.025f;
// エフェクトの透過度
static const float ALPHA = 0.8f;
// 移動スピード
static const float MOVE_SPEED = 1.3f;
// 横の移動量の減衰値
static const float VELO_RESIST = 0.9f;
// 横の移動量の範囲
static const float VELO_MAX = 3;
// ビューポート変換の行列
static const D3DXMATRIX VIEW_PORT = {
	SCREEN_WIDTH*0.5f, 0, 0, 0,
	0, SCREEN_HEIGHT*0.5f, 0, 0,
	0, 0, 1, 0,
	SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0, 1
};
// 飛ぶ先の座標 HPと歓声ゲージ 添え字はプレイヤ番号と対応
static const D3DXVECTOR3 POS_HP_BAR[2] = {
	D3DXVECTOR3(335, 121, 0),
	D3DXVECTOR3(SCREEN_WIDTH - 335, 121, 0)
};
static const D3DXVECTOR3 POS_CROWD_BAR[2] = {
	D3DXVECTOR3(425, 75, 0),
	D3DXVECTOR3(SCREEN_WIDTH - 425, 75, 0)
};
// 飛ぶまでのフレーム数
static const int FLY_INTERVAL = 60;
// 飛ぶ時の移動スピード 補間するtime(0~1)の加算値
static const float FLY_TIME_SPEED = 0.1f;
// 縮小スピード
static const float SIZE_RESIST = 0.9f;
// 画面外に行かないように制限をつける
static const float POS2D_MIN = 100.f;


//=============================================================================
// コンストラクタ
//=============================================================================
CJumpEffectBillbord::CJumpEffectBillbord(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice, CScene::OBJTYPE_2D)
{
	m_pCameraManager = NULL;
	m_pD3DDevice = pDevice;
	m_Velo = 0;
	m_Color = D3DXCOLOR(1, 1, 1, ALPHA);
	m_PlayerNum = 0;
	m_FlyCount = 0;
	m_isFly = false;
	m_FlyTime = 0;
	m_Pos3D = D3DXVECTOR3(0, 0, 0);
	m_Pos2D = D3DXVECTOR3(0, 0, 0);
	m_Velo = 0;
	m_Pos2DStart = D3DXVECTOR3(0, 0, 0);
	m_Pos2DEnd = D3DXVECTOR3(0, 0, 0);
	m_isCrowd = false;
	m_Size = JUMP_EFFECT_SIZE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CJumpEffectBillbord::~CJumpEffectBillbord(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CJumpEffectBillbord::Init(D3DXVECTOR3& pos, TEXTURE_TYPE texType)
{
	m_Pos3D = pos;
	// 横の移動量をランダムで決める
	m_Velo = mersenne_twister_float(-VELO_MAX, VELO_MAX);

	// 3Dの座標を2Dに変換
	m_Pos2D = D3DXVECTOR3(0, 0, 0);
	D3DXMATRIX proj = m_pCameraManager->GetMtxProj();
	D3DXMATRIX view = m_pCameraManager->GetMtxView();
	D3DXVec3TransformCoord(&m_Pos2D, &m_Pos3D, &view);
	D3DXVec3TransformCoord(&m_Pos2D, &m_Pos2D, &proj);
	D3DXVec3TransformCoord(&m_Pos2D, &m_Pos2D, &VIEW_PORT);

	// 一旦3Dから2Dに変換する
	CScene2D::Init(m_Pos2D, JUMP_EFFECT_SIZE, JUMP_EFFECT_SIZE, texType);

	// 最終的に飛ぶ先の座標を計算しておく
	m_isCrowd = (texType != TEXTURE_JUMP_EFFECT_HP);

}

//=============================================================================
// 終了
//=============================================================================
void CJumpEffectBillbord::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CJumpEffectBillbord::Update(void)
{
	// 一定カウントしたらそれぞれの座標に飛ぶ
	if (!m_isFly)
	{
		// 3D座標系で出現時のアニメーション
		m_Pos3D.y -= MOVE_SPEED;
		m_Pos3D.x += m_Velo;
		m_Velo *= VELO_RESIST;

		// 飛ぶ準備
		m_FlyCount++;
		if (m_FlyCount > FLY_INTERVAL || m_Pos2D.y < POS2D_MIN)
		{
			m_isFly = true;
			m_Pos2DStart = m_Pos2D;
			if (m_isCrowd)
			{
				m_Pos2DEnd.x = m_pUiManager->GetPosCrowdCenter(m_PlayerNum);
				m_Pos2DEnd.y = POS_CROWD_BAR[m_PlayerNum].y;
			}
			else
			{
				m_Pos2DEnd.x = m_pUiManager->GetPosHpCenter(m_PlayerNum);
				m_Pos2DEnd.y = POS_HP_BAR[m_PlayerNum].y;
			}
		}

		// 3Dの座標を2Dに変換
		D3DXMATRIX proj = m_pCameraManager->GetMtxProj();
		D3DXMATRIX view = m_pCameraManager->GetMtxView();
		D3DXVec3TransformCoord(&m_Pos2D, &m_Pos3D, &view);
		D3DXVec3TransformCoord(&m_Pos2D, &m_Pos2D, &proj);
		D3DXVec3TransformCoord(&m_Pos2D, &m_Pos2D, &VIEW_PORT);

		// CScene2Dの情報を更新してスプライトの大きさをLockで変更
		m_Pos = m_Pos2D;
		SetVertexPolygon();
	}
	// 飛んでるときの処理
	else
	{
		// 補間で移動
		if (m_FlyTime <= 1.0f)
		{
			m_Pos2D.x = EasingInterpolation(m_Pos2DStart.x, m_Pos2DEnd.x, m_FlyTime);
			m_Pos2D.y = EasingInterpolation(m_Pos2DStart.y, m_Pos2DEnd.y, m_FlyTime);
			// CScene2Dの情報を更新してスプライトの大きさをLockで変更
			m_Pos = m_Pos2D;
			SetVertexPolygon();
			m_FlyTime += FLY_TIME_SPEED;
		}
		// 移動が終わったら小さくなって透過して消える
		else
		{
			m_Size *= SIZE_RESIST;
			SetVertexPolygon(m_Pos, m_Size, m_Size);

			// 透過
			m_Color.a -= ALPHA_SPEED;
			this->SetColorPolygon(m_Color);

			// 透過したら消す
			if (m_Color.a <= 0.0f)
			{
				Release();
			}
		}
	}

}

//=============================================================================
// 描画
//=============================================================================
void CJumpEffectBillbord::DrawUIRender(void)
{
	// ポリゴンの描画
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);							// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// プリミティブの種類
		, 0											// 描画を開始する頂点番号
		, DRAW_SQUARE_PRINITIV_NUM);				// 書きたいポリゴン数
}

//=============================================================================
// 作成
//=============================================================================
CJumpEffectBillbord* CJumpEffectBillbord::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3& pos, TEXTURE_TYPE texType, CCameraManager* pCamera, CUiManager* pUiManager, int playerNum)
{
	CJumpEffectBillbord* p = new CJumpEffectBillbord(pDevice);
	p->m_pCameraManager = pCamera;
	p->m_PlayerNum = playerNum;
	p->m_pUiManager = pUiManager;
	p->Init(pos, texType);
	p->AddLinkList(CRenderer::TYPE_RENDER_UI);
	return p;
}

//----EOF----