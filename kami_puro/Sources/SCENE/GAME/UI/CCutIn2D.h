//=============================================================================
//
// CCutIn2D2Dクラス [CCutIn2D.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CCUTIN2D_H_
#define _CCUTIN2D_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCutIn2D : public CScene2D
{
public:

	CCutIn2D(LPDIRECT3DDEVICE9 *pDevice);
	~CCutIn2D(void);

	void Update();
	void Uninit();
	void DrawNormalRender();

	// 高さ、Y座標、左端の座標、右端の座標、デバイス
	static CCutIn2D *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3& pos,
		float width, float height, TEXTURE_TYPE texType);

	// 白さ加減の調整
	bool AddWhite(float f);

	// 白さのリセット
	void InitWhite()
	{
		m_WhiteColor = 0;
	}

private:
	// 初期化
	void Init(D3DXVECTOR3& pos,
		float width, float height, TEXTURE_TYPE texType);

	// 白さ加減
	float m_WhiteColor;
};

#endif
//----EOF----