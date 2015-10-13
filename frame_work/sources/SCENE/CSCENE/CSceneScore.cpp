//=============================================================================
//
// CSceneScoreクラス [CSceneScore.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneScore.h"
#include "../UI/CNumber.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
LPDIRECT3DDEVICE9	CSceneScore:: *m_pD3DDevice = NULL;
float				CSceneScore:: m_fWidth = 0.f;
float				CSceneScore:: m_fHeight = 0.f;
int					CSceneScore:: m_nScore = 0;
CNumber*			CSceneScore:: m_Number[MAX_SCORE_NUM] = {NULL};
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneScore::CSceneScore(LPDIRECT3DDEVICE9 *pDevice, int nPriority) :CScene2D(pDevice, nPriority)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneScore ::~CSceneScore(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CSceneScore :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	// 原点の位置
	SetPos(pos);

	// 幅と高さの設定
	m_fWidth = width;
	m_fHeight = height;

	m_nScore = 0;

	pos.x += (MAX_SCORE_NUM - 1) * width;

	// 桁数分数字作成
	for(int i = 0; i < MAX_SCORE_NUM; i++)
	{
		m_Number[i] = m_Number[i]->Create(m_pD3DDevice, pos, width, height, texType);
		pos.x -= width;
	}
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneScore ::Uninit(void)
{
	for(int i = 0; i < MAX_SCORE_NUM; i++)
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
void CSceneScore ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneScore ::Draw(void)
{
	// 桁数分数字表示
	for(int i = 0; i < MAX_SCORE_NUM; i++)
	{
		m_Number[i]->Draw();
	}
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneScore* CSceneScore::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum)
{
	// 作成
	CSceneScore* p = new CSceneScore(pDevice);

	// 初期化
	p->Init(pos, width, height, texType);

	return p;
}

//*****************************************************************************
// 数字変更関数
//*****************************************************************************
void CSceneScore ::Add(int addNum)
{
	m_nScore += addNum;
	if(m_nScore >= MAX_SCORE)
	{
		m_nScore = MAX_SCORE;
	}

	if(m_nScore <= 0)
	{
		m_nScore = 0;
	}

	for(int nCntPlace = 0; nCntPlace < MAX_SCORE_NUM; nCntPlace++)
	{
		int number = 0;
		//桁の数字割出処理
		number = (m_nScore / (int)pow(10.0,(double)(nCntPlace))) % 10;	// 桁ごとの10進の数字を出すために10を使っている

		// 数字セット
		m_Number[nCntPlace]->SetNumber(number);
	}
}
//----EOF----