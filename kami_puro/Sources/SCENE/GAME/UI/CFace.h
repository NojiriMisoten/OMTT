//=============================================================================
//
// CFaceクラス [CFace.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CFACE_H_
#define _CFACE_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFace
{
public:

	CFace(LPDIRECT3DDEVICE9 *pDevice);
	~CFace(void);

	void Update();
	void Uninit();
	void Draw();

	// 作成
	static CFace *Create(
		D3DXVECTOR2 &posLeft,
		D3DXVECTOR2 &sizeLeft,
		D3DXVECTOR2 &posRight,
		D3DXVECTOR2 &sizeRight,
		LPDIRECT3DDEVICE9 *pDevice);

private:
	// 初期化
	void Init(
		D3DXVECTOR2 &posLeft,
		D3DXVECTOR2 &sizeLeft,
		D3DXVECTOR2 &posRight,
		D3DXVECTOR2 &sizeRight);

	// 座標
	D3DXVECTOR2 m_PosLeft;
	D3DXVECTOR2 m_PosRight;

	// 左側の顔
	CScene2D *m_pFaceLeft;
	// 左側の顔の枠
	CScene2D *m_pBackLeft;
	// 右側の顔
	CScene2D *m_pFaceRight;
	// 右側の顔の枠
	CScene2D *m_pBackRight;

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----