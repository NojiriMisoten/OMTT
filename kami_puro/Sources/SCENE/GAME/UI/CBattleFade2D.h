//=============================================================================
//
// CBattleFade2Dクラス [CBattleFade2D.h]
// Author : 塚本俊彦
//
// 加算合成するよ
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
class CBattleFade2D : public CScene2D
{
public:

	CBattleFade2D(LPDIRECT3DDEVICE9 *pDevice);
	~CBattleFade2D(void);

	void Update();
	void Uninit();
	void DrawUIBattleFadeRender();

	// 高さ、Y座標、左端の座標、右端の座標、デバイス
	static CBattleFade2D *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3& pos,
		float width, float height, TEXTURE_TYPE texType);


private:
	// 初期化
	void Init(D3DXVECTOR3& pos,
		float width, float height, TEXTURE_TYPE texType);
};

#endif
//----EOF----