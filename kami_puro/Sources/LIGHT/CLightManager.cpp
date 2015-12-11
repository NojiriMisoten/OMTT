//=============================================================================
//
// CLightManagerクラス [CLightManager.cpp]
// Author : つかも
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CLightManager.h"
#include "../MANAGER/CManager.h"

//===========================================================================
// 定数
//===========================================================================
// ライトを回転したときの動く範囲
static const float LIGHT_DIST_1F = 480.0f;
static const float LIGHT_DIST_2F = 720.0f;
// ライトの1フレームの回転速度
static const float LIGHT_ROT1F = 0.01f;
static const float LIGHT_ROT2F = -0.005f;
// ライトのＹ座標
static const float LIGHT_Y1F = 30.f;
static const float LIGHT_Y2F = 50.f;
// 移動都下の計算で使うライトの数
static const short LIGHT_NUM_1F = 4;
static const short LIGHT_NUM_2F = 4;
// ライトの色 基本白だけど赤っぽいのと青っぽいやつ
static const D3DXCOLOR COLOR_RED = D3DXCOLOR(1.0f, 0.7f, 0.7f, 1.0f);
static const D3DXCOLOR COLOR_BLUE = D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f);

//===========================================================================
// コンストラクタ
//===========================================================================
CLightManager::CLightManager(CManager* pManager)
{
	m_pManager = pManager;
	m_Rot1F = 0;
	m_Rot2F = 0;

	m_RadOne1F = D3DX_PI * 2.0f / LIGHT_NUM_1F;
	m_RadOne2F = D3DX_PI * 2.0f / LIGHT_NUM_2F;

	// ライトの生成
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		m_pLight[i] = new CLight();
	}

	// 初期位置
	float rad = m_Rot1F;
	for (int i = 0; i < LIGHT_NUM_1F; i++){
		float pos_x = cosf(rad) * (LIGHT_DIST_1F);
		float pos_z = sinf(rad) * (LIGHT_DIST_1F);
		m_pLight[i]->SetPos(D3DXVECTOR3(pos_x, LIGHT_Y1F, pos_z));
		rad += m_RadOne1F;
		m_pLight[i]->SetColor(i % 2 == 0 ? COLOR_RED : COLOR_BLUE);
	}
	rad = m_Rot2F;
	for (int i = LIGHT_NUM_1F; i < LIGHT_NUM; i++){
		float pos_x = cosf(rad) * (LIGHT_DIST_2F);
		float pos_z = sinf(rad) * (LIGHT_DIST_2F);
		m_pLight[i]->SetPos(D3DXVECTOR3(pos_x, LIGHT_Y2F, pos_z));
		m_pLight[i]->SetColor(i % 2 == 0 ? COLOR_RED : COLOR_BLUE);
		rad += m_RadOne2F;
	}

}

//===========================================================================
// デストラクタ
//===========================================================================
CLightManager::~CLightManager(void)
{

}

//===========================================================================
// 終了
//===========================================================================
void CLightManager::Uninit(void)
{
	// ライトの削除
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		SAFE_DELETE(m_pLight[i]);
	}
}

//===========================================================================
// 更新
//===========================================================================
void CLightManager::Update(void)
{
	// 角度を加算
	m_Rot1F += LIGHT_ROT1F;
	m_Rot2F += LIGHT_ROT2F;
	NormalizeRotation(&m_Rot1F);
	NormalizeRotation(&m_Rot2F);

	// 移動
	float rad = m_Rot1F;
	for (int i = 0; i < LIGHT_NUM_1F; i++){
		float pos_x = cosf(rad) * (LIGHT_DIST_1F);
		float pos_z = sinf(rad) * (LIGHT_DIST_1F);
		m_pLight[i]->SetPos(D3DXVECTOR3(pos_x, LIGHT_Y1F, pos_z));
		rad += m_RadOne1F;
	}
	rad = m_Rot2F;
	for (int i = LIGHT_NUM_1F; i < LIGHT_NUM; i++){
		float pos_x = cosf(rad) * (LIGHT_DIST_2F);
		float pos_z = sinf(rad) * (LIGHT_DIST_2F);
		m_pLight[i]->SetPos(D3DXVECTOR3(pos_x, LIGHT_Y2F, pos_z));
		rad += m_RadOne2F;
	}

}

//----EOF----