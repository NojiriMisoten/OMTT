//=============================================================================
//
// CBackGroundManagerクラス [CBackGroundManager.h]
// Author : 塚本　俊彦
//
// ゲーム開始のカウントダウンを行う
//
//=============================================================================
#ifndef _CBACKGROUND_H_
#define _CBACKGROUND_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../RENDERER/CRenderer.h"
#include "../TEXTURE/CTexture.h"

//=============================================================================
// 定数
//=============================================================================
// 背景のレイヤー数（スプライトの数）
static const short BACKGROUND_MAX = 3;

enum BG_DIRECTION{
	BG_DIRECTION_NONE = 0,
	BG_DIRECTION_LEFT,
	BG_DIRECTION_RIGHT,
};

//=============================================================================
// 前方宣言
//=============================================================================
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBackGroundManager
{
	// 公開メンバ
public:
	CBackGroundManager(LPDIRECT3DDEVICE9 *pDevice);
	~CBackGroundManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// 背景を作る関数3枚まで生成可能で最大個数を超えたらfalse
	bool CreateBG(TEXTURE_TYPE texture);
	// 背景を作る関数UVアニメーション用 横に移動する量を入れる
	bool CreateBG(TEXTURE_TYPE texture, float velocityX);

	// 非公開メンバ
private:
	LPDIRECT3DDEVICE9 *m_pDevice;

	CScene2D *m_pSprite[BACKGROUND_MAX];

	D3DXVECTOR3 m_vPosition[BACKGROUND_MAX];
	float m_fVelocityX[BACKGROUND_MAX];
	bool m_bAnimation[BACKGROUND_MAX];
	BG_DIRECTION m_bDirection[BACKGROUND_MAX];

	// 現在の枚数
	short m_nBackGroundNum;


};

#endif
//----EOF----