//=============================================================================
//
// CSceneAnimeクラス [CSceneAnime.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneAnime.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneAnime::CSceneAnime(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene2D(pDevice, nPriority, objType)
{
	m_nMaxLoopNum = 0;
	m_nCurLoopNum = 0;
	m_bAutoUpdate = true;
	m_bDraw = true;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneAnime ::~CSceneAnime(void)
{
}

//*****************************************************************************
// 初期化関数 ループなしバー
//*****************************************************************************
HRESULT CSceneAnime :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int endTime)
{
	CScene2D ::Init(pos, width, height, texType);

	// 値のチェック
	if(sepalateX <= 0 || sepalateY <= 0)
	{
		return E_FAIL;
	}
	m_nSepalateNumX = sepalateX;
	m_nSepalateNumY = sepalateY;
	m_nMaxIdx = sepalateX * sepalateY;
	m_nEndTime = endTime;
	m_nChangeTime = m_nEndTime / ((m_nSepalateNumX) * (m_nSepalateNumY));
	m_nDoAnimCounter = 0;

	// テクスチャ座用セット
	SetIndex(0);

	return S_OK;
}

//*****************************************************************************
// 初期化関数 ループありバー loopNumがマイナス値なら無限ループ
//*****************************************************************************
HRESULT CSceneAnime :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int animSpd, int loopNum)
{
	CScene2D ::Init(pos, width, height, texType);

	// 値のチェック
	if(sepalateX <= 0 || sepalateY <= 0)
	{
		return E_FAIL;
	}
	m_nSepalateNumX = sepalateX;
	m_nSepalateNumY = sepalateY;
	m_nMaxIdx = sepalateX * sepalateY;
	m_nChangeTime = animSpd;
	m_nDoAnimCounter = 0;
	m_nMaxLoopNum = loopNum;

	// テクスチャ座用セット
	SetIndex(0);

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneAnime ::Uninit(void)
{
	CScene2D :: Uninit();
	//Release();

}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneAnime ::Update(void)
{
	if(!m_bAutoUpdate)
	{
		return;
	}
	// ループなし
	if(m_nMaxLoopNum == 0)
	{
		Update_Normal();
	}

	// ループあり
	else
	{
		Update_Loop();
	}
}

// ループなし
void CSceneAnime ::Update_Normal(void)
{
	m_nDoAnimCounter++;

	// アニメーション時間超えたら
	if(m_nDoAnimCounter >= m_nEndTime)
	{
		this->Release();
	}

	else if(m_nDoAnimCounter % m_nChangeTime == 0)
	{
		m_nIdx++;
		if(m_nIdx >= m_nMaxIdx)
		{
			m_nIdx = 0;
		}
		SetIndex(m_nIdx);
	}
}

// ループver
void CSceneAnime ::Update_Loop(void)
{
	m_nDoAnimCounter++;

	// アニメーション時間超えたら
	if(m_nCurLoopNum >= m_nMaxLoopNum && m_nMaxLoopNum >= 0)
	{
		this->Release();
	}

	else if(m_nDoAnimCounter % m_nChangeTime == 0)
	{
		m_nIdx++;
		if(m_nIdx >= m_nMaxIdx)
		{
			m_nIdx = 0;
		}
		SetIndex(m_nIdx);
	}
}


//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneAnime ::Draw(void)
{
	if(m_bDraw)
	{
		CScene2D ::Draw();
	}
}

//*****************************************************************************
// クリエイト関数　ループなしVer
//*****************************************************************************
CSceneAnime* CSceneAnime::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int endTime)
{
	// 作成
	CSceneAnime* p = new CSceneAnime(pDevice);

	p->m_pD3DDevice = pDevice;

	// 初期化
	p->Init(pos, width, height, texType, sepalateX, sepalateY, endTime);

	return p;
}

//*****************************************************************************
// クリエイト関数　ループVer
//*****************************************************************************
CSceneAnime* CSceneAnime::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int animSpd, int loopNum, int priority)
{
	// 作成
	CSceneAnime* p = new CSceneAnime(pDevice, priority);

	p->m_pD3DDevice = pDevice;

	// 初期化
	p->Init(pos, width, height, texType, sepalateX, sepalateY, animSpd, loopNum);

	return p;
}

//=============================================================================
// インデックスの設定
//=============================================================================
void CSceneAnime::SetIndex(int nIdx)
{
	float fU, fV;

	// 値のチェック
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
	m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
	m_fTop = fV * (m_nIdx / m_nSepalateNumX);
	m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}
//=============================================================================
// インデックスの設定
//=============================================================================
void CSceneAnime::SetIndex(int nIdx, bool reverse)
{
	float fU, fV;

	// 値のチェック
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	if (reverse){
		m_fRight = fU * (m_nIdx % m_nSepalateNumX);
		m_fLeft = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}
	else{
		m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
		m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}
//----EOF----