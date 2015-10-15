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
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFace : public CScene2D
{
public:

	CFace(LPDIRECT3DDEVICE9 *pDevice);
	~CFace(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 作成
	static CFace *Create(
		D3DXVECTOR2 posLeft,
		D3DXVECTOR2 sizeLeft,
		D3DXVECTOR2 posRight,
		D3DXVECTOR2 sizeRight,
		LPDIRECT3DDEVICE9 *pDevice);

private:
	// 初期化
	void Init(
		D3DXVECTOR2 posLeft,
		D3DXVECTOR2 sizeLeft,
		D3DXVECTOR2 posRight,
		D3DXVECTOR2 sizeRight);

	// 座標
	D3DXVECTOR2 m_PosLeft;
	D3DXVECTOR2 m_PosRight;

	// 左側の顔
	CScene2D *m_FaceLeft;
	// 左側の顔の枠
	CScene2D *m_BackLeft;
	// 右側の顔
	CScene2D *m_FaceRight;
	// 右側の顔の枠
	CScene2D *m_BackRight;


};

#endif
//----EOF----