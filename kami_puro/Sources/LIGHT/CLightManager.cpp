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
static const float LIGHT_DIST = 100.0f;
// ライトの1フレームの回転速度
static const float LIGHT_ROT = 0.01f;
// ライトのＹ座標
static const float LIGHT_Y = 10.f;

//===========================================================================
// コンストラクタ
//===========================================================================
CLightManager::CLightManager(CManager* pManager)
{
	m_pManager = pManager;
	m_Count = 0;

	m_RadOne = D3DX_PI * 2.0f / LIGHT_NUM;

	// ライトの生成
	for (int i = 0; i < LIGHT_NUM; i++)
	{
		m_pLight[i] = new CLight();
	}

	// 初期位置
	float rad = m_Count;
	for (int i = 0; i < LIGHT_NUM; i++){
		float pos_x = cosf(rad) * (LIGHT_DIST);
		float pos_z = sinf(rad) * (LIGHT_DIST);
		m_pLight[i]->SetPos(D3DXVECTOR3(pos_x, LIGHT_Y, pos_z));
		rad += m_RadOne;
	}

	// test　ＲＧＢで確認できる
	//	m_pLight[0]->SetColor(D3DXCOLOR(1, 0, 0, 1));
	//	m_pLight[1]->SetColor(D3DXCOLOR(0, 1, 0, 1));
	//	m_pLight[2]->SetColor(D3DXCOLOR(0, 0, 1, 1));
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
	m_Count += LIGHT_ROT;
	NormalizeRotation(&m_Count);

	// 移動
	float rad = m_Count;
	for (int i = 0; i < LIGHT_NUM; i++){
		float pos_x = cosf(rad) * (LIGHT_DIST);
		float pos_z = sinf(rad) * (LIGHT_DIST);
		m_pLight[i]->SetPos(D3DXVECTOR3(pos_x, LIGHT_Y, pos_z));
		rad += m_RadOne;
	}
}

//----EOF----