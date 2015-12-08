//=============================================================================
//
// CFieldManagerクラス [CFieldManager.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CFIELDMANAGER_H_
#define _CFIELDMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
class CRopeManager;
class CSceneX;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFieldManager
{
public:
	CFieldManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager);
	~CFieldManager(void);

	// 作成
	static CFieldManager *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		CManager *pManager);

	void Init();
	void Uninit();
	void Update();

	// 描画しない
	void SetImvisible(void);
	// 描画する
	void SetVisible(void);
private:
	// デバイスの保存
	LPDIRECT3DDEVICE9 *m_pDevice;
	// マネージャー
	CManager *m_pManager;

	// ロープ管理
	CRopeManager *m_pRopeManger;

	// 会場
	CSceneX *m_pDome;
};

#endif
//----EOF----