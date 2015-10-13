//=============================================================================
//
// CThreadManagerクラス [CThreadManager.h]
// Author : 塚本　俊彦
//
// 糸マネージャー
//
//=============================================================================
#ifndef _CTHREADMANAGER_H_
#define _CTHREADMANAGER_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "../PLAYER/CPlayer.h"

//=============================================================================
// 定数
//=============================================================================
enum ThreadType{
	THREAD_TYPE_NORMAL,
	THREAD_TYPE_ATTACK,		// 攻撃特化形態の糸
	THREAD_TYPE_SPEED,		// 移動特化形態の糸
	THREAD_TYPE_TRAP,		// 罠特化形態の糸
	THREAD_TYPE_MAX
};

//=============================================================================
// 前方宣言
//=============================================================================
class CThread;
class CEffectManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadManager
{
	// 公開メンバ
public:
	CThreadManager(LPDIRECT3DDEVICE9 *pDevice);
	~CThreadManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// 糸を作る関数(糸タイプ,プレイヤ番号,座標)
	void CreateThread(ThreadType type, int nPlayerNum, D3DXVECTOR3 pos, DIRECTION_PLAYER_FACING playerFacing, CEffectManager *pEffectManager);

	// 非公開メンバ
private:
	LPDIRECT3DDEVICE9 *m_pDevice;
};

#endif
//----EOF----