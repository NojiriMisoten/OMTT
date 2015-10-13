//=============================================================================
//
// CSceneTimeクラス [CSceneTime.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneTime.h"
#include "../UI/CNumber.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const int	MINUS_TIME_COUNT = 180;			// タイムが１減るまでの時間
static const int	DEFAULT_MINUS_TIME_VALUE = -1;	// タイムが減るデフォルトの量

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneTime ::CSceneTime(LPDIRECT3DDEVICE9 *pDevice):CScene2D(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneTime ::~CSceneTime(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CSceneTime :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum)
{
	// 原点の位置
	SetPos(pos);

	// 幅と高さの設定
	m_fWidth = width;
	m_fHeight = height;

	// 桁数
	m_nNumberNum = numberNum;

	// タイマー初期化
	m_nTime = 0;

	// カウンター初期化
	m_nCounter = 0;

	// １の位の場所
	pos.x += (MAX_TIME_NUM - 1) * width;

	// 桁数分数字作成
	for(int i = 0; i < MAX_TIME_NUM; i++)
	{
		m_Number[i] = m_Number[i]->Create(m_pD3DDevice, pos, width, height, texType);
		pos.x -= width;
	}
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneTime ::Uninit(void)
{
	for(int i = 0; i < MAX_TIME_NUM; i++)
	{
		if(m_Number[i])
		{
			m_Number[i]->Uninit();
			delete m_Number[i];
			m_Number[i] = NULL;
		}
	}
	Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneTime ::Update(void)
{
	// カウントアップ
	m_nCounter++;

	// 一定時間過ぎたら
	if(m_nCounter >= MINUS_TIME_COUNT)
	{
		// タイマー減らす
		Add(DEFAULT_MINUS_TIME_VALUE);

		// カウント初期化
		m_nCounter = 0;
	}
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneTime ::Draw(void)
{
	// 桁数分数字表示
	for(int i = 0; i < MAX_TIME_NUM; i++)
	{
		m_Number[i]->Draw();
	}
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneTime* CSceneTime::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum)
{
	// 作成
	CSceneTime* p = new CSceneTime(pDevice);

	// 初期化
	p->Init(pos, width, height, texType, numberNum);

	return p;
}

//*****************************************************************************
// 数字変更関数
//*****************************************************************************
void CSceneTime ::Add(int addNum)
{
	m_nTime += addNum;
	if(m_nTime >= MAX_TIME)
	{
		m_nTime = MAX_TIME;
	}

	if(m_nTime <= 0)
	{
		m_nTime = 0;
	}

	for(int nCntPlace = 0; nCntPlace < MAX_TIME_NUM; nCntPlace++)
	{
		int number = 0;
		//桁の数字割出処理
		number = (m_nTime / (int)pow(10.0,(double)(nCntPlace))) % 10;	// 10進の数字を出すので10を使ってる

		// 数字セット
		m_Number[nCntPlace]->SetNumber(number);
	}
}
//----EOF----