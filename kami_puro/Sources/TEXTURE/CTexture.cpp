//=============================================================================
//
// テクスチャ処理 [CTexture.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CTexture.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
// テクスチャのパス
static const char *TEXTURE_PATH[ TEXTURE_MAX ] = 
{
	"../data/TEXTURE/mono.png",
	"../data/TEXTURE/default.png",
	"../data/TEXTURE/number.png",
	"../data/TEXTURE/title.png",
	"../data/TEXTURE/result.png",
	"../data/TEXTURE/HpColorGreen.png",
	"../data/TEXTURE/HpColorRed.png",
	"../data/TEXTURE/HpFrame.png",
	"../data/TEXTURE/HpFrameTop.png",
	"../data/TEXTURE/CrowdFrame.png",
	"../data/TEXTURE/CrowdHuman.png",
	"../data/TEXTURE/CrowdSpark.png",
	"../data/TEXTURE/jijii.png",
	"../data/TEXTURE/jijii_back.png",
	"../data/TEXTURE/blue.png",
	"../data/TEXTURE/red.png",
	"../data/TEXTURE/metalicgray.png",
	"../data/TEXTURE/windows.jpg",
	"../data/TEXTURE/CommandButton.png",				// ボタンのテクスチャ
	"../data/TEXTURE/rope.png",
	"../data/TEXTURE/FadeEffectLine.png",		// バトルフェード
	"../data/TEXTURE/FadeEffectBall.png",		// バトルフェード
	"../data/TEXTURE/FadeEffectBall2.png",		// バトルフェード
	"../data/TEXTURE/CutIn0.png",				// カットイン0
	"../data/TEXTURE/CutIn1.png",				// カットイン1
	"../data/TEXTURE/CutInBack.png",			// カットイン背景blue
	"../data/TEXTURE/CutInBack2.png",			// カットイン背景red
	"../data/TEXTURE/SKILL_NAME/Chop.png",	
	"../data/TEXTURE/SKILL_NAME/Elbow.png",	
	"../data/TEXTURE/SKILL_NAME/Lariat.png",	
	"../data/TEXTURE/SKILL_NAME/Slap.png",	
	"../data/TEXTURE/SKILL_NAME/BackDrop.png",	
	"../data/TEXTURE/SKILL_NAME/Stunner.png",	
	"../data/TEXTURE/SKILL_NAME/RollingElbow.png",	
	"../data/TEXTURE/SKILL_NAME/FlyingShoulder.png",	
	"../data/TEXTURE/SKILL_NAME/DropKick.png",
	"../data/TEXTURE/SKILL_NAME/SmallAttack.png",
	"../data/TEXTURE/SKILL_NAME/BigAttack.png",
	"../data/TEXTURE/SKILL_NAME/Throw.png",
	"../data/TEXTURE/SKILL_NAME/rope.png",
	"../data/TEXTURE/SKILL_NAME/Finish.png",
	"../data/TEXTURE/ElectricalScoreboard.png",
	"../data/TEXTURE/commandchart_back.png",
	"../data/TEXTURE/tempCommandBackPoly.png",
	"../data/TEXTURE/TITLE/black.png",
	"../data/TEXTURE/TITLE/logo.png",
	"../data/TEXTURE/TITLE/logoany.png",
	"../data/TEXTURE/TITLE/star.png",
	"../data/TEXTURE/TITLE/fire.png",
	"../data/TEXTURE/chain.png",
	"../data/TEXTURE/READY.png",
	"../data/TEXTURE/FIGHT.png",
	"../data/TEXTURE/K.O.png",
	"../data/TEXTURE/human.png",		// 観客のテクスチャ
};

//*****************************************************************************
// スタティックメンバ変数宣言
//*****************************************************************************
bool CTexture::m_bDoneCreateFlag = false;
LPDIRECT3DTEXTURE9 CTexture::m_pD3DTex[TEXTURE_MAX] = {NULL};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTexture::Init( LPDIRECT3DDEVICE9 *pDevice )
{
	// ゲームで使う全テクスチャを作成
	for( int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++ )
	{
		if(TEXTURE_PATH[TexCnt] != NULL)
		{
			// テクスチャ読み込み
			if( FAILED( D3DXCreateTextureFromFile((*pDevice), TEXTURE_PATH[TexCnt], &m_pD3DTex[TexCnt])))
			{
#ifdef _DEBUG
				assert(!"テクスチャがdataにない！");
#endif
				m_pD3DTex[TexCnt] = NULL;
				return E_FAIL;
			}
		}
		else
		{
			m_pD3DTex[TexCnt] = NULL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTexture::Uninit( void )
{
	// 使った全テクスチャポインタを開放
	for( int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++ )
	{
		// テクスチャポインタが使われている場合
		if(m_pD3DTex[TexCnt])
		{
			m_pD3DTex[TexCnt]->Release();
			m_pD3DTex[TexCnt] = NULL;
		}
	}

}

//=============================================================================
// テクスチャ作成処理
//=============================================================================
void CTexture::CreateTexture( LPDIRECT3DDEVICE9 *pDevice)
{
	CTexture::Init(pDevice);
}

//=============================================================================
// テクスチャポインタ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture( const TEXTURE_TYPE type )
{
	// 正しいテクスチャタイプか判定
	if( type < 0 || type > TEXTURE_MAX )
	{
#ifdef _DEBUG
		assert(!"不正なテクスチャタイプ！");
#endif
		return NULL;
	}
	return m_pD3DTex[type];
}

//----EOF----