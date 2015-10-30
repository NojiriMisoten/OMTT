//=============================================================================
//
// CRopeManagerクラス [CRopeManager.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CROPEMANAGER_H_
#define _CROPEMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"


//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRope;
class CManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRopeManager
{
public:
	CRopeManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CRopeManager(void);

	void Init();
	void Update();
	void Uninit();

	// ゴムアニメ―ションスタート
	// 1フレームで引っ張る力、何フレーム引っ張るか
	void Pull(float pullPower, int pullInterval);

private:

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
	// マネージャー
	CManager *m_pManager;

	// ロープ
	CRope *m_pRopeOut[4];
	CRope *m_pRopeIn[4];
};

#endif
//----EOF----