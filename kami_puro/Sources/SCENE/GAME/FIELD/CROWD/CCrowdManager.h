//=============================================================================
//
// CCrowdManagerクラス [CCrowdManager.h]
// Author : 塚本俊彦
//
// リングの前後にいる観客たち
// ３Ｄの板ポリ
//
//=============================================================================
#ifndef _CCROWDMANAGER_H_
#define _CCROWDMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
class CCrowd;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCrowdManager
{
public:
	CCrowdManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CCrowdManager();

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// クリエイト
	static CCrowdManager* Create(
		LPDIRECT3DDEVICE9 *pDevice,
		CManager *pManager);

private:
	void Init();

	// 保存しときたいポインタ
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
	CManager *m_pManager;

	// 観客の前後の板ポリ
	CCrowd *m_pCrowd3DFront;
	CCrowd *m_pCrowd3DBack;
};

#endif
//----EOF----